#include "DataManager.h"
//Functions
bool data_LoadUserData() {
	char* temp = fManager_ReadFile(fopen(userFilePath, FOPEN_APPEND_READ), true); //userFile stream will be closed by this function
	char result[3000] = "";
	strcpy(result, temp); //cJSON_Parse doesn't work with char pointers, so we have to transfer its value to a char array.
	cJSON* fullJSON = cJSON_Parse(result);
	if (fullJSON != NULL) {
		cJSON* userJSON = cJSON_GetObjectItemCaseSensitive(fullJSON, "User");
		cJSON* nameJSON = cJSON_GetObjectItemCaseSensitive(userJSON, "Name");
		cJSON* ageJSON = cJSON_GetObjectItemCaseSensitive(userJSON, "Age");
		cJSON* ratesJSON = cJSON_GetObjectItemCaseSensitive(fullJSON, "Interest Rates");
		cJSON* elementRatesJSON;
		if (cJSON_IsString(nameJSON)) {
			strcpy(user.name, nameJSON->valuestring);
		}
		else {
			strcpy(user.name, DEFAULT_NAME);
			data_UpdateUserData(false);
			fprintf(stderr, "\nError identifying name!\n");
		}
		if (cJSON_IsNumber(ageJSON)) {
			user.age = ageJSON->valueint;
		}
		else {
			user.age = DEFAULT_AGE;
			data_UpdateUserData(false);
			fprintf(stderr, "\nError identifying age!\n");
		}
		int i = 0;
		cJSON_ArrayForEach(elementRatesJSON, ratesJSON){
			strcpy(interestRates[i].name, cJSON_GetObjectItemCaseSensitive(elementRatesJSON, "Name")->valuestring);
			interestRates[i].rate1 = cJSON_GetObjectItemCaseSensitive(elementRatesJSON, "Rate 1")->valuedouble;
			interestRates[i].rate2 = cJSON_GetObjectItemCaseSensitive(elementRatesJSON, "Rate 2")->valuedouble;
			interestRates[i].rate3 = cJSON_GetObjectItemCaseSensitive(elementRatesJSON, "Rate 3")->valuedouble;
			i++;
		}
		cJSON_Delete(fullJSON);
		return true;
	}
	else {
		return false;
	}
}
void data_FirstTime() {
	CLEAR;
	PeriodicPrintf(DEFAULT_PRINTSPEED, "Welcome to the world of mathematical finance.\n");
	Sleep(500);
	for (;;) {
		PeriodicPrintf(DEFAULT_PRINTSPEED, "Before we proceed, please specify your name.\nName: ");
		scanf("%30[^\t\n]s", &user.name);
		FLUSH;
		if (IsEmpty(user.name)) {
			CLEAR;
			PeriodicPrintf(DEFAULT_PRINTSPEED, "You can't be nameless. (Invalid input!)\n");
			continue;
		}
		break;
	}
	CLEAR;
	for (;;) {
		char text[40] = "What about your age?";
		PeriodicPrintf(DEFAULT_PRINTSPEED, text);
		PeriodicPrintf(DEFAULT_PRINTSPEED, "\nAge: ");
		scanf("%3d", &user.age);
		FLUSH;
		CLEAR;
		if (user.age <= 0){
			PeriodicPrintf(DEFAULT_PRINTSPEED, "You got to be kidding me. (Invalid input!)\n");
		}
		else if(user.age > 115){
			PeriodicPrintf(DEFAULT_PRINTSPEED, "Congrats, you've just become the world's oldest person. (Invalid input!)\n");
		}
		else {
			break;
		}
	}
	data_UpdateUserData(false);
}
void data_UpdateUserData(bool reset) {
	cJSON* fullJSON = cJSON_CreateObject();
	cJSON* userJSON = cJSON_CreateObject();
	cJSON* ratesJSON = cJSON_CreateArray();
	if (reset) {
		strcpy(user.name, DEFAULT_NAME);
		user.age = DEFAULT_AGE;
	}
	else {
		for (int i = 0; i < DEFAULT_INTEREST_RATES_MAX; i++) {
			cJSON* element = cJSON_CreateObject();
			cJSON_AddStringToObject(element, "Name", interestRates[i].name);
			cJSON_AddNumberToObject(element, "Rate 1", interestRates[i].rate1);
			cJSON_AddNumberToObject(element, "Rate 2", interestRates[i].rate2);
			cJSON_AddNumberToObject(element, "Rate 3", interestRates[i].rate3);
			cJSON_AddItemToArray(ratesJSON, element);
		}
	}
	cJSON_AddItemToObject(fullJSON, "User", userJSON);
	cJSON_AddItemToObject(fullJSON, "Interest Rates", ratesJSON);
	cJSON_AddStringToObject(userJSON, "Name", user.name);
	cJSON_AddNumberToObject(userJSON, "Age", user.age);
	fManager_WriteFile(fopen(userFilePath, FOPEN_WRITE), cJSON_Print(fullJSON), true);
	cJSON_Delete(fullJSON);
}
bool IsEmpty(const char *s) {
	while (*s != '\0') {
		if (!isspace((unsigned char)*s))

			return false;
		s++;
	}
	return true; //If all whitespace
}
bool ContainsAlphabets(char str[]) {
	int length = strlen(str);
	for (int i = 0; i < length; i++) {
		if (isalpha(str[i])) {
			return true; //Contains alphabet
		}
	}
	return false; //No alphabets
}
bool IsInteger(char str[]) {
	int length = strlen(str);
	for (int i = 0; i < length; i++) {
		if (!isdigit(str[i])) {
			return false;
		}
	}
	return true; //All numbers
}
bool IsDouble(char str[]) {
	int length = strlen(str);
	if (ExceedCharLimit(str, '.', 1) || ExceedCharLimit(str, '-', 1)) {
		return false;
	}
	else {
		for (int i = 0; i < length; i++) {
			if (!isdigit(str[i]) && str[i] != '.' && str[i] != '-') {
				return false;
			}
		}
	}
	return true;
}
bool IsDate(char str[]) {
	int length = strlen(str);
	if (ExceedCharLimit(str, '/', 1) || !ExceedCharLimit(str, '/', 0) || str[0] == '/') {
		return false;
	}
	else {
		for (int i = 0; i < length; i++) {
			if (!isdigit(str[i]) && str[i] != '/') {
				return false;
			}
		}
	}
	return true;
}
bool ExceedCharLimit(char s[], char delimiter, int max) {
	int length = strlen(s);
	int count = 0;
	for (int i = 0; i < length; i++) {
		if (s[i] == delimiter) {
			count++;
		}
	}
	if (count > max) {
		return true;
	}
	return false;
}
void SplitString(char* destination[], char source[], char delimiter[]) {
	//CAUTION: The source string will be modified and become obsolete.
	//The destination string should be empty.

	char* ptr = strtok(source, delimiter);
	int i = 0;
	while (ptr != NULL)
	{
		/*
		//An alternative using 2D char arrays instead of char pointers
		destination[i]=malloc(sizeof(char) * maxLength);
		strcpy(destination[i], ptr);
		*/
		destination[i] = ptr; //Store isolated portion in the destination string 
		i++;
		ptr = strtok(NULL, delimiter); //Move on to the next token/delimiter
	}
}
char* IntToMonth(int i) {
	if (i < 13 && i>0) {
		switch (i) {
		case 1:
			return "January";
		case 2:
			return "February";
		case 3:
			return "March";
		case 4:
			return "April";
		case 5:
			return "May";
		case 6:
			return "June";
		case 7:
			return "July";
		case 8:
			return "August";
		case 9:
			return "September";
		case 10:
			return "October";
		case 11:
			return "November";
		case 12:
			return "December";
		}
	}
	else {
		return "Out of bounds!";
	}
}
struct Date GetCurrentDate() {
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	//printf("now: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	struct Date date;
	date.month = tm.tm_mon + 1;
	date.year = tm.tm_year + 1900;
	return date;
}
char* IntToString(int i) {
	int length = snprintf(NULL, 0, "%d", i); //Obtain the length
	char* str = malloc(length + 1); //Add one for the string terminator
	snprintf(str, length + 1, "%d", i); //Convert int to string
	return str;
}
void DrawGraph(char title[], void (*display)(), double(*func)(double), double xMax, char* xTitle, double yOffset, double yMax, char* yTitle) {
	CLEAR;
	printf(title);
	display();
	double divisionXLength = 10;
	double divisionsX = 5;
	double divisionYLength = 5;
	double divisionsY = 5;
	double xScale = xMax / divisionsX / divisionXLength;
	double yScale = (yMax - yOffset) / divisionYLength / divisionsY;
	double errorMargin = yScale / 2;
	printf("\n%12.12s\n            ^\n", yTitle);
	for (int y = (int)divisionsY * divisionYLength; y >= -1; y--) {
		if (y % (int)divisionsY == 0) {
			printf("%11.1lf--", y * yScale + yOffset);
		}
		else {
			printf("            ");
			if (y != -1) {
				printf("-");
			}
		}
		for (int x = 0; x <= divisionsX * divisionXLength; x++) {
			if (y > 0) {
				if (fabs((y * yScale + yOffset) - func(x * xScale)) < errorMargin) {
					printf("*");
					Sleep(DEFAULT_GRAPH_DELAY);
				}
				else {
					printf(" ");
				}
			}
			else if (y == 0) {
				if (x % (int)divisionsX == 0) {
					printf("|");
					if (x == divisionsX * divisionXLength) {
						printf("> %s", xTitle);
					}
				}
				else {
					printf("-");
				}
			}
			else {
				if (x % (int)divisionsX == 0) {
					if (x == 0) {
						//Workaround to avoid printing a negative zero
						printf("0 ");
					}
					else {
						printf("%02d", abs(x * xScale));
					}
				}
				else if ((x + 1) % (int)divisionsX != 0) {
					printf(" ");
				}
			}
		}
		NEWLINE;
	}
	NEWLINE;
	PAUSE;
}
void OpenFAQ(enum FAQTypes type) {
	//Online FAQ
	switch (type) {
	case l_I:
		system("cmd /c start https://drive.google.com/open?id=18vj12V13rrt-qWLHC870wseK4PVclCZ8");
		break;
	case l_B:
		system("cmd /c start https://drive.google.com/open?id=1GLWiA_kD0HWvNqHEhkIvC1pV8w3XqgCX");
		break;
	case l_C:
		system("cmd /c start https://drive.google.com/open?id=1xyEHCUAhnWDOxlkcPRDYamM0hNTHq7Wk");
		break;
	case s_F:
		system("cmd /c start https://drive.google.com/open?id=1wCc3-QwJKwlQ8ygztJBeIcSDUhavzYHb");
		break;
	case s_G:
		system("cmd /c start https://drive.google.com/open?id=1daOoEdssw4R6ie0LNyx_mLGgmxaLZ9Sd");
		break;
	case i_C:
		system("cmd /c start https://drive.google.com/open?id=1lbVRzECfDEi0-xO4eJUf2IXgzBTsyD8R");
		break;
	case i_P:
		system("cmd /c start https://drive.google.com/open?id=1tJdTDPtyU9b0eJvYUWAjj9CLzH7AY82Y");
		break;
	}
}