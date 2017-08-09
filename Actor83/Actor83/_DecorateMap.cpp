#include "all.h"

static void OutputFieldMap(void)
{
	autoTable<uint> *bmp = new autoTable<uint>(getUIZero, noop_ui, MM_W, MM_H);

	for(int x = 0; x < MM_W; x++)
	for(int y = 0; y < MM_H; y++)
	{
		FMCell_t *cell = FDc.FMap->GetCell(x, y);
		int color;

		if(cell->IsWall)
		{
			if(IsTakaraSFId(cell->SFId.Surface))
				color = 0xffff00;
			else
				color = 0x000000;
		}
		else
			color = 0x0000ff;

		bmp->SetCell(x, y, color);
	}
#if 1
	writeBmpFile_xx(xcout("C:\\tmp\\A83Map\\A83Map_%02d%02d.bmp", GDc.MapX + 1, GDc.MapY + 1), bmp);
#else
	writeBmpFile("C:\\tmp\\A83Map.bmp", bmp);
	delete bmp;
#endif
}
static bitTable *GetUnaccessMap(void)
{
	autoList<i2D_t> *uaPosList = GetUnaccessPosList(GDc.MapX, GDc.MapY);
	bitTable *uaMap = new bitTable(MM_W);

	while(uaPosList->GetCount())
	{
		i2D_t pos = uaPosList->UnaddElement();

		errorCase(!m_isRange(pos.X, 0, MM_W - 1));
		errorCase(!m_isRange(pos.Y, 0, MM_H - 1));

		FMCell_t *cell = FDc.FMap->GetCell(pos.X, pos.Y);

		if(
			!cell->IsWall &&
			!uaMap->RefBit(pos.X, pos.Y)
			)
		{
			uaPosList->AddElement(makeI2D(pos.X - 1, pos.Y));
			uaPosList->AddElement(makeI2D(pos.X + 1, pos.Y));
			uaPosList->AddElement(makeI2D(pos.X, pos.Y - 1));
			uaPosList->AddElement(makeI2D(pos.X, pos.Y + 1));
			uaMap->PutBit(pos.X, pos.Y, 1);
		}
	}
	delete uaPosList;
	return uaMap;
}

static void ToKasanWood(FMCell_t *i)
{
	switch(i->SFId.Surface)
	{
	case SF_WOOD_1:
	case SF_WOOD_9:
		i->SFId.Surface = SF_WOOD_19;
		break;

	case SF_WOOD_3:
	case SF_WOOD_7:
		i->SFId.Surface = SF_WOOD_37;
		break;
	}
}
static void PutWood(FMCell_t *i, int sfId, int kasaneSFId)
{
	i->SFId.Surface = i->SFId.Surface ? kasaneSFId : sfId;
}
static int IsExistTakaraInAround(int x, int y)
{
	for(int xc = -1; xc <= 1; xc++)
	for(int yc = -1; yc <= 1; yc++)
	{
		FMCell_t *cell = FDc.FMap->RefCell(x + xc, y + yc, NULL);

		if(cell && IsTakaraSFId(cell->SFId.Surface))
		{
			return 1;
		}
	}
	return 0;
}
void DecorateMap(void) // called by FieldMain()
{
	for(int orig = 0; orig <= 1; orig++)
	for(int x = orig; x < MM_W - 1; x += 2)
	for(int y = orig; y < MM_H - 1; y += 2)
	{
		if(
			FDc.FMap->GetCell(x + 0, y + 0)->IsWall &&
			FDc.FMap->GetCell(x + 0, y + 1)->IsWall &&
			FDc.FMap->GetCell(x + 1, y + 0)->IsWall &&
			FDc.FMap->GetCell(x + 1, y + 1)->IsWall
			)
		{
			PutWood(FDc.FMap->GetCell(x + 0, y + 0), SF_WOOD_7, SF_WOOD_37);
			PutWood(FDc.FMap->GetCell(x + 0, y + 1), SF_WOOD_1, SF_WOOD_19);
			PutWood(FDc.FMap->GetCell(x + 1, y + 0), SF_WOOD_9, SF_WOOD_19);
			PutWood(FDc.FMap->GetCell(x + 1, y + 1), SF_WOOD_3, SF_WOOD_37);
		}
	}
	for(int x = 0; x < MM_W; x++)
	for(int y = 0; y < MM_H; y++)
	{
		if(
			FDc.FMap->GetCell(x, y)->IsWall &&
			FDc.FMap->GetCell(x, y)->SFId.Surface == 0
			)
		{
			FDc.FMap->GetCell(x, y)->SFId.Surface = FDc.Rnd->RndPct(30) ? SF_SHRUB_FLOWER : SF_SHRUB;
		}
	}
	// ˜gŠO‚Ì–Ø
	{
		for(int x = 0; x < MM_W - 1; x++)
		{
			if(
				FDc.FMap->GetCell(x + 0, 0)->SFId.Surface == SF_WOOD_9 &&
				FDc.FMap->GetCell(x + 1, 0)->SFId.Surface == SF_WOOD_7
				)
			{
				FDc.FMap->GetCell(x + 0, 0)->SFId.Surface = SF_WOOD_19;
				FDc.FMap->GetCell(x + 1, 0)->SFId.Surface = SF_WOOD_37;
			}
			if(
				FDc.FMap->GetCell(x + 0, MM_H - 1)->SFId.Surface == SF_WOOD_3 &&
				FDc.FMap->GetCell(x + 1, MM_H - 1)->SFId.Surface == SF_WOOD_1
				)
			{
				FDc.FMap->GetCell(x + 0, MM_H - 1)->SFId.Surface = SF_WOOD_37;
				FDc.FMap->GetCell(x + 1, MM_H - 1)->SFId.Surface = SF_WOOD_19;
			}
		}
		for(int y = 0; y < MM_H - 1; y++)
		{
			if(
				FDc.FMap->GetCell(0, y + 0)->SFId.Surface == SF_WOOD_1 &&
				FDc.FMap->GetCell(0, y + 1)->SFId.Surface == SF_WOOD_7
				)
			{
				FDc.FMap->GetCell(0, y + 0)->SFId.Surface = SF_WOOD_19;
				FDc.FMap->GetCell(0, y + 1)->SFId.Surface = SF_WOOD_37;
			}
			if(
				FDc.FMap->GetCell(MM_W - 1, y + 0)->SFId.Surface == SF_WOOD_3 &&
				FDc.FMap->GetCell(MM_W - 1, y + 1)->SFId.Surface == SF_WOOD_9
				)
			{
				FDc.FMap->GetCell(MM_W - 1, y + 0)->SFId.Surface = SF_WOOD_37;
				FDc.FMap->GetCell(MM_W - 1, y + 1)->SFId.Surface = SF_WOOD_19;
			}
		}
		// Žl‹÷
		{
			ToKasanWood(FDc.FMap->GetCell(0, 0));
			ToKasanWood(FDc.FMap->GetCell(0, MM_H - 1));
			ToKasanWood(FDc.FMap->GetCell(MM_W - 1, 0));
			ToKasanWood(FDc.FMap->GetCell(MM_W - 1, MM_H - 1));
		}
	}

	int tPosError = 0;

	for(int index = 0; index < FDc.TakaraPosCount; index++)
	{
		i2D_t *tPos = FDc.TakaraPosList + index;

		if(
			tPos->X < 0 || MM_W <= tPos->X ||
			tPos->Y < 0 || MM_H <= tPos->Y ||
			FDc.FMap->GetCell(tPos->X, tPos->Y)->IsWall ||
			IsExistTakaraInAround(tPos->X, tPos->Y)
			)
		{
			LOG("[ERROR-tPos] %d, %d,\n", tPos->X, tPos->Y);
			tPosError = 1;
			continue;
		}
		FMCell_t *c = FDc.FMap->GetCell(tPos->X, tPos->Y);
		int sfId;

		switch(FDc.Rnd->Rnd(4))
		{
		case 0: sfId = SF_TAKARA_APPLE; break;
		case 1: sfId = SF_TAKARA_POTATE; break;
		case 2: sfId = SF_TAKARA_GOLD; break;
		case 3: sfId = SF_TAKARA_LEAF; break;

		default:
			error();
		}
		if(GetTakaraGotList()->RefBit(index)) // ? opened
		{
			sfId = GetEmptyTakaraSFId(sfId);
		}
		c->SFId.Surface = sfId;
		c->IsWall = 1;
	}
#if LOG_ENABLED
	OutputFieldMap();
#endif
	errorCase(tPosError);

	{
		bitTable *uaMap = GetUnaccessMap();

		for(int x = 0; x < MM_W; x++)
		for(int y = 0; y < MM_H; y++)
		{
			if(uaMap->RefBit(x, y))
			{
				FMCell_t *cell = FDc.FMap->GetCell(x, y);

				if(
					uaMap->RefBit(x - 1, y) ||
					uaMap->RefBit(x + 1, y) ||
					uaMap->RefBit(x, y - 1) ||
					uaMap->RefBit(x, y + 1)
					)
					cell->SFId.Ground = SF_WATER;
				else
					cell->SFId.Surface = SF_SABOTEN_01 + FDc.Rnd->Rnd(3);

				cell->IsWall = 1;
			}
		}
		delete uaMap;
	}

	MM_Evacuate();
	{
		MM_Init(FDc.Rnd->Rnd(IMAX), FDc.FMap->GetWidth(), FDc.FMap->GetHeight());
		MM_Randomize(480);
		MM_Wipe(FDc.Rnd->Rnd(4));
		MM_Fnlz();
		MM_EraseHosoitokoro();

		for(int x = 0; x < MM_W; x++)
		for(int y = 0; y < MM_H; y++)
		{
			if(MM_Map->RefBit(x, y))
			{
				FMCell_t *cell = FDc.FMap->GetCell(x, y);

				if(cell->SFId.Ground != SF_WATER)
					cell->SFId.Ground = SF_SHIBA;
			}
		}
		delete MM_Map;

		MM_Init(FDc.Rnd->Rnd(IMAX), FDc.FMap->GetWidth(), FDc.FMap->GetHeight());
		MM_Randomize(400);
		MM_Wipe(FDc.Rnd->Rnd(4));
		MM_Forestize();
		MM_Fnlz();

		for(int x = 0; x < MM_W; x++)
		for(int y = 0; y < MM_H; y++)
		{
			if(MM_Map->RefBit(x, y))
			{
				const int MARGIN = 10;

				if(
					MARGIN <= x && x < MM_W - MARGIN &&
					MARGIN <= y && y < MM_H - MARGIN
					)
				{
					FMCell_t *cell = FDc.FMap->GetCell(x, y);

					for(int xc = -1; xc <= 1; xc++)
					for(int yc = -1; yc <= 1; yc++)
					{
						int sx = x + xc;
						int sy = y + yc;

						if(
							/*
							sx < 0 || FDc.FMap->GetWidth() <= sx ||
							sy < 0 || FDc.FMap->GetHeight() <= sy ||
							*/
							FDc.FMap->GetCell(sx, sy)->IsWall
							)
						{
							MM_Map->PutBit(x, y, 0);

							// to break
							xc = IMAX;
							yc = IMAX;
						}
					}
				}
				else
				{
					MM_Map->PutBit(x, y, 0);
				}
			}
		}
		MM_EraseHosoitokoro();

		for(int x = 0; x < MM_W; x++)
		for(int y = 0; y < MM_H; y++)
		{
			if(MM_Map->RefBit(x, y))
			{
				FMCell_t *cell = FDc.FMap->GetCell(x, y);

				cell->SFId.Ground = SF_WATER;
				cell->IsWall = 1;
			}
		}
		delete MM_Map;
	}
	MM_Unevacuate();

	for(int x = 0; x < MM_W; x++)
	for(int y = 0; y < MM_H; y++)
	{
		FMCell_t *cell = FDc.FMap->GetCell(x, y);

		if(!cell->IsWall && FDc.Rnd->RndPct(1))
		{
			int sfId;

			switch(FDc.Rnd->Rnd(8))
			{
			case 0: sfId = SF_KUSA_01; break;
			case 1: sfId = SF_KUSA_02; break;
			case 2: sfId = SF_KUSA_03; break;
			case 3: sfId = SF_KUSA_04; break;
			case 4: sfId = SF_FLOWER_01; break;
			case 5: sfId = SF_FLOWER_02; break;
			case 6: sfId = SF_ISHI; break;
			case 7: sfId = SF_KUSA_05; break;

			default:
				error();
			}
			cell->SFId.Surface = sfId;
		}
	}
#if LOG_ENABLED
	OutputFieldMap();
#endif
}
