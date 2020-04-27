#ifndef CUSTOMPRINTF_H
#define CUSTOMPRINTF_H
#define DEFAULT_PRINTSPEED 15
#include "DataManager.h"
void PeriodicPrintf(int interval, char text[]);
void PrintLoop(char text[], int repetitions, bool newLine);
void PrintInvalidInput();
void PrintInvalidNegative();
#endif
