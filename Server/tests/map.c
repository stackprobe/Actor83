#include "..\map.h"

int main(int argc, char **argv)
{
readArgs:
	if(argIs("/S"))
	{
		int mapX;
		int mapY;
		int fieldX;
		int fieldY;
		autoBlock_t *cell;

		mapX = toValue(nextArg());
		mapY = toValue(nextArg());
		fieldX = toValue(nextArg());
		fieldY = toValue(nextArg());
		cell = makeBlockHexLine(nextArg());

		errorCase(getSize(cell) != FIELDCELLSIZE);

		SetFieldCell(mapX, mapY, fieldX, fieldY, (uchar *)directGetBuffer(cell));

		releaseAutoBlock(cell);
		goto readArgs;
	}
	if(argIs("/G"))
	{
		int mapX;
		int mapY;
		int fieldX;
		int fieldY;
		autoBlock_t *cells;

		mapX = toValue(nextArg());
		mapY = toValue(nextArg());
		fieldX = toValue(nextArg());
		fieldY = toValue(nextArg());

		cells = GetFieldArea(mapX, mapY, fieldX, fieldY);

		cout("%s\n", c_makeHexLine(cells));
		cout("(%d)\n", getSize(cells));

		releaseAutoBlock(cells);
		goto readArgs;
	}
}
