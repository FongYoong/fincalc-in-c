#include "InputManager.h"
bool input_String(char* title, char query[], char*ptr) {
	for (;;) {
		CLEAR;
		printf(title);
		PrintLoop("-", 45, true);
		PeriodicPrintf(DEFAULT_PRINTSPEED, query);
		printf(" [ <0> | 'Exit' ]\n>> ");
		scanf("%30[^\t\n]s", ptr);
		FLUSH;
		if (IsEmpty(ptr)) {
			PrintInvalidInput();
		}
		else if (strcmp(ptr, "0") == 0) {
			return false;
		}
		else {
			return true;
		}
	}
}
bool input_Int(char* title, char query[], int* ptr, bool positiveValues) {
	for (;;) {
		CLEAR;
		printf(title);
		PrintLoop("-", 45, true);
		PeriodicPrintf(DEFAULT_PRINTSPEED, query);
		printf(" [ <0> | 'Exit' ]\n>> ");
		char option[DEFAULT_CHAR_MAX];
		scanf("%s", &option);
		FLUSH;
		if (strcmp(option, "0") == 0) {
			return false;
		}
		else if (!IsInteger(option)) {
			PrintInvalidInput();
		}
		else {
			sscanf(option, "%d", ptr); //Convert string to int
			if (*ptr < 0 && positiveValues)
			{
				PrintInvalidNegative();
			}
			else {
				return true;
			}
		}
	}
}
bool input_Double(char* title, char query[], double* ptr, bool positiveValues) {
	for (;;) {
		CLEAR;
		printf(title);
		PrintLoop("-", 45, true);
		PeriodicPrintf(DEFAULT_PRINTSPEED, query);
		printf(" [ <0> | 'Exit' ]\n>> ");
		char option[DEFAULT_CHAR_MAX];
		scanf("%s", &option);
		FLUSH;
		if (strcmp(option, "0") == 0) {
			return false;
		}
		else if (!IsDouble(option)) {
			PrintInvalidInput();
		}
		else {
			sscanf(option, "%lf", ptr); //Convert string to float
			if (*ptr < 0 && positiveValues)
			{
				PrintInvalidNegative();
			}
			else {
				return true;
			}
		}
	}
}
//For linked lists with quasi-infinite lengths
int input_Double_List(char* title, char query[], double* ptr, bool positiveValues) {
	for (;;) {
		CLEAR;
		printf(title);
		PrintLoop("-", 45, true);
		PeriodicPrintf(DEFAULT_PRINTSPEED, query);
		printf("[ <f> | 'Finish / Stop adding' ] [ <0> | 'Exit' ]\n>> ");
		char option[DEFAULT_CHAR_MAX];
		scanf("%s", &option);
		FLUSH;
		if (strcmp(option, "0") == 0) {
			return false;
		}
		else if (strcmp(option, "f") == 0 || strcmp(option, "F") == 0) {
			return -1; //User has finished adding values
		}
		else if (!IsDouble(option)) {
			PrintInvalidInput();
		}
		else {
			sscanf(option, "%lf", ptr); //Convert string to float
			if (*ptr < 0 && positiveValues)
			{
				PrintInvalidNegative();
			}
			else {
				return true;
			}
		}
	}
}
bool input_Interest(char* title, double* ptr) {
	for (;;) {
		CLEAR;
		printf(title);
		PrintLoop("-", 45, true);
		PeriodicPrintf(DEFAULT_PRINTSPEED, "Annual interest rate (%)?");
		printf(" [ <d> | 'Database' ] [ <0> | 'Exit' ]\n>> ");
		char option[DEFAULT_CHAR_MAX];
		scanf("%s", &option);
		FLUSH;
		if (strcmp(option, "d") == 0 || strcmp(option, "D") == 0) {
			*ptr = rates_Select_Value(title);
			if (*ptr == -1) {
				continue;
			}
			else {
				return true;
			}
		}
		else if (strcmp(option, "0") == 0) {
			return false;
		}
		else if (!IsDouble(option)) {
			PrintInvalidInput();
		}
		else {
			sscanf(option, "%lf", ptr); //Convert string to float
			if (*ptr < 0)
			{
				PrintInvalidInput();
			}
			else {
				return true;
			}
		}
	}
}
bool input_Duration(char* title, struct Duration *duration) {
	for (;;) {
		CLEAR;
		printf(title);
		PrintLoop("-", 45, true);
		PeriodicPrintf(DEFAULT_PRINTSPEED, "Please specify the duration type.\n");
		printf("[ <1> | 'No. of Months' ] [ <2> | 'Date' ] [ <0> | 'Exit' ]\n>> ");
		char option[DEFAULT_CHAR_MAX];
		scanf("%s", &option);
		FLUSH;
		if (strcmp(option, "0") == 0) {
			return false;
		}
		else if (!IsInteger(option)) {
			PrintInvalidInput();
		}
		else if (strcmp(option, "1") == 0) {
			duration->usesDate = false;
			for (;;) {
				CLEAR;
				printf(title);
				PrintLoop("-", 45, true);
				PeriodicPrintf(DEFAULT_PRINTSPEED, "Number of months? [ <0> | 'Exit' ]\n>> ");
				char option2[DEFAULT_CHAR_MAX];
				scanf("%s", &option2);
				FLUSH;
				if (strcmp(option2, "0") == 0) {
					break;
				}
				else if (!IsInteger(option2)) {
					PrintInvalidInput();
				}
				else {
					sscanf(option2, "%d", &(duration->months)); //Convert string to float
					if (duration->months <= 0)
					{
						PrintInvalidInput();
					}
					else {
						return true;
					}
				}
			}
		}
		else if (strcmp(option, "2") == 0) {
			duration->usesDate = true;
			struct DateDataElement dates[] = { {.name = "Start",.pointer = &(duration->startDate)}, {.name = "End",.pointer = &(duration->endDate)} };
			int i = 0;
			for (;;) {
				if (i >= 2) {
					return true;
				}
				CLEAR;
				printf(title);
				PrintLoop("-", 45, true);
				if (i == 1) {
					printf("Start date: %d/%d\n", duration->startDate.month, duration->startDate.year);
				}
				printf("%s date? (Format: month/year) (Example: 8/2000)\n[ <1> | 'Current Date' ] [ <0> | 'Exit' ]\n>> ", dates[i].name);
				char option2[DEFAULT_CHAR_MAX];
				scanf("%30[^\t\n]s", &option2);
				FLUSH;
				if (strcmp(option2, "0") == 0) {
					break;
				}
				else if (strcmp(option2, "1") == 0) {
					struct Date d = GetCurrentDate();
					dates[i].pointer->month = d.month;
					dates[i].pointer->year = d.year;
					if (i == 1) {
						duration->months = input_Date_Duration(
							duration->startDate.month, duration->startDate.year, duration->endDate.month, duration->endDate.year);
						if (duration->months < 1) {
							CLEAR;
							PeriodicPrintf(DEFAULT_PRINTSPEED, "The dates given are not chronologically consistent.");
							Sleep(400);
							i = 0;
							continue;
						}
						else {
							i++;
						}
					}
					else {
						i++;
					}
				}
				else if (!IsDate(option2)) {
					PrintInvalidInput();
				}
				else {
					char* split[2];
					SplitString(split, &option2, "/");
					int* dateValues[] = { &dates[i].pointer->month, &dates[i].pointer->year };
					for (int j = 0; j < 2; j++) {
						if (IsInteger(split[j])) {
							sscanf(split[j], "%d", dateValues[j]); //Convert string to int
							if (*dateValues[j] <= 0) {
								CLEAR;
								PeriodicPrintf(DEFAULT_PRINTSPEED, "Two possibilities. You're either:\na) insane.\n or\nb) careless.\nI hope the latter is true.\n\n");
								PAUSE;
								break;
							}
							else if (j == 0) {
								if (*dateValues[j] > 12) {
									CLEAR;
									PeriodicPrintf(DEFAULT_PRINTSPEED, "Please Google the Gregorian Calendar if you are oblivious to it.\n\n");
									PAUSE;
									break;
								}
							}
							else if (j == 1) {
								if (i == 1) {
									duration->months = input_Date_Duration(
										duration->startDate.month, duration->startDate.year, duration->endDate.month, duration->endDate.year);
									if (duration->months < 1) {
										CLEAR;
										PeriodicPrintf(DEFAULT_PRINTSPEED, "The dates given are not chronologically consistent.");
										Sleep(400);
										i = 0;
									}
									else {
										i++;
									}
								}
								else {
									i++;
								}
							}
						}
						else {
							PrintInvalidInput();
							break;
						}
					}
				}
			}
		}
		else {
			PrintInvalidInput();
		}
	}
}
int input_Date_Duration(int month1, int year1, int month2, int year2) {
	return ((year2 - year1) * 12 + (month2 - month1) + 1);
}