#include "DataManager.h"
int main() {
	//Initialise data element names
	char rateNames[][DEFAULT_CHAR_MAX] = { "Rate 1", "Rate 2", "Rate 3" };
	for (int rate = 0; rate < DEFAULT_RATES_ELEMENTS; rate++) {
		strcpy(rateElements[rate].name, rateNames[rate]);
	}
	for (;;) {
		CLEAR;
		PeriodicPrintf(DEFAULT_PRINTSPEED, " [ Financial Calculator ]\n\n Load profile?\n");
		printf(" [ <y> | 'Yes' ] [ <n> | 'No' ] [ <w> | 'WTF?' ]\n>> ");
		char option = getchar();
		FLUSH;
		if (option == 'y' || option == 'Y') {
			userFilePath = tinyfd_openFileDialog("Choose User Data File", "C:\\Users\\user\\Desktop\\", 0, "*.txt", "Text files", false);
			if (userFilePath == NULL) {
				continue;
			}
			if (data_LoadUserData()) {
				break;
			}
			else {
				PrintInvalidInput();
			}
		}
		else if (option == 'N' || option == 'n') {
			userFilePath = tinyfd_saveFileDialog("Save User Data File", "C:\\Users\\user\\Desktop\\userData.txt", 0, "*.txt", "Text files");
			if (userFilePath == NULL) {
				continue;
			}
			data_FirstTime();
			break;
		}
		else if (option == 'W' || option == 'w') {
			CLEAR;
			PeriodicPrintf(DEFAULT_PRINTSPEED, " [ Financial Calculator -> WTF? ]\n");
			printf(" This program is designed with a profile-based approach in mind.\n\
 ~ New users are to create custom profiles which will be utilised by some program components.\n\
 The file can be saved anywhere, with any file name.\n\
 The file extension does not matter too.\n\
 ~ To load a profile, enter the \"Load\" mode and select the relevant file.\n\n");
			PAUSE;
		}
		else {
			PrintInvalidInput();
		}
	}
	for (;;) {
		CLEAR;
		char welcome[DEFAULT_CHAR_MAX * 2] = "\nWelcome, ";
		strcat(welcome, user.name);//Concatenate the name of the user
		PeriodicPrintf(DEFAULT_PRINTSPEED, " [ MENU ]\n");
		PrintLoop("-", 37, false);
		printf("\n[  OPTIONS  ][ <0> |      Exit      ]");
		printf("\n             [ <1> | Interest Rates ]");
		printf("\n             [ <2> |  General Loan  ]");
		printf("\n             [ <3> |     Savings    ]");
		printf("\n             [ <4> |   Investments  ]");
		printf("\n             [ <5> |      About     ]\n");
		PrintLoop("-", 37, false);
		PeriodicPrintf(DEFAULT_PRINTSPEED, welcome);
		PeriodicPrintf(DEFAULT_PRINTSPEED, ".\nWhat would you like to do?\n>> ");
		char option = getchar();
		FLUSH;
		if (option == '0') {
			CLEAR;
			PeriodicPrintf(DEFAULT_PRINTSPEED, "Thank you for using this program.");
			Sleep(500);
			return 1;
		}
		else if (option == '1') {
			OpenInterestRates();
		}
		else if (option == '2') {
			OpenLoans();
		}
		else if (option == '3') {
			OpenSavings();
		}
		else if (option == '4') {
			OpenInvest();
		}
		else if (option == '5') {
			CLEAR;
			PeriodicPrintf(DEFAULT_PRINTSPEED, " [ Main -> ABOUT ]\n\n");
			printf(" ~ Program name: Financial Calculator\n\
 ~ Programming language: The Infamous C, the one which gives programmers the Big C.\n\
 ~ Purpose: To pass our current semester.\n\n\
 ~ Credits:\n\
  > UTAR for delaying our road to dementia.\n\
  > UTAR lecturers for their guidance,\n\
    especially Puan.B A Jesica Malani Binti B A Sugathapala, our practical lecturer.\n\
  > varielle from SourceForge for the TinyFileDialogs library.\n\
  > DaveGamble from Github for the cJSON library.\n\
  > God, for keeping us alive.\n\n\
 ~ Team:\n\
  > Aaron, the human.\n\
  > Fong, the male.\n\
  > Gary, the individual.\n\
  > Pei Yuan, the man.\n\n\
 ~ Team motto: \"Mamak mou?\"\n\n");
			PAUSE;
		}
		else {
			PrintInvalidInput();
		}
	}
}