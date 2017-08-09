typedef struct Enemy_st
{
	int Frame;

	// <-- init @ ctor

	struct
	{
		int X;
		int Y;
	}
	Origin; // 発生源のマップのセルの位置

	double X;
	double Y;

	int (*EachFrame)(struct Enemy_st *, void *);
	void (*Release)(void *);
	void *ExtraData;

	// <-- init @ AddEnemy()

	int WeaponCrashed;
	int BeToTop;
}
Enemy_t;

Enemy_t *CreateEnemy(void);
void ReleaseEnemy(Enemy_t *i);

// <-- cdtor

// <-- accessor

extern int Enemy_SFId;
extern int Enemy_FMapX;
extern int Enemy_FMapY;

void AddEnemy_ved(int (*eachFrame)(Enemy_t *, void *), void (*release)(void *), void *extraData);

template <class ExtraData_t>
void AddEnemy(int (*eachFrame)(Enemy_t *, ExtraData_t *), void (*release)(ExtraData_t *), ExtraData_t *extraData)
{
	AddEnemy_ved((int (*)(Enemy_t *, void *))eachFrame, (void (*)(void *))release, extraData);
}

void DrawEnemyBefore(void);
void DrawEnemyAfter(void);

void EachFrameAllEnemy(void);

int GetEnemyPicId(int color, int kind, int dir, int koma);
int IsExistEnemy(int fldX, int fldY);

// ---- repulsion ----

void ClearRepulsion(void);
void Repulsion(double &me_x, double &me_y);

// ---- crashed ----

void EnemyWeaponCrashed(Enemy_t *e);

// ----
