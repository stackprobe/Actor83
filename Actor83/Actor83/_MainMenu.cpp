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

	if(padId == -1) // ? ����`
		padId = 0; // �K���ȃf�t�H

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
		"��",
		"��",
		"��",
		"�E",
		"����^�A�N�V����",
		"�L�����Z���^�ᑬ�ړ�",
		"�����ړ�",
		"�|�[�Y",
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
			memcpy(&Gnd.PadBtnno, backup, sizeof(Gnd.PadBtnno)); // ����
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
		Print("�p�b�h�̃{�^���ݒ�");
		PrintRet();

		for(int index = 0; index < btnMax; index++)
		{
			Print_x(xcout("[%c] %s�{�^��", index == btnIndex ? '>' : ' ', btnNames[index]));

			if(index < btnIndex)
			{
				Print_x(xcout(" -> %d", *pPadBtns[index]));
			}
			PrintRet();
		}
		PrintRet();
		Print("�� �J�[�\�����w���{�^���������ĉ������B");
		PrintRet();
		Print("�� �y�L�[�܂��͂w�L�[�������ƃL�����Z�����܂��B");
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
			*p++; == *(p++); ��u�����I
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
		Print_x(xcout("�E�B���h�E��%s", heightMode ? "����" : "��"));
		PrintRet();
		Print_x(xcout("[ %4d ] �s�N�Z��", *pSizeValue));
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
		Print("�E�B���h�E�T�C�Y�̐ݒ�^�T�C�Y���w�肷��");
		PrintRet();
		Print_x(xcout("[%c]�@�� ( %d )", selectIndex == 0 ? '>' : ' ', SM_Screen_W));
		PrintRet();
		Print_x(xcout("[%c]�@���� ( %d )", selectIndex == 1 ? '>' : ' ', SM_Screen_H));
		PrintRet();
		Print_x(xcout("[%c]�@�ύX�𔽉f����", selectIndex == 2 ? '>' : ' '));
		PrintRet();
		Print_x(xcout("[%c]�@�߂�", selectIndex == 3 ? '>' : ' '));
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
		errorCase(SetWindowSizeExtendRate(1.0)); // ? ���s
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
				SetScreenSize(SCREEN_W, SCREEN_H); // �f�t�H���g
				break;

			case 1:
				// �[���t���X�N���[��
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
		Print("�E�B���h�E�T�C�Y�̐ݒ�");
		PrintRet();
		Print_x(xcout("[%c]�@�f�t�H���g�̃T�C�Y", selectIndex == 0 ? '>' : ' '));
		PrintRet();
		Print_x(xcout("[%c]�@�[���t���X�N���[��", selectIndex == 1 ? '>' : ' '));
		PrintRet();
		Print_x(xcout("[%c]�@�T�C�Y���w�肷��", selectIndex == 2 ? '>' : ' '));
		PrintRet();
		Print_x(xcout("[%c]�@�߂�", selectIndex == 3 ? '>' : ' '));
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
			*p++; == *(p++); ��u�����I
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
			// ���[�g���f
			{
				SetScreenSize(SCREEN_W, SCREEN_H); // �f�t�H���g
				int mdm = GetMouseDispMode();
				errorCase(SetWindowSizeExtendRate(rate / (double)rateDenom)); // ? ���s
				SetMouseDispMode(mdm);
			}
			break;
		}
		DrawTitleWall_Curtain();

		PE_Border(GetColor(0, 100, 0));
		SetPrint(60, 270, 40);
		Print("�E�B���h�E�T�C�Y�̐ݒ�^���[�g���w�肷��");
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
		Print_x(xcout("%s�{�����[���ݒ�", seMode ? "�r�d" : "�a�f�l"));
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
		Print("�ݒ�");
		PrintRet();
		int c = 0;
		Print_x(xcout("[%c]�@�p�b�h�̃{�^���ݒ�", selectIndex == c++ ? '>' : ' '));
		PrintRet();
		Print_x(xcout("[%c]�@�E�B���h�E�T�C�Y�̕ύX", selectIndex == c++ ? '>' : ' '));
		PrintRet();
//		Print_x(xcout("[%c]�@�E�B���h�E�T�C�Y�̃��[�g (�b��)", selectIndex == c++ ? '>' : ' '));
//		PrintRet();
		Print_x(xcout("[%c]�@�a�f�l�{�����[���ݒ�", selectIndex == c++ ? '>' : ' '));
		PrintRet();
		Print_x(xcout("[%c]�@�r�d�{�����[���ݒ�", selectIndex == c++ ? '>' : ' '));
		PrintRet();
		Print_x(xcout("[%c]�@�}�E�X�J�[�\����%s���� (���݂�%s)", selectIndex == c++ ? '>' : ' '
			,GetMouseDispMode() ? "��\����" : "�\��"
			,GetMouseDispMode() ? "�\��" : "��\��"
			));
		PrintRet();
		Print_x(xcout("[%c]�@�t�B�[���h���ԑт�%s�ɂ��� (���݂�%s)", selectIndex == c++ ? '>' : ' '
			,Gnd.FieldDaylightDisabled ? "�L��" : "����"
			,Gnd.FieldDaylightDisabled ? "����" : "�L��"
			));
		PrintRet();
		Print_x(xcout("[%c]�@�T�[�o�[�ɐڑ�%s (���݂͐ڑ�%s)", selectIndex == c++ ? '>' : ' '
			,Gnd.ServerConnectDisabled ? "����" : "���Ȃ�"
			,Gnd.ServerConnectDisabled ? "���Ȃ�" : "����"
			));
		PrintRet();
		Print_x(xcout("[%c]�@�߂�", selectIndex == c++ ? '>' : ' '));
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
	error(); // HACK
}
static void SM_MenuScreenSize(void)
{
	PublicMenu *pm = new PublicMenu("�E�B���h�E�T�C�Y�̕ύX");

	pm->AddInt("�E�B���h�E��", &Gnd.RealScreen_W, SCREEN_W, SCREEN_W_MAX, 1);
	pm->AddInt("�E�B���h�E��", &Gnd.RealScreen_H, SCREEN_H, SCREEN_H_MAX, 1);
	pm->AddFunc("�E�B���h�E�T�C�Y�̔��f", ApplyScreenSize);

	pm->Show(DrawTitleWall_Curtain);

	delete pm;
}
static void SettingMenu(void)
{
	PublicMenu *pm = new PublicMenu("�ݒ�");

	pm->AddFunc("�p�b�h�̃{�^���ݒ�", SM_PadButtonSetting);
	pm->AddIntFunc("�}�E�X�J�[�\����\��", GetMouseDispMode, SM_ChangeMouseDispMode);
	pm->AddFunc("�[���t���X�N���[��", SM_QuasiFullScreenMode);
	pm->AddFunc("�E�B���h�E�T�C�Y�̕���", SM_ResetScreenSize);
	pm->AddFunc("�E�B���h�E�T�C�Y�̕ύX", SM_MenuScreenSize);
	pm->AddDoubleFunc("�a�f�l�{�����[��", SM_GetMusicVolume, SM_AddMusicVolume);
	pm->AddDoubleFunc("���ʉ��{�����[��", SM_GetSEVolume, SM_AddSEVolume);

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
		Print("�Z�[�u�f�[�^������܂���...");
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
			month == 9 && day == 3 // ���̓�
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
			Print("�����M�̃f�[�^������܂��I");
			PE_Reset();
		}
		*/
		if(Gnd.BestTimeFrame) // �x�X�g�^�C���\��
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
			Print_x(xcout("BEST TIME�@%02d:%02d:%02d", h, m, s));
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
