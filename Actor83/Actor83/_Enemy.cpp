#include "all.h"

Enemy_t *CreateEnemy(void)
{
	Enemy_t *e = nb(Enemy_t);

	e->Frame = -1;

	return e;
}
void ReleaseEnemy(Enemy_t *e)
{
	if(!e)
		return;

	e->Release(e->ExtraData);
	memFree(e);
}

// <-- cdtor

// <-- accessor

int Enemy_SFId = -1;
int Enemy_FMapX = -1;
int Enemy_FMapY = -1;

void AddEnemy_ved(int (*eachFrame)(Enemy_t *, void *), void (*release)(void *), void *extraData)
{
	Enemy_t *e = CreateEnemy();

	e->Origin.X = Enemy_FMapX;
	e->Origin.Y = Enemy_FMapY;

	e->X = Enemy_FMapX * 32 + 16;
	e->Y = Enemy_FMapY * 32 + 16;

	e->EachFrame = eachFrame;
	e->Release = release;
	e->ExtraData = extraData;

	FDc.EnemyList->AddElement(e);
}
void DrawEnemyBefore(void)
{
	DPE.TL = FDc.DrawEnemy;
	DPE.IntPosOn = 1;
}
void DrawEnemyAfter(void)
{
	DPE.TL = NULL;
}
void EachFrameAllEnemy(void)
{
	for(int index = 0; index < FDc.EnemyList->GetCount(); index++)
	{
		Enemy_t *e = FDc.EnemyList->GetElement(index);

		e->Frame++; // 0～

		if(e->Frame == 0) // init
		{
			double INNER_MARGIN = 20.0;
			double dr_x = e->X - FDc.CameraX;
			double dr_y = e->Y - FDc.CameraY;

			/*
				画面内から登場 -> エフェクト出す。
			*/
			if(
				INNER_MARGIN < dr_x && dr_x < SCREEN_W - INNER_MARGIN &&
				INNER_MARGIN < dr_y && dr_y < SCREEN_H - INNER_MARGIN
				)
			{
				// TODO 作成時のエフェクトと重なる。重なっても変じゃないように！
				/*
				CEE.LastPicId = D_MAGICSTAR_00_LAST | DTP;
				AddCommonEffect(Gnd.EL, 0, D_MAGICSTAR_00 | DTP, e->X, e->Y);
				CEE_Reset();
				*/
			}
		}

		if(!e->EachFrame(e, e->ExtraData)) // ? 死亡
		{
			ReleaseEnemy(e);
			FDc.EnemyList->FastDesertElement(index);
			index--;
		}
	}
}

/*
	color: 0 - (MONSTER_COLOR_MAX - 1)
	kind: 0 - (MONSTER_MAX - 1)
	dir: 2468
	koma: 0 - 3
*/
int GetEnemyPicId(int color, int kind, int dir, int koma)
{
	errorCase(!m_isRange(color, 0, MONSTER_COLOR_MAX - 1));
	errorCase(!m_isRange(kind, 0, MONSTER_MAX - 1));
	// dir
	errorCase(!m_isRange(koma, 0, 3));

	int row;

	switch(dir)
	{
	case 8: row = 0; break;
	case 6: row = 1; break;
	case 2: row = 2; break;
	case 4: row = 3; break;

	default:
		error();
	}
	row += (kind / 4) * 4;
	int col = (kind % 4) * 3;

	if(koma == 3)
		koma = 1;

	return D_MONSTER_00 + color * 12 * 8 + row * 12 + col + koma | DTP;
}
int IsExistEnemy(int fldX, int fldY)
{
	for(int index = 0; index < FDc.EnemyList->GetCount(); index++)
	{
		Enemy_t *e = FDc.EnemyList->GetElement(index);

		int eFldX = (int)(e->X / 32.0);
		int eFldY = (int)(e->Y / 32.0);

		if(eFldX == fldX && eFldY == fldY)
			return 1;
	}
	return 0;
}

// ---- repulsion ----

void ClearRepulsion(void)
{
	FDc.RepulsionList->Clear();
	FDc.RepulsionTellList->Clear();
}
void Repulsion(double &me_x, double &me_y, int *tell)
{
	const double RANGE = 32.0;

	for(int index = 0; index * 2 < FDc.RepulsionList->GetCount(); index++)
	{
		double os_x = FDc.RepulsionList->GetElement(index * 2 + 0);
		double os_y = FDc.RepulsionList->GetElement(index * 2 + 1);
		int *os_tell = FDc.RepulsionTellList->GetElement(index);

		double dx = me_x - os_x;
		double dy = me_y - os_y;
		double dP2 = dx * dx + dy * dy;

		if(dP2 < RANGE * RANGE)
		{
			double rme_x = me_x - os_x;
			double rme_y = me_y - os_y;
			double rme_d = sqrt(rme_x * rme_x + rme_y * rme_y);

			if(rme_d < D_MICRO)
			{
				const double R = 2.0;

				me_x = eRnd() * R;
				me_y = eRnd() * R;
			}
			else
			{
				rme_x *= RANGE / rme_d;
				rme_y *= RANGE / rme_d;

				me_x = os_x + rme_x;
				me_y = os_y + rme_y;
			}
			if(os_tell)
				*os_tell = 1;
		}
	}

	FDc.RepulsionList->AddElement(me_x);
	FDc.RepulsionList->AddElement(me_y);
	FDc.RepulsionTellList->AddElement(tell);
}

// ---- crashed ----

void EnemyWeaponCrashed(Enemy_t *e)
{
	SEPlay(SE_ENEMY_DEATH);

	AddCommonEffect(
		Gnd.EL,
		0,
		D_WEAPON_00 + 1 | DTP,
		e->X,
		e->Y,
		eRnd() * 0.3,
		1.5,
		0.9,
		0.0,
		-2.0,
		eRnd() * 0.1,
		0.0,
		-0.1
		);

	int fldX = e->Origin.X;
	int fldY = e->Origin.Y;

	FMCell_t *cell = GetFMCell(fldX, fldY);

	if(cell)
	{
		cell->SFId.Extra = SF_EXTRA_0;

		MapChanged(GDc.MapX, GDc.MapY, fldX, fldY, cell);
	}
}

// ----
