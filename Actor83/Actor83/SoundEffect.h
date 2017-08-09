enum
{
	SE_DUMMY,

	// app >

	SE_GET,
	SE_COMPLETE,
	SE_SAVE,
	SE_MAGIC,
	SE_SHOT_WEAPON,
	SE_ENEMY_DEATH,

	// < app

	SE_MAX, // num of member
};

#define SE_HANDLE_MAX 64

typedef struct SEInfo_st
{
	int HandleList[SE_HANDLE_MAX];
	int HandleIndex;
	double Volume; // 0.0 - 1.0, def: 0.5
}
SEInfo_t;

int SEEachFrame(void);
void SEPlay(int seId);
void UpdateSEVolume(void);
