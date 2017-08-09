#include "all.h"

#define NAME_MINLEN 6

static char *Names[] =
{
	"整地",
	"緑",
	"芝",
	"水域",
	"草1",
	"草2",
	"草3",
	"草4",
	"草5",
	"花1",
	"花2",
	"石",
	"砂地",
	"地面",
	"花畑",
	"石畳",
	"柵1",
	"柵2",
	"橋縦",
	"橋横",
	"仙人掌",
	"野菜",
	"切り株",
	"立て札",
	"案山子",
	"スラ",
	"蜂",
	"モ5",
	"モ6",
	"火",
	"蛇",
	"モ7",
	"モ8",
	"武器",
	"石柱",
};

char *GetNameOfEFId(int efId)
{
	static char *name;

	memFree(name);

	if(m_isRange(efId, 0, lengthof(Names) - 1))
		name = strx(Names[efId]);
	else
		name = xcout("EF_%02d", efId);

	while(strlen(name) < NAME_MINLEN)
		name = addChar(name, '*');

	return name;
}
