#define MONSTER_COLOR_MAX 5
#define MONSTER_MAX 8

enum // as SFId (Surface)
{
	// ---- 共通 ----

	SF_EMPTY, // as init value?

	// ---- SFId.Ground ----

	SF_GROUND_0 = 0,

	/*
		順序を変更してはならない！
	*/

	SF_GREEN,
	SF_WATER,
	SF_SHIBA,
	SF_SAND,
	SF_SOIL,
	SF_HANABATAKE,
	SF_ISHIDATAMI,
	SF_GREEN_SAKU,
	SF_SHIMETTA_GREEN,
	SF_HASHI_TATE,
	SF_HASHI_YOKO,

	SF_GROUND_END, // end of member

	// ---- SFId.Surface ----

	SF_SURFACE_0 = 0,

	/*
		順序を変更してはならない！
	*/

	SF_WOOD_1,
	SF_WOOD_19,
	SF_WOOD_3,
	SF_WOOD_37,
	SF_WOOD_7,
	SF_WOOD_9,
	SF_SHRUB,
	SF_SHRUB_FLOWER,
	SF_TAKARA_APPLE,
	SF_TAKARA_POTATE,
	SF_TAKARA_GOLD,
	SF_TAKARA_LEAF,
	SF_EMPTY_TAKARA,
	SF_EMPTY_TAKARA_GOLD,
	SF_SABOTEN_01,
	SF_SABOTEN_02,
	SF_SABOTEN_03,
	SF_KUSA_01,
	SF_KUSA_02,
	SF_KUSA_03,
	SF_KUSA_04,
	SF_FLOWER_01,
	SF_FLOWER_02,
	SF_ISHI,
	SF_KUSA_05,
	SF_SAKU,
	SF_YASAI_01,
	SF_YASAI_02,
	SF_YASAI_03,
	SF_YASAI_04,
	SF_KIRIKABU_01,
	SF_KIRIKABU_02,
	SF_KIRIKABU_03,
	SF_KIRIKABU_04,
	SF_TATEFUDA_01,
	SF_TATEFUDA_02,
	SF_TATEFUDA_03,
	SF_TATEFUDA_04,
	SF_KAKASHI,
	SF_IWA,
	SF_HASHIRA,
	SF_HASHIRA_KOKE,
	SF_HASHIRA_LONG,

	SF_SURFACE_END, // end of member

	// ---- SFId.Extra ----

	SF_EXTRA_0 = 0,

	/*
		順序を変更してはならない！
	*/

	ENUM_RANGE(SF_MONSTER_00, MONSTER_MAX)

	SF_EXTRA_END, // end of member

	// ----

//	SF_MAX, // num of member ..になってない！
};

enum // as MCId (MapChip)
{
	MC_TILE_A1,
	MC_TILE_A2,
	MC_TILE_A3,
	MC_TILE_A4,
	MC_TILE_A5,
	MC_TILE_B,
	MC_TILE_C,
	MC_TILE_D,
	MC_TILE_E,

	MC_MAX, // num of member
};

#define MAP_W 4
#define MAP_H 4

#define PLAYTIMEFRAME_MAX 21599999 // 99:59:59(.59)

enum // as EFId (EditField)
{
	EF_NONE,
	EF_GREEN,
	EF_SHIBA,
	EF_WATER,
	EF_KUSA_01,
	EF_KUSA_02,
	EF_KUSA_03,
	EF_KUSA_04,
	EF_KUSA_05,
	EF_FLOWER_01,
	EF_FLOWER_02,
	EF_ISHI,
	EF_SAND,
	EF_SOIL,
	EF_HANABATAKE,
	EF_ISHIDATAMI,
	EF_SAKU_01,
	EF_SAKU_02,
	EF_HASHI_TATE,
	EF_HASHI_YOKO,
	EF_SABOTEN,
	EF_YASAI,
	EF_KIRIKABU,
	EF_TATEFUDA,
	EF_KAKASHI,
	ENUM_RANGE(EF_MONSTER_00, MONSTER_MAX)
	EF_WEAPON_01,
	EF_HASHIRA,

	EF_MAX, // num of member
};
