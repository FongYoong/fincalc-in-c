#include "InterestRates.h"

void OpenInterestRates() {
	for (;;) {
		CLEAR;
		PeriodicPrintf(DEFAULT_PRINTSPEED, "[ Menu -> INTEREST RATES ]\n");
		PrintLoop("-", 35, false);
		printf("\n[  OPTIONS  ][ <0> |     Menu     ]");
		printf("\n             [ <1> |     Add      ]");
		printf("\n             [ <2> |    Modify    ]");
		printf("\n             [ <3> |    Delete    ]");
		printf("\n             [ <4> |  Delete All  ]");
		printf("\n             [ <5> |     FAQ      ]\n");
		PrintLoop("-", 35, false);
		rates_Display();
		printf(">> ");
		char option = getchar();
		FLUSH;
		if (option == '0') {
			return;
		}
		else if (option == '1') {
			rates_Add();
		}
		else if (option == '2') {
			rates_Modify();
		}
		else if (option == '3') {
			rates_Delete(false);
		}
		else if (option == '4') {
			rates_Delete(true);
		}
		else if (option == '5') {
			rates_FAQ();
		}
		else {
			PrintInvalidInput();
		}
	}
}
void rates_Add() {
	char title[] = "[ Interest Rates -> ADD ]\n";
	for (int i = 0; i < DEFAULT_INTEREST_RATES_MAX; i++) {
		if (IsEmpty(interestRates[i].name)) {
			rates_UpdateDataElements(i);
			if (!input_String(title, "Name?", &interestRates[i].name)) {
				return;
			}
			for (int data = 0; data < DEFAULT_RATES_ELEMENTS; data++) {
				char query[DEFAULT_CHAR_MAX];
				strcpy(query, rateElements[data].name);
				strcat(query, " ?");
				if (!input_Double(title, query, rateElements[data].pointer, true)) {
					strcpy(interestRates[i].name, "");
					return;
				}
			}
			data_UpdateUserData(false);
			return;
		}
		if (i == DEFAULT_INTEREST_RATES_MAX) {
			CLEAR;
			PeriodicPrintf(DEFAULT_PRINTSPEED, "The list is fully occupied. Only a maximum of 10 interest rates are allowed.");
			Sleep(500);
			return;
		}
	}
}
void rates_Modify() {
	int index = rates_Select_Index("[ Interest Rates -> MODIFY ]\n");
	if (index == -1) {
		return;
	}
	else {
		for (;;) {
			CLEAR;
			printf("[ Interest Rates -> MODIFY ]\n");
			PrintLoop("-", 32, false);
			printf("\n[    EDIT   ][ <0> |   Back   ]");
			printf("\n             [ <1> |   Name   ]");
			printf("\n             [ <2> |  Rate 1  ]");
			printf("\n             [ <3> |  Rate 2  ]");
			printf("\n             [ <4> |  Rate 3  ]\n");
			PrintLoop("-", 32, true);
			rates_DisplayElement(index);
			printf(">> ");
			char option[2];
			option[0] = getchar();
			FLUSH;
			int optionValue = 0;
			sscanf(option, "%d", &optionValue);
			if (option[0] == '0') {
				break;
			}
			else if (optionValue == 1) {
				for (;;) {
					CLEAR;
					printf("[ Interest Rates -> MODIFY ]\n");
					printf("Current name: %s\n", interestRates[index].name);
					PeriodicPrintf(DEFAULT_PRINTSPEED, "New name?\n>> ");
					char temp[30];
					scanf("%30[^\t\n]s", &temp);
					FLUSH;
					if (IsEmpty(temp)) {
						PrintInvalidInput();
					}
					else {
						strcpy(interestRates[index].name, temp);
						data_UpdateUserData(false);
						break;
					}
				}
			}
			else if (optionValue > 1 && optionValue <= DEFAULT_RATES_ELEMENTS + 1) {
				optionValue -= 2;
				rates_UpdateDataElements(index);
				for (;;) {
					CLEAR;
					printf("[ Interest Rates -> MODIFY ]\n");
					printf("Current %s: ", rateElements[optionValue].name);
					printf("%lf\n", *rateElements[optionValue].pointer);
					PeriodicPrintf(DEFAULT_PRINTSPEED, "New rate?\n>> ");
					char temp[DEFAULT_CHAR_MAX];
					scanf("%s", &temp);
					FLUSH;
					if (!IsDouble(temp)) {
						PrintInvalidInput();
					}
					else {
						sscanf(temp, "%lf", rateElements[optionValue].pointer); //Convert string to float
						if (*rateElements[optionValue].pointer < 0) {
							CLEAR;
							PeriodicPrintf(DEFAULT_PRINTSPEED, "Interest rates must be positive values!");
							Sleep(400);
						}
						else {
							data_UpdateUserData(false);
							break;
						}
					}
				}
			}
			else {
				PrintInvalidInput();
			}
		}
	}
}
void rates_Delete(bool all) {
	if (all) {
		for (;;) {
			CLEAR;
			printf("[ Interest Rates -> DELETE ALL ]\n");
			PeriodicPrintf(DEFAULT_PRINTSPEED, "Are you sure?\n");
			printf("[ <y> | 'Yes' ] [ <n> | 'No' ]\n>> ");
			char option = getchar();
			FLUSH;
			if (option == 'y' || option == "Y") {
				for (int i = 0; i < DEFAULT_INTEREST_RATES_MAX; i++) {
					rates_ResetRate(i);
				}
				data_UpdateUserData(false);
				break;
			}
			else if (option == 'n' || option == "N") {
				return;
			}
			else {
				PrintInvalidInput();
			}
		}
	}
	else {
		for (;;) {
			int index = rates_Select_Index("[ Interest Rates -> DELETE ]\n");
			if (index == -1) {
				return;
			}
			else {
				for (;;) {
					CLEAR;
					rates_DisplayElement(index);
					rates_ResetRate(index);
					for (int i = index; i < DEFAULT_INTEREST_RATES_MAX - 1; i++) {
						interestRates[i] = interestRates[i + 1];
					}
					rates_ResetRate(DEFAULT_INTEREST_RATES_MAX - 1);
					printf("DELETE successful.\n");
					PeriodicPrintf(DEFAULT_PRINTSPEED, "Delete another item? [ <y> | 'Yes' ] [ <n> | 'No' ]\n>> ");
					char option = getchar();
					FLUSH;
					if (option == 'y' || option == 'Y') {
						break;
					}
					else if (option == 'n' || option == 'N') {
						goto END;
					}
					else {
						PrintInvalidInput();
					}
				}
			}
		}
		goto END;
	}
END:
	data_UpdateUserData(false);
}
void rates_FAQ() {
	char title[] = " [ Interest Rates -> FAQ ]\n\n";
	int numberOfQuestions = 3;
	for (;;) {
		char* questions[] =
		{ "What is this for?" ,
			"Where will the data be stored?",
			"Any limitations?" };
		char* answers[] =
		{ " It is a database where you can add, modify and delete items comprising of interest rates that you can manually maintain.",
			" All persistent data will be stored in the user file created when you first used the program.",
			" Only a maximum of 10 items/interest groups are allowed for technical reasons. \n\
 This implies that 30 is the maximum number of interest rates that can be stored." };
		CLEAR;
		printf("%s | <0> | Exit\n", title);
		for (int i = 0; i < numberOfQuestions; i++) {
			printf(" | <%d> | %s\n", i + 1, questions[i]);
		}
		printf("\n>> ");
		char option[2];
		option[0]= getchar();
		FLUSH;
		int optionValue = 0;
		sscanf(option, "%d", &optionValue);
		optionValue--;
		if (option[0] == '0') {
			return;
		}
		else if (optionValue > -1 && optionValue < numberOfQuestions) {
			CLEAR;
			printf("%s | <%d> | %s\n\n%s\n\n", title, optionValue + 1, questions[optionValue], answers[optionValue]);
			PAUSE;
		}
		else {
			PrintInvalidInput();
		}
	}
}
void rates_ResetRate(int index) {
	strcpy(interestRates[index].name, "");
	interestRates[index].rate1 = 0;
	interestRates[index].rate2 = 0;
	interestRates[index].rate3 = 0;
}
int rates_Select_Index(char title[]) {
	// -1 indicates user's intention to exit
	for (;;) {
		CLEAR;
		printf(title);
		rates_Display();
		PeriodicPrintf(DEFAULT_PRINTSPEED, "Specify the index of the item to be selected.\n");
		printf("[ <0> | 'Exit' ]\n>> ");
		int index;
		scanf("%d", &index);
		FLUSH;
		index--;
		if (index == -1) {
			return -1;
		}
		else if (index >= DEFAULT_INTEREST_RATES_MAX || index < -1) {
			CLEAR;
			PeriodicPrintf(DEFAULT_PRINTSPEED, "The specified index is invalid.");
			Sleep(400);
		}
		else {
			if (IsEmpty(interestRates[index].name)) {
				CLEAR;
				PeriodicPrintf(DEFAULT_PRINTSPEED, "The specified index is invalid.");
				Sleep(400);
			}
			else {
				return index;
			}
		}
	}
}
double rates_Select_Value(char title[]) {
	for (;;) {
		int baseIndex = rates_Select_Index(title);
		if (baseIndex == -1) {
			return -1;
		}
		rates_UpdateDataElements(baseIndex);
		for (;;) {
			CLEAR;
			printf(title);
			rates_DisplayElement_Index(baseIndex);
			PeriodicPrintf(DEFAULT_PRINTSPEED, "Specify the index of the interest rate to be selected.\n");
			printf("[ <0> | 'Exit' ]\n>> ");
			int index;
			scanf("%d", &index);
			FLUSH;
			index--;
			if (index == -1) {
				break;
			}
			else if (index >= DEFAULT_RATES_ELEMENTS || index < -1) {
				CLEAR;
				PeriodicPrintf(DEFAULT_PRINTSPEED, "The specified index is invalid.");
				Sleep(400);
			}
			else {
				return *rateElements[index].pointer;
			}
		}
	}
}
void rates_Display() {
	//This function modifies the rateElements. Any necessary modifications to the array should be done after calling this function.
	NEWLINE;
	PrintLoop("-", rates_DisplayLength(), true);
	//Top row
	char nameFormat[] = "| %8.8s ";
	printf("[ %3.3s | %8.8s ", "No.", "Name");
	for (int data = 0; data < DEFAULT_RATES_ELEMENTS; data++) {
		printf(nameFormat, rateElements[data].name);
	}
	printf("]\n");
	//Subsequent rows
	char rateFormat[] = "| %8.4lf ";
	int count = 0;
	for (int i = 0; i < DEFAULT_INTEREST_RATES_MAX; i++) {
		if (!IsEmpty(interestRates[i].name)) {
			rates_UpdateDataElements(i);
			count++;
			printf("[ %3d | %8.8s ", i + 1, interestRates[i].name);
			for (int data = 0; data < DEFAULT_RATES_ELEMENTS; data++) {
				printf(rateFormat, *rateElements[data].pointer);
			}
			printf("]\n");
			PrintLoop("-", rates_DisplayLength(), true);
			//Sleep(100);
		}
	}
	if (count <= 0) {
		PrintLoop("-", rates_DisplayLength(), true);
	}
}
void rates_DisplayElement(int index) {
	PrintLoop("-", rates_DisplayLength(), true);
	//Top row
	char nameFormat[] = "| %8.8s ";
	printf("[ %3.3s | %8.8s ", "No.", "Name");
	for (int data = 0; data < DEFAULT_RATES_ELEMENTS; data++) {
		printf(nameFormat, rateElements[data].name);
	}
	printf("]\n");
	//Subsequent rows
	char rateFormat[] = "| %8.4lf ";
	printf("[ %3d | %8.8s ", index + 1, interestRates[index].name);
	for (int data = 0; data < DEFAULT_RATES_ELEMENTS; data++) {
		printf(rateFormat, *rateElements[data].pointer);
	}
	printf("]\n");
	PrintLoop("-", rates_DisplayLength(), true);
}
void rates_DisplayElement_Index(int index) {
	printf("\nClass name: %s\nIndex: %d\n", interestRates[index].name, index + 1);
	PrintLoop("-", 29, true);
	//Top row
	printf("[ %3.3s | %8.8s | %8.8s ]\n", "No.", "Name", "Value");
	//Subsequent rows
	char nameFormat[] = "| %8.8s ";
	char rateFormat[] = "| %8.4lf ]\n";
	for (int data = 0; data < DEFAULT_RATES_ELEMENTS; data++) {
		printf("[ %3d ", data + 1);
		printf(nameFormat, rateElements[data].name);
		printf(rateFormat, *rateElements[data].pointer);
		PrintLoop("-", 29, true);
		//Sleep(100);
	}
}
bool rates_IsFull() {
	for (int i = 0; i < DEFAULT_INTEREST_RATES_MAX; i++) {
		if (IsEmpty(interestRates[i].name)) {
			return false;
		}
	}
	return true;
}
void rates_UpdateDataElements(int index) {
	double* pointers[] = { &interestRates[index].rate1, &interestRates[index].rate2,  &interestRates[index].rate3 };
	for (int i = 0; i < DEFAULT_RATES_ELEMENTS; i++) {
		rateElements[i].pointer = pointers[i];
	}
}
int rates_DisplayLength() {
	return (18 + 11 * sizeof(rateElements) / sizeof(rateElements[0]));
}