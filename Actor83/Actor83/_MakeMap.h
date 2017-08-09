extern bitTable *MM_Map;
extern int MM_W;
extern int MM_H;

void MM_Evacuate(void);
void MM_Unevacuate(void);

void MM_Init(int seed, int w, int h);
void MM_Fnlz(void);

void MM_Randomize(int wallPermil);
void MM_BattlePos(int x, int y);
void MM_Battle(int count);
void MM_Wipe(int direct, int count = 1);
void MM_Forestize(void);
void MM_Rim(int depth, int northPos, int northSize, int southPos, int southSize, int westPos, int westSize, int eastPos, int eastSize);
void MM_Expand(int expand);
void MM_Rerandomize(int spaceWallPermil, int wallWallPermil);
void MM_BombBox(int x, int y, int reach, int flag = 0);
void MM_Bypass(int x, int y);
void MM_Bypass_WE(int x, int y);
void MM_Bypass_NS(int x, int y);
void MM_Extra_01(void);
void MM_EraseHosoitokoro(void);
