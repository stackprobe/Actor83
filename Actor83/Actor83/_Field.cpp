#include "all.h"

static int GetCenterSpace_WE(int y)
{
	int xMin = MM_W;
	int xMax = -1;

	for(int x = 0; x < MM_W; x++)
	{
		if(MM_Map->RefBit(x, y) == 0) // ? space
		{
			m_minim(xMin, x);
			m_maxim(xMax, x);
		}
	}
	return (xMin + xMax + 1) * 16; // px
}
static int GetCenterSpace_NS(int x)
{
	int yMin = MM_H;
	int yMax = -1;

	for(int y = 0; y < MM_H; y++)
	{
		if(MM_Map->RefBit(x, y) == 0) // ? space
		{
			m_minim(yMin, y);
			m_maxim(yMax, y);
		}
	}
	return (yMin + yMax + 1) * 16; // px
}

// PKP >

static int PKP_XPos;
static int PKP_YPos;
static int PKP_Wall1;
static int PKP_Wall3;
static int PKP_Wall7;
static int PKP_Wall9;
static int PKP_OnWall;

static void PKP_XTurn(void)
{
	PKP_XPos *= -1;
	m_swap(PKP_Wall1, PKP_Wall3, int);
	m_swap(PKP_Wall7, PKP_Wall9, int);
}
static void PKP_YTurn(void)
{
	PKP_YPos *= -1;
	m_swap(PKP_Wall1, PKP_Wall7, int);
	m_swap(PKP_Wall3, PKP_Wall9, int);
}
static void PlayerKabeProc(void)
{
	switch(
		(PKP_Wall1 ? 1 : 0) |
		(PKP_Wall3 ? 2 : 0) |
		(PKP_Wall7 ? 4 : 0) |
		(PKP_Wall9 ? 8 : 0)
		)
	{
	case 0: // 壁なし
		break;

	case 15: // 壁の中
		PKP_OnWall = 1;
		break;

	case 1: // 左下のみ
	case 4: // 左上のみ
	case 5: // 左
	case 7: // 右上のみ空き
	case 13: // 右下のみ空き
		PKP_XTurn();
		PlayerKabeProc();
		PKP_XTurn();
		break;

	case 8: // 右上のみ
	case 9: // 右上と左下
	case 12: // 上
	case 14: // 左下のみ空き
		PKP_YTurn();
		PlayerKabeProc();
		PKP_YTurn();
		break;

	case 2: // 右下のみ
		{
			if(-16 < PKP_XPos && -16 < PKP_YPos)
			{
				if(PKP_XPos < PKP_YPos)
				{
					PKP_XPos = -16;

					if(PKP_YPos < 14)
					{
						PKP_YPos--;
					}
				}
				else
				{
					if(PKP_XPos < 14)
					{
						PKP_XPos--;
					}
					PKP_YPos = -16;
				}
			}
		}
		break;

	case 10: // 右
		PKP_XPos = -16;
		break;

	case 3: // 下
		PKP_YPos = -16;
		break;

	case 6: // 左上と右下
		{
			if(PKP_XPos < PKP_YPos)
			{
				PKP_XPos = -16;
				PKP_YPos = 16;
			}
			else
			{
				PKP_XPos = 16;
				PKP_YPos = -16;
			}
		}
		break;

	case 11: // 左上のみ空き
		{
			PKP_XPos = -16;
			PKP_YPos = -16;
		}
		break;

	default:
		error();
	}
}

// < PKP

static void DrawField(int drawSurfaceLater = 0)
{
	int fmx = FDc.ICameraX / 32;
	int fmy = FDc.ICameraY / 32;

	int fmOrigX = fmx * 32;
	int fmOrigY = fmy * 32;

	int drOrigX = fmOrigX - FDc.ICameraX;
	int drOrigY = fmOrigY - FDc.ICameraY;

	/*
		800 / 32 == 25.00 -> 25 + 1 == 26
		600 / 32 == 18.75 -> 19 + 2 == 21
	*/
	for(int y = 0; y < 21; y++)
	for(int x = 0; x < 26; x++)
	{
		int sx = fmx + x;
		int sy = fmy + y;

		if(
			0 <= sx && sx < FDc.FMap->GetWidth() &&
			0 <= sy && sy < FDc.FMap->GetHeight()
			)
		{
			DrawFieldCell(
				drOrigX + x * 32,
				drOrigY + y * 32,
				FDc.FMap->GetCell(sx, sy),
				sx,
				sy
				);
		}
	}
	if(!drawSurfaceLater)
		FDc.DrawFront->ExecuteAllTask();
}

static int EFDC_SleepFrame;

static void EachFrameDaylightChecker(void)
{
	int &sleepFrame = EFDC_SleepFrame;
	sleepFrame--;

	if(
		Gnd.FieldDaylightDisabled == 0 &&
		sleepFrame < 0
		)
	{
		GDc.DaylightMode = GetDaylightMode();
		sleepFrame = 60 * 60;
	}
}
static void EachFrameDaylightFilter(int quickMode = 0)
{
	double SPEED = 0.0003;

	if(quickMode)
		SPEED = 1.0;

	switch(GDc.DaylightMode)
	{
	case DL_DAYTIME:
		m_constApproach(GDc.Daylight.A, 0.0, SPEED);
		break;

	case DL_SUNSET:
		GDc.Daylight.R = 1.0;
		GDc.Daylight.G = 0.15;
		GDc.Daylight.B = 0.0;
		m_constApproach(GDc.Daylight.A, 0.15, SPEED);
		break;

	case DL_NIGHT:
		m_constApproach(GDc.Daylight.R, 0.0, SPEED * 10);
		m_constApproach(GDc.Daylight.G, 0.0, SPEED * 1.5);
		m_constApproach(GDc.Daylight.B, 0.2, SPEED * 2);
		m_constApproach(GDc.Daylight.A, 0.4, SPEED);
		break;

	default:
		error();
	}
}
static void DrawDaylightFilter(void)
{
	if(Gnd.FieldDaylightDisabled)
		return;

	if(0.0 < GDc.Daylight.A)
	{
		DPE_SetAlpha(GDc.Daylight.A);
		DPE_SetBright(GDc.Daylight.R, GDc.Daylight.G, GDc.Daylight.B);
		DrawRect(P_WHITEBOX, 0, 0, SCREEN_W, SCREEN_H);
		DPE_Reset();
	}
}

// Pause >

static double PM_InSqrA;
static int PM_Full;

static void PM_DrawBegin(void)
{
	EachFrameDaylightChecker();
	EachFrameDaylightFilter();

	DrawField();
	DrawDaylightFilter();

	DPE_SetAlpha(PM_InSqrA);
	DPE_SetBright(0.0, 0.0, 0.0);

	if(PM_Full)
		DrawRect(P_WHITEBOX, 0, 0, SCREEN_W, SCREEN_H);
	else
		DrawRect(P_WHITEBOX, 50, 50, SCREEN_W - 100, SCREEN_H - 100);

	DPE_Reset();

	CurtainEachFrame();
}
static void PM_EachFrame(void)
{
	EachFrame();

	PM_InSqrA += 0.01;
	m_range(PM_InSqrA, 0.0, 0.2);

	GDc.PlayTimeFrame++;
	m_range(GDc.PlayTimeFrame, 0, PLAYTIMEFRAME_MAX);
}
static void PM_ActionMenu(void)
{
	FreezeInput();
	PM_Full = 1;

	int efId = FDc.Player.Action.EFId;
	int back_efId = efId;

	for(; ; )
	{
		const int COL_NUM = 8;

		if(GetPound(INP_DIR_2))
			efId += COL_NUM;

		if(GetPound(INP_DIR_4))
			efId--;

		if(GetPound(INP_DIR_6))
			efId++;

		if(GetPound(INP_DIR_8))
			efId -= COL_NUM;

		efId = (efId + EF_MAX) % EF_MAX;
		m_range(efId, 0, EF_MAX);

		if(GetInput(INP_PAUSE) == 1)
			break;

		if(GetInput(INP_B) == 1)
		{
			if(efId == back_efId)
				break;

			efId = back_efId;
		}
		if(GetInput(INP_A) == 1)
		{
			break;
		}
		PM_DrawBegin();

		PE_Border(GetColor(100, 16, 16));
		SetPrint(20, 20, 40);
		Print("アクションを選択して下さい。");

		for(int c = 0; c < EF_MAX; c++)
		{
			if(c % COL_NUM == 0)
				PrintRet();

			Print_x(xcout(
				"%c %s %c"
				,c == efId ? '[' : ' '
				,GetNameOfEFId(c)
				,c == efId ? ']' : ' '
				));
		}
		PE_Reset();

		PM_EachFrame();
	}
//endLoop:
	FreezeInput();
	PM_Full = 0;
	FDc.Player.Action.EFId = efId;
}
static int PM_WithoutSaveConfirm(void)
{
	int retval = 0;

	FreezeInput();

	const int selectMax = 2;
	int selectIndex = 0;

	for(; ; )
	{
		if(GetPound(INP_DIR_8))
			selectIndex--;

		if(GetPound(INP_DIR_2))
			selectIndex++;

		m_range(selectIndex, 0, selectMax - 1);

		if(GetInput(INP_PAUSE) == 1)
			break;

		if(GetInput(INP_B) == 1)
			break;

		if(GetInput(INP_A) == 1)
		{
			switch(selectIndex)
			{
			case 0:
				goto endLoop;

			case 1:
				retval = 1;
				goto endLoop;

			default:
				error();
			}
		}
		PM_DrawBegin();

		PE_Border(GetColor(100, 16, 16));
		SetPrint(200, 250, 40);
		Print("現在のプレイデータを破棄してタイトルに戻ります。");
		PrintRet();
		Print_x(xcout("[%c] ゲームに戻る", selectIndex == 0 ? '>' : ' '));
		PrintRet();
		Print_x(xcout("[%c] タイトルに戻る", selectIndex == 1 ? '>' : ' '));
		PrintRet();
		PE_Reset();

		PM_EachFrame();
	}
endLoop:
	FreezeInput();

	return retval;
}
static void PauseMain(void)
{
	PM_InSqrA = 0.0;

	SetCurtain(30, -0.5);

	const int selectMax = 4;
	int selectIndex = 0;

	for(; ; )
	{
		if(GetPound(INP_DIR_8))
			selectIndex--;

		if(GetPound(INP_DIR_2))
			selectIndex++;

		m_range(selectIndex, 0, selectMax - 1);

		if(GetInput(INP_PAUSE) == 1)
			break;

		if(GetInput(INP_B) == 1)
			break;

		if(GetInput(INP_A) == 1)
		{
			switch(selectIndex)
			{
			case 0:
				goto endLoop;

			case 1:
				PM_ActionMenu();
				break;

			case 2:
				FDc.GameEndFlag = 1;
				goto endLoop;

			case 3:
				if(PM_WithoutSaveConfirm())
				{
					FDc.GameEndFlag = 1;
					FDc.ExitWithoutSave = 1;
					goto endLoop;
				}
				break;

			default:
				error();
			}
		}
		PM_DrawBegin();

		// menu >

		PE_Border(GetColor(64, 48, 16));
		SetPrint(100, 230, 40);
		int c = 0;
		Print_x(xcout("[%c] ゲームに戻る", selectIndex == c++ ? '>' : ' '));
		PrintRet();
		Print_x(xcout("[%c] アクション", selectIndex == c++ ? '>' : ' '));
		PrintRet();
		Print_x(xcout("[%c] セーブしてタイトルに戻る", selectIndex == c++ ? '>' : ' '));
		PrintRet();
		Print_x(xcout("[%c] セーブしないでタイトルに戻る", selectIndex == c++ ? '>' : ' '));
		PrintRet();
		PE_Reset();

		// < menu

		{
			int h = GDc.PlayTimeFrame / (60 * 60 * 60);
			int m = (GDc.PlayTimeFrame / (60 * 60)) % 60;
			int s = (GDc.PlayTimeFrame / 60) % 60;

			PE_Border(GetColor(16, 32, 64));
			SetPrint(450, 210, 40);
			Print_x(xcout("プレイ時間　%02d:%02d:%02d", h, m, s));
			PrintRet();
			Print_x(xcout("クリアタイム　"));

			if(GDc.ClearTimeFrame)
			{
				h = GDc.ClearTimeFrame / (60 * 60 * 60);
				m = (GDc.ClearTimeFrame / (60 * 60)) % 60;
				s = (GDc.ClearTimeFrame / 60) % 60;

				Print_x(xcout("%02d:%02d:%02d", h, m, s));
			}
			else
				Print("--:--:--");

			PrintRet();
			Print_x(xcout("ベストタイム　"));

			if(Gnd.BestTimeFrame)
			{
				h = Gnd.BestTimeFrame / (60 * 60 * 60);
				m = (Gnd.BestTimeFrame / (60 * 60)) % 60;
				s = (Gnd.BestTimeFrame / 60) % 60;

				Print_x(xcout("%02d:%02d:%02d", h, m, s));
			}
			else
				Print("--:--:--");

			PrintRet();
			Print_x(xcout("回収率　%d / %d"
				,FDc.LocalTakaraTotalCount - FDc.LocalTakaraRemainCount
				,FDc.LocalTakaraTotalCount
				));
			PrintRet();
			Print_x(xcout("全体の回収率　%d / %d"
				,GDc.GlobalTakaraTotalCount - GDc.GlobalTakaraRemainCount
				,GDc.GlobalTakaraTotalCount
				));
			PE_Reset();
		}

		PM_EachFrame();
	}
endLoop:
	SetCurtain();
}

// < Pause

static char *WarningMessage; // 開放しない！
static int WM_RemFrame;

void SetFieldWarningMessage(char *str_const, int frame) // frame: 0 以下 == ずっと出す。
{
	WarningMessage = str_const;
	WM_RemFrame = frame;
}

void FieldMain(void)
{
	errorCase(!FDc.MapLoader);
	errorCase(!FDc.TakaraPosList);
	errorCase(!m_isRange(FDc.TakaraPosCount, 1, IMAX)); // ゼロはだめでいいや。

	FDc.Rnd = new Random(FDc.RndSeed);

	// load map
	{
		FDc.MapLoader();

		errorCase(!MM_Map);
		errorCase(!m_isRange(MM_W, 1, IMAX));
		errorCase(!m_isRange(MM_H, 1, IMAX));

		FDc.FMap->Resize(MM_W, MM_H);
		FDc.Field_W = MM_W * 32;
		FDc.Field_H = MM_H * 32;

		// check prm >

		switch(FDc.EnterDir)
		{
		case 5:
			errorCase(FDc.EnterX < 0 || FDc.Field_W <= FDc.EnterX);
			errorCase(FDc.EnterY < 0 || FDc.Field_H <= FDc.EnterY);

		case 2:
		case 4:
		case 6:
		case 8:
			break;

		default:
			error();
		}

		// <

		for(int x = 0; x < MM_W; x++)
		for(int y = 0; y < MM_H; y++)
		{
			FDc.FMap->GetCell(x, y)->IsWall = MM_Map->RefBit(x, y);
		}
		DecorateMap();

		{
			int plX = -1; // px
			int plY = -1; // px

			switch(FDc.EnterDir)
			{
			case 5:
				plX = FDc.EnterX;
				plY = FDc.EnterY;
				break;

			case 2:
				plX = GetCenterSpace_WE(MM_H - 1);
				plY = FDc.Field_H - 48;
				break;

			case 4:
				plX = 48;
				plY = GetCenterSpace_NS(0);
				break;

			case 6:
				plX = FDc.Field_W - 48;
				plY = GetCenterSpace_NS(MM_W - 1);
				break;

			case 8:
				plX = GetCenterSpace_WE(0);
				plY = 48;
				break;

			default:
				error();
			}

			FDc.Player.X = plX;
			FDc.Player.Y = plY;
		}

		switch(FDc.EnterDir)
		{
		case 2: FDc.Player.Dir = 8; break;
		case 4: FDc.Player.Dir = 6; break;
		case 6: FDc.Player.Dir = 4; break;
		case 8: FDc.Player.Dir = 2; break;

		default:
			FDc.Player.Dir = 2;
			break;
		}

		delete MM_Map;
		MM_Map = NULL;
		MM_W = -1;
		MM_H = -1;
	}

	UpdateLocalTakaraCount();

	if(FDc.EnterDir == 5)
	{
		GDc.DaylightMode = GetDaylightMode();
		EachFrameDaylightFilter(1);
	}
	SetCurtain();
	FreezeInput();
	ActFrame = 0;

	MusicPlay(FDc.BGMMusId);

	for(; ; )
	{
		if(ActFrame < 120)
			SetSockConnectWaitMillisMax(2000);

		// カメラ位置
		{
			m_approach(FDc.CameraX, FDc.Player.X - SCREEN_CENTER_X, ActFrame ? 0.85 : 0.0);
			m_approach(FDc.CameraY, FDc.Player.Y - SCREEN_CENTER_Y, ActFrame ? 0.85 : 0.0);

			m_range(FDc.CameraX, 0.0, FDc.Field_W - SCREEN_W);
			m_range(FDc.CameraY, 0.0, FDc.Field_H - SCREEN_H);

			FDc.ICameraX = m_d2i(FDc.CameraX);
			FDc.ICameraY = m_d2i(FDc.CameraY);

			errorCase(FDc.ICameraX < 0 || FDc.Field_W - SCREEN_W < FDc.ICameraX); // 2bs
			errorCase(FDc.ICameraY < 0 || FDc.Field_H - SCREEN_H < FDc.ICameraY); // 2bs

			// FDc -> CommonEffect
			CameraX = FDc.CameraX;
			CameraY = FDc.CameraY;
		}

		int actionOn;
		int takaraGot = 0;

		// Player
		{
			int dir2 = GetInput(INP_DIR_2);
			int dir4 = GetInput(INP_DIR_4);
			int dir6 = GetInput(INP_DIR_6);
			int dir8 = GetInput(INP_DIR_8);
			int dir = 5; // 2468-1379-5
			int slowOn = GetInput(INP_B);
			int fastOn = GetInput(INP_R);
			actionOn = GetInput(INP_A) == 1;
			int pauseOn = GetInput(INP_PAUSE) == 1;

			if(dir2) dir = 2;
			if(dir4) dir = 4;
			if(dir6) dir = 6;
			if(dir8) dir = 8;
			if(dir2 && dir4) dir = 1;
			if(dir2 && dir6) dir = 3;
			if(dir4 && dir8) dir = 7;
			if(dir6 && dir8) dir = 9;

			double SPEED = 3.0;

			if(slowOn)
				SPEED = 2.0;

			if(fastOn)
				SPEED *= 3.0;

			if(FDc.Player.Pushing)
				SPEED *= 0.5;

			double NANAME_SPEED = SPEED / 1.414213562373;

			switch(dir)
			{
			case 2:
				FDc.Player.Y += SPEED;
				break;

			case 4:
				FDc.Player.X -= SPEED;
				break;

			case 6:
				FDc.Player.X += SPEED;
				break;

			case 8:
				FDc.Player.Y -= SPEED;
				break;

			case 1:
				FDc.Player.X -= NANAME_SPEED;
				FDc.Player.Y += NANAME_SPEED;
				break;

			case 3:
				FDc.Player.X += NANAME_SPEED;
				FDc.Player.Y += NANAME_SPEED;
				break;

			case 7:
				FDc.Player.X -= NANAME_SPEED;
				FDc.Player.Y -= NANAME_SPEED;
				break;

			case 9:
				FDc.Player.X += NANAME_SPEED;
				FDc.Player.Y -= NANAME_SPEED;
				break;

			case 5:
				break;

			default:
				error();
			}
			if(dir != 5 && !slowOn)
				FDc.Player.Dir = dir;

			FDc.Player.Moving = dir != 5;

			/*
				IX, IY ここでセット！
			*/
			FDc.Player.IX = m_nd2i(FDc.Player.X);
			FDc.Player.IY = m_nd2i(FDc.Player.Y);

			if(!FDc.Player.Moving)
			{
				FDc.Player.X = FDc.Player.IX;
				FDc.Player.Y = FDc.Player.IY;
			}

#if LOG_ENABLED
			if(!GetKeyInput(KEY_INPUT_S)) // ★★★S押しっぱなし==壁すり抜け★★★
#endif
			// 壁にめり込み対処
			{
				const int OUTBND = 10;
				int fx = FDc.Player.IX + OUTBND * 32 - 16;
				int fy = FDc.Player.IY + OUTBND * 32 - 16;

				if(fx < 0 || fy < 0)
					goto endPlayerKabeProc;

				int fml = fx / 32 - OUTBND;
				int fmt = fy / 32 - OUTBND;
				int walltbl[2][2];

				for(int xc = 0; xc < 2; xc++)
				for(int yc = 0; yc < 2; yc++)
				{
					int fmx = fml + xc;
					int fmy = fmt + yc; 
					int wall = 0;

					if(
						0 <= fmx && fmx < FDc.FMap->GetWidth() &&
						0 <= fmy && fmy < FDc.FMap->GetHeight()
						)
					{
						wall = FDc.FMap->GetCell(fmx, fmy)->IsWall;
					}
					walltbl[xc][yc] = wall;
				}
				int xPos = FDc.Player.IX - (fml + 1) * 32;
				int yPos = FDc.Player.IY - (fmt + 1) * 32;

				errorCase(xPos < -16 || 16 <= xPos); // 2bs
				errorCase(yPos < -16 || 16 <= yPos); // 2bs

				PKP_XPos = xPos;
				PKP_YPos = yPos;
				PKP_Wall1 = walltbl[0][1];
				PKP_Wall3 = walltbl[1][1];
				PKP_Wall7 = walltbl[0][0];
				PKP_Wall9 = walltbl[1][0];
				PKP_OnWall = 0;

				PlayerKabeProc();

				if(PKP_XPos != xPos || PKP_YPos != yPos)
				{
					FDc.Player.X = PKP_XPos + (fml + 1) * 32;
					FDc.Player.Y = PKP_YPos + (fmt + 1) * 32;

					// 再設定
					FDc.Player.IX = m_d2i(FDc.Player.X);
					FDc.Player.IY = m_d2i(FDc.Player.Y);
				}
				if(PKP_OnWall) // ? 壁の中 -> 脱出する方向で...
				{
					if(rnd(2))
						FDc.Player.X += eRnd() * 32.0;
					else
						FDc.Player.Y += eRnd() * 32.0;

					m_range(FDc.Player.X, 32.0, FDc.Field_W - 48.0);
					m_range(FDc.Player.Y, 32.0, FDc.Field_H - 48.0);

					// 再設定
					FDc.Player.IX = m_d2i(FDc.Player.X);
					FDc.Player.IY = m_d2i(FDc.Player.Y);
				}
endPlayerKabeProc:;
			}
			if(actionOn)
			{
				int fmx = m_d2i(FDc.Player.X) / 32;
				int fmy = m_d2i(FDc.Player.Y) / 32;
				int tFML;
				int tFMT;
				int tFMW;
				int tFMH;

				switch(FDc.Player.Dir)
				{
				case 1:
					tFML = fmx - 1;
					tFMT = fmy;
					tFMW = 2;
					tFMH = 2;
					break;

				case 2:
					tFML = fmx - 1;
					tFMT = fmy + 1;
					tFMW = 3;
					tFMH = 1;
					break;

				case 3:
					tFML = fmx;
					tFMT = fmy;
					tFMW = 2;
					tFMH = 2;
					break;

				case 4:
					tFML = fmx - 1;
					tFMT = fmy - 1;
					tFMW = 1;
					tFMH = 3;
					break;

				case 5: // たぶん無い。
					tFML = fmx;
					tFMT = fmy;
					tFMW = 1;
					tFMH = 1;
					break;

				case 6:
					tFML = fmx + 1;
					tFMT = fmy - 1;
					tFMW = 1;
					tFMH = 3;
					break;

				case 7:
					tFML = fmx - 1;
					tFMT = fmy - 1;
					tFMW = 2;
					tFMH = 2;
					break;

				case 8:
					tFML = fmx - 1;
					tFMT = fmy - 1;
					tFMW = 3;
					tFMH = 1;
					break;

				case 9:
					tFML = fmx;
					tFMT = fmy - 1;
					tFMW = 2;
					tFMH = 2;
					break;

				default:
					error();
				}
				for(int sx = 0; sx < tFMW; sx++)
				for(int sy = 0; sy < tFMH; sy++)
				{
					fmx = tFML + sx;
					fmy = tFMT + sy;

					if(
						0 <= fmx && fmx < FDc.FMap->GetWidth() &&
						0 <= fmy && fmy < FDc.FMap->GetHeight()
						)
					{
						FMCell_t *cell = FDc.FMap->GetCell(fmx, fmy);

						if(IsTakaraSFId(cell->SFId.Surface)) // 箱オープン
						{
							OpenTakara(fmx, fmy);
							cell->SFId.Surface = GetEmptyTakaraSFId(cell->SFId.Surface);

							if(GDc.GlobalTakaraRemainCount == 0) // ? コンプリート
							{
								SEPlay(SE_COMPLETE);
								errorCase(GDc.PlayTimeFrame < 1); // まあ無いだろう...
								GDc.ClearTimeFrame = GDc.PlayTimeFrame;

								if(!Gnd.BestTimeFrame || GDc.ClearTimeFrame < Gnd.BestTimeFrame) // ? 記録が無い || 記録更新
									Gnd.BestTimeFrame = GDc.ClearTimeFrame;

								FDc.CompleteTimeCount = 300;
							}
							else
								SEPlay(SE_GET);

							takaraGot = 1;
							goto endTakaraOpen;
						}
					}
				}
endTakaraOpen:;
			}
			if(pauseOn)
			{
				FreezeInput();
				PauseMain();
				FreezeInput();

				if(FDc.GameEndFlag) // ゲーム終了
				{
					FDc.ExitDir = 5;
					FDc.ExitX = FDc.Player.IX;
					FDc.ExitY = FDc.Player.IY;
					break;
				}
			}
		}
		if(FDc.Field_H < FDc.Player.Y)
		{
			FDc.ExitDir = 2;
			break;
		}
		if(FDc.Player.X < 0.0)
		{
			FDc.ExitDir = 4;
			break;
		}
		if(FDc.Field_W < FDc.Player.X)
		{
			FDc.ExitDir = 6;
			break;
		}
		if(FDc.Player.Y < 0.0)
		{
			FDc.ExitDir = 8;
			break;
		}

		// フィールドマップの変更！
		{
			int frontIX = FDc.Player.IX / 32;
			int frontIY = FDc.Player.IY / 32;

			switch(FDc.Player.Dir)
			{
			case 2: frontIY++; break;
			case 4: frontIX--; break;
			case 6: frontIX++; break;
			case 8: frontIY--; break;

			case 1: frontIX--; frontIY++; break;
			case 3: frontIX++; frontIY++; break;
			case 7: frontIX--; frontIY--; break;
			case 9: frontIX++; frontIY--; break;

			default:
				error();
			}
			if(actionOn && takaraGot == 0)
			{
				if(EditField(FDc.Player.Action.EFId, frontIX, frontIY))
				{
					double x = frontIX * 32 + 16;
					double y = frontIY * 32 + 16;

					CEE.LastPicId = D_MAGICSTAR_00_LAST | DTP;
					AddCommonEffect(Gnd.EL, 0, D_MAGICSTAR_00 | DTP, x, y);
					CEE_Reset();

					SEPlay(SE_MAGIC);
				}
			}
		}

		int plRepulTell = 0;

		ClearRepulsion();
		Repulsion(FDc.Player.X, FDc.Player.Y, &plRepulTell);

		EachFrameAllEnemy();
		EachFrameAllWeapon();

		FDc.Player.Pushing = plRepulTell;

		// BeToTop 処理
		{
			for(int index = 0; index < FDc.EnemyList->GetCount(); index++)
			{
				Enemy_t *e = FDc.EnemyList->GetElement(index);

				if(e->BeToTop)
				{
					if(1 <= index)
						FDc.EnemyList->Swap(index - 1, index);

					e->BeToTop = 0;
				}
			}
		}

		/*
			★★★ テスト操作 ★★★ ここから
		*/
#if LOG_ENABLED // test
		if(GetKeyPound(KEY_INPUT_NUMPAD2))
		{
			FDc.ExitDir = 2;
			break;
		}
		if(GetKeyPound(KEY_INPUT_NUMPAD4))
		{
			FDc.ExitDir = 4;
			break;
		}
		if(GetKeyPound(KEY_INPUT_NUMPAD5))
		{
			for(int x = 0; x < MAP_W; x++)
			for(int y = 0; y < MAP_H; y++)
			{
				if(GDc.MapX != x || GDc.MapY != y)
				{
					for(int index = 0; index < 500; index++)
					{
						GDc.TakaraGotTable->GetCell(x, y)->PutBit(index, 1);
					}
				}
			}

			UpdateGlobalTakaraCount();
			UpdateLocalTakaraCount();
		}
		if(GetKeyPound(KEY_INPUT_NUMPAD6))
		{
			FDc.ExitDir = 6;
			break;
		}
		if(GetKeyPound(KEY_INPUT_NUMPAD8))
		{
			FDc.ExitDir = 8;
			break;
		}
		if(GetKeyPound(KEY_INPUT_MULTIPLY))
		{
			GDc.DaylightMode = DL_DAYTIME;
		}
		if(GetKeyPound(KEY_INPUT_SUBTRACT))
		{
			GDc.DaylightMode = DL_SUNSET;
		}
		if(GetKeyPound(KEY_INPUT_ADD))
		{
			GDc.DaylightMode = DL_NIGHT;
		}
#endif
		/*
			★★★ テスト操作 ★★★ ここまで
		*/

		EachFrameDaylightChecker();
		EachFrameDaylightFilter();

		if(!Gnd.ServerConnectDisabled)
		{
			int fieldX = FDc.Player.IX / 32;
			int fieldY = FDc.Player.IY / 32;

			EachFrameMapUpdater(GDc.MapX, GDc.MapY, fieldX, fieldY);
		}

		// Draw ...

//		DrawCurtain(); // 不要
		DrawField(1);

		int iPlayerCamX = m_d2i(FDc.Player.X - FDc.CameraX);
		int iPlayerCamY = m_d2i(FDc.Player.Y - FDc.CameraY);

		// Player
		{
			int dirIndex = 0;
			switch(FDc.Player.Dir)
			{
			case 2: dirIndex = 0; break;
			case 4: dirIndex = 1; break;
			case 6: dirIndex = 2; break;
			case 8: dirIndex = 3; break;
			case 1: dirIndex = 16; break;
			case 3: dirIndex = 18; break;
			case 7: dirIndex = 17; break;
			case 9: dirIndex = 19; break;

			default:
				error();
			}
			int animIndex = 1;
			if(FDc.Player.Moving)
			{
				animIndex = (ActFrame / 5) % 4;
				if(animIndex == 3) animIndex = 1;
			}

			DPE.IntPosOn = 1;
			DrawCenter(
				D_ACTOR83_00 + 3 * dirIndex + animIndex | DTP,
				iPlayerCamX,
				iPlayerCamY
				);
			DPE_Reset();
		}
		FDc.DrawEnemy->ExecuteAllTask();
		FDc.DrawFront->ExecuteAllTask();
		DrawDaylightFilter();

		if(FDc.CompleteTimeCount)
		{
			int FADE_MAX = 100;
			double a = 0.7;

			if(FDc.CompleteTimeCount < FADE_MAX)
				a *= FDc.CompleteTimeCount / (double)FADE_MAX;

			DPE_SetAlpha(a);
			DPE_SetBright(255, 255, 128);
			DrawRect(P_WHITEBOX, 180, 200, 440, 200);
			DPE_Reset();

			if(FADE_MAX / 2 < FDc.CompleteTimeCount)
			{
				PE_Border(GetColor(100, 100, 0));
				SetPrint(265, 300 - 8);
				Print("回収コンプリート　おめでとう！！");
				PE_Reset();
			}
			m_countDown(FDc.CompleteTimeCount);
		}

		// monitor >

		DPE_SetAlpha(0.3);
		DPE_SetBright(0, 0, 0);
		DrawRect(P_WHITEBOX, 665, 0, 135, 45);
		DPE_Reset();

		PE_Border(GetColor(64, 64, 16));
		SetPrint(685, 5);
		Print_x(xcout("%4d / %4d"
			,GDc.GlobalTakaraTotalCount - GDc.GlobalTakaraRemainCount
			,GDc.GlobalTakaraTotalCount
			));
		PE_Border(GetColor(16, 96, 24));
		SetPrint(685, 25);
		Print_x(xcout("%4d / %4d"
			,FDc.LocalTakaraTotalCount - FDc.LocalTakaraRemainCount
			,FDc.LocalTakaraTotalCount
			));
		PE_Reset();

		{
			char *message = GetSockErrorMessage();

			if(*message)
			{
				PE_Border(GetColor(255, 0, 0));
				PE.Color = GetColor(255, 255, 0);
				SetPrint(0, 0);
				Print("NETWORK_PROBLEM_");
				Print(GetSockErrorMessage());
				PrintRet();
				PE_Reset();
			}
			else if(WarningMessage)
			{
				PE_Border(GetColor(255, 0, 0));
				PE.Color = GetColor(255, 255, 0);
				SetPrint(0, 0);
				Print(WarningMessage);
				PrintRet();
				PE_Reset();

				if(m_countDown(WM_RemFrame) == 1)
				{
					WarningMessage = NULL;
				}
			}
		}

		// < monitor

#if LOG_ENABLED // debug
		SetPrint(0, 16);
		Print_x(xcout("Map: %d %d (%d %d), ", GDc.MapX, GDc.MapY, FDc.FMap->GetWidth(), FDc.FMap->GetHeight()));
		Print_x(xcout("Pos: %.3f %.3f (%.3f %.3f), ", FDc.Player.X, FDc.Player.Y, FDc.Player.X / 32.0, FDc.Player.Y / 32.0));
		Print_x(xcout("%d:%02d:%02d", GDc.PlayTimeFrame / 3600, (GDc.PlayTimeFrame / 60) % 60, GDc.PlayTimeFrame % 60));
		PrintRet();
		Print_x(xcout("%I64d (%I64x) ", FrameStartTime, FrameStartTime));
		Print_x(xcout("DL:(%.3f %.3f %.3f %.3f)%d:%d", GDc.Daylight.R, GDc.Daylight.G, GDc.Daylight.B, GDc.Daylight.A, GDc.DaylightMode, EFDC_SleepFrame));
		PrintRet();
		Print_x(xcout("ConnectWaitMillis: %d, SCount: %d, RCount: %d, [", GetSockConnectWaitMillis(), MU_GetSCount(), MU_GetRCount()));
		Print(GetSockInfoMessage());
		Print("], [");
		Print(GetSockErrorMessage());
		Print("]");
		PrintRet();
		Print_x(xcout("E: %d, W: %d", FDc.EnemyList->GetCount(), FDc.WeaponList->GetCount()));
		PrintRet();
#endif

		EachFrame();

		GDc.PlayTimeFrame++;
		m_range(GDc.PlayTimeFrame, 0, PLAYTIMEFRAME_MAX);
	}

	if(!Gnd.ServerConnectDisabled) // ? サーバーに接続する。
	if(FDc.GameEndFlag) // ? 次はタイトルメニューに戻る。
	if(MU_GetSCount()) // ? 未送信のデータ有り
	{
		while(MU_GetSCount())
		{
			DrawField();
			DrawDaylightFilter();

			DPE_SetAlpha(0.5);
			DPE_SetBright(0, 0, 0);
			DrawRect(P_WHITEBOX, 0, 0, 580, 50);
			DPE_Reset();
			
			PE_Border();
			SetPrint(16, 16);
			Print_x(xcout("未送信データがあります！　あと %.3f 秒くらいお待ち下さい...", GetSockConnectWaitMillis() / 1000.0));
			PE_Reset();

			EachFrameMapUpdater(GDc.MapX, GDc.MapY, 0, 0);
			EachFrame();
		}
	}

	FreezeInput();
	SetCurtain(30, -1.0);

	forscene(40)
	{
		DrawField();
		DrawDaylightFilter();
		EachFrame();
	}
	sceneLeave();

	// 戻す。
	CameraX = 0.0;
	CameraY = 0.0;
}
