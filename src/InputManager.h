#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#include "DataManager.h"
bool input_String(char* title, char query[], char*ptr);
bool input_Int(char* title, char query[], int* ptr, bool positiveValues);
bool input_Double(char* title, char query[], double* ptr, bool positiveValues);
int input_Double_List(char* title, char query[], double* ptr, bool positiveValues);
bool input_Interest(char* title, double* ptr);
bool input_Duration(char* title, struct Duration *duration);
int input_Date_Duration(int month1, int year1, int month2, int year2);
#endif