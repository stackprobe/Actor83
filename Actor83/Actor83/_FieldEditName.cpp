#include "all.h"

#define NAME_MINLEN 6

static char *Names[] =
{
	"���n",
	"��",
	"��",
	"����",
	"��1",
	"��2",
	"��3",
	"��4",
	"��5",
	"��1",
	"��2",
	"��",
	"���n",
	"�n��",
	"�Ԕ�",
	"�Ώ�",
	"��1",
	"��2",
	"���c",
	"����",
	"��l��",
	"���",
	"�؂芔",
	"���ĎD",
	"�ĎR�q",
	"�X��",
	"�I",
	"��5",
	"��6",
	"��",
	"��",
	"��7",
	"��8",
	"����",
	"�Β�",
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
