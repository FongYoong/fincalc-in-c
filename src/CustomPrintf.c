#include "CustomPrintf.h"
void PeriodicPrintf(int interval, char text[]) {
	int length = strlen(text);
	for (int i = 0; i < length; i++) {
		printf("%c", text[i]);
		Sleep(interval);
	}
}
void PrintLoop(char text[], int repetitions, bool newLine) {
	for (int i = 0; i < repetitions; i++) {
		printf(text);
	}
	if (newLine) {
		printf("\n");
	}
}
void PrintInvalidInput() {
	CLEAR;
	PeriodicPrintf(DEFAULT_PRINTSPEED, "(Invalid input!)");
	Sleep(400);
}
void PrintInvalidNegative() {
	CLEAR;
	PeriodicPrintf(DEFAULT_PRINTSPEED, "(The value must be positive!)");
	Sleep(400);
}