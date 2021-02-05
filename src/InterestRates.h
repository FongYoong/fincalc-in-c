#ifndef INTERESTRATES_H
#define INTERESTRATES_H
#define DEFAULT_INTEREST_RATES_MAX 10
#include "DataManager.h"
struct InterestRate{
	char name[DEFAULT_CHAR_MAX];
	double rate1;
	double rate2;
	double rate3;
} interestRates[DEFAULT_INTEREST_RATES_MAX];//Maximum : 10
void OpenInterestRates();
void rates_Add();
void rates_Modify();
void rates_Delete(bool all);
void rates_FAQ();
void rates_ResetRate(int index);
int rates_Select_Index(char title[]);
double rates_Select_Value(char title[]);
void rates_Display();
void rates_DisplayElement(int index);
void rates_DisplayElement_Index(int index);
bool rates_IsFull();
void rates_UpdateDataElements(int index);
int rates_DisplayLength();
#endif