#include "all.h"

typedef struct FieldInfo_st
{
	int MapX;
	int MapY;
	int FieldX;
	int FieldY;
	uchar Data[8];
}
FieldInfo_t;

#define SEND_MAX 100
#define RECV_MAX 10000

static oneObject(FieldInfo_t, na(FieldInfo_t, SEND_MAX), GetSData)
static int SCount;
static oneObject(FieldInfo_t, na(FieldInfo_t, RECV_MAX), GetRData)
static int RCount;

static int OverflowLocked;

void MapChanged(int mapX, int mapY, int fieldX, int fieldY, FMCell_t *cell)
{
	if(OverflowLocked)
		return;

	if(SEND_MAX <= SCount)
	{
		SetFieldWarningMessage("OVERFLOW", 0);
		SCount = 0;
		OverflowLocked = 1;
		return;
	}
	FieldInfo_t *fi = GetSData() + SCount;

	fi->MapX = mapX;
	fi->MapY = mapY;
	fi->FieldX = fieldX;
	fi->FieldY = fieldY;
	FMCellToBlock(cell, fi->Data);

	SCount++;
}

static int PlayerMapX;
static int PlayerMapY;
static int PlayerFieldX;
static int PlayerFieldY;

static int IsInSData(int mapX, int mapY, int fieldX, int fieldY)
{
	for(int index = 0; index < SCount; index++)
	{
		FieldInfo_t *fi = GetSData() + index;

		if(
			fi->MapX == mapX &&
			fi->MapY == mapY &&
			fi->FieldX == fieldX &&
			fi->FieldY == fieldY
			)
			return 1; // found
	}
	return 0; // not found
}
static void ReadRData(void)
{
//	LOGPOS();

	FieldInfo_t fi;

	// load fi
	{
		int index = rnd(RCount);

		fi = GetRData()[index];

		RCount--;
		GetRData()[index] = GetRData()[RCount];
	}

	/*
	LOG(
		"fi: %d %d %d %d [%02x %02x %02x %02x %02x %02x %02x %02x], RCount: %d\n"
		,fi.MapX
		,fi.MapY
		,fi.FieldX
		,fi.FieldY
		,fi.Data[0]
		,fi.Data[1]
		,fi.Data[2]
		,fi.Data[3]
		,fi.Data[4]
		,fi.Data[5]
		,fi.Data[6]
		,fi.Data[7]
		,RCount
		);
		*/

	if(PlayerMapX != fi.MapX || PlayerMapY != fi.MapY) // ? マップが違う
	{
//		LOGPOS();
		return;
	}
	if(PlayerFieldX == fi.FieldX && PlayerFieldY == fi.FieldY) // ? プレイヤーの位置と同じ
	{
//		LOGPOS();
		return;
	}
	if(IsInSData(fi.MapX, fi.MapY, fi.FieldX, fi.FieldY)) // ? 絶妙なタイミングで(鯖と同期してからマップに反映されるまでの間に)プレイヤーが変更した。
	{
		LOGPOS();
		return;
	}
	static const uchar zeroData[8];

	if(!memcmp(fi.Data, zeroData, 8)) // ? all zero -> 未設定
	{
//		LOGPOS();
		return;
	}
	FMCell_t *cell = FDc.FMap->GetCell(fi.FieldX, fi.FieldY);
	uchar currData[8];

	FMCellToBlock(cell, currData);

	if(!memcmp(fi.Data, currData, 8)) // ? 現在のセルと同じ -> 適用する必要ナシ
	{
//		LOGPOS();
		return;
	}
	FMCellFromBlock(cell, fi.Data);
//	LOGPOS();

	// 更新エフェクト
	{
		int x = fi.FieldX * 32 + 16;
		int y = fi.FieldY * 32 + 16;

		AddCommonEffect(Gnd.EL, 0, P_WHITE_32x32,
			x, y, 0.0, 1.0, 0.5,
			0, 0, 0.0, 0.0, -0.01
			);
	}
}
static void LoadRData(autoList<uchar> *recvData)
{
//	LOGPOS();

	int rIndex = 0;
	int mapX = readUI32(recvData, rIndex);
	int mapY = readUI32(recvData, rIndex);
	int fieldXBgn = readUI32(recvData, rIndex);
	int fieldYBgn = readUI32(recvData, rIndex);
	int w = readUI32(recvData, rIndex);
	int h = readUI32(recvData, rIndex);

//	LOGPOS();

	for(int y = 0; y < h; y++)
	for(int x = 0; x < w; x++)
	{
		errorCase(RECV_MAX <= RCount);

		uchar data[8];
		readBlock(recvData, rIndex, data, 8);

		FieldInfo_t *fi = GetRData() + RCount;

		fi->MapX = mapX;
		fi->MapY = mapY;
		fi->FieldX = fieldXBgn + x;
		fi->FieldY = fieldYBgn + y;
		memcpy(fi->Data, data, 8);

		RCount++;
	}
//	LOGPOS();
}
static void SendSData(void)
{
//	LOGPOS();

	autoList<uchar> *sendData = new autoList<uchar>(16000);

	writeUI32(sendData, SCount);

	for(int index = 0; index < SCount; index++)
	{
		FieldInfo_t *fi = GetSData() + index;

		writeUI32(sendData, fi->MapX);
		writeUI32(sendData, fi->MapY);
		writeUI32(sendData, fi->FieldX);
		writeUI32(sendData, fi->FieldY);
		writeBlock(sendData, fi->Data, 8);
	}
//	LOGPOS();

	writeUI32(sendData, PlayerMapX);
	writeUI32(sendData, PlayerMapY);
	writeUI32(sendData, PlayerFieldX);
	writeUI32(sendData, PlayerFieldY);

	SCount = 0;

//	LOGPOS();
	SockSend(sendData);
//	LOGPOS();
}

/*
	FDc を触るので、FieldMain() から呼ぶこと！
*/
void EachFrameMapUpdater(int mapX, int mapY, int fieldX, int fieldY) // (mapX, mapY, fieldX, fieldY) == プレイヤーの現在位置
{
//	LOG("efmu: %d %d %d %d\n", mapX, mapY, fieldX, fieldY);

	PlayerMapX = mapX;
	PlayerMapY = mapY;
	PlayerFieldX = fieldX;
	PlayerFieldY = fieldY;

	if(0 < RCount)
	{
		for(int c = m_max(RCount / 10, 1); 0 < c; c--)
			ReadRData();

		return;
	}

	{
		autoList<uchar> *recvData = SockRecv();

		if(recvData)
		{
			LoadRData(recvData);
			delete recvData;
			return;
		}
	}

	if(IsSockSending())
		return;

	SendSData();
}

int MU_GetSCount(void)
{
	return SCount;
}
int MU_GetRCount(void)
{
	return RCount;
}
