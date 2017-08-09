#include "all.h"

int HasPos(autoList<i2D_t> *list, int x, int y)
{
	for(int index = 0; index < list->GetCount(); index++)
	{
		i2D_t *pos = list->ElementAt(index);

		if(pos->X == x && pos->Y == y)
		{
			return 1; // found
		}
	}
	return 0; // not found
}
int GetDaylightMode(void) // ret: DL_*
{
	LOGPOS();

	time_t t = time(NULL);
	struct tm *lt = localtime(&t);
	errorCase(!lt);

	int hm = lt->tm_hour * 100 + lt->tm_min;
	int ret;
	if(hm < 600)
		ret = DL_NIGHT;
	else if(hm < 1630)
		ret = DL_DAYTIME;
	else if(hm < 1930)
		ret = DL_SUNSET;
	else
		ret = DL_NIGHT;

	return ret;
}
