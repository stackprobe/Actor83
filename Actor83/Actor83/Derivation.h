enum
{
	ENUM_RANGE(D_DUMMY_00, 4)

	// app >

	ENUM_RANGE(D_TITLEITEM_00, 4)

	/*
		マップチップ画像の縦横タイル数

		大きさ:
			D_MAPCHIP_00     -> 32 x 32
			D_MAPCHIPMINI_00 -> 16 x 16

		ステップ -> 16 x 16
	*/
#define MAPCHIP_X_MAX 32
#define MAPCHIP_Y_MAX 32

	ENUM_RANGE(D_MAPCHIP_00, MC_MAX * MAPCHIP_X_MAX * MAPCHIP_Y_MAX)
	ENUM_RANGE(D_MAPCHIPMINI_00, MC_MAX * MAPCHIP_X_MAX * MAPCHIP_Y_MAX)

	ENUM_RANGE(D_ACTOR83_00, 5 * 4 * 3)

	ENUM_RANGE(D_MAGICSTAR_00, 5 * 3)

	ENUM_RANGE(D_MONSTER_00, 5 * 12 * 8)

	ENUM_RANGE(D_WEAPON_00, 16 * 71)

	// < app

	D_MAX, // num of member
};

typedef struct DerInfo_st
{
	int ParentPicId;
	int X;
	int Y;
	int W;
	int H;
}
DerInfo_t;

int Der(resCluster<PicInfo_t *> *resclu, int derId);
int Der_W(int derId);
int Der_H(int derId);

void UnloadAllDer(resCluster<PicInfo_t *> *resclu);
