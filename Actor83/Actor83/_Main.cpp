/*
	float, double ���[���N���A�����l����эL��̏����l�� 0.0 �Ƃ���B
	float, double �̌v�Z���ʂ͊��ɂ���Č덷��������̂Ƃ���B

	----

	�t���[�����[�v�̊�{�`

//	ActFrame = 0; // �K�v�Ȃ��
	FreezeInput();

	for(; ; )
	{
		1. ����
			���[�v�̒E�o
			���荞�ݏ���
				���E�����E�ē������͂����ōs���H
				FreezeInput();
		2. �`��
		3. EachFrame();
	}
//	FreezeInput(); // �O�̂���
*/

#include "all.h"

void ProcMain(void)
{
#if !LOG_ENABLED
	Logo();
	MainMenu();
#elif 1 // test
	MainMenu();
#elif 0 // �}�b�v����
	MMT_Main();
#endif
}