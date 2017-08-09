#include "TakaraPos.h"

static i2D_t *S_List;
static int S_Count;

#define LL(list, count) \
	do { \
	S_List = (list); \
	S_Count = (count); \
	return; \
	} while(0)

static void LoadList(int mapX, int mapY)
{
	if(mapX == 0 && mapY == 0) LL(TakaraPosList_0101, TakaraPosCount_0101);
	if(mapX == 0 && mapY == 1) LL(TakaraPosList_0102, TakaraPosCount_0102);
	if(mapX == 0 && mapY == 2) LL(TakaraPosList_0103, TakaraPosCount_0103);
	if(mapX == 0 && mapY == 3) LL(TakaraPosList_0104, TakaraPosCount_0104);

	if(mapX == 1 && mapY == 0) LL(TakaraPosList_0201, TakaraPosCount_0201);
	if(mapX == 1 && mapY == 1) LL(TakaraPosList_0202, TakaraPosCount_0202);
	if(mapX == 1 && mapY == 2) LL(TakaraPosList_0203, TakaraPosCount_0203);
	if(mapX == 1 && mapY == 3) LL(TakaraPosList_0204, TakaraPosCount_0204);

	if(mapX == 2 && mapY == 0) LL(TakaraPosList_0301, TakaraPosCount_0301);
	if(mapX == 2 && mapY == 1) LL(TakaraPosList_0302, TakaraPosCount_0302);
	if(mapX == 2 && mapY == 2) LL(TakaraPosList_0303, TakaraPosCount_0303);
	if(mapX == 2 && mapY == 3) LL(TakaraPosList_0304, TakaraPosCount_0304);

	if(mapX == 3 && mapY == 0) LL(TakaraPosList_0401, TakaraPosCount_0401);
	if(mapX == 3 && mapY == 1) LL(TakaraPosList_0402, TakaraPosCount_0402);
	if(mapX == 3 && mapY == 2) LL(TakaraPosList_0403, TakaraPosCount_0403);
	if(mapX == 3 && mapY == 3) LL(TakaraPosList_0404, TakaraPosCount_0404);

	error();
}
int IsTakaraPos(int mapX, int mapY, int fieldX, int fieldY)
{
	uint index;

//	cout("IsTakaraPos(): %d %d %d %d\n", mapX, mapY, fieldX, fieldY);

	LoadList(mapX, mapY);

//	cout("S_List: %08x, S_Count %d\n", S_List, S_Count);

	for(index = 0; index < S_Count; index++)
	{
		i2D_t *i = S_List + index;

		if(i->X == fieldX && i->Y == fieldY)
		{
//			cout("FOUND @ %d\n", index);
			return 1;
		}
	}
//	cout("not_found\n");
	return 0;
}
