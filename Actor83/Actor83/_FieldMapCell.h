typedef struct FMCell_st
{
	struct
	{
		uint16 Ground;
		uint16 Surface;
		uint16 Extra;
	}
	SFId;

	uint16 IsWall : 1; // ? •Ç
	uint16 Reserved_02 : 1;
	uint16 Reserved_03 : 1;
	uint16 Reserved_04 : 1;
	uint16 Reserved_05 : 1;
	uint16 Reserved_06 : 1;
	uint16 Reserved_07 : 1;
	uint16 Reserved_08 : 1;

	uint16 Reserved_11 : 1;
	uint16 Reserved_12 : 1;
	uint16 Reserved_13 : 1;
	uint16 Reserved_14 : 1;
	uint16 Reserved_15 : 1;
	uint16 Reserved_16 : 1;
	uint16 Reserved_17 : 1;
	uint16 Reserved_18 : 1;
}
FMCell_t;

FMCell_t *CreateFMCell(void);
void ReleaseFMCell(FMCell_t *i);

void FMCellToBlock(FMCell_t *cell, uchar data[8]);
void FMCellFromBlock(FMCell_t *cell, uchar data[8]);

int GetSFPicId(int mcId, int x, int y, int miniMode);
int IsTakaraSFId(int sfId);

int IsWoodCell(FMCell_t *cell);
int IsTakaraCell(FMCell_t *cell);
int IsEditableCell(FMCell_t *cell);
