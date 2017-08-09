#include "all.h"

static void (*MapLoaderTable[MAP_H][MAP_W])(void) =
{
	MakeMap_0101, MakeMap_0201, MakeMap_0301, MakeMap_0401,
	MakeMap_0102, MakeMap_0202, MakeMap_0302, MakeMap_0402,
	MakeMap_0103, MakeMap_0203, MakeMap_0303, MakeMap_0403,
	MakeMap_0104, MakeMap_0204, MakeMap_0304, MakeMap_0404,
};
static i2D_t *TakaraPosListTable[MAP_H][MAP_W] =
{
	TakaraPosList_0101, TakaraPosList_0201, TakaraPosList_0301, TakaraPosList_0401,
	TakaraPosList_0102, TakaraPosList_0202, TakaraPosList_0302, TakaraPosList_0402,
	TakaraPosList_0103, TakaraPosList_0203, TakaraPosList_0303, TakaraPosList_0403,
	TakaraPosList_0104, TakaraPosList_0204, TakaraPosList_0304, TakaraPosList_0404,
};
static int TakaraPosCountTable[MAP_H][MAP_W] =
{
	TakaraPosCount_0101, TakaraPosCount_0201, TakaraPosCount_0301, TakaraPosCount_0401,
	TakaraPosCount_0102, TakaraPosCount_0202, TakaraPosCount_0302, TakaraPosCount_0402,
	TakaraPosCount_0103, TakaraPosCount_0203, TakaraPosCount_0303, TakaraPosCount_0403,
	TakaraPosCount_0104, TakaraPosCount_0204, TakaraPosCount_0304, TakaraPosCount_0404,
};

void GameMain(void)
{
	UpdateGlobalTakaraCount();

	if(GDc.LoadFlag)
	{
		// LoadPlayData
		{
			GDc.MapX = Gnd.PlayData.MapX;
			GDc.MapY = Gnd.PlayData.MapY;
			GDc.PlayerEnterX = Gnd.PlayData.EnterX;
			GDc.PlayerEnterY = Gnd.PlayData.EnterY;
			GDc.PlayTimeFrame = Gnd.PlayData.PlayTimeFrame;
			GDc.ClearTimeFrame = Gnd.PlayData.ClearTimeFrame;

			for(int x = 0; x < MAP_W; x++)
			for(int y = 0; y < MAP_H; y++)
			{
				GDc.TakaraGotTable->GetCell(x, y)->Overwrite(Gnd.PlayData.TakaraOpenTable[x][y]);
			}
		}
		UpdateGlobalTakaraCount();
	}
	// 何となく...
	{
		m_range(GDc.MapX, 0, MAP_W - 1);
		m_range(GDc.MapY, 0, MAP_H - 1);
		m_range(GDc.PlayerEnterX, 0, IMAX);
		m_range(GDc.PlayerEnterX, 0, IMAX);
		m_range(GDc.PlayTimeFrame, 0, IMAX);
		m_range(GDc.ClearTimeFrame, 0, IMAX);
	}

	int &mapX = GDc.MapX;
	int &mapY = GDc.MapY;
	int entDir = 5;
	int entX = GDc.PlayerEnterX;
	int entY = GDc.PlayerEnterY;
	int exitDir = -1;
	int exitX = -1;
	int exitY = -1;

	for(; ; )
	{
//		GDc.DaylightMode = GetDaylightMode(); // ここではやらない。

		FieldInit();

		FDc.MapLoader = MapLoaderTable[mapY][mapX];
		FDc.TakaraPosList = TakaraPosListTable[mapY][mapX];
		FDc.TakaraPosCount = TakaraPosCountTable[mapY][mapX];
		FDc.EnterDir = entDir;
		FDc.EnterX = entX;
		FDc.EnterY = entY;
		FDc.RndSeed = mapX * 4 + mapY;
		FDc.BGMMusId = MUS_FIELD_AROUND;

		if(mapX == 1 && mapY == 1) FDc.BGMMusId = MUS_FIELD_CENTER;
		if(mapX == 1 && mapY == 2) FDc.BGMMusId = MUS_FIELD_CENTER;
		if(mapX == 2 && mapY == 1) FDc.BGMMusId = MUS_FIELD_CENTER;
		if(mapX == 2 && mapY == 2) FDc.BGMMusId = MUS_FIELD_CENTER;

		FieldMain();

		exitDir = FDc.ExitDir;
		exitX = FDc.ExitX;
		exitY = FDc.ExitY;

		if(exitDir == 5 && FDc.ExitWithoutSave == 0) // セーブ
		{
			SEPlay(SE_SAVE);

			// SavePlayData
			{
				Gnd.PlayData.SavedFlag = 1;

				Gnd.PlayData.MapX = GDc.MapX;
				Gnd.PlayData.MapY = GDc.MapY;
				Gnd.PlayData.EnterX = exitX;
				Gnd.PlayData.EnterY = exitY;
				Gnd.PlayData.PlayTimeFrame = GDc.PlayTimeFrame;
				Gnd.PlayData.ClearTimeFrame = GDc.ClearTimeFrame;

				for(int x = 0; x < MAP_W; x++)
				for(int y = 0; y < MAP_H; y++)
				{
					Gnd.PlayData.TakaraOpenTable[x][y]->Overwrite(GDc.TakaraGotTable->GetCell(x, y));
				}
			}
			forscene(20) // ただの待ち
			{
				DrawCurtain();
				EachFrame();
			}
			sceneLeave();
		}
		FieldFnlz();

		switch(exitDir)
		{
		case 5:
			goto endLoop;

		case 2:
			mapY++;
			entDir = 8;
			break;

		case 4:
			mapX--;
			entDir = 6;
			break;

		case 6:
			mapX++;
			entDir = 4;
			break;

		case 8:
			mapY--;
			entDir = 2;
			break;

		default:
			error();
		}
		mapX = (mapX + MAP_W) % MAP_W;
		mapY = (mapY + MAP_H) % MAP_H;

		errorCase(mapX < 0 || MAP_W <= mapX);
		errorCase(mapY < 0 || MAP_H <= mapY);
	}
endLoop:;
}
int GetTakaraPosCount(int x, int y)
{
	errorCase(x < 0 || MAP_W <= x);
	errorCase(y < 0 || MAP_H <= y);

	return TakaraPosCountTable[y][x];
}
