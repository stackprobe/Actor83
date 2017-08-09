#include "C:\Factory\Common\all.h"
#include "C:\Factory\SubTools\libs\bmp.h"

#define R_DIR "C:\\var\\Actor83マップデータ_20160611"
#define W_DIR "C:\\temp"

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

static autoList_t *LoadMapFile(char *file, uint mapX, uint mapY)
{
	FILE *fp = fileOpen(file, "rb");
	autoList_t *rows = newList();
	autoList_t *row;
	uint w = MapSizes[mapX][mapY][0];
	uint h = MapSizes[mapX][mapY][1];
	uint x;
	uint y;

	for(y = 0; y < h; y++)
	{
		row = newList();

		for(x = 0; x < w; x++)
		{
			autoBlock_t *cell = neReadBinaryBlock(fp, 8);

			if(*(uint64 *)directGetBuffer(cell) != 0ui64)
				addElement(row, 0xffff00);
			else
				addElement(row, 0x000000);

			releaseAutoBlock(cell);
		}
		addElement(rows, (uint)row);
	}
	fileClose(fp);
	return rows;
}
int main(int argc, char **argv)
{
	uint x;
	uint y;

	for(x = 0; x < 4; x++)
	for(y = 0; y < 4; y++)
	{
		char *rFile = combine_cx(R_DIR, xcout("%u_%u.map", x, y));
		char *wFile = combine_cx(W_DIR, xcout("%u_%u.bmp", x, y));
		autoList_t *bmp;

		bmp = LoadMapFile(rFile, x, y);
		writeBMPFile(wFile, bmp);

		memFree(rFile);
		memFree(wFile);
		releaseDim_BR(bmp, 2, NULL);
	}
}
