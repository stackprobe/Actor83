/*
	Service.dat Ç…à»â∫Çìoò^Ç∑ÇÈÇ±Ç∆ÅB

		Actor83 <ê‚ëŒÉpÉX>\Actor83.exe
*/

#include "C:\Factory\SubTools\HTT\libs\CRPC_Aes.h"
#include "map.h"

autoBlock_t *CRPC_GetRawKey(void)
{
#if 1
	////// ///////////////////////////////////////////////////// // $_git:secret
#else // dummy
	return makeBlockHexLine("ffffffffffffffffffffffffffffffff");
#endif
}
void CRPC_Init(void)
{
	/*
		COUNT       : 4

		{
			MAP_X   : 4
			MAP_Y   : 4
			FIELD_X : 4
			FIELD_Y : 4
			CELL    : 8
		}           = 24 x 100_max

		MAP_X       : 4
		MAP_Y       : 4
		FIELD_X     : 4
		FIELD_Y     : 4

		CRPC_HASH   : 16
	*/
	RPC_RecvSizeMax = 2436;

	RPC_ConnectTimeoutSec = 20;
}

static autoBlock_t *RData;
static uint RPos;

static uint ReadInt(void)
{
	uint ret = ab_getValue(RData, RPos);

	RPos += 4;
	return ret;
}
static void ReadBytes(uchar *buff, uint readSize)
{
	uint index;

	for(index = 0; index < readSize; index++)
	{
		buff[index] = getByte(RData, RPos);
		RPos++;
	}
}

autoBlock_t *ServiceCRPC(autoBlock_t *recvData, uint phase)
{
	autoBlock_t *sendData;
	uint count;

	RData = recvData;
	RPos = 0;

	count = ReadInt();

	cout("SetFieldCell_count: %u\n", count);

	while(count)
	{
		uint mapX;
		uint mapY;
		uint fieldX;
		uint fieldY;
		uchar cell[FIELDCELLSIZE];

		mapX = ReadInt();
		mapY = ReadInt();
		fieldX = ReadInt();
		fieldY = ReadInt();
		ReadBytes(cell, FIELDCELLSIZE);

		SetFieldCell(mapX, mapY, fieldX, fieldY, cell);

		count--;
	}

	{
		uint mapX;
		uint mapY;
		uint fieldX;
		uint fieldY;

		mapX = ReadInt();
		mapY = ReadInt();
		fieldX = ReadInt();
		fieldY = ReadInt();

		sendData = GetFieldArea(mapX, mapY, fieldX, fieldY);
	}

	RData = NULL;
	RPos = 0;

	RPC_SendCompleteAndDisconnect();
	return sendData;
}
