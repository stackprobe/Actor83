#include "all.h"

// ---- Še“G¶¬ ----

static void DoCreateEnemy_Monster(int kind, int animeAlways)
{
	int color;

	     if(dRnd() < 0.6) color = 0;
	else if(dRnd() < 0.6) color = 1;
	else if(dRnd() < 0.6) color = 2;
	else if(dRnd() < 0.6) color = 3;
	else                  color = 4;

	AddEnemy(E_Monster_EachFrame, E_Monster_Release, E_Monster_Create(color, kind, animeAlways));
}
static void DoCreateEnemy(void)
{
	switch(Enemy_SFId)
	{
	case SF_MONSTER_00 + 0: DoCreateEnemy_Monster(0, 0); break;
	case SF_MONSTER_00 + 1: DoCreateEnemy_Monster(1, 1); break;
	case SF_MONSTER_00 + 2: DoCreateEnemy_Monster(2, 0); break;
	case SF_MONSTER_00 + 3: DoCreateEnemy_Monster(3, 0); break;
	case SF_MONSTER_00 + 4: DoCreateEnemy_Monster(4, 1); break;
	case SF_MONSTER_00 + 5: DoCreateEnemy_Monster(5, 0); break;
	case SF_MONSTER_00 + 6: DoCreateEnemy_Monster(6, 0); break;
	case SF_MONSTER_00 + 7: DoCreateEnemy_Monster(7, 0); break;

	default:
		error();
	}
}

// ----

static int IsAliveEnemyByOrigin(int originX, int originY)
{
	for(int index = 0; index < FDc.EnemyList->GetCount(); index++)
	{
		Enemy_t *e = FDc.EnemyList->GetElement(index);

		if(e->Origin.X == originX && e->Origin.Y == originY)
		{
			return 1;
		}
	}
	return 0;
}
void AutoCreateEnemy(int sfId, int fMapX, int fMapY) // sfId: SF_EXTRA_*
{
	if(100 < FDc.EnemyList->GetCount()) // ? “G‘å™
		return;

	if(IsAliveEnemyByOrigin(fMapX, fMapY))
		return;

	Enemy_SFId = sfId;
	Enemy_FMapX = fMapX;
	Enemy_FMapY = fMapY;

	DoCreateEnemy();
}
