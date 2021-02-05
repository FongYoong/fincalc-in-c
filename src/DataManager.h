#ifndef DEFAULT_USER
#define DEFAULT_GRAPH_DELAY 20
#define DEFAULT_RATES_ELEMENTS 3 //Number of rate elements // For data check and storage purposes
#define DEFAULT_CHAR_MAX 30 //Char limit for names and miscellaneous stuff
#define DEFAULT_NAME "Anonymous"
#define DEFAULT_AGE 1
#define FOPEN_WRITE "w"//A new file will be created. Existing files will be erased.
#define FOPEN_WRITE_READ "w+"//A new file will be created. Existing files will be erased.
#define FOPEN_READ "r"//File must exist.
#define FOPEN_READ_WRITE "r+"//File must exist.
#define FOPEN_APPEND "a"//A new file will be created.
#define FOPEN_APPEND_READ "a+"////A new file will be created.
#endif
#ifndef FLUSH
#define FLUSH while ((flushChar = getchar()) != '\n' && flushChar != EOF)
#endif
#ifndef PAUSE
#define PAUSE system("pause")//Has to be called twice to resolve some buffer issue related to the function.
#endif
#ifndef CLEAR
#define CLEAR system("cls")
#endif
#ifndef NEWLINE
#define NEWLINE printf("\n")
#endif
#ifndef DataManager_H
#define DataManager_H
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <windows.h>
#include <math.h>
#include "cJSON.h"
#include "tinyfiledialogs.h"
#include "InterestRates.h"
#include "FileManager.h"
#include "InputManager.h"
#include "CustomPrintf.h"
enum FAQTypes {
	l_I, l_B, l_C, s_F, s_G, i_C, i_P
};
struct UserProfile {
	char name[DEFAULT_CHAR_MAX];
	int age;
} user; //The user variable is stored here
struct Date {
	int month;
	int year;
};
struct Duration {
	bool usesDate; //Record if user has specified the dates
	int months; //Total time in months
	int years; //Total time in years
	//The variables below are an alternative to the duration
	struct Date startDate;//Format: month/year //Example: 1/2000
	struct Date endDate;
};
struct DoubleDataElement {
	char name[DEFAULT_CHAR_MAX];
	double* pointer;//Pointer to the original address
} rateElements[DEFAULT_RATES_ELEMENTS];
struct DateDataElement {
	char name[DEFAULT_CHAR_MAX];
	struct Date* pointer;//Pointer to the original address
};
char* userFilePath;
char flushChar; //Variable necessary when flushing the input buffers using the predefined FLUSH. 
//Functions
bool data_LoadUserData();
void data_UpdateUserData(bool reset);
void data_FirstTime();
//void data_GenerateDataElements(struct DoubleDataElement* data, double* loanRate, double* carRate);
bool IsEmpty(const char *s);
bool ContainsAlphabets(char str[]);
bool IsInteger(char str[]);
bool IsDouble(char str[]);
bool IsDate(char str[]);
bool ExceedCharLimit(char s[], char delimiter, int max);
void SplitString(char* destination[], char source[], char delimiter[]); //Split string into an array of smaller strings
char* IntToMonth(int i);
struct Date GetCurrentDate();
char* IntToString(int i);
void DrawGraph(char title[], void(*display)(), double(*func)(double), double xMax, char* xTitle, double yOffset, double yMax, char* yTitle);
void OpenFAQ(enum FAQTypes type);
#endif
