typedef struct Weapon_st
{
	int Frame;
	double X;
	double Y;
	double AddX;
	double AddY;

	// <-- init @ ctor
}
Weapon_t;

Weapon_t *CreateWeapon(double x, double y, int dir);
void ReleaseWeapon(Weapon_t *i);

// <-- cdtor

// <-- accessor

void EachFrameAllWeapon(void);
