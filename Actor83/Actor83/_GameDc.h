enum // DL(Daylight)
{
	DL_DAYTIME, // def
	DL_SUNSET,
	DL_NIGHT,
};

typedef struct GDc_st
{
	// Params {
	int MapX; // 0 - (MAP_W - 1)
	int MapY; // 0 - (MAP_H - 1)
	int PlayerEnterX; // px
	int PlayerEnterY; // px

	int LoadFlag; // ? ロードする。
	// }

	// Return {
	// }

	int PlayTimeFrame;
	int ClearTimeFrame; // 0 == 未クリア (0でクリアは無いだろう)

	autoTable<bitList *> *TakaraGotTable;

	int GlobalTakaraRemainCount;
	int GlobalTakaraTotalCount;

	int DaylightMode;
	struct
	{
		double R;
		double G;
		double B;
		double A;
	}
	Daylight;
}
GDc_t;

extern GDc_t GDc;

void GameInit(void);
void GameFnlz(void);
