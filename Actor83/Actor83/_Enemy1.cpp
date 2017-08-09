#include "all.h"

// ---- E_Monster ----

E_Monster_t *E_Monster_Create(int color, int kind, int animeAlways)
{
	E_Monster_t *ed = nb(E_Monster_t);

	ed->Color = color;
	ed->Kind = kind;
	ed->AnimeAlways = animeAlways;

	return ed;
}
void E_Monster_Release(E_Monster_t *ed)
{
	memFree(ed);
}
int E_Monster_EachFrame(Enemy_t *e, E_Monster_t *ed)
{
	if(e->Frame == 0) // init
	{
		ed->Dir = 2;
		ed->TrueFMapX = e->Origin.X;
		ed->TrueFMapY = e->Origin.Y;
		ed->TrueX = e->X;
		ed->TrueY = e->Y;
	}

	if(e->WeaponCrashed)
	{
		EnemyWeaponCrashed(e);
		return 0;
	}

	double MV_RATE = 0.01;
	double MV_SPEED = 0.5;

	switch(ed->Kind)
	{
	case 0: // スラ
		break;

	case 1: // 蜂
		MV_RATE = 0.03;
		MV_SPEED = 1.0;
		break;

	case 2: // モ5
		MV_RATE = 0.025;
		MV_SPEED = 1.5;
		break;

	case 3: // モ7
		MV_RATE = 0.03;
		MV_SPEED = 1.6;
		break;

	case 4: // 火
		MV_RATE = 0.02;
		MV_SPEED = 0.4;
		break;

	case 5: // 蛇
		MV_RATE = 0.015;
		MV_SPEED = 0.5;
		break;

	case 6: // モ6
		MV_RATE = 0.035;
		MV_SPEED = 1.7;
		break;

	case 7: // モ8
		MV_RATE = 0.015;
		MV_SPEED = 1.8;
		break;

	default:
		error();
	}

	if(dRnd() < MV_RATE)
	{
		int moved = 0;

		int x = ed->TrueFMapX;
		int y = ed->TrueFMapY;

		switch(rnd(4))
		{
		case 0: x++; break;
		case 1: x--; break;
		case 2: y++; break;
		case 3: y--; break;

		default:
			error();
		}

		if(!IsCommonEnemyMovable(x, y)) // ? 移動可能な場所ではない。
			goto moveCancelled;

		ed->TrueFMapX = x;
		ed->TrueFMapY = y;

		ed->TrueX = ed->TrueFMapX * 32 + 16;
		ed->TrueY = ed->TrueFMapY * 32 + 16;

		moved = 1;
moveCancelled:;
		if(!moved)
		{
			switch(rnd(4))
			{
			case 0: ed->Dir = 2; break;
			case 1: ed->Dir = 4; break;
			case 2: ed->Dir = 6; break;
			case 3: ed->Dir = 8; break;

			default:
				error();
			}
		}
	}

	int mvDir = 5; // 5-2468

	{
		const double MV_MARGIN = 0.1;

		     if(e->X < ed->TrueX - MV_MARGIN) mvDir = 6;
		else if(ed->TrueX + MV_MARGIN < e->X) mvDir = 4;
		else if(e->Y < ed->TrueY - MV_MARGIN) mvDir = 2;
		else if(ed->TrueY + MV_MARGIN < e->Y) mvDir = 8;
	}

	Repulsion(e->X, e->Y);

	double dr_x = e->X - FDc.CameraX;
	double dr_y = e->Y - FDc.CameraY;

	if(
		dr_x < 0.0 || SCREEN_W < dr_x ||
		dr_y < 0.0 || SCREEN_H < dr_y
		)
		return 0;

	/*
		現在のセル位置
	*/
	int fldX = (int)(e->X / 32);
	int fldY = (int)(e->Y / 32);

	if(!IsCommonEnemyMovable(fldX, fldY, 1)) // ? こいつ今壁の中
		e->BeToTop = 1; // 先頭へ -> 斥力から逃れる。

	int koma = 0;

	if(mvDir == 5)
	{
		if(ed->AnimeAlways)
			koma = (ActFrame / 10) % 4;
	}
	else
	{
		switch(mvDir)
		{
		case 6:
		case 4:
			m_constApproach(e->X, ed->TrueX, MV_SPEED);
			break;

		case 2:
		case 8:
			m_constApproach(e->Y, ed->TrueY, MV_SPEED);
			break;

		default:
			error();
		}
		ed->Dir = mvDir;
		koma = (ActFrame / 10) % 4;
	}

	DrawEnemyBefore();
	DrawCenter(GetEnemyPicId(ed->Color, ed->Kind, ed->Dir, koma), dr_x, dr_y);
	DrawEnemyAfter();

	return 1;
}

// ----
