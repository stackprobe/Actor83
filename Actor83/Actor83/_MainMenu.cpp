#include "all.h"

void Logo(void)
{
	forscene(30)
	{
		DPE_SetBright(0.0, 0.0, 0.0);
		DrawRect(P_WHITEBOX, 0, 0, SCREEN_W, SCREEN_H);
		DPE_Reset();

		EachFrame();
	}
	sceneLeave();

	double z1 = 0.3;
	double z2 = 2.0;
	double z3 = 3.7;

	forscene(60)
	{
		DPE_SetBright(0.0, 0.0, 0.0);
		DrawRect(P_WHITEBOX, 0, 0, SCREEN_W, SCREEN_H);
		DPE_Reset();

		/*
		DPE_SetAlpha(sc_rate);
		DrawCenter(P_COPYRIGHT, SCREEN_CENTER_X, SCREEN_CENTER_Y);
		DPE_Reset();
		*/

		DPE_SetAlpha(sc_rate);
		DrawBegin(P_COPYRIGHT, SCREEN_CENTER_X, SCREEN_CENTER_Y);
		DrawZoom(z1);
		DrawEnd();
		DPE_Reset();

		DPE_SetAlpha((1.0 - sc_rate) * 0.7);
		DrawBegin(P_COPYRIGHT, SCREEN_CENTER_X, SCREEN_CENTER_Y);
		DrawZoom(0.8 + 0.5 * sc_rate);
		DrawEnd();
		DPE_Reset();

		DPE_SetAlpha((1.0 - sc_rate) * 0.5);
		DrawBegin(P_COPYRIGHT, SCREEN_CENTER_X, SCREEN_CENTER_Y);
		DrawZoom(z2);
		DrawEnd();
		DPE_Reset();

		DPE_SetAlpha((1.0 - sc_rate) * 0.3);
		DrawBegin(P_COPYRIGHT, SCREEN_CENTER_X, SCREEN_CENTER_Y);
		DrawZoom(z3);
		DrawEnd();
		DPE_Reset();

		m_approach(z1, 1.0, 0.9);
		m_approach(z2, 1.0, 0.98);
		m_approach(z3, 1.0, 0.95);

		EachFrame();
	}
	sceneLeave();

	__int64 lpBgnTm = GetCurrTime();

	forscene(90)
	{
		if(lpBgnTm + 1500 < GetCurrTime())
		{
LOG("[lpBgnTm-OF] %u\n", sc_numer);
			break;
		}
		if(sc_numer == 10)
		{
SW_Start();
			MusicTouch(MUS_TITLE);
SW_End();
		}
		DPE_SetBright(0.0, 0.0, 0.0);
		DrawRect(P_WHITEBOX, 0, 0, SCREEN_W, SCREEN_H);
		DPE_Reset();

		DrawCenter(P_COPYRIGHT, SCREEN_CENTER_X, SCREEN_CENTER_Y);

		EachFrame();
	}
	sceneLeave();
	forscene(60)
	{
		DPE_SetBright(0.0, 0.0, 0.0);
		DrawRect(P_WHITEBOX, 0, 0, SCREEN_W, SCREEN_H);
		DPE_Reset();

		/*
		DPE_SetAlpha(1.0 - sc_rate);
		DrawCenter(P_COPYRIGHT, SCREEN_CENTER_X, SCREEN_CENTER_Y);
		DPE_Reset();
		*/

		DPE_SetAlpha((1.0 - sc_rate) * 0.5);
		DrawBegin(P_COPYRIGHT, SCREEN_CENTER_X, SCREEN_CENTER_Y);
		DrawZoom(1.0 - 0.3 * sc_rate);
		DrawRotate(sc_rate * -0.1);
		DrawEnd();
		DPE_Reset();

		DPE_SetAlpha((1.0 - sc_rate) * 0.5);
		DrawBegin(P_COPYRIGHT, SCREEN_CENTER_X, SCREEN_CENTER_Y);
		DrawZoom(1.0 + 0.8 * sc_rate);
		DrawRotate(sc_rate * 0.1);
		DrawEnd();
		DPE_Reset();

		DPE_SetAlpha((1.0 - sc_rate) * 0.3);
		DrawCenter(P_COPYRIGHT, SCREEN_CENTER_X + sc_rate * 100.0, SCREEN_CENTER_Y);
		DPE_Reset();

		DPE_SetAlpha((1.0 - sc_rate) * 0.3);
		DrawCenter(P_COPYRIGHT, SCREEN_CENTER_X, SCREEN_CENTER_Y + sc_rate * 50.0);
		DPE_Reset();

		EachFrame();
	}
	sceneLeave();
}
static int DTW_WallPicId = P_TITLEWALL_MIDORI;

static void DrawTitleWall(void)
{
	double dx = cos(ActFrame / 199.0) * 40.0;
	double dy = cos(ActFrame / 211.0) * 30.0;

	DrawBegin(DTW_WallPicId, SCREEN_CENTER_X + dx, SCREEN_CENTER_Y + dy);
	DrawZoom(1.1);
	DrawEnd();

	if(dRnd() < 0.03 + sin(ActFrame / 307.0) * 0.02)
	{
		CEE.Z_B = 1.1;
		CEE.SlideX = dx;
		CEE.SlideY = dy;
		AddCommonEffect(
			Gnd.EL, 1, DTW_WallPicId,
			SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0, 1.0, 0.3,
			eRnd() * 0.1,
			eRnd() * 0.1,
			dRnd() < 0.1 ? eRnd() * 0.0015 : 0.0,
			eRnd() * 0.0003 + 0.0001,
			-0.001 - dRnd() * 0.001
			);
		CEE_Reset();
	}
	Gnd.EL->ExecuteAllTask();

	double titleX = 600.0 + dx * 0.4;
	double titleY = 300.0 + dy * 0.4;

	double tba = 0.5 + sin(ActFrame / 103.0) * 0.185 + sin(ActFrame / 3.0) * 0.015 * sin(ActFrame / 107.0);
	double tfa = 0.3;

	ifsceneBegin(300, ActFrame)
	{
		DPE_SetBlendAdd(tba * sc_rate);
		DrawCenter(P_TITLE, titleX, titleY);
		DPE_Reset();
	}
	ifsceneEnd
	{
		DPE_SetBlendAdd(tba);
		DrawCenter(P_TITLE, titleX, titleY);
		DPE_Reset();
	}
	sceneLeave();

	ifsceneBegin(700, ActFrame)
	{
		DPE_SetAlpha(tfa * sc_rate);
		DrawCenter(P_TITLE, titleX, titleY);
		DPE_Reset();
	}
	ifsceneEnd
	{
		DPE_SetAlpha(tfa);
		DrawCenter(P_TITLE, titleX, titleY);
		DPE_Reset();
	}
	sceneLeave();
}
static void DrawTitleWall_Curtain(void)
{
	DrawTitleWall();
	CurtainEachFrame();
}

// ---- SettingMenu ----

#if 1

static int SM_GetPressPadBtnId(void)
{
	int padId = Gnd.PrimaryPadId;

	if(padId == -1) // ? 未定義
		padId = 0; // 適当なデフォ

	for(int btnId = 0; btnId < PAD_BUTTON_MAX; btnId++)
	{
		if(GetPadInput(padId, btnId) == 1)
		{
			return btnId;
		}
	}
	return -1; // not found
}
static void SM_PadConfig(void)
{
	int *pPadBtns[] =
	{
		&Gnd.PadBtnno.Dir_8,
		&Gnd.PadBtnno.Dir_2,
		&Gnd.PadBtnno.Dir_4,
		&Gnd.PadBtnno.Dir_6,
		&Gnd.PadBtnno.A,
		&Gnd.PadBtnno.B,
		&Gnd.PadBtnno.R,
		&Gnd.PadBtnno.Pause,
	};
	char *btnNames[] =
	{
		"上",
		"下",
		"左",
		"右",
		"決定／アクション",
		"キャンセル／低速移動",
		"高速移動",
		"ポーズ",
	};
	errorCase(lengthof(pPadBtns) != lengthof(btnNames));
	int btnMax = lengthof(pPadBtns);
	int btnIndex = 0;
	void *backup = memClone(&Gnd.PadBtnno, sizeof(Gnd.PadBtnno));

	FreezeInput();

	while(btnIndex < btnMax)
	{
		if(
			GetKeyInput(KEY_INPUT_Z) == 1 ||
			GetKeyInput(KEY_INPUT_X) == 1
			)
		{
			memcpy(&Gnd.PadBtnno, backup, sizeof(Gnd.PadBtnno)); // 復元
			break;
		}

		{
			int btnId = SM_GetPressPadBtnId();

			if(btnId != -1)
			{
				*pPadBtns[btnIndex] = btnId;
				btnIndex++;
			}
		}

		DrawTitleWall_Curtain();

		PE_Border(GetColor(0, 100, 0));
		SetPrint(60, 70, 40);
		Print("パッドのボタン設定");
		PrintRet();

		for(int index = 0; index < btnMax; index++)
		{
			Print_x(xcout("[%c] %sボタン", index == btnIndex ? '>' : ' ', btnNames[index]));

			if(index < btnIndex)
			{
				Print_x(xcout(" -> %d", *pPadBtns[index]));
			}
			PrintRet();
		}
		PrintRet();
		Print("★ カーソルが指すボタンを押して下さい。");
		PrintRet();
		Print("★ ＺキーまたはＸキーを押すとキャンセルします。");
		PE_Reset();

		EachFrame();
	}
	FreezeInput();

	memFree(backup);
}
static void SM_WindowSizeChange(int *pSizeValue, int heightMode)
{
	int sizeOrig = *pSizeValue;

	FreezeInput();

	for(; ; )
	{
		/*
			*p++; == *(p++); 後置が先よ！
		*/

		if(GetPound(INP_DIR_4))
			*pSizeValue -= 1;

		if(GetPound(INP_DIR_6))
			*pSizeValue += 1;

		if(GetPound(INP_DIR_2))
			*pSizeValue -= 10;

		if(GetPound(INP_DIR_8))
			*pSizeValue += 10;

		if(GetPound(INP_B))
		{
			if(*pSizeValue == sizeOrig)
				break;

			*pSizeValue = sizeOrig;
		}
		if(heightMode)
			m_range(*pSizeValue, SCREEN_H, SCREEN_H_MAX);
		else
			m_range(*pSizeValue, SCREEN_W, SCREEN_W_MAX);

		if(GetPound(INP_A))
			break;

		DrawTitleWall_Curtain();

		PE_Border(GetColor(0, 100, 0));
		SetPrint(60, 270, 40);
		Print_x(xcout("ウィンドウの%s", heightMode ? "高さ" : "幅"));
		PrintRet();
		Print_x(xcout("[ %4d ] ピクセル", *pSizeValue));
		PE_Reset();

		EachFrame();
	}
	FreezeInput();
}

static int SM_Screen_W;
static int SM_Screen_H;

static void SM_WindowSizeCustom(void)
{
	const int selectMax = 4;
	int selectIndex = 0;

	SM_Screen_W = Gnd.RealScreen_W;
	SM_Screen_H = Gnd.RealScreen_H;

	FreezeInput();

	for(; ; )
	{
		if(GetPound(INP_DIR_8))
			selectIndex--;

		if(GetPound(INP_DIR_2))
			selectIndex++;

		if(GetPound(INP_B))
		{
			if(selectIndex == selectMax - 1)
				break;

			selectIndex = selectMax - 1;
		}
		m_range(selectIndex, 0, selectMax - 1);

		if(GetPound(INP_A))
		{
			switch(selectIndex)
			{
			case 0:
				SM_WindowSizeChange(&SM_Screen_W, 0);
				break;

			case 1:
				SM_WindowSizeChange(&SM_Screen_H, 1);
				break;

			case 2:
				SetScreenSize(SM_Screen_W, SM_Screen_H);
				break;

			case 3:
				goto endLoop;

			default:
				error();
			}
		}
		DrawTitleWall_Curtain();

		PE_Border(GetColor(0, 100, 0));
		SetPrint(60, 210, 40);
		Print("ウィンドウサイズの設定／サイズを指定する");
		PrintRet();
		Print_x(xcout("[%c]　幅 ( %d )", selectIndex == 0 ? '>' : ' ', SM_Screen_W));
		PrintRet();
		Print_x(xcout("[%c]　高さ ( %d )", selectIndex == 1 ? '>' : ' ', SM_Screen_H));
		PrintRet();
		Print_x(xcout("[%c]　変更を反映する", selectIndex == 2 ? '>' : ' '));
		PrintRet();
		Print_x(xcout("[%c]　戻る", selectIndex == 3 ? '>' : ' '));
		PE_Reset();

		EachFrame();
	}
endLoop:
	FreezeInput();
}
static void SM_WindowSizeSetting(void)
{
	// zantei?
	{
		int mdm = GetMouseDispMode();
		errorCase(SetWindowSizeExtendRate(1.0)); // ? 失敗
		SetMouseDispMode(mdm);
	}

	const int selectMax = 4;
	int selectIndex = 0;

	FreezeInput();

	for(; ; )
	{
		if(GetPound(INP_DIR_8))
			selectIndex--;

		if(GetPound(INP_DIR_2))
			selectIndex++;

		if(GetPound(INP_B))
		{
			if(selectIndex == selectMax - 1)
				break;

			selectIndex = selectMax - 1;
		}
		m_range(selectIndex, 0, selectMax - 1);

		if(GetPound(INP_A))
		{
			switch(selectIndex)
			{
			case 0:
				SetScreenSize(SCREEN_W, SCREEN_H); // デフォルト
				break;

			case 1:
				// 擬似フルスクリーン
				SetScreenSize(
					GetSystemMetrics(SM_CXSCREEN),
					GetSystemMetrics(SM_CYSCREEN)
					);
				break;

			case 2:
				SM_WindowSizeCustom();
				break;

			case 3:
				goto endLoop;

			default:
				error();
			}
		}
		DrawTitleWall_Curtain();

		PE_Border(GetColor(0, 100, 0));
		SetPrint(60, 210, 40);
		Print("ウィンドウサイズの設定");
		PrintRet();
		Print_x(xcout("[%c]　デフォルトのサイズ", selectIndex == 0 ? '>' : ' '));
		PrintRet();
		Print_x(xcout("[%c]　擬似フルスクリーン", selectIndex == 1 ? '>' : ' '));
		PrintRet();
		Print_x(xcout("[%c]　サイズを指定する", selectIndex == 2 ? '>' : ' '));
		PrintRet();
		Print_x(xcout("[%c]　戻る", selectIndex == 3 ? '>' : ' '));
		PE_Reset();

		EachFrame();
	}
endLoop:
	FreezeInput();
}
static void SM_WindowSizeRate(void)
{
	const int rateDenom = 100;
	int rateOrig = m_d2i(GetWindowSizeExtendRate() * rateDenom);
	int rate = rateOrig;

	FreezeInput();

	for(; ; )
	{
		/*
			*p++; == *(p++); 後置が先よ！
		*/

		if(GetPound(INP_DIR_4))
			rate -= 1;

		if(GetPound(INP_DIR_6))
			rate += 1;

		if(GetPound(INP_DIR_2))
			rate -= 10;

		if(GetPound(INP_DIR_8))
			rate += 10;

		if(GetPound(INP_B))
		{
			if(rate == rateOrig)
				break;

			rate = rateOrig;
		}
		m_range(rate, 50, 500);

		if(GetPound(INP_A))
		{
			// レート反映
			{
				SetScreenSize(SCREEN_W, SCREEN_H); // デフォルト
				int mdm = GetMouseDispMode();
				errorCase(SetWindowSizeExtendRate(rate / (double)rateDenom)); // ? 失敗
				SetMouseDispMode(mdm);
			}
			break;
		}
		DrawTitleWall_Curtain();

		PE_Border(GetColor(0, 100, 0));
		SetPrint(60, 270, 40);
		Print("ウィンドウサイズの設定／レートを指定する");
		PrintRet();
		Print_x(xcout("[ %2d.%02d ]", rate / rateDenom, rate % rateDenom));
		PE_Reset();

		EachFrame();
	}
	FreezeInput();
}
static void SM_VolumeSetting(int seMode)
{
	double dvol = seMode ? Gnd.SEVolume : Gnd.MusicVolume;
	int volume = m_d2i(dvol * 100);
	int volorig = volume;
	int lastvol = volume;

	FreezeInput();

	for(; ; )
	{
		if(GetPound(INP_DIR_4))
			volume--;

		if(GetPound(INP_DIR_6))
			volume++;

		if(GetPound(INP_DIR_2))
			volume -= 10;

		if(GetPound(INP_DIR_8))
			volume += 10;

		if(GetPound(INP_B))
		{
			if(volume == volorig)
				break;

			volume = volorig;
		}
		m_range(volume, 0, 100);

		if(GetPound(INP_A))
			break;

		if(lastvol != volume)
		{
			if(seMode)
			{
				Gnd.SEVolume = volume / 100.0;
				UpdateSEVolume();
			}
			else
			{
				Gnd.MusicVolume = volume / 100.0;
				UpdateMusicVolume();
			}
			lastvol = volume;
		}
		if(seMode && ProcFrame % 60 == 0)
		{
			SEPlay(SE_GET);
		}
		DrawTitleWall_Curtain();

		PE_Border(GetColor(0, 100, 0));
		SetPrint(60, 270, 40);
		Print_x(xcout("%sボリューム設定", seMode ? "ＳＥ" : "ＢＧＭ"));
		PrintRet();
		Print_x(xcout("[ %3d ]", volume));
		PE_Reset();

		EachFrame();
	}
	FreezeInput();
}
static void SettingMenu(void)
{
	const int selectMax = 8;
	int selectIndex = 0;

	FreezeInput();

	for(; ; )
	{
		if(GetPound(INP_DIR_8))
			selectIndex--;

		if(GetPound(INP_DIR_2))
			selectIndex++;

		if(GetPound(INP_B))
		{
			if(selectIndex == selectMax - 1)
				break;

			selectIndex = selectMax - 1;
		}
		m_range(selectIndex, 0, selectMax - 1);

		if(GetPound(INP_A))
		{
			switch(selectIndex)
			{
			case 0:
				SM_PadConfig();
				break;

			case 1:
				SM_WindowSizeSetting();
				break;

//			case 2:
//				SM_WindowSizeRate();
//				break;

			case 2:
				SM_VolumeSetting(0);
				break;

			case 3:
				SM_VolumeSetting(1);
				break;

			case 4:
				SetMouseDispMode(GetMouseDispMode() ? 0 : 1);
				break;

			case 5:
				Gnd.FieldDaylightDisabled = Gnd.FieldDaylightDisabled ? 0 : 1;
				break;

			case 6:
				Gnd.ServerConnectDisabled = Gnd.ServerConnectDisabled ? 0 : 1;
				break;

			case 7:
				goto endLoop;

			default:
				error();
			}
		}
		DrawTitleWall_Curtain();

		PE_Border(GetColor(0, 100, 0));
		SetPrint(60, 130, 40);
		Print("設定");
		PrintRet();
		int c = 0;
		Print_x(xcout("[%c]　パッドのボタン設定", selectIndex == c++ ? '>' : ' '));
		PrintRet();
		Print_x(xcout("[%c]　ウィンドウサイズの変更", selectIndex == c++ ? '>' : ' '));
		PrintRet();
//		Print_x(xcout("[%c]　ウィンドウサイズのレート (暫定)", selectIndex == c++ ? '>' : ' '));
//		PrintRet();
		Print_x(xcout("[%c]　ＢＧＭボリューム設定", selectIndex == c++ ? '>' : ' '));
		PrintRet();
		Print_x(xcout("[%c]　ＳＥボリューム設定", selectIndex == c++ ? '>' : ' '));
		PrintRet();
		Print_x(xcout("[%c]　マウスカーソルを%sする (現在は%s)", selectIndex == c++ ? '>' : ' '
			,GetMouseDispMode() ? "非表示に" : "表示"
			,GetMouseDispMode() ? "表示" : "非表示"
			));
		PrintRet();
		Print_x(xcout("[%c]　フィールド時間帯を%sにする (現在は%s)", selectIndex == c++ ? '>' : ' '
			,Gnd.FieldDaylightDisabled ? "有効" : "無効"
			,Gnd.FieldDaylightDisabled ? "無効" : "有効"
			));
		PrintRet();
		Print_x(xcout("[%c]　サーバーに接続%s (現在は接続%s)", selectIndex == c++ ? '>' : ' '
			,Gnd.ServerConnectDisabled ? "する" : "しない"
			,Gnd.ServerConnectDisabled ? "しない" : "する"
			));
		PrintRet();
		Print_x(xcout("[%c]　戻る", selectIndex == c++ ? '>' : ' '));
		PE_Reset();

		EachFrame();
	}
endLoop:
	FreezeInput();
}

#else

static void SM_ChangeMouseDispMode(int dummy)
{
	SetMouseDispMode(!GetMouseDispMode());
}
static void SM_ResetScreenSize(void)
{
	Gnd.RealScreen_W = SCREEN_W;
	Gnd.RealScreen_H = SCREEN_H;

	ApplyScreenSize();
}
static void SM_QuasiFullScreenMode(void)
{
	int w = GetSystemMetrics(SM_CXSCREEN);
	int h = GetSystemMetrics(SM_CYSCREEN);

	m_range(w, SCREEN_W, SCREEN_W_MAX);
	m_range(h, SCREEN_H, SCREEN_H_MAX);

	Gnd.RealScreen_W = w;
	Gnd.RealScreen_H = h;

	ApplyScreenSize();
}
static double SM_GetMusicVolume(void)
{
	return Gnd.MusicVolume;
}
static void SM_AddMusicVolume(int volume)
{
	Gnd.MusicVolume += volume * 0.01;
	m_accur(Gnd.MusicVolume, 100);
	m_range(Gnd.MusicVolume, 0.0, 1.0);
	MusicFade(1, 1.0);
}
static double SM_GetSEVolume(void)
{
	return Gnd.SEVolume;
}
static void SM_AddSEVolume(int volume)
{
	Gnd.SEVolume += volume * 0.01;
	m_accur(Gnd.SEVolume, 100);
	m_range(Gnd.SEVolume, 0.0, 1.0);
	UpdateSEVolume();
}
static void SM_PadButtonSetting(void)
{
	error(); // todo
}
static void SM_MenuScreenSize(void)
{
	PublicMenu *pm = new PublicMenu("ウィンドウサイズの変更");

	pm->AddInt("ウィンドウ幅", &Gnd.RealScreen_W, SCREEN_W, SCREEN_W_MAX, 1);
	pm->AddInt("ウィンドウ高", &Gnd.RealScreen_H, SCREEN_H, SCREEN_H_MAX, 1);
	pm->AddFunc("ウィンドウサイズの反映", ApplyScreenSize);

	pm->Show(DrawTitleWall_Curtain);

	delete pm;
}
static void SettingMenu(void)
{
	PublicMenu *pm = new PublicMenu("設定");

	pm->AddFunc("パッドのボタン設定", SM_PadButtonSetting);
	pm->AddIntFunc("マウスカーソルを表示", GetMouseDispMode, SM_ChangeMouseDispMode);
	pm->AddFunc("擬似フルスクリーン", SM_QuasiFullScreenMode);
	pm->AddFunc("ウィンドウサイズの復元", SM_ResetScreenSize);
	pm->AddFunc("ウィンドウサイズの変更", SM_MenuScreenSize);
	pm->AddDoubleFunc("ＢＧＭボリューム", SM_GetMusicVolume, SM_AddMusicVolume);
	pm->AddDoubleFunc("効果音ボリューム", SM_GetSEVolume, SM_AddSEVolume);

	pm->Show(DrawTitleWall_Curtain);

	delete pm;
}

#endif

// ----

static void LeaveMainMenu(void)
{
	MusicFade();
	SetCurtain(30, -1.0);

	forscene(40)
	{
		DrawTitleWall();
		EachFrame();
	}
	sceneLeave();

	Gnd.EL->Clear();
}
static void ReturnMainMenu(void)
{
	1; // noop
}
static void StartNewGame(void)
{
	GameInit();
	GameMain();
	GameFnlz();
}
static void ContinueGame(void)
{
	GameInit();
	GDc.LoadFlag = 1;
	GameMain();
	GameFnlz();
}
static void ContinueGame_NoData(void)
{
	FreezeInput();

	for(; ; )
	{
		if(
			GetInput(INP_A) == 1 ||
			GetInput(INP_B) == 1
			)
			break;

		DrawTitleWall_Curtain();

		PE_Border(GetColor(96, 0, 0));
		SetPrint(120, 292);
		Print("セーブデータがありません...");
		PE_Reset();

		EachFrame();
	}
	FreezeInput();
}

// ----

typedef struct ItemPos_st
{
	double X;
	double Y;
	double A;
	double RealX;
	double RealY;
	double RealA;
}
ItemPos_t;

#define ITEM_MAX 4

static ItemPos_t ItemPosList[ITEM_MAX];

void MainMenu(void)
{
	int selectIndex = 0;

	if(Gnd.PlayData.SavedFlag)
		selectIndex = 1;

restart:
	ActFrame = 0;
	FreezeInput();

	MusicPlay(MUS_TITLE);
	SetCurtain(30, -1.0, -1.0);
	SetCurtain(30, 0.0, -1.0);

	// Set DRW_WallPicId
	{
		time_t t = time(NULL);
		struct tm *lt = localtime(&t);
		errorCase(!lt);

		int hm = lt->tm_hour * 100 + lt->tm_min;

		if(hm < 600)
			DTW_WallPicId = P_FOREST_YORU;
		else if(hm < 1630)
			DTW_WallPicId = P_FOREST_HIRU;
		else if(hm < 1930)
			DTW_WallPicId = P_FOREST_YUUGATA;
		else
			DTW_WallPicId = P_FOREST_YORU;

		int month = lt->tm_mon + 1; // 1 - 12
		int day = lt->tm_mday; // 1 - 31

		if(
			month == 9 && day == 3 // 草の日
			)
			DTW_WallPicId = P_TITLEWALL_MIDORI;
	}
//	DTW_WallPicId = P_FOREST_HIRU; // test
//	DTW_WallPicId = P_FOREST_YUUGATA; // test
//	DTW_WallPicId = P_FOREST_YORU; // test
//	DTW_WallPicId = P_TITLEWALL_MIDORI;

	const double ITEM_NS_X = 120.0;
	const double ITEM_NS_A = 0.5;
	const double ITEM_S_X = 140.0;
	const double ITEM_S_A = 1.0;
	const double ITEM_Y = 225.0;
	const double ITEM_Y_STEP = 50.0;

	for(int index = 0; index < ITEM_MAX; index++)
	{
		ItemPos_t *i = ItemPosList + index;

		i->X = ITEM_NS_X;
		i->Y = ITEM_Y + index * ITEM_Y_STEP;
		i->A = ITEM_NS_A;
		i->RealX = -100.0 - index * 200.0;
		i->RealY = i->Y;
		i->RealA = ITEM_NS_A;
	}
	for(; ; )
	{
		if(GetPound(INP_DIR_8))
			selectIndex--;

		if(GetPound(INP_DIR_2))
			selectIndex++;

		m_range(selectIndex, 0, ITEM_MAX - 1);

		if(GetPound(INP_A))
		{
			switch(selectIndex)
			{
			case 0:
				LeaveMainMenu();
				StartNewGame();
				ReturnMainMenu();
				goto restart;

			case 1:
				if(Gnd.PlayData.SavedFlag)
				{
					LeaveMainMenu();
					ContinueGame();
					ReturnMainMenu();
					goto restart;
				}
				SetCurtain(30, -0.5);
				ContinueGame_NoData();
				SetCurtain();
				break;

			case 2:
				SetCurtain(30, -0.5);
				SettingMenu();
				SetCurtain();
				break;

			case 3:
				goto endMenu;

			default:
				error();
			}
		}
		if(GetPound(INP_B))
		{
			if(selectIndex == ITEM_MAX - 1)
				break;

			selectIndex = ITEM_MAX - 1;
		}
		DrawTitleWall();

		/*
		if(!Gnd.ServerConnectDisabled && MU_GetSCount())
		{
			PE_Border(GetColor(128, 0, 0));
			PE.Color = GetColor(255, 255, 128);
			SetPrint(0, 0);
			Print("未送信のデータがあります！");
			PE_Reset();
		}
		*/
		if(Gnd.BestTimeFrame) // ベストタイム表示
		{
			int h = Gnd.BestTimeFrame / (60 * 60 * 60);
			int m = (Gnd.BestTimeFrame / (60 * 60)) % 60;
			int s = (Gnd.BestTimeFrame / 60) % 60;

			if(DTW_WallPicId == P_FOREST_YORU)
			{
				PE.Color = GetColor(0, 200, 200);
				PE_Border(GetColor(0, 100, 100));
			}
			else
			{
				PE.Color = GetColor(200, 200, 0);
				PE_Border(GetColor(100, 100, 0));
			}
			SetPrint(SCREEN_W - 170 - 4, SCREEN_H - 20);
			Print_x(xcout("BEST TIME　%02d:%02d:%02d", h, m, s));
			PE_Reset();
		}

		for(int index = 0; index < ITEM_MAX; index++)
		{
			ItemPos_t *i = ItemPosList + index;

			i->X = index == selectIndex ? ITEM_S_X : ITEM_NS_X;
			i->A = index == selectIndex ? ITEM_S_A : ITEM_NS_A;

			m_approach(i->RealX, i->X, 0.93);
			m_approach(i->RealY, i->Y, 0.93);
			m_approach(i->RealA, i->A, 0.93);

			DPE_SetAlpha(i->RealA);
			DrawCenter(D_TITLEITEM_00 + index | DTP, i->RealX, i->RealY);
			DPE_Reset();
		}
		EachFrame();
	}
endMenu:
	MusicFade();
	SetCurtain(30, -1.0);

	forscene(40)
	{
		DrawTitleWall();
		EachFrame();
	}
	sceneLeave();
}
