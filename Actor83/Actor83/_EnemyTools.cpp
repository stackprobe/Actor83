#include "all.h"

int IsCommonEnemyMovable(int fldX, int fldY, int ignoreOtherEnemyFlag)
{
	FMCell_t *cell = GetFMCell(fldX, fldY);

	if(!cell)
		return false;

	if(cell->IsWall)
		return false;

	if(!ignoreOtherEnemyFlag && IsExistEnemy(fldX, fldY))
		return false;

	return true;
}
