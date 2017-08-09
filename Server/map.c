#include "map.h"

#define APPDATA_DIR "C:\\appdata\\Actor83"
#define AREA_BOUND 100
#define CELLNUMMAX 100
#define MAP_W 4
#define MAP_H 4

static int MapSizes[MAP_W][MAP_H][2] =
{
	{
		{ 90, 30 },
		{ 30, 90 },
		{ 100, 50 },
		{ 200, 200 },
	},
	{
		{ 30, 30 },
		{ 2000, 2000 },
		{ 2000, 1000 },
		{ 50, 100 },
	},
	{
		{ 1000, 500 },
		{ 1000, 2000 },
		{ 1000, 1000 },
		{ 300, 300 },
	},
	{
		{ 500, 500 },
		{ 300, 3000 },
		{ 50, 50 },
		{ 50, 50 },
	},
};

static int Map_X;
static int Map_Y;
static int Field_W;
static int Field_H;
static char *FieldFile;
static FILE *FieldFP;
static int Field_X;
static int Field_Y;

// ---- マップのロード ----

static void UnloadFieldMap(void)
{
	LOGPOS();
	memFree(FieldFile);
	fileClose(FieldFP);
	FieldFile = NULL;
	FieldFP = NULL;
}
static void SetFieldFileSize(char *file, int size)
{
	cout("FIELD_SIZE: %d\n", size);

	errorCase(size < 0); // 2bs

	if(!existFile(file))
	{
		LOGPOS();
		createPath(file, 'F');
	}
	if(getFileSize(file) != size)
	{
		LOGPOS();
		setFileSize(file, size);
	}
}
static void LoadFieldMap(int mapX, int mapY)
{
//	cout("MAP_X: %d\n", mapX);
//	cout("MAP_Y: %d\n", mapY);

	if(FieldFP) // ? already loaded
	{
		if(mapX == Map_X && mapY == Map_Y)
		{
//			cout("SAME_MAP\n");
			return;
		}
		UnloadFieldMap();
	}
	m_range(mapX, 0, MAP_W - 1);
	m_range(mapY, 0, MAP_H - 1);

	Map_X = mapX;
	Map_Y = mapY;
	Field_W = MapSizes[mapX][mapY][0];
	Field_H = MapSizes[mapX][mapY][1];
	FieldFile = combine_cx(APPDATA_DIR, xcout("%d_%d.map", mapX, mapY));

	cout("FIELD_W: %d\n", Field_W);
	cout("FIELD_H: %d\n", Field_H);
	cout("FIELD_FILE: %s\n", FieldFile);

	SetFieldFileSize(FieldFile, Field_W * Field_H * FIELDCELLSIZE);

	FieldFP = fileOpen(FieldFile, "r+b"); // 閉じない。-> termination_fileCloseAll() に任せる。
	Field_X = 0;
	Field_Y = 0;
}

// ---- マップ内のシーク ----

static void SeekFieldCell(int fieldX, int fieldY)
{
	errorCase(!FieldFP); // not loaded

//	cout("FIELD_X: %d\n", fieldX);
//	cout("FIELD_Y: %d\n", fieldY);

	m_range(fieldX, 0, Field_W - 1);
	m_range(fieldY, 0, Field_H - 1);

	fileSeek(FieldFP, SEEK_SET, (fieldX + fieldY * Field_W) * FIELDCELLSIZE);
}
static void ReadFieldCell(uchar *cells, int cellNum)
{
	autoBlock_t gab;

//	cout("CELLS_R: %d\n", cellNum);

	errorCase(!cells);
	errorCase(!m_isRange(cellNum, 0, CELLNUMMAX));

	fileRead(FieldFP, gndBlockVar(cells, cellNum * FIELDCELLSIZE, gab));
}
static void WriteFieldCell(uchar *cells, int cellNum)
{
	autoBlock_t gab;

//	cout("CELLS_W: %d\n", cellNum);

	errorCase(!cells);
	errorCase(!m_isRange(cellNum, 0, CELLNUMMAX));

	fileWrite(FieldFP, gndBlockVar(cells, cellNum * FIELDCELLSIZE, gab));
}

// ----

autoBlock_t *GetFieldArea(int mapX, int mapY, int fieldX, int fieldY)
{
	autoBlock_t *buff = newBlock();
	int xBgn;
	int yBgn;
	int xEnd;
	int yEnd;
	int w;
	int h;
	int y;

	LoadFieldMap(mapX, mapY);

	m_range(fieldX, 0, Field_W - 1);
	m_range(fieldY, 0, Field_H - 1);

	xBgn = fieldX - AREA_BOUND / 2;
	yBgn = fieldY - AREA_BOUND / 2;
	xEnd = fieldX + AREA_BOUND / 2;
	yEnd = fieldY + AREA_BOUND / 2;

	m_range(xBgn, 0, Field_W - 1);
	m_range(yBgn, 0, Field_H - 1);
	m_range(xEnd, 1, Field_W);
	m_range(yEnd, 1, Field_H);

	w = xEnd - xBgn;
	h = yEnd - yBgn;

	cout("X_BGN: %d\n", xBgn);
	cout("Y_BGN: %d\n", yBgn);
	cout("W: %d\n", w);
	cout("H: %d\n", h);

	ab_addValue(buff, mapX);
	ab_addValue(buff, mapY);
	ab_addValue(buff, xBgn);
	ab_addValue(buff, yBgn);
	ab_addValue(buff, w);
	ab_addValue(buff, h);

	{
		autoBlock_t *tmp = nobCreateBlock(w * FIELDCELLSIZE);

		for(y = yBgn; y < yEnd; y++)
		{
			SeekFieldCell(xBgn, y);
			ReadFieldCell(directGetBuffer(tmp), w);
			addBytes(buff, tmp);
		}
		releaseAutoBlock(tmp);
	}
	cout("BUFF_SIZE: %u\n", getSize(buff));
	return buff;
}
void SetFieldCell(int mapX, int mapY, int fieldX, int fieldY, uchar *cell)
{
	if(IsTakaraPos(mapX, mapY, fieldX, fieldY))
	{
		static uchar initCell[FIELDCELLSIZE];

		cell = initCell;
	}
	LoadFieldMap(mapX, mapY);
	SeekFieldCell(fieldX, fieldY);
	WriteFieldCell(cell, 1);
}
