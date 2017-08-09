#include "all.h"

char *addChar(char *str, int chr) // ret: strr(str)
{
	char *ret = xcout("%s%c", str, chr);
	memFree(str);
	return ret;
}
