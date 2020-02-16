#include "all.h"

// ---- cdtor ----

FMCell_t *CreateFMCell(void)
{
	FMCell_t *i = nb_(FMCell_t);

	i->SFId.Ground = SF_GREEN;

	return i;
}
void ReleaseFMCell(FMCell_t *i)
{
	memFree(i);
}

// ---- ToBlock, FromBlock ----

void FMCellToBlock(FMCell_t *cell, uchar data[8])
{
	data[0] = cell->SFId.Ground % 256;
	data[1] = cell->SFId.Ground / 256;
	data[2] = cell->SFId.Surface % 256;
	data[3] = cell->SFId.Surface / 256;
	data[4] = cell->SFId.Extra % 256;
	data[5] = cell->SFId.Extra / 256;
	data[6] = m_01(cell->IsWall);
	data[7] = 0;
}
void FMCellFromBlock(FMCell_t *cell, uchar data[8])
{
	cell->SFId.Ground  = data[0] + data[1] * 256;
	cell->SFId.Surface = data[2] + data[3] * 256;
	cell->SFId.Extra   = data[4] + data[5] * 256;
	cell->IsWall       = data[6] & 1;
}

// ----

int GetSFPicId(int mcId, int x, int y, int miniMode)
{
	errorCase(mcId < 0 || MC_MAX <= mcId);
	errorCase(x < 0 || MAPCHIP_X_MAX <= x);
	errorCase(y < 0 || MAPCHIP_X_MAX <= y);

	return
		(miniMode ? D_MAPCHIPMINI_00 : D_MAPCHIP_00) +
		mcId * MAPCHIP_X_MAX * MAPCHIP_Y_MAX +
		y * MAPCHIP_X_MAX +
		x | DTP;
}
int IsTakaraSFId(int sfId)
{
	switch(sfId)
	{
	case SF_TAKARA_APPLE:
	case SF_TAKARA_POTATE:
	case SF_TAKARA_GOLD:
	case SF_TAKARA_LEAF:
		return 1;
	}
	return 0;
}

int IsWoodCell(FMCell_t *cell) // ret: ? –Ø_’á–Ø
{
	switch(cell->SFId.Surface)
	{
	case SF_WOOD_1:
	case SF_WOOD_19:
	case SF_WOOD_3:
	case SF_WOOD_37:
	case SF_WOOD_7:
	case SF_WOOD_9:
	case SF_SHRUB:
	case SF_SHRUB_FLOWER:
		return 1;
	}
	return 0;
}
int IsTakaraCell(FMCell_t *cell) // ret: ? ” _‹ó” 
{
	switch(cell->SFId.Surface)
	{
	case SF_TAKARA_APPLE:
	case SF_TAKARA_POTATE:
	case SF_TAKARA_GOLD:
	case SF_TAKARA_LEAF:
	case SF_EMPTY_TAKARA:
	case SF_EMPTY_TAKARA_GOLD:
		return 1;
	}
	return 0;
}
int IsEditableCell(FMCell_t *cell)
{
	return !IsWoodCell(cell) && !IsTakaraCell(cell);
}
