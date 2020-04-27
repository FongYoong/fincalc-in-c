#ifndef FileManager_H
#define FileManager_H
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

//Functions
char* fManager_ReadFile(FILE* file, bool closeStream);
void fManager_WriteFile(FILE* file, char content[],  bool closeStream);
#endif