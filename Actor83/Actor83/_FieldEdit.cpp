#include "all.h"

static int EFId;
static int FieldX;
static int FieldY;
static FMCell_t *Cell;

static int Edit(int wallFlg = 0, int ground = SF_GREEN, int surface = SF_EMPTY, int extra = SF_EMPTY) // ret: 1
{
	errorCase(!m_isRange(ground,  -1, SF_GROUND_END - 1));
	errorCase(!m_isRange(surface, -1, SF_SURFACE_END - 1));
	errorCase(!m_isRange(extra,   -1, SF_EXTRA_END - 1));

	if(ground != -1)  Cell->SFId.Ground = ground;
	if(surface != -1) Cell->SFId.Surface = surface;
	if(extra != -1)   Cell->SFId.Extra = extra;
	if(wallFlg != -1) Cell->IsWall = m_01(wallFlg);

	MapChanged(GDc.MapX, GDc.MapY, FieldX, FieldY, Cell);

	return 1;
}
static int Edit_D(int wallFlg = -1, int ground = -1, int surface = -1, int extra = -1) // ret: 1
{
	return Edit(wallFlg, ground, surface, extra);
}
static int EF_Main(void) // ret: ? edited
{
	if(Cell->SFId.Ground == SF_WATER) // …ˆæƒIƒ“ƒŠ[
	{
		switch(EFId)
		{
		case EF_HASHI_TATE: return Edit(0, SF_HASHI_TATE);
		case EF_HASHI_YOKO: return Edit(0, SF_HASHI_YOKO);
		}
	}
	if(!IsEditableCell(Cell))
	{
		switch(EFId)
		{
		case EF_GREEN: return Edit_D(-1, SF_GREEN);
		case EF_SHIBA: return Edit_D(-1, SF_SHIBA);
		case EF_SAND: return Edit_D(-1, SF_SAND);
		case EF_SOIL: return Edit_D(-1, SF_SOIL);
		}
		return 0; // not edited
	}

	{
		switch(EFId)
		{
		case EF_NONE: return Edit(0, SF_SHIMETTA_GREEN);
		case EF_GREEN: return Edit();
		case EF_SHIBA: return Edit(0, SF_SHIBA);
		case EF_WATER: return Edit(1, SF_WATER);
		case EF_SAND: return Edit(0, SF_SAND);
		case EF_SOIL: return Edit(0, SF_SOIL);
		case EF_HANABATAKE: return Edit(0, SF_HANABATAKE);
		case EF_ISHIDATAMI: return Edit(0, SF_ISHIDATAMI);
		case EF_SAKU_01: return Edit(1, SF_GREEN_SAKU);
		}
	}

	if(!Cell->IsWall)
	{
		switch(EFId)
		{
		case EF_KUSA_01: return Edit(0, -1, SF_KUSA_01);
		case EF_KUSA_02: return Edit(0, -1, SF_KUSA_02);
		case EF_KUSA_03: return Edit(0, -1, SF_KUSA_03);
		case EF_KUSA_04: return Edit(0, -1, SF_KUSA_04);
		case EF_KUSA_05: return Edit(0, -1, SF_KUSA_05);
		case EF_FLOWER_01: return Edit(0, -1, SF_FLOWER_01);
		case EF_FLOWER_02: return Edit(0, -1, SF_FLOWER_02);
		case EF_ISHI:
			switch(rnd(10))
			{
			case 0: return Edit(1, -1, SF_IWA);
			default: return Edit(0, -1, SF_ISHI);
			}
		case EF_SAKU_02: return Edit(1, -1, SF_SAKU);
		case EF_SABOTEN:
			switch(rnd(3))
			{
			case 0: return Edit(1, -1, SF_SABOTEN_01);
			case 1: return Edit(1, -1, SF_SABOTEN_02);
			case 2: return Edit(1, -1, SF_SABOTEN_03);
			}
		case EF_YASAI:
			switch(rnd(4))
			{
			case 0: return Edit(0, -1, SF_YASAI_01);
			case 1: return Edit(0, -1, SF_YASAI_02);
			case 2: return Edit(0, -1, SF_YASAI_03);
			case 3: return Edit(0, -1, SF_YASAI_04);
			}
		case EF_KIRIKABU:
			switch(rnd(5) ? 1 : rnd(4))
			{
			case 0: return Edit(1, -1, SF_KIRIKABU_01);
			case 1: return Edit(1, -1, SF_KIRIKABU_02);
			case 2: return Edit(1, -1, SF_KIRIKABU_03);
			case 3: return Edit(1, -1, SF_KIRIKABU_04);
			}
		case EF_TATEFUDA:
			switch(rnd(5) ? 1 : rnd(4))
			{
			case 0: return Edit(1, -1, SF_TATEFUDA_01);
			case 1: return Edit(1, -1, SF_TATEFUDA_02);
			case 2: return Edit(1, -1, SF_TATEFUDA_03);
			case 3: return Edit(1, -1, SF_TATEFUDA_04);
			}
		case EF_KAKASHI: return Edit(1, -1, SF_KAKASHI);
		case EF_MONSTER_00 + 0: return Edit(-1, -1, -1, SF_MONSTER_00 + 0);
		case EF_MONSTER_00 + 1: return Edit(-1, -1, -1, SF_MONSTER_00 + 1);
		case EF_MONSTER_00 + 2: return Edit(-1, -1, -1, SF_MONSTER_00 + 2);
		case EF_MONSTER_00 + 3: return Edit(-1, -1, -1, SF_MONSTER_00 + 3);
		case EF_MONSTER_00 + 4: return Edit(-1, -1, -1, SF_MONSTER_00 + 4);
		case EF_MONSTER_00 + 5: return Edit(-1, -1, -1, SF_MONSTER_00 + 5);
		case EF_MONSTER_00 + 6: return Edit(-1, -1, -1, SF_MONSTER_00 + 6);
		case EF_MONSTER_00 + 7: return Edit(-1, -1, -1, SF_MONSTER_00 + 7);
		case EF_HASHIRA:
			{
			if(rnd(3)) return Edit(1, -1, SF_HASHIRA);
			if(rnd(3)) return Edit(1, -1, SF_HASHIRA_KOKE);
			           return Edit(1, -1, SF_HASHIRA_LONG);
			}
		}
	}
	return 0; // not edited
}
static int ShowWeapon(int efId) // ret: ? •Ší‚¾‚Á‚½B
{
	switch(efId)
	{
	case EF_WEAPON_01:
		FDc.WeaponList->AddElement(CreateWeapon(FDc.Player.X, FDc.Player.Y, FDc.Player.Dir));
		SEPlay(SE_SHOT_WEAPON);
		return 1;
	}
	return 0;
}
int EditField(int efId, int fldX, int fldY) // ret: ? edited
{
	int edited = 0;

	if(ShowWeapon(efId))
		goto endFunc;

	if(
		m_isRange(fldX, 0, FDc.FMap->GetWidth() - 1) &&
		m_isRange(fldY, 0, FDc.FMap->GetHeight() - 1)
		)
	{
		EFId = efId;
		FieldX = fldX;
		FieldY = fldY;
		Cell = FDc.FMap->GetCell(fldX, fldY);

		edited = EF_Main();
	}
endFunc:
	return edited;
}
