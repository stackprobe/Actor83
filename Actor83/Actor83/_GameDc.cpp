#include "all.h"

GDc_t GDc;

void GameInit(void)
{
	memset(&GDc, 0x00, sizeof(GDc));

	// スタート地点
	{
		GDc.MapX = 3;
		GDc.MapY = 3;
		GDc.PlayerEnterX = 25 * 32;
		GDc.PlayerEnterY = 25 * 32;
	}

	GDc.TakaraGotTable = new autoTable<bitList *>(createBitList, releaseBitList, MAP_W, MAP_H);
}
void GameFnlz(void)
{
	delete GDc.TakaraGotTable;

	memset(&GDc, 0x00, sizeof(GDc));
}
