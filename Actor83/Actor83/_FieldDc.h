typedef struct FDc_st
{
	// Params {
	void (*MapLoader)(void); // ref: MM_Map, MM_W, MM_H
	i2D_t *TakaraPosList;
	int TakaraPosCount;

	int EnterDir; // 2468, 5 == EnterX, EnterY �ˑ�
	int EnterX; // px
	int EnterY; // px

	int RndSeed;
	int BGMMusId;
	// }

	// Return {
	int ExitDir; // 2468, 5 == �^�C�g���֖߂�I��
	int ExitX; // px
	int ExitY; // px
	int ExitWithoutSave;
	// }

	Random *Rnd;

	autoTable<FMCell_t *> *FMap;

	int Field_W; // px
	int Field_H; // px

	struct
	{
		double X;
		double Y;
		int IX;
		int IY;
		int Dir; // 1234-6789
		int Moving;

		struct
		{
			int EFId;
		}
		Action;

		int Pushing;
	}
	Player;

	double CameraX;
	double CameraY;
	/*
		�n��I�u�W�F�N�g�\���̊�Ƃ���
	*/
	int ICameraX;
	int ICameraY;

	int LocalTakaraRemainCount;
	int LocalTakaraTotalCount;

	int GameEndFlag;

	int CompleteTimeCount;

	taskList *DrawFront;
	taskList *DrawEnemy;
	autoList<Enemy_t *> *EnemyList;
	autoList<double> *RepulsionList; // { x, y }...
	autoList<int *> *RepulsionTellList;
	autoList<Weapon_t *> *WeaponList;
}
FDc_t;

extern FDc_t FDc;

void FieldInit(void);
void FieldFnlz(void);

bitList *GetTakaraGotList(void);
int GetTakaraIndex(int x, int y);
void OpenTakara(int x, int y);
int GetEmptyTakaraSFId(int sfId);

void UpdateLocalTakaraCount(void);
void UpdateGlobalTakaraCount(void);

FMCell_t *GetFMCell(int x, int y);
int IsWallOfWeapon(FMCell_t *cell);
