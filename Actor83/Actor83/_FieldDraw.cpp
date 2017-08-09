#include "all.h"

static int (*DFW_IsWaterFunc)(int, int);

static int DFW_IsWater(int x, int y)
{
	if(
		0 <= x && x < FDc.FMap->GetWidth() &&
		0 <= y && y < FDc.FMap->GetHeight()
		)
	{
		return DFW_IsWaterFunc(x, y);
	}
	return 1;
}
static void DrawFieldWater(int drawX, int drawY, int fMapX, int fMapY, int mcId, int mcOrigX, int mcOrigY, int (*funcIsWater)(int, int), int animeMode)
{
	DFW_IsWaterFunc = funcIsWater;

	int tX1, tY1;
	int tX3, tY3;
	int tX7, tY7;
	int tX9, tY9;

	switch((DFW_IsWater(fMapX - 1, fMapY) ? 1 : 0) | (DFW_IsWater(fMapX, fMapY + 1) ? 2 : 0))
	{
	case 0: tX1 = 0; tY1 = 1; break;
	case 1: tX1 = 2; tY1 = 5; break;
	case 2: tX1 = 0; tY1 = 3; break;
	case 3: tX1 = 2; tY1 = DFW_IsWater(fMapX - 1, fMapY + 1) ? 3 : 1; break;

	default:
		error();
	}
	switch((DFW_IsWater(fMapX + 1, fMapY) ? 1 : 0) | (DFW_IsWater(fMapX, fMapY + 1) ? 2 : 0))
	{
	case 0: tX3 = 1; tY3 = 1; break;
	case 1: tX3 = 1; tY3 = 5; break;
	case 2: tX3 = 3; tY3 = 3; break;
	case 3: DFW_IsWater(fMapX + 1, fMapY + 1) ? (tX3 = 1, tY3 = 3) : (tX3 = 3, tY3 = 1); break;

	default:
		error();
	}
	switch((DFW_IsWater(fMapX - 1, fMapY) ? 1 : 0) | (DFW_IsWater(fMapX, fMapY - 1) ? 2 : 0))
	{
	case 0: tX7 = 0; tY7 = 0; break;
	case 1: tX7 = 2; tY7 = 2; break;
	case 2: tX7 = 0; tY7 = 4; break;
	case 3: tX7 = 2; tY7 = DFW_IsWater(fMapX - 1, fMapY - 1) ? 4 : 0; break;

	default:
		error();
	}
	switch((DFW_IsWater(fMapX + 1, fMapY) ? 1 : 0) | (DFW_IsWater(fMapX, fMapY - 1) ? 2 : 0))
	{
	case 0: tX9 = 1; tY9 = 0; break;
	case 1: tX9 = 1; tY9 = 2; break;
	case 2: tX9 = 3; tY9 = 4; break;
	case 3: DFW_IsWater(fMapX + 1, fMapY - 1) ? (tX9 = 1, tY9 = 4) : (tX9 = 3, tY9 = 0); break;

	default:
		error();
	}
	int animeCnt = 0;
	int animeNumer = 0;
	const int ANIME_DENOM = 30;

	if(animeMode)
	{
		animeCnt = ProcFrame / ANIME_DENOM;
		animeNumer = ProcFrame % ANIME_DENOM;
	}
	for(int ac = 0; ac < 2; ac++)
	{
		int mcRelX = (animeCnt + ac) % 3;
		mcRelX *= 4;

		if(ac)
			DPE_SetAlpha(animeNumer / (double)ANIME_DENOM);

		DrawSimple(GetSFPicId(mcId, mcOrigX + mcRelX + tX1, mcOrigY + tY1, 1), drawX + 0, drawY + 16);
		DrawSimple(GetSFPicId(mcId, mcOrigX + mcRelX + tX3, mcOrigY + tY3, 1), drawX + 16, drawY + 16);
		DrawSimple(GetSFPicId(mcId, mcOrigX + mcRelX + tX7, mcOrigY + tY7, 1), drawX + 0, drawY + 0);
		DrawSimple(GetSFPicId(mcId, mcOrigX + mcRelX + tX9, mcOrigY + tY9, 1), drawX + 16, drawY + 0);

		if(ac)
			DPE_Reset();
	}
	DFW_IsWaterFunc = NULL;
}
static void DrawFieldBridge(int drawX, int drawY, int fMapX, int fMapY, int mcId, int mcOrigX, int mcOrigY, int (*funcIsBridge)(int, int), int tateMode)
{
	DFW_IsWaterFunc = funcIsBridge;

	int tX1, tY1;
	int tX3, tY3;
	int tX7, tY7;
	int tX9, tY9;

	tX7 = 0;
	tY7 = 0;

	if(tateMode)
	{
		tX1 = 0;
		tY1 = 5;

		if(DFW_IsWater(fMapX, fMapY - 1))
			tY7 += 2;

		if(DFW_IsWater(fMapX, fMapY + 1))
			tY1 -= 2;

		tX9 = 1;
		tY9 = tY7;
		tX3 = 1;
		tY3 = tY1;
	}
	else
	{
		tX9 = 5;
		tY9 = 0;

		if(DFW_IsWater(fMapX - 1, fMapY))
			tX7 += 2;

		if(DFW_IsWater(fMapX + 1, fMapY))
			tX9 -= 2;

		tX1 = tX7;
		tY1 = 1;
		tX3 = tX9;
		tY3 = 1;
	}

	DrawSimple(GetSFPicId(mcId, mcOrigX + tX1, mcOrigY + tY1, 1), drawX + 0, drawY + 16);
	DrawSimple(GetSFPicId(mcId, mcOrigX + tX3, mcOrigY + tY3, 1), drawX + 16, drawY + 16);
	DrawSimple(GetSFPicId(mcId, mcOrigX + tX7, mcOrigY + tY7, 1), drawX + 0, drawY + 0);
	DrawSimple(GetSFPicId(mcId, mcOrigX + tX9, mcOrigY + tY9, 1), drawX + 16, drawY + 0);

	DFW_IsWaterFunc = NULL;
}
static int DFC_IsWater(int x, int y)
{
	FMCell_t *cell = FDc.FMap->GetCell(x, y);

	return
		cell->SFId.Ground == SF_WATER ||
		cell->SFId.Ground == SF_HASHI_TATE ||
		cell->SFId.Ground == SF_HASHI_YOKO;
}
static int DFC_IsShiba(int x, int y)
{
	return FDc.FMap->GetCell(x, y)->SFId.Ground == SF_SHIBA;
}
static int DFC_IsSand(int x, int y)
{
	return FDc.FMap->GetCell(x, y)->SFId.Ground == SF_SAND;
}
static int DFC_IsSoil(int x, int y)
{
	FMCell_t *cell = FDc.FMap->GetCell(x, y);

	return
		cell->SFId.Ground == SF_SOIL ||
		cell->SFId.Ground == SF_HASHI_TATE ||
		cell->SFId.Ground == SF_HASHI_YOKO;
}
static int DFC_IsHanabatake(int x, int y)
{
	return FDc.FMap->GetCell(x, y)->SFId.Ground == SF_HANABATAKE;
}
static int DFC_IsIshidatami(int x, int y)
{
	return FDc.FMap->GetCell(x, y)->SFId.Ground == SF_ISHIDATAMI;
}
static int DFC_IsGreenSaku(int x, int y)
{
	return FDc.FMap->GetCell(x, y)->SFId.Ground == SF_GREEN_SAKU;
}
static int DFC_IsSaku(int x, int y)
{
	return FDc.FMap->GetCell(x, y)->SFId.Surface == SF_SAKU;
}
static int DFC_IsShimettaGreen(int x, int y)
{
	return FDc.FMap->GetCell(x, y)->SFId.Ground == SF_SHIMETTA_GREEN;
}
static int DFC_IsHashiTate(int x, int y)
{
	return FDc.FMap->GetCell(x, y)->SFId.Ground == SF_HASHI_TATE;
}
static int DFC_IsHashiYoko(int x, int y)
{
	return FDc.FMap->GetCell(x, y)->SFId.Ground == SF_HASHI_YOKO;
}

static int DFC_DrawX;
static int DFC_DrawY;
static FMCell_t *DFC_Cell;
static int DFC_FMapX;
static int DFC_FMapY;

void DrawFieldCell_Ground(void);
void DrawFieldCell_Surface(void);
void DrawFieldCell_Extra(void);

void DrawFieldCell(int drawX, int drawY, FMCell_t *cell, int fMapX, int fMapY)
{
	DFC_DrawX = drawX;
	DFC_DrawY = drawY;
	DFC_Cell = cell;
	DFC_FMapX = fMapX;
	DFC_FMapY = fMapY;

	switch(cell->SFId.Ground)
	{
	case SF_EMPTY:
		error();

	case SF_GREEN:
		DrawSimple(GetSFPicId(MC_TILE_A2, 0, 0, 0), drawX, drawY);
		break;

	case SF_WATER:
		DrawFieldWater(drawX, drawY, fMapX, fMapY, MC_TILE_A1, 16, 0, DFC_IsWater, 1);
		break;

	case SF_SHIBA:
		DrawFieldWater(drawX, drawY, fMapX, fMapY, MC_TILE_A2, 4, 6, DFC_IsShiba, 0);
		break;

	case SF_SAND:
		DrawFieldWater(drawX, drawY, fMapX, fMapY, MC_TILE_A2, 12, 0, DFC_IsSand, 0);
		break;

	case SF_SOIL:
		DrawFieldWater(drawX, drawY, fMapX, fMapY, MC_TILE_A2, 8, 6, DFC_IsSoil, 0);
		break;

	case SF_HANABATAKE:
		DrawFieldWater(drawX, drawY, fMapX, fMapY, MC_TILE_A2, 0, 6, DFC_IsHanabatake, 0);
		break;

	case SF_ISHIDATAMI:
		DrawFieldWater(drawX, drawY, fMapX, fMapY, MC_TILE_A2, 8, 0, DFC_IsIshidatami, 0);
		break;

	case SF_GREEN_SAKU:
		DrawFieldWater(drawX, drawY, fMapX, fMapY, MC_TILE_A2, 4, 0, DFC_IsGreenSaku, 0);
		break;

	case SF_SHIMETTA_GREEN:
		DrawFieldWater(drawX, drawY, fMapX, fMapY, MC_TILE_A2, 0, 12, DFC_IsShimettaGreen, 0);
		break;

	case SF_HASHI_TATE:
		DrawFieldBridge(drawX, drawY, fMapX, fMapY, MC_TILE_B, 12, 0, DFC_IsHashiTate, 1);
		break;

	case SF_HASHI_YOKO:
		DrawFieldBridge(drawX, drawY, fMapX, fMapY, MC_TILE_B, 6, 4, DFC_IsHashiYoko, 0);
		break;

	default:
		DrawFieldCell_Ground();
		break;
	}

	switch(cell->SFId.Surface)
	{
	case SF_EMPTY:
		break;

	case SF_WOOD_1:
		DrawSimple(GetSFPicId(MC_TILE_B, 12, 10, 0), drawX, drawY);
		break;

	case SF_WOOD_3:
		DrawSimple(GetSFPicId(MC_TILE_B, 14, 10, 0), drawX, drawY);
		break;

	case SF_WOOD_7:
		DrawSimple(GetSFPicId(MC_TILE_B, 12, 8, 0), drawX, drawY);
		break;

	case SF_WOOD_9:
		DrawSimple(GetSFPicId(MC_TILE_B, 14, 8, 0), drawX, drawY);
		break;

	case SF_WOOD_19:
		DrawSimple(GetSFPicId(MC_TILE_B, 12, 12, 0), drawX, drawY);
		break;

	case SF_WOOD_37:
		DrawSimple(GetSFPicId(MC_TILE_B, 14, 12, 0), drawX, drawY);
		break;

	case SF_SHRUB:
		DrawSimple(GetSFPicId(MC_TILE_B, 8, 12, 0), drawX, drawY);
		break;

	case SF_SHRUB_FLOWER:
		DrawSimple(GetSFPicId(MC_TILE_B, 6, 12, 0), drawX, drawY);
		break;

	case SF_TAKARA_APPLE:
		DrawSimple(GetSFPicId(MC_TILE_B, 20, 4, 0), drawX, drawY);
		break;

	case SF_TAKARA_POTATE:
		DrawSimple(GetSFPicId(MC_TILE_B, 16, 6, 0), drawX, drawY);
		break;

	case SF_TAKARA_GOLD:
		DrawSimple(GetSFPicId(MC_TILE_E, 2, 26, 0), drawX, drawY);
		break;

	case SF_TAKARA_LEAF:
		DrawSimple(GetSFPicId(MC_TILE_B, 20, 6, 0), drawX, drawY);
		break;

	case SF_EMPTY_TAKARA:
		DrawSimple(GetSFPicId(MC_TILE_B, 18, 4, 0), drawX, drawY);
		break;

	case SF_EMPTY_TAKARA_GOLD:
		DrawSimple(GetSFPicId(MC_TILE_D, 16, 22, 0), drawX, drawY);
		break;

	case SF_SABOTEN_01:
		DrawSimple(GetSFPicId(MC_TILE_B, 10, 26, 0), drawX, drawY);
		break;

	case SF_SABOTEN_02:
		DrawSimple(GetSFPicId(MC_TILE_B, 12, 26, 0), drawX, drawY);
		break;

	case SF_SABOTEN_03:
		DrawSimple(GetSFPicId(MC_TILE_B, 14, 26, 0), drawX, drawY);
		break;

	case SF_KUSA_01:
		DrawSimple(GetSFPicId(MC_TILE_B, 0, 8, 0), drawX, drawY);
		break;

	case SF_KUSA_02:
		DrawSimple(GetSFPicId(MC_TILE_B, 2, 8, 0), drawX, drawY);
		break;

	case SF_KUSA_03:
		DrawSimple(GetSFPicId(MC_TILE_B, 4, 8, 0), drawX, drawY);
		break;

	case SF_KUSA_04:
		DrawSimple(GetSFPicId(MC_TILE_B, 6, 8, 0), drawX, drawY);
		break;

	case SF_FLOWER_01:
		DrawSimple(GetSFPicId(MC_TILE_B, 8, 8, 0), drawX, drawY);
		break;

	case SF_FLOWER_02:
		DrawSimple(GetSFPicId(MC_TILE_B, 10, 8, 0), drawX, drawY);
		break;

	case SF_ISHI:
		DrawSimple(GetSFPicId(MC_TILE_B, 10, 18, 0), drawX, drawY);
		break;

	case SF_KUSA_05:
		DrawSimple(GetSFPicId(MC_TILE_B, 10, 30, 0), drawX, drawY);
		break;

	case SF_SAKU:
		DrawFieldWater(drawX, drawY, fMapX, fMapY, MC_TILE_A2, 16, 0, DFC_IsSaku, 0);
		break;

	case SF_IWA:
		DrawSimple(GetSFPicId(MC_TILE_B, 8, 18, 0), drawX, drawY);
		break;

	default:
		DrawFieldCell_Surface();
		break;
	}

	DrawFieldCell_Extra();
}
static void DrawFieldCell_Ground(void)
{
	switch(DFC_Cell->SFId.Ground)
	{
	case 0:
	default:
		DPE_SetBright(0.0, 0.5, 1.0);
		DrawRect(P_WHITEBOX, DFC_DrawX, DFC_DrawY, 32, 32);
		DPE_Reset();
		break;
	}
}
static void DrawFieldCell_Surface(void)
{
	switch(DFC_Cell->SFId.Surface)
	{
	case SF_YASAI_01:
		DrawSimple(GetSFPicId(MC_TILE_B, 12, 28, 0), DFC_DrawX, DFC_DrawY);
		break;

	case SF_YASAI_02:
		DrawSimple(GetSFPicId(MC_TILE_B, 14, 28, 0), DFC_DrawX, DFC_DrawY);
		break;

	case SF_YASAI_03:
		DrawSimple(GetSFPicId(MC_TILE_B, 12, 30, 0), DFC_DrawX, DFC_DrawY);
		break;

	case SF_YASAI_04:
		DrawSimple(GetSFPicId(MC_TILE_B, 14, 30, 0), DFC_DrawX, DFC_DrawY);
		break;

	case SF_KIRIKABU_01:
		DrawSimple(GetSFPicId(MC_TILE_B, 4, 10, 0), DFC_DrawX, DFC_DrawY);
		break;

	case SF_KIRIKABU_02:
		DrawSimple(GetSFPicId(MC_TILE_B, 6, 10, 0), DFC_DrawX, DFC_DrawY);
		break;

	case SF_KIRIKABU_03:
		DrawSimple(GetSFPicId(MC_TILE_B, 8, 10, 0), DFC_DrawX, DFC_DrawY);
		break;

	case SF_KIRIKABU_04:
		DrawSimple(GetSFPicId(MC_TILE_B, 10, 10, 0), DFC_DrawX, DFC_DrawY);
		break;

	case SF_TATEFUDA_01:
		DrawSimple(GetSFPicId(MC_TILE_B, 0, 10, 0), DFC_DrawX, DFC_DrawY);
		break;

	case SF_TATEFUDA_02:
		DrawSimple(GetSFPicId(MC_TILE_B, 2, 10, 0), DFC_DrawX, DFC_DrawY);
		break;

	case SF_TATEFUDA_03:
		DrawSimple(GetSFPicId(MC_TILE_B, 0, 12, 0), DFC_DrawX, DFC_DrawY);
		break;

	case SF_TATEFUDA_04:
		DrawSimple(GetSFPicId(MC_TILE_B, 2, 12, 0), DFC_DrawX, DFC_DrawY);
		break;

	case SF_KAKASHI:
		DPE.TL = FDc.DrawFront;
		DrawSimple(GetSFPicId(MC_TILE_B, 2, 26, 0), DFC_DrawX, DFC_DrawY - 32);
		DrawSimple(GetSFPicId(MC_TILE_B, 2, 28, 0), DFC_DrawX, DFC_DrawY);
		DPE.TL = NULL;
		break;

	case SF_HASHIRA:
		DrawSimple(GetSFPicId(MC_TILE_E, 22, 20, 0), DFC_DrawX, DFC_DrawY);
		break;

	case SF_HASHIRA_KOKE:
		DrawSimple(GetSFPicId(MC_TILE_E, 22, 18, 0), DFC_DrawX, DFC_DrawY);
		break;

	case SF_HASHIRA_LONG:
		DPE.TL = FDc.DrawFront;
		DrawSimple(GetSFPicId(MC_TILE_E, 24, 18, 0), DFC_DrawX, DFC_DrawY - 32);
		DrawSimple(GetSFPicId(MC_TILE_E, 24, 20, 0), DFC_DrawX, DFC_DrawY);
		DPE.TL = NULL;
		break;

	default:
		DPE_SetBright(1.0, 0.5, 0.0);
		DrawRect(P_WHITEBOX, DFC_DrawX + 6 + eRnd() * 6, DFC_DrawY + 6 + eRnd() * 6, 20, 20);
		DPE_Reset();
		break;
	}
}
static void DrawFieldCell_Extra(void)
{
	switch(DFC_Cell->SFId.Extra)
	{
	case SF_EMPTY:
		break;

	case SF_MONSTER_00 + 0:
	case SF_MONSTER_00 + 1:
	case SF_MONSTER_00 + 2:
	case SF_MONSTER_00 + 3:
	case SF_MONSTER_00 + 4:
	case SF_MONSTER_00 + 5:
	case SF_MONSTER_00 + 6:
	case SF_MONSTER_00 + 7:
		AutoCreateEnemy(DFC_Cell->SFId.Extra, DFC_FMapX, DFC_FMapY);
		break;

	default:
		DPE_SetBright(0.9, 0.8, 0.0);
		DrawRect(P_WHITEBOX, DFC_DrawX + 6 + eRnd() * 6, DFC_DrawY + 6 + eRnd() * 6, 20, 20);
		DPE_Reset();
		break;
	}
}
