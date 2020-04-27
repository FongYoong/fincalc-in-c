                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               #include "Savings.h"

void OpenSavings() {
	for (;;) {
		CLEAR;
		PeriodicPrintf(DEFAULT_PRINTSPEED, "[ Menu -> SAVINGS ]\n");
		PrintLoop("-", 40, false);
		printf("\n[  OPTIONS  ][ <0> |       Menu        ]");
		printf("\n             [ <1> |   Fixed Deposit   ]");
		printf("\n             [ <2> |   Deposit Goals   ]\n");
		PrintLoop("-", 40, true);
		printf(">> ");
		char option = getchar();
		FLUSH;
		if (option == '0') {
			return;
		}
		else if (option == '1') {
			save_Fixed();
		}
		else if (option == '2') {
			save_Goals();
		}
		else {
			PrintInvalidInput();
		}
	}
}
//Fixed Deposits
void save_Fixed() {
	for (;;) {
		CLEAR;
		PeriodicPrintf(DEFAULT_PRINTSPEED, "[ Savings -> FIXED DEPOSIT ]\n");
		PrintLoop("-", 49, false);
		printf("\n[  OPTIONS  ][ <0> |            Back            ]");
		printf("\n             [ <1> |  Calculate Total Interest  ]");
		printf("\n             [ <2> |   Calculate Interest Rate  ]");
		printf("\n             [ <3> |     Calculate Duration     ]");
		printf("\n             [ <4> |             FAQ            ]\n");
		PrintLoop("-", 49, true);
		printf(">> ");
		char option = getchar();
		FLUSH;
		if (option == '0') {
			return;
		}
		else if (option == '1') {
			save_Fixed_Results(1);
		}
		else if (option == '2') {
			save_Fixed_Results(2);
		}
		else if (option == '3') {
			save_Fixed_Results(3);
		}
		else if (option == '4') {
			save_Fixed_FAQ();
		}
		else {
			PrintInvalidInput();
		}
	}
}
void save_Fixed_Results(int type) {
	char title[] = "[ Fixed Deposit -> CALCULATE TOTAL INTEREST ]\n";
	switch (type) {
	case 1:
		if (!input_Double(title, "Principal amount?", &savings.principal, true)
			|| !input_Interest(title, &savings.interest)
			|| !input_Duration(title, &savings.duration)) {
			return;
		}
		break;
	case 2:
		strcpy(title, "[ Fixed Deposit -> CALCULATE INTEREST RATE ]\n");
		if (!input_Double(title, "Principal amount?", &savings.principal, true)
			|| !input_Double(title, "Total? (Interest + Principal)", &savings.total, true)
			|| !input_Duration(title, &savings.duration)) {
			return;
		}
		break;
	case 3:
		strcpy(title, "[ Fixed Deposit -> CALCULATE DURATION ]\n");
		if (!input_Double(title, "Principal amount?", &savings.principal, true)
			|| !input_Interest(title, &savings.interest)
			|| !input_Double(title, "Total? (Interest + Principal)", &savings.total, true)) {
			return;
		}
		break;
	}
	bool showDetails = false;
	for (;;) {
		CLEAR;
		printf(title);
		PrintLoop("-", 45, false);
		printf("\n[  OPTIONS  ][ <0> |          Back          ]");
		if (showDetails) {
			printf("\n             [ <1> |  Hide Savings Details  ]");
		}
		else {
			printf("\n             [ <1> |  Show Savings Details  ]");
		}
		printf("\n             [ <2> |        Forecast        ]");
		printf("\n             [ <3> |         Graph          ]");
		printf("\n             [ <4> |         Change         ]\n");
		PrintLoop("-", 45, true);
		if (showDetails) {
			save_Fixed_Display_Details();
		}
		switch (type) {
		case 1:
			savings.total = savings.principal * pow(1 + savings.interest / 1200, savings.duration.months);
			printf("Total interest: %.4lf\n>> ", savings.total - savings.principal);
			break;
		case 2:
			savings.interest = 1200 * (pow(savings.total / savings.principal, 1.0 / savings.duration.months) - 1);
			if (savings.interest <= 0) {
				CLEAR;
				printf(title);
				save_Fixed_Display_Details();
				PeriodicPrintf(DEFAULT_PRINTSPEED, "The calculated interest is negative.\n");
				printf("Your total (%.4lf) must be higher than the principal (%.4lf).\n", savings.total, savings.principal);
				PAUSE;
				return;
			}
			printf("Annual interest rate: %.4lf %%\n>> ", savings.interest);
			break;
		case 3:
			savings.duration.months = ceil(log(savings.total / savings.principal) / log(1 + savings.interest / 1200));
			if (savings.duration.months <= 0) {
				CLEAR;
				printf(title);
				save_Fixed_Display_Details();
				PeriodicPrintf(DEFAULT_PRINTSPEED, "The calculated duration is negative.\n");
				printf("Your total (%.4lf) must be higher than the principal (%.4lf).\n", savings.total, savings.principal);
				PAUSE;
				return;
			}
			printf("Duration: %4d months\n>> ", savings.duration.months);
			break;
		}
		char option = getchar();
		FLUSH;
		if (option == '0') {
			return;
		}
		else if (option == '1') {
			showDetails = !showDetails;
		}
		else if (option == '2') {
			save_Fixed_Forecast(title);
		}
		else if (option == '3') {
			DrawGraph(title, &save_Fixed_Display_Details, &save_Fixed_Compute_Graph, savings.duration.months, "Months", savings.principal, savings.total, "Balance");
		}
		else if (option == '4') {
			save_Fixed_Results(type);
			break;
		}
		else {
			PrintInvalidInput();
		}
	}
}
void save_Fixed_Forecast(char title[]) {
	int loopLength = 83;
	CLEAR;
	printf(title);
	save_Fixed_Display_Details();
	NEWLINE;
	PrintLoop("-", loopLength, true);
	printf("[ %5.5s | %15.15s | %16.16s | %19.19s | %12.12s ]\n",
		"Month", "Interest Factor", "Interest Payment", "Cumulative Interest", "Net");
	PrintLoop("-", loopLength, true);
	double interest;
	for (int i = 1; i <= savings.duration.months; i++) {
		interest = pow(1 + savings.interest / 1200, i);
		printf("[ %5d | %15.4lf | %16.4lf | %19.4lf | %12.4lf ]\n",
			i, interest, savings.principal * save_Fixed_Compute_Specific_Interest(i), savings.principal * (interest - 1), savings.principal * interest);
		PrintLoop("-", loopLength, true);
		//Sleep(50);
	}
	PAUSE;
}
void save_Fixed_FAQ() {
	char title[] = " [ Fixed Deposit -> FAQ ]\n\n";
	int numberOfQuestions = 3;
	for (;;) {
		char* questions[] =
		{ "How does this savings model works?", 
			"Why adopt this model?", 
			"\"I'm hungry for some Maths!\"" };
		char* answers[] =
		{ " It promises the investor a fixed rate of interest and, in return,\n\
 the investor agrees not to withdraw or access his / her funds for a fixed period of time.\n\
 Interests are added compoundedly monthly but only paid in total at the end of the investment period.\n\
 In reality, the longer the duration, the higher the interest rate will be.\n\
 However, this program does not take that into account and uses a constant rate throughout the duration.",
		" >> Advantages:\n\
\t~ Generally, almost all banks offer comparatively higher interest rates for fixed deposits relative to normal savings account.\n\
\t~ Some banks allow you to have the earned interest deposited or transferred to your preferred account.\n\
\t  While investors do not have access to their entire fixed deposit savings,\n\
\t  they will however have periodic cashflow in the form of regular interest payments.\n\
\t~ Most financial institutions / banks that offer fixed deposits are credible, stable, and regulated by government agencies.\n\
 >> Disadvantages:\n\
\t~ One of the lowest profit-generating investments.\n\
\t~ No active participation from the investor."};
		CLEAR;
		printf("%s | <0> | Exit\n", title);
		for (int i = 0; i < numberOfQuestions; i++) {
			printf(" | <%d> | %s\n", i + 1, questions[i]);
		}
		printf("\n>> ");
		char option[2];
		option[0] = getchar();
		FLUSH;
		int optionValue = 0;
		sscanf(option, "%d", &optionValue);
		optionValue--;
		if (option[0] == '0') {
			return;
		}
		else if (optionValue > -1 && optionValue < numberOfQuestions) {
			if (optionValue == numberOfQuestions - 1) {
				OpenFAQ(s_F);
			}
			else {
				CLEAR;
				printf("%s | <%d> | %s\n\n%s\n\n", title, optionValue + 1, questions[optionValue], answers[optionValue]);
				PAUSE;
			}
		}
		else {
			PrintInvalidInput();
		}
	}
}
void save_Fixed_Display_Details() {
	int loopLength = (savings.duration.usesDate) ? 46 : 33;
	PrintLoop("-", loopLength, true);
	printf(" | Principal: %8.4lf \n | Annual interest rate: %8.4lf %%", savings.principal, savings.interest);
	if (savings.duration.usesDate) {
		printf("\n | Interval: %9.9s %04d - %9.9s %04d ", IntToMonth(savings.duration.startDate.month), savings.duration.startDate.year, IntToMonth(savings.duration.endDate.month), savings.duration.endDate.year);
	}
	printf("\n | Number of months: %4d", savings.duration.months);
	printf("\n | Total interest: %8.4lf", savings.total - savings.principal);
	printf("\n | Final balance: %8.4lf\n", savings.total);
	PrintLoop("-", loopLength, true);
}
double save_Fixed_Compute_Graph(double months) {
	//Computes balance for graphing purposes
	return savings.principal * pow(1 + savings.interest / 1200, floor(months));
}
double save_Fixed_Compute_Specific_Interest(int iteration) {
	//NOTE: Does not compute the sum of interest. This function returns the interest at the particular iteration.
	return (pow(1 + savings.interest / 1200, iteration) - pow(1 + savings.interest / 1200, iteration - 1));
}
//Deposit Goals
void save_Goals() {
	for (;;) {
		CLEAR;
		PeriodicPrintf(DEFAULT_PRINTSPEED, "[ Savings -> DEPOSIT GOALS ]\n");
		PrintLoop("-", 50, false);
		printf("\n[  OPTIONS  ][ <0> |            Back             ]");
		printf("\n             [ <1> |       Calculate Total       ]");
		printf("\n             [ <2> |  Calculate Monthly Deposit  ]");
		printf("\n             [ <3> |   Calculate Interest Rate   ]");
		printf("\n             [ <4> |     Calculate Duration      ]");
		printf("\n             [ <5> |             FAQ             ]\n");
		PrintLoop("-", 50, true);
		printf(">> ");
		char option = getchar();
		FLUSH;
		if (option == '0') {
			return;
		}
		else if (option == '1') {
			save_Goals_Results(1);
		}
		else if (option == '2') {
			save_Goals_Results(2);
		}
		else if (option == '3') {
			save_Goals_Results(3);
		}
		else if (option == '4') {
			save_Goals_Results(4);
		}
		else if (option == '5') {
			save_Goals_FAQ();
		}
		else {
			PrintInvalidInput();
		}
	}
}
void save_Goals_Results(int type) {
	char title[48] = "[ Deposit Goals -> CALCULATE TOTAL ]\n";
	switch (type) {
	case 1:
		if (!input_Double(title, "Principal amount?", &savings.principal, true)
			|| !input_Interest(title, &savings.interest)
			|| !input_Double(title, "Monthly deposit?", &savings.monthlyDeposit, true)
			|| !input_Duration(title, &savings.duration)) {
			return;
		}
		break;
	case 2:
		strcpy(title, "[ Deposit Goals -> CALCULATE MONTHLY DEPOSIT ]\n");
		if (!input_Double(title, "Principal amount?", &savings.principal, true)
			|| !input_Double(title, "Target / Final Balance?", &savings.total, true)
			|| !input_Interest(title, &savings.interest)
			|| !input_Duration(title, &savings.duration)) {
			return;
		}
		break;
	case 3:
		strcpy(title, "[ Deposit Goals -> CALCULATE INTEREST RATE ]\n");
		if (!input_Double(title, "Principal amount?", &savings.principal, true)
			|| !input_Double(title, "Target / Final Balance?", &savings.total, true)
			|| !input_Double(title, "Monthly deposit?", &savings.monthlyDeposit, true)
			|| !input_Duration(title, &savings.duration)) {
			return;
		}
		break;
	case 4:
		strcpy(title, "[ Deposit Goals -> CALCULATE DURATION ]\n");
		if (!input_Double(title, "Principal amount?", &savings.principal, true)\
			|| !input_Double(title, "Target / Final Balance?", &savings.total, true)
			|| !input_Interest(title, &savings.interest)
			|| !input_Double(title, "Monthly deposit?", &savings.monthlyDeposit, true)){
			return;
		}
		break;
	}
	bool showDetails = false;
	for (;;) {
		CLEAR;
		printf(title);
		PrintLoop("-", 45, false);
		printf("\n[  OPTIONS  ][ <0> |          Back          ]");
		if (showDetails) {
			printf("\n             [ <1> |  Hide Savings Details  ]");
		}
		else {
			printf("\n             [ <1> |  Show Savings Details  ]");
		}
		printf("\n             [ <2> |        Forecast        ]");
		printf("\n             [ <3> |         Graph          ]");
		printf("\n             [ <4> |         Change         ]\n");
		PrintLoop("-", 45, true);
		if (showDetails) {
			save_Goals_Display_Details();
		}
		switch (type) {
		case 1:
			savings.actualTotal = save_Goals_Compute_Total(savings.principal, savings.interest, savings.monthlyDeposit, savings.duration.months);
			printf("Total / Final balance: %.4lf\n>> ", savings.actualTotal);
			break;
		case 2:
			savings.monthlyDeposit = save_Goals_Compute_MonthlyDeposit(savings.principal, savings.total, savings.interest, savings.duration.months);
			printf("Monthly deposit: %.4lf\n>> ", savings.monthlyDeposit);
			break;
		case 3:
			savings.interest = save_Goals_Compute_InterestRate(savings.principal, savings.total, savings.monthlyDeposit, savings.duration.months, 1.1, 50);
			if (savings.interest <= 0) {
				CLEAR;
				printf(title);
				save_Goals_Display_Details();
				PeriodicPrintf(DEFAULT_PRINTSPEED, "The calculated interest is negative.\n");
				printf("Your target (%.4lf) must be higher than the principal (%.4lf).\n", savings.total, savings.principal);
				PAUSE;
				return;
			}
			printf("Annual interest rate: %.4lf %%\n>> ", savings.interest);
			break;
		case 4:
			savings.duration.months = save_Goals_Compute_Duration(savings.principal, savings.total, savings.interest, savings.monthlyDeposit);
			if (savings.duration.months <= 0) {
				CLEAR;
				printf(title);
				save_Goals_Display_Details();
				PeriodicPrintf(DEFAULT_PRINTSPEED, "The calculated duration is negative.\n");
				printf("Your target (%.4lf) must be higher than the principal (%.4lf).\n", savings.total, savings.principal);
				PAUSE;
				return;
			}
			printf("Duration: %4d months\n>> ", savings.duration.months);
			break;
		}
		savings.actualTotal = save_Goals_Compute_Total(savings.principal, savings.interest, savings.monthlyDeposit, savings.duration.months);
		char option = getchar();
		FLUSH;
		if (option == '0') {
			return;
		}
		else if (option == '1') {
			showDetails = !showDetails;
		}
		else if (option == '2') {
			save_Goals_Forecast(title);
		}
		else if (option == '3') {
			DrawGraph(title, &save_Goals_Display_Details, &save_Goals_Compute_Graph, savings.duration.months, "Months", savings.principal, savings.actualTotal, "Balance");
		}
		else if (option == '4') {
			save_Goals_Results(type);
			break;
		}
		else {
			PrintInvalidInput();
		}
	}
}
void save_Goals_Forecast(char title[]) {
	int loopLength = 94;
	CLEAR;
	printf(title);
	save_Goals_Display_Details();
	NEWLINE;
	PrintLoop("-", loopLength, true);
	printf("[ %5.5s | %15.15s | %12.12s | %14.14s | %17.17s | %12.12s ]\n",
		"Month", "Monthly Deposit", "Interest", "Monthly Change", "Cumulative Change", "Net");
	PrintLoop("-", loopLength, true);
	double net = savings.principal;
	double interest;
	for (int i = 1; i <= savings.duration.months; i++) {
		net += savings.monthlyDeposit;
		interest = net * savings.interest / 1200;
		net += interest;
		printf("[ %5d | %15.4lf | %12.4lf | %14.4lf | %17.4lf | %12.4lf ]\n",
			i, savings.monthlyDeposit, interest, interest + savings.monthlyDeposit, net - savings.principal, net);
		PrintLoop("-", loopLength, true);
		//Sleep(50);
	}
	PAUSE;
}
void save_Goals_FAQ() {
	char title[] = " [ Deposit Goals -> FAQ ]\n\n";
	int numberOfQuestions = 2;
	for (;;) {
		char* questions[] =
		{ "How does this savings model works?",
			"\"I'm hungry for some Maths!\"" };
		char* answers[] =
		{ " The loan involves monthly deposits and interests which accrues over time.\n\
 The program assumes that the former and the monthly interest rate are constant throught the specified period.\n\
 The motivation behind this model is simple, that is to expand capitals reserved in savings accounts.\n\
 The principal grows faster than that of a fixed deposit model due to monthly deposits.\n\
 Generally, money can be withdrawn at any period in time, unless contract terms prohibit such freedom."};
		CLEAR;
		printf("%s | <0> | Exit\n", title);
		for (int i = 0; i < numberOfQuestions; i++) {
			printf(" | <%d> | %s\n", i + 1, questions[i]);
		}
		printf("\n>> ");
		char option[2];
		option[0] = getchar();
		FLUSH;
		int optionValue = 0;
		sscanf(option, "%d", &optionValue);
		optionValue--;
		if (option[0] == '0') {
			return;
		}
		else if (optionValue > -1 && optionValue < numberOfQuestions) {
			if (optionValue == numberOfQuestions - 1) {
				OpenFAQ(s_G);
			}
			else {
				CLEAR;
				printf("%s | <%d> | %s\n\n%s\n\n", title, optionValue + 1, questions[optionValue], answers[optionValue]);
				PAUSE;
			}
		}
		else {
			PrintInvalidInput();
		}
	}
}
void save_Goals_Display_Details() {
	int loopLength = (savings.duration.usesDate) ? 46 : 33;
	PrintLoop("-", loopLength, true);
	printf(" | Principal: %8.4lf \n | Annual interest rate: %8.4lf %%", savings.principal, savings.interest);
	if (savings.duration.usesDate) {
		printf("\n | Interval: %9.9s %04d - %9.9s %04d ", IntToMonth(savings.duration.startDate.month), savings.duration.startDate.year, IntToMonth(savings.duration.endDate.month), savings.duration.endDate.year);
	}
	printf("\n | Monthly deposit: %8.4lf", savings.monthlyDeposit);
	printf("\n | Number of months: %4d", savings.duration.months);
	printf("\n | Target: %8.4lf", savings.total);
	printf("\n | Actual balance: %8.4lf\n", savings.actualTotal);
	PrintLoop("-", loopLength, true);
}
double save_Goals_Compute_Graph(double months) {
	//Computes balance for graphing purposes
	double net = savings.principal;
	double interest;
	for (int i = 1; i <= months; i++) {
		net += savings.monthlyDeposit;
		interest = net * savings.interest / 1200;
		net += interest;
	}
	return net;
}
double save_Goals_Compute_Total(double principal, double annualInterest, double monthlyPayment, int months) {
	double i = 1 + annualInterest / 1200;
	double power = pow(i, months);
	return (principal * power + monthlyPayment * i * (power - 1) / (i - 1));
}
double save_Goals_Compute_MonthlyDeposit(double principal, double total, double annualInterest, int months) {
	double i = 1 + annualInterest / 1200;
	double power = pow(i, months);
	return (total - principal * power) / (i * (power - 1) / (i - 1));
}
double save_Goals_Compute_InterestRate(double principal, double total, double monthlyPayment, int months, double start, int iterations) {
	//NOTE: The start value must be greater than 1.
	double r = start;
	for (int i = 0; i < iterations; i++) {
		double power = pow(r, months);
		double power2 = pow(r, months - 1);
		double function = total - principal * power - monthlyPayment * r * (power - 1) / (r - 1);
		double differential = principal * months * power2 + monthlyPayment * (((r - 1) * ((months + 1) * power - 1)) - r * (power - 1)) / pow(r - 1, 2);
		r = function / differential + r;
		//printf("%lf\n", (r - 1) * 1200);
		//Sleep(20);
	}
	return (r - 1 ) * 1200;
}
int save_Goals_Compute_Duration(double principal, double total, double annualInterest, double monthlyPayment) {
	double i = 1 + annualInterest / 1200;
	return ceil(((int)((log((total * (i - 1) + i * monthlyPayment) / (principal * (i - 1) + monthlyPayment * i)) / log(i)) * 1000)) / 1000.0);
	//Precision adjusted to 3 decimal places to avoid exceeding the exact value when using ceil().
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                