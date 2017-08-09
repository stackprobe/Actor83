#include "all.h"

#define BMP_FILE "C:\\tmp\\Map.bmp"

static void MapToBmp(void)
{
	if(!MMT_TakaraPosList)
		MMT_TakaraPosList = new autoList<i2D_t>();

	autoTable<uint> *bmp = new autoTable<uint>(getUIZero, noop_ui);

	for(int x = 0; x < MM_W; x++)
	for(int y = 0; y < MM_H; y++)
	{
		uint color;

		if(HasPos(MMT_TakaraPosList, x, y))
		{
			errorCase(MM_Map->RefBit(x, y)); // ? •Ç‚Ì’†‚É•ó” 
			color = 0xff8000;
		}
		else
			color = MM_Map->RefBit(x, y) ? 0x112233 : 0xddeeff;

		bmp->PutCell(x, y, color);
	}
	writeBmpFile(BMP_FILE, bmp);

	LOG("TakaraPosList:\n");
	for(int index = 0; index < MMT_TakaraPosList->GetCount(); index++)
	{
		i2D_t *pos = MMT_TakaraPosList->ElementAt(index);
		LOG("\t%d, %d,\n", pos->X, pos->Y);
	}
	delete bmp;
	delete MM_Map;
	delete MMT_TakaraPosList;

//	system("START " BMP_FILE);
	system("START mspaint.exe " BMP_FILE);
}
void MMT_Main(void)
{
//	MakeMap_0101(); //MakeTakaraPosList(777, 7);
//	MakeMap_0102(); //MakeTakaraPosList(777, 7);
//	MakeMap_0103(); //MakeTakaraPosList(777, 10);
//	MakeMap_0104(); //MakeTakaraPosList(777, 10); MakeTakaraPosList_R(777, 10, MMT_TakaraPosList);
	
//	MakeMap_0201(); //MakeTakaraPosList_R(2013, 3);
	MakeMap_0202(); //MakeTakaraPosList_R(202, 500);
//	MakeMap_0203(); //MakeTakaraPosList_R(203, 80);
//	MakeMap_0204(); //MakeTakaraPosList(1341398, 5); MakeTakaraPosList_R(1341893, 5, MMT_TakaraPosList);

//	MakeMap_0301(); //MakeTakaraPosList(301, 20); MakeTakaraPosList_R(301, 30, MMT_TakaraPosList);
//	MakeMap_0302(); //MakeTakaraPosList_R(302, 80);
//	MakeMap_0303(); //MakeTakaraPosList_R(1, 100);
//	MakeMap_0304(); //MakeTakaraPosList(304, 10); MakeTakaraPosList_R(305, 20, MMT_TakaraPosList);

//	MakeMap_0401(); //MakeTakaraPosList(401, 30);
//	MakeMap_0402(); //MakeTakaraPosList_R(402, 80);
//	MakeMap_0403(); //MakeTakaraPosList(403, 5);
//	MakeMap_0404(); //MakeTakaraPosList(404, 5);

	MapToBmp();
}
