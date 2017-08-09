/*
	MakeTakaraPos()
	MakeTakaraPosList()
		MM_Init() ～ MM_Fnlz() の直後に呼ぶこと。
		ref: MM_Map, MM_W, MM_H
*/

#include "all.h"

static autoTable<__int64> *PaceTable; // -1 == wall
static Random *Rnd;

static void INIT(int seed)
{
	PaceTable = new autoTable<__int64>(getI64Zero, noop_i64, MM_W, MM_H);
	Rnd = new Random(seed);

	for(int x = 0; x < MM_W; x++)
	for(int y = 0; y < MM_H; y++)
	{
		PaceTable->SetCell(x, y, MM_Map->RefBit(x, y) ? -1 : 0);
	}
}
static void FNLZ(void)
{
	delete PaceTable;
	delete Rnd;
}

static void WalkMap(int xPos, int yPos)
{
	errorCase(PaceTable->GetCell(xPos, yPos) == -1); // ? 壁の中からスタート

	autoTable<int> *pMap = new autoTable<int>(getZero, noop_i, MM_W, MM_H); // INT_MAX == 未到達
	autoList<int> *dirStack = new autoList<int>();
	int backed = 0;
	int dir = -1;
	int doubleBack = 0;

	for(int x = 0; x < MM_W; x++)
	for(int y = 0; y < MM_H; y++)
	{
		pMap->SetCell(x, y, INT_MAX);
	}
	for(; ; )
	{
		int doProg  = 0;

		if(backed)
		{
			if(doubleBack)
			{
				doubleBack = 0;
			}
			else if(dir < 8)
			{
				dir += 2;
				doProg = 1;
			}
		}
		else
		{
			int currPace = dirStack->GetCount();

			if(
				0 <= xPos && xPos < MM_W &&
				0 <= yPos && yPos < MM_H &&
				PaceTable->GetCell(xPos, yPos) != -1 // ? not wall
				)
			{
				int pace = pMap->GetCell(xPos, yPos);

				if(currPace < pace) // ? 今の歩数 < 既知の歩数 || 未到達
				{
					pMap->SetCell(xPos, yPos, currPace);
					dir = 2;
					doProg = 1;
				}
				else if(pace <= currPace - 3) // ? 既知の歩数が３歩以上少ない。-> 既知の歩数から探索すれば１歩以上少なくなる。
				{
					doubleBack = 1;
				}
			}
		}
		if(doProg)
		{
			dirStack->AddElement(dir);

			switch(dir) // 進む
			{
			case 2: yPos++; break;
			case 4: xPos--; break;
			case 6: xPos++; break;
			case 8: yPos--; break;

			default:
				error();
			}
		}
		else
		{
			if(dirStack->GetCount() == 0)
				break;

			dir = dirStack->UnaddElement();

			switch(dir) // 戻る
			{
			case 2: yPos--; break;
			case 4: xPos++; break;
			case 6: xPos--; break;
			case 8: yPos++; break;

			default:
				error();
			}
		}
		backed = !doProg;
	}
	for(int x = 0; x < MM_W; x++)
	for(int y = 0; y < MM_H; y++)
	{
		int pace = pMap->GetCell(x, y);

		if(pace != INT_MAX) // ? not 未到達
		{
			if(pace < 20)
				pace -= IMAX; // スタート地点周辺の強制沈下

			*PaceTable->CellAt(x, y) += pace;
		}
	}
	delete pMap;
	delete dirStack;
}
static void WalkMapFromRect(int l, int t, int w, int h)
{
	for(int x = 0; x < w; x++)
	for(int y = 0; y < h; y++)
	{
		int rx = l + x;
		int ry = t + y;

		if(PaceTable->GetCell(rx, ry) != -1) // ? not wall
		{
			WalkMap(rx, ry);
			goto endFunc;
		}
	}
//	error(); // ? 空間が無い。-> 無いこともある。
endFunc:;
}
static void WalkMapFromAround(void)
{
LOGPOS();
SW_Start();
	WalkMapFromRect(0, 0, 1, MM_H);
LOGPOS();
SW_Lap();
	WalkMapFromRect(0, 0, MM_W, 1);
LOGPOS();
SW_Lap();
	WalkMapFromRect(MM_W - 1, 0, 1, MM_H);
LOGPOS();
SW_Lap();
	WalkMapFromRect(0, MM_H - 1, MM_W, 1);
SW_End();
LOGPOS();
}
static void EraseUnaccess(void)
{
	for(int x = 0; x < MM_W; x++)
	for(int y = 0; y < MM_H; y++)
	{
		if(PaceTable->GetCell(x, y) == 0) // ? 未到達, 唯一のスタート地点は潰されるけど問題無いだろう。
		{
			PaceTable->SetCell(x, y, -1); // to wall
		}
	}
}
static i2D_t GetFarestOne(autoList<i2D_t> *list)
{
	__int64 maxPace = -1;
	int maxNum = 0;

	for(int x = 0; x < MM_W; x++)
	for(int y = 0; y < MM_H; y++)
	{
		if(HasPos(list, x, y)) // ? 既に置いてある。
			continue;

		__int64 pace = PaceTable->GetCell(x, y);

		if(maxPace < pace)
		{
			maxPace = pace;
			maxNum = 1;
		}
		else if(pace == maxPace)
		{
			maxNum++;
		}
	}
	errorCase(maxNum == 0);

	int count = Rnd->Rnd(maxNum);
	i2D_t pos = { -1, -1 };

	for(int x = 0; x < MM_W; x++)
	for(int y = 0; y < MM_H; y++)
	{
		if(PaceTable->GetCell(x, y) == maxPace)
		{
			if(!count)
			{
				pos.X = x;
				pos.Y = y;

				goto endFunc;
			}
			count--;
		}
	}
	error(); // ? not found

endFunc:
	return pos;
}
static void GotoFarest(autoList<i2D_t> *list, int &xPos, int &yPos)
{
	autoList<int> *dirs = new autoList<int>();

	for(int count = 0; count < 1000; count++)
	{
		dirs->Clear();

		for(int dir = 2; dir <= 8; dir += 2)
		{
			int x = xPos;
			int y = yPos;

			switch(dir)
			{
			case 2: y++; break;
			case 4: x--; break;
			case 6: x++; break;
			case 8: y--; break;

			default:
				error();
			}
			if(
				0 <= x && x < MM_W &&
				0 <= y && y < MM_H &&
				PaceTable->GetCell(x, y) != -1 &&
				!HasPos(list, x, y) &&
				PaceTable->GetCell(xPos, yPos) <= PaceTable->GetCell(x, y)
				)
			{
				dirs->AddElement(dir);
			}
		}
		if(dirs->GetCount() == 0)
		{
			break;
		}

		// 移動
		{
			int dir = dirs->GetElement(Rnd->Rnd(dirs->GetCount()));

			switch(dir)
			{
			case 2: yPos++; break;
			case 4: xPos--; break;
			case 6: xPos++; break;
			case 8: yPos--; break;

			default:
				error();
			}
		}
	}
	delete dirs;
}

// ret {
autoList<i2D_t> *MMT_TakaraPosList; // 呼び出し側で開放
// }

void MakeTakaraPosList(int seed, int count)
{
	autoList<i2D_t> *list = new autoList<i2D_t>();

	INIT(seed);
	WalkMapFromAround();
	EraseUnaccess();

	for(; 0 < count; count--)
	{
LOG("MTPL %d\n", count);
		i2D_t pos = GetFarestOne(list);

		list->AddElement(pos);

		if(1 < count)
		{
SW_Start();
			WalkMap(pos.X, pos.Y);
SW_End();
		}
	}
	FNLZ();

	MMT_TakaraPosList = list;
}
void MakeTakaraPosList_R(int seed, int count, autoList<i2D_t> *list)
{
	if(!list)
		list = new autoList<i2D_t>();

	INIT(seed);
	WalkMapFromAround();
	EraseUnaccess();

	for(; 0 < count; count--)
	{
		int x;
		int y;

		for(; ; )
		{
			x = Rnd->Rnd(MM_W);
			y = Rnd->Rnd(MM_H);

			if(
				PaceTable->GetCell(x, y) != -1 &&
				!HasPos(list, x ,y)
				)
				break;
		}
		GotoFarest(list, x, y);

		{
			i2D_t pos;

			pos.X = x;
			pos.Y = y;

			list->AddElement(pos);
		}
	}
	FNLZ();

	MMT_TakaraPosList = list;
}
