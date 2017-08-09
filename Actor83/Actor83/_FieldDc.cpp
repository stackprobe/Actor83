#include "all.h"

FDc_t FDc;

void FieldInit(void)
{
	memset(&FDc, 0x00, sizeof(FDc));

	FDc.FMap = new autoTable<FMCell_t *>(CreateFMCell, ReleaseFMCell);
	FDc.Field_W = 32;
	FDc.Field_H = 32;

	FDc.Player.Action.EFId = rnd(EF_MAX); // 開始アクションをランダムに

	FDc.DrawFront = new taskList();
	FDc.DrawEnemy = new taskList();
	FDc.EnemyList = new autoList<Enemy_t *>();
	FDc.RepulsionList = new autoList<double>();
	FDc.WeaponList = new autoList<Weapon_t *>();
}
void FieldFnlz(void)
{
	delete FDc.Rnd; // fnlz only
	delete FDc.FMap;

	delete FDc.DrawFront;
	delete FDc.DrawEnemy;
	releaseList(FDc.EnemyList, ReleaseEnemy);
	delete FDc.RepulsionList;
	releaseList(FDc.WeaponList, ReleaseWeapon);

	memset(&FDc, 0x00, sizeof(FDc));
}

bitList *GetTakaraGotList(void)
{
	return GDc.TakaraGotTable->GetCell(GDc.MapX, GDc.MapY);
}
int GetTakaraIndex(int x, int y)
{
	for(int index = 0; index < FDc.TakaraPosCount; index++)
	{
		i2D_t *tPos = FDc.TakaraPosList + index;

		if(tPos->X == x && tPos->Y == y)
		{
			return index;
		}
	}
	error(); // not found
	return -1; // dummy
}
void OpenTakara(int x, int y)
{
//	errorCase(GetTakaraGotList()->RefBit(GetTakaraIndex(x, y))); // ? already opened

	GetTakaraGotList()->PutBit(GetTakaraIndex(x, y), 1);

	m_countDown(FDc.LocalTakaraRemainCount);
	m_countDown(GDc.GlobalTakaraRemainCount);
}
int GetEmptyTakaraSFId(int sfId)
{
	return sfId == SF_TAKARA_GOLD ?
		SF_EMPTY_TAKARA_GOLD :
		SF_EMPTY_TAKARA;
}

void UpdateLocalTakaraCount(void)
{
	bitList *tgl = GetTakaraGotList();
	int gotnum = 0;

	for(int index = 0; index < FDc.TakaraPosCount; index++)
		if(tgl->RefBit(index))
			gotnum++;

	FDc.LocalTakaraRemainCount = FDc.TakaraPosCount - gotnum;
	FDc.LocalTakaraTotalCount = FDc.TakaraPosCount;
}
void UpdateGlobalTakaraCount(void) // for GDc!?
{
	int total = 0;
	int gotnum = 0;

	for(int x = 0; x < MAP_W; x++)
	for(int y = 0; y < MAP_H; y++)
	{
		bitList *tgl = GDc.TakaraGotTable->GetCell(x, y);
		int count = GetTakaraPosCount(x, y);

		for(int index = 0; index < count; index++)
			if(tgl->RefBit(index))
				gotnum++;

		total += count;
	}

	GDc.GlobalTakaraRemainCount = total - gotnum;
	GDc.GlobalTakaraTotalCount = total;
}

FMCell_t *GetFMCell(int x, int y) // ret: NULL == セル無し
{
	return FDc.FMap->RefCell(x, y, NULL);
}
int IsWallOfWeapon(FMCell_t *cell)
{
	errorCase(!cell);
	return cell->IsWall && cell->SFId.Ground != SF_WATER;
}
