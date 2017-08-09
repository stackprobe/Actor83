/*
	float, double をゼロクリアした値および広域の初期値は 0.0 とする。
	float, double の計算結果は環境によって誤差があるものとする。

	----

	フレームループの基本形

//	ActFrame = 0; // 必要ならば
	FreezeInput();

	for(; ; )
	{
		1. 処理
			ループの脱出
			割り込み処理
				離脱処理・再入処理はここで行う？
				FreezeInput();
		2. 描画
		3. EachFrame();
	}
//	FreezeInput(); // 念のため
*/

#include "all.h"

void ProcMain(void)
{
#if !LOG_ENABLED
	Logo();
	MainMenu();
#elif 1 // test
	MainMenu();
#elif 0 // マップ生成
	MMT_Main();
#endif
}
