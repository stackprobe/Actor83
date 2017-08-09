#pragma once

#include "C:\Factory\Common\all.h"
#include "TakaraPos.h"

#define FIELDCELLSIZE 8

autoBlock_t *GetFieldArea(int mapX, int mapY, int fieldX, int fieldY);
void SetFieldCell(int mapX, int mapY, int fieldX, int fieldY, uchar *cell);
