#include "all.h"

static Random *MM_Rnd;

// ret {
/*
	0 = 空間
	1 = 壁
*/
bitTable *MM_Map; // 呼び出し側で開放すること。
int MM_W;
int MM_H;
// }

static oneObject(autoList<int>, new autoList<int>(), GetEBuff);

/*
	ex.
		MM_Evacuate();
		MM_Init();
		MM_Fnlz();
		delete MM_Map;
		MM_Unevacuate();
*/
void MM_Evacuate(void)
{
	GetEBuff()->AddElement((int)MM_Rnd);
	GetEBuff()->AddElement((int)MM_Map);
	GetEBuff()->AddElement(MM_W);
	GetEBuff()->AddElement(MM_H);
}
void MM_Unevacuate(void)
{
	MM_H = GetEBuff()->UnaddElement();
	MM_W = GetEBuff()->UnaddElement();
	MM_Map = (bitTable *)GetEBuff()->UnaddElement();
	MM_Rnd = (Random *)GetEBuff()->UnaddElement();
}

void MM_Init(int seed, int w, int h)
{
	MM_Rnd = new Random(seed);
	MM_Map = new bitTable(w);
	MM_W = w;
	MM_H = h;
}
void MM_Fnlz(void)
{
	delete MM_Rnd;
}

#define MM_EACH_FRAME() \
	do { \
	if(FrameStartTime + 10 < GetCurrTime()) EachFrame(); \
	} while(0)

void MM_Randomize(int wallPermil) // wallPermil: 0 - 1000
{
	for(int x = 0; x < MM_W; x++)
	for(int y = 0; y < MM_H; y++)
	{
		MM_Map->PutBit(x, y, MM_Rnd->RndPermil(wallPermil));
	}
	MM_EACH_FRAME();
}
void MM_BattlePos(int x, int y)
{
	int wallcnt = 0;

	for(int sx = -1; sx <= 1; sx++)
	for(int sy = -1; sy <= 1; sy++)
	{
		int ax = x + sx;
		int ay = y + sy;

		if(
			0 <= ax && ax < MM_W &&
			0 <= ay && ay < MM_H
			)
		{
			if(MM_Map->RefBit(ax, ay))
				wallcnt++;
		}
		else
			wallcnt++; // 枠の外は壁
	}
	MM_Map->PutBit(x, y, 5 <= wallcnt);
}
void MM_Battle(int count)
{
	for(; 0 < count; count--)
	{
		int x = MM_Rnd->Rnd(MM_W);
		int y = MM_Rnd->Rnd(MM_H);

		MM_BattlePos(x, y);
	}
	MM_EACH_FRAME();
}
void MM_Wipe(int direct, int count) // direct: 0 - 3
{
	for(; 0 < count; count--)
	{
		for(int x = 0; x < MM_W; x++)
		for(int y = 0; y < MM_H; y++)
		{
			int rx = x;
			int ry = y;

			if(direct & 1) rx = MM_W - 1 - rx;
			if(direct & 2) ry = MM_H - 1 - ry;

			MM_BattlePos(rx, ry);
		}
		MM_EACH_FRAME();
	}
}

// forestize >

static bitTable *NewMap;

static void MM_ForestizePos(int x, int y)
{
	for(int sx = 0; sx < 2; sx++)
	for(int sy = 0; sy < 2; sy++)
	{
		if(MM_Map->RefBit(x + sx, y + sy))
		{
			goto hasWall;
		}
	}
	return;

hasWall:
	for(int sx = 0; sx < 2; sx++)
	for(int sy = 0; sy < 2; sy++)
	{
		NewMap->PutBit(x + sx, y + sy, 1);
	}
}
static void MM_ForestizeOrig(int xOrig, int yOrig)
{
	for(int x = xOrig; x < MM_W - 1; x += 2)
	for(int y = yOrig; y < MM_H - 1; y += 2)
	{
		MM_ForestizePos(x, y);
	}
	MM_EACH_FRAME();
}
void MM_Forestize(void)
{
	NewMap = new bitTable(MM_W);

	MM_ForestizeOrig(0, 0);
	MM_ForestizeOrig(1, 1);

	delete MM_Map;
	MM_Map = NewMap;
}

// < forestize

void MM_Rim(int depth, int northPos, int northSize, int southPos, int southSize, int westPos, int westSize, int eastPos, int eastSize)
{
	errorCase(depth < 1 || m_min(MM_W, MM_H) < depth);
	errorCase(!m_isRange(northPos, 0, MM_W));
	errorCase(!m_isRange(northSize, 0, MM_W - northPos));
	errorCase(!m_isRange(southPos, 0, MM_W));
	errorCase(!m_isRange(southSize, 0, MM_W - southPos));
	errorCase(!m_isRange(westPos, 0, MM_H));
	errorCase(!m_isRange(westSize, 0, MM_H - westPos));
	errorCase(!m_isRange(eastPos, 0, MM_H));
	errorCase(!m_isRange(eastSize, 0, MM_H - eastPos));

	for(int c = 0; c < depth; c++)
	{
		for(int x = 0; x < MM_W; x++)
		{
			MM_Map->PutBit(x, c, x < northPos || northPos + northSize <= x);
			MM_Map->PutBit(x, MM_H - 1 - c, x < southPos || southPos + southSize <= x);
		}
		for(int y = 0; y < MM_H; y++)
		{
			MM_Map->PutBit(c, y, y < westPos || westPos + westSize <= y);
			MM_Map->PutBit(MM_W - 1 - c, y, y < eastPos || eastPos + eastSize <= y);
		}
	}
	MM_EACH_FRAME();
}
void MM_Expand(int expand)
{
	errorCase(expand < 2);

	MM_W *= expand;
	MM_H *= expand;

	NewMap = new bitTable(MM_W);

	for(int x = 0; x < MM_W; x++)
	for(int y = 0; y < MM_H; y++)
	{
		NewMap->PutBit(x, y, MM_Map->RefBit(x / expand, y / expand));
	}
	delete MM_Map;
	MM_Map = NewMap;

	MM_EACH_FRAME();
}
void MM_Rerandomize(int spaceWallPermil, int wallWallPermil)
{
	for(int x = 0; x < MM_W; x++)
	for(int y = 0; y < MM_H; y++)
	{
		MM_Map->PutBit(x, y, MM_Rnd->RndPermil(MM_Map->RefBit(x, y) ? wallWallPermil : spaceWallPermil));
	}
	MM_EACH_FRAME();
}
void MM_BombBox(int x, int y, int reach, int flag)
{
	for(int sx = -reach; sx <= reach; sx++)
	for(int sy = -reach; sy <= reach; sy++)
	{
		MM_Map->PutBit(x + sx, y + sy, flag);
	}
}
void MM_Bypass(int x, int y, int xa, int ya)
{
	while(MM_Map->RefBit(x, y))
	{
		MM_Map->PutBit(x, y, 0);

		x += xa;
		y += ya;
	}
}
void MM_Bypass_WE(int x, int y)
{
	MM_Bypass(x - 1, y, -1, 0);
	MM_Bypass(x + 0, y, +1, 0);
}
void MM_Bypass_NS(int x, int y)
{
	MM_Bypass(x, y - 1, 0, -1);
	MM_Bypass(x, y + 0, 0, +1);
}
void MM_Extra_01(void)
{
	for(int x = 0; x < MM_W - 1; x++)
	for(int y = 0; y < MM_H - 1; y++)
	{
		if(
			MM_Map->RefBit(x + 0, y + 0) == 0 &&
			MM_Map->RefBit(x + 0, y + 1) != 0 &&
			MM_Map->RefBit(x + 1, y + 0) != 0 &&
			MM_Map->RefBit(x + 1, y + 1) == 0 ||
			MM_Map->RefBit(x + 0, y + 0) != 0 &&
			MM_Map->RefBit(x + 0, y + 1) == 0 &&
			MM_Map->RefBit(x + 1, y + 0) == 0 &&
			MM_Map->RefBit(x + 1, y + 1) != 0
			)
		{
			MM_Map->PutBit(x + 0, y + 0, 0);
			MM_Map->PutBit(x + 1, y + 0, 0);
			MM_Map->PutBit(x + 0, y + 1, 0);
			MM_Map->PutBit(x + 1, y + 1, 0);
		}
	}
	MM_EACH_FRAME();
}
void MM_EraseHosoitokoro(void)
{
	for(; ; )
	{
		int eraseCount = 0;

		for(int x = 1; x < MM_W - 1; x++)
		for(int y = 1; y < MM_H - 1; y++)
		{
			if(MM_Map->RefBit(x, y))
			{
				if(
					!MM_Map->RefBit(x - 1, y) &&
					!MM_Map->RefBit(x + 1, y) ||
					!MM_Map->RefBit(x, y - 1) &&
					!MM_Map->RefBit(x, y + 1)
					)
				{
					MM_Map->PutBit(x, y, 0);
					eraseCount++;
				}
			}
		}
		if(!eraseCount)
			break;
	}
}
