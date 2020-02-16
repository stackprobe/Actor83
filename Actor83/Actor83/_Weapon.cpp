#include "all.h"

Weapon_t *CreateWeapon(double x, double y, int dir)
{
	Weapon_t *i = nb_(Weapon_t);

	i->Frame = -1;
	i->X = x;
	i->Y = y;
	i->AddX = 0.0;
	i->AddY = 0.0;

	const double SPEED = 7.0;
	const double SPEED_NANAME = SPEED / ROOT_2;

	switch(dir)
	{
	case 1:
		i->AddX = -SPEED_NANAME;
		i->AddY =  SPEED_NANAME;
		break;

	case 3:
		i->AddX =  SPEED_NANAME;
		i->AddY =  SPEED_NANAME;
		break;

	case 7:
		i->AddX = -SPEED_NANAME;
		i->AddY = -SPEED_NANAME;
		break;

	case 9:
		i->AddX =  SPEED_NANAME;
		i->AddY = -SPEED_NANAME;
		break;

	case 2: i->AddY =  SPEED; break;
	case 4: i->AddX = -SPEED; break;
	case 6: i->AddX =  SPEED; break;
	case 8: i->AddY = -SPEED; break;

	default:
		error();
	}
	return i;
}
void ReleaseWeapon(Weapon_t *i)
{
	if(!i)
		return;

	memFree(i);
}

// <-- cdtor

// <-- accessor

static int IsCrashed(Weapon_t *i, Enemy_t *e, double r)
{
	double x1 = i->X;
	double y1 = i->Y;
	double x2 = e->X;
	double y2 = e->Y;

	double dx = x1 - x2;
	double dy = y1 - y2;

	double dP2 = dx * dx + dy * dy;
	double rP2 = r * r;

	return dP2 < rP2;
}
static int CheckCrashed(Weapon_t *i) // ret: ? crashed
{
	for(int index = 0; index < FDc.EnemyList->GetCount(); index++)
	{
		Enemy_t *e = FDc.EnemyList->GetElement(index);

		if(IsCrashed(i, e, 24.0))
		{
			e->WeaponCrashed = 1;
			return 1;
		}
	}
	return 0;
}
static int EachFrameWeapon(Weapon_t *i) // ret: ? ¶‘¶
{
	i->X += i->AddX;
	i->Y += i->AddY;

	double dr_x = i->X - FDc.CameraX;
	double dr_y = i->Y - FDc.CameraY;

	if(
		dr_x < 0.0 || SCREEN_W < dr_x ||
		dr_y < 0.0 || SCREEN_H < dr_y
		)
		return 0;

	int fldX = (int)(i->X / 32.0);
	int fldY = (int)(i->Y / 32.0);

	{
		FMCell_t *cell = GetFMCell(fldX, fldY);

		if(cell)
		{
			if(IsWallOfWeapon(cell))
			{
				return 0;
			}
		}
	}

	if(CheckCrashed(i))
		return 0;

	DPE.TL = FDc.DrawFront;
	DrawBegin(D_WEAPON_00 + 931 | DTP, dr_x, dr_y);
	DrawRotate(-i->Frame / 3.0);
	DrawEnd();
	DPE.TL = NULL;

	return 1;
}
void EachFrameAllWeapon(void)
{
	for(int index = 0; index < FDc.WeaponList->GetCount(); index++)
	{
		Weapon_t *i = FDc.WeaponList->GetElement(index);

		i->Frame++; // 0`

		if(!EachFrameWeapon(i)) // ? Ž€–S
		{
			FDc.WeaponList->FastDesertElement(index);
			index--;
			ReleaseWeapon(i);
		}
	}
}
