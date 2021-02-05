#include "Loan.h"

void OpenLoans() {
	for (;;) {
		CLEAR;
		PeriodicPrintf(DEFAULT_PRINTSPEED, "[ Menu -> GENERAL LOAN ]\n");
		PrintLoop("-", 52, false);
		printf("\n[  OPTIONS  ][ <0> |              Menu             ]");
		printf("\n             [ <1> |         Interest Only         ]");
		printf("\n             [ <2> |  Interest & Monthly Payments  ]");
		printf("\n             [ <3> |            Car Loan           ]\n");
		PrintLoop("-", 52, true);
		printf(">> ");
		char option = getchar();
		FLUSH;
		if (option == '0') {
			return;
		}
		else if (option == '1') {
			loan_I();
		}
		else if (option == '2') {
			loan_B();
		}
		else if (option == '3') {
			loan_Car();
		}
		else {
			PrintInvalidInput();
		}
	}
}
//I for interest-only loans
void loan_I() {
	for (;;) {
		CLEAR;
		PeriodicPrintf(DEFAULT_PRINTSPEED, "[ General Loan -> INTEREST ONLY ]\n");
		PrintLoop("-", 48, false);
		printf("\n[  OPTIONS  ][ <0> |            Back           ]");
		printf("\n             [ <1> |      Calculate Total      ]");
		printf("\n             [ <2> |  Calculate Interest Rate  ]");
		printf("\n             [ <3> |     Calculate Duration    ]");
		printf("\n             [ <4> |            FAQ            ]\n");
		PrintLoop("-", 48, true);
		printf(">> ");
		char option = getchar();
		FLUSH;
		if (option == '0') {
			return;
		}
		else if (option == '1') {
			loan_I_Results(1);
		}
		else if (option == '2') {
			loan_I_Results(2);
		}
		else if (option == '3') {
			loan_I_Results(3);
		}
		else if (option == '4') {
			loan_I_FAQ();
		}
		else {
			PrintInvalidInput();
		}
	}
}
void loan_I_Results(const int type) {
	char title[] = "[ Interest Only -> CALCULATE TOTAL INTEREST ]\n";
	switch (type) {
	case 1:
		if (!input_Double(title, "Principal amount?", &loan.principal, true)
			|| !input_Interest(title, &loan.interest)
			|| !input_Duration(title, &loan.duration)) {
			return;
		}
		break;
	case 2:
		strcpy(title, "[ Interest Only -> CALCULATE INTEREST RATE ]\n");
		if (!input_Double(title, "Principal amount?", &loan.principal, true)
			|| !input_Double(title, "Total? (Interest + Principal)", &loan.total, true)
			|| !input_Duration(title, &loan.duration)) {
			return;
		}
		break;
	case 3:
		strcpy(title, "[ Interest Only -> CALCULATE DURATION ]\n");
		if (!input_Double(title, "Principal amount?", &loan.principal, true)
			|| !input_Interest(title, &loan.interest)
			|| !input_Double(title, "Total? (Interest + Principal)", &loan.total, true)) {
			return;
		}
		break;
	}
	bool showDetails = false;
	for (;;) {
		CLEAR;
		printf(title);
		PrintLoop("-", 42, false);
		printf("\n[  OPTIONS  ][ <0> |         Back        ]");
		if (showDetails) {
			printf("\n             [ <1> |  Hide Loan Details  ]");
		}
		else {
			printf("\n             [ <1> |  Show Loan Details  ]");
		}
		printf("\n             [ <2> |       Forecast      ]");
		printf("\n             [ <3> |        Graph        ]");
		printf("\n             [ <4> |        Change       ]\n");
		PrintLoop("-", 42, true);
		if (showDetails) {
			loan_I_Display_Details();
		}
		switch (type) {
		case 1:
			loan.total = loan.principal * (1 + (loan.interest / 1200 * loan.duration.months));
			printf("Total interest: %.4lf\n>> ", loan.total - loan.principal);
			break;
		case 2:
			loan.interest = ((loan.total / loan.principal) - 1) * (1200.0 / loan.duration.months);
			if (loan.interest <= 0) {
				CLEAR;
				printf(title);
				loan_I_Display_Details();
				PeriodicPrintf(DEFAULT_PRINTSPEED, "The calculated interest is negative.\n");
				printf("Your total (%.4lf) must be higher than the principal (%.4lf).\n", loan.total, loan.principal);
				PAUSE;
				return;
			}
			printf("Annual interest rate: %.4lf %%\n>> ", loan.interest);
			break;
		case 3:
			loan.duration.months = ceil(((loan.total / loan.principal) - 1) * (1200 / loan.interest));
			if (loan.duration.months <= 0) {
				CLEAR;
				printf(title);
				loan_I_Display_Details();
				PeriodicPrintf(DEFAULT_PRINTSPEED, "The calculated duration is negative.\n");
				printf("Your total (%.4lf) must be higher than the principal (%.4lf).\n", loan.total, loan.principal);
				PAUSE;
				return;
			}
			printf("Duration: %4d months\n>> ", loan.duration.months);
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
			loan_I_Forecast(title);
		}
		else if (option == '3') {
			DrawGraph(title, &loan_I_Display_Details, &loan_I_Compute_Graph, loan.duration.months, "Months", loan.principal, loan.total, "Net");
		}
		else if (option == '4') {
			loan_I_Results(type);
			break;
		}
		else {
			PrintInvalidInput();
		}
	}
}
void loan_I_Forecast(char title[]) {
	int loopLength = 84;
	CLEAR;
	printf(title);
	loan_I_Display_Details();
	NEWLINE;
	PrintLoop("-", loopLength, true);
	printf("[ %5.5s | %15.15s | %16.16s | %19.19s | %12.12s ]\n",
		"Month", "Interest Factor", "Interest Payment", "Cumulative Interest", "Net");
	PrintLoop("-", loopLength, true);
	double interest = loan.principal * loan.interest / 1200;
	double cumulative;
	for (int i = 1; i <= loan.duration.months; i++) {
		cumulative = loan.principal * (loan.interest / 1200) * i;
		printf("[ %5d | %15.4lf | %16.4lf | %19.4lf | %12.4lf ]\n",
			i, loan.interest / 1200, interest, cumulative, cumulative + loan.principal);
		PrintLoop("-", loopLength, true);
		//Sleep(50);
	}
	PAUSE;
}
void loan_I_FAQ() {
	char title[] = " [ Interest Only -> FAQ ]\n\n";
	int numberOfQuestions = 3;
	for (;;) {
		char* questions[] =
		{ "How does this loan works?" ,
			"When should I use a loan like this?",
			"\"I'm hungry for some Maths!\"" };
		char* answers[] =
		{ " The loan has a constant interest rate which is calculated from the principal. \n\
 The interest is paid monthly and no amount is allocated for the principal." ,
			" Since the principal is not deducted throughout the loan, \
a lump of sum needs to be paid to clear the entire principal amount at the end of the loan.\n\
 >> You may consider this loan if you:\n\
\t~ are selling the home after a short period.\n\
\t~ want lower initial payments and are confident in making large payments later.\n\
 >> Advantages:\n\
\t~ Low monthly payments\n\
\t~ Place extra money into other investments\n\
 >> Disadvantages:\n\
\t~ Some may not afford the principal payment at the end ot the loan.\n\
\t~ Rising interest rates if the loan is an adjustable-rate-mortgage (ARM)." };
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
				OpenFAQ(l_I);
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
void loan_I_Display_Details() {
	int loopLength = (loan.duration.usesDate) ? 46 : 33;
	PrintLoop("-", loopLength, true);
	printf(" | Principal: %8.4lf \n | Annual interest rate: %8.4lf %%", loan.principal, loan.interest);
	if (loan.duration.usesDate) {
		printf("\n | Interval: %9.9s %04d - %9.9s %04d ", IntToMonth(loan.duration.startDate.month), loan.duration.startDate.year, IntToMonth(loan.duration.endDate.month), loan.duration.endDate.year);
	}
	printf("\n | Number of months: %4d", loan.duration.months);
	printf("\n | Net: %8.4lf\n", loan.total);
	PrintLoop("-", loopLength, true);
}
double loan_I_Compute_Graph(double months) {
	return loan.principal * (1 + loan.interest / 1200 * floor(months));
}
//B for both, i.e. - interests and monthly payment
void loan_B() {
	for (;;) {
		CLEAR;
		PeriodicPrintf(DEFAULT_PRINTSPEED, "[ General Loan -> INTEREST & MONTHLY PAYMENTS ]\n");
		PrintLoop("-", 50, false);
		printf("\n[  OPTIONS  ][ <0> |             Back            ]");
		printf("\n             [ <1> |  Calculate Monthly Payment  ]");
		printf("\n             [ <2> |  Calculate Annual interest  ]");
		printf("\n             [ <3> |      Calculate Duration     ]");
		printf("\n             [ <4> |             FAQ             ]\n");
		PrintLoop("-", 50, true);
		printf(">> ");
		char option = getchar();
		FLUSH;
		if (option == '0') {
			return;
		}
		else if (option == '1') {
			loan_B_Results(1);
		}
		else if (option == '2') {
			loan_B_Results(2);
		}
		else if (option == '3') {
			loan_B_Results(3);
		}
		else if (option == '4') {
			loan_B_FAQ();
		}
		else {
			PrintInvalidInput();
		}
	}
}
void loan_B_Results(int type) {
	char title[] = "[ Interest & Monthly Payments -> CALCULATE MONTHLY PAYMENT ]\n";
	switch (type) {
	case 1:
		if (!input_Double(title, "Principal amount?", &loan.principal, true)
			|| !input_Interest(title, &loan.interest) 
			|| !input_Duration(title, &loan.duration)) {
			return;
		}
		break;
	case 2:
		strcpy(title, "[ Interest & Monthly Payments -> CALCULATE INTEREST RATE ]\n");
		if (!input_Double(title, "Principal amount?", &loan.principal, true)
			|| !input_Double(title, "Monthly payment?", &loan.monthlyPayment, true) 
			|| !input_Duration(title, &loan.duration)) {
			return;
		}
		break;
	case 3:
		strcpy(title, "[ Interest & Monthly Payments -> CALCULATE DURATION ]\n");
		if (!input_Double(title, "Principal amount?", &loan.principal, true)
			|| !input_Interest(title, &loan.interest) 
			|| !input_Double(title, "Monthly payment?", &loan.monthlyPayment, true)) {
			return;
		}
		break;
	}
	bool showDetails = false;
	for (;;) {
		CLEAR;
		printf(title);
		PrintLoop("-", 42, false);
		printf("\n[  OPTIONS  ][ <0> |        Back         ]");
		if (showDetails) {
			printf("\n             [ <1> |  Hide Loan Details  ]");
		}
		else {
			printf("\n             [ <1> |  Show Loan Details  ]");
		}
		printf("\n             [ <2> |       Forecast      ]");
		printf("\n             [ <3> |        Graph        ]");
		printf("\n             [ <4> |        Change       ]\n");
		PrintLoop("-", 42, true);
		if (showDetails) {
			loan_B_Display_Details();
		}
		switch (type) {
		case 1:
			loan.monthlyPayment = loan_B_Compute_MonthlyPayment(loan.principal, loan.interest, loan.duration.months);
			printf("Monthly payment: %.4lf\n>> ", loan.monthlyPayment);
			break;
		case 2:
			loan.interest = loan_B_Compute_InterestRate(loan.principal, loan.duration.months, loan.monthlyPayment, 1, 50);
			if (loan.interest <= 0) {
				CLEAR;
				printf(title);
				loan_B_Display_Details();
				PeriodicPrintf(DEFAULT_PRINTSPEED, "The calculated interest is negative.\n");
				PAUSE;
				return;
			}
			printf("Annual interest rate: %.4lf %%\n>> ", loan.interest);
			break;
		case 3:
			loan.duration.months = loan_B_Compute_Duration(loan.principal, loan.monthlyPayment, loan.interest);
			if (loan.duration.months <= 0) {
				CLEAR;
				printf(title);
				loan_B_Display_Details();
				printf("Your monthly payment must be higher than %.4lf.\n", loan.principal * loan.interest /1200);
				PAUSE;
				return;
			}
			printf("Duration: %4d months\n>> ", loan.duration.months);
			break;
		}
		loan.total = loan.monthlyPayment * loan.duration.months;
		char option = getchar();
		FLUSH;
		if (option == '0') {
			return;
		}
		else if (option == '1') {
			showDetails = !showDetails;
		}
		else if (option == '2') {
			loan_B_Forecast(title);
		}
		else if (option == '3') {
			DrawGraph(title, &loan_B_Display_Details, &loan_B_Compute_Graph, loan.duration.months, "Months", 0, loan.principal, "Balance");
		}
		else if (option == '4') {
			loan_B_Results(type);
			break;
		}
		else {
			PrintInvalidInput();
		}
	}
}
void loan_B_Forecast(char title[]) {
	int loopLength = 112;
	CLEAR;
	printf(title);
	loan_B_Display_Details();
	NEWLINE;
	PrintLoop("-", loopLength, true);
	printf("[ %5.5s | %15.15s | %14.14s | %13.13s | %19.19s | %12.12s | %12.12s ]\n",
		"Month", "Monthly Payment", "Principal Part", "Interest Part", "Cumulative Interest", "Balance", "Net");
	PrintLoop("-", loopLength, true);
	double balance = loan.principal;
	double interestPart = 0;
	double principalPart = 0;
	double cumulativeInterest = 0;
	for (int i = 1; i <= loan.duration.months; i++) {
		interestPart = balance * loan.interest / 1200;
		cumulativeInterest += interestPart;
		principalPart = loan.monthlyPayment - interestPart;
		balance -= principalPart;
		if (balance < 0) {
			balance = 0;
		}
		printf("[ %5d | %15.4lf | %14.4lf | %13.4lf | %19.4lf | %12.4lf | %12.4lf ]\n",
			i, loan.monthlyPayment, principalPart, interestPart, cumulativeInterest, balance, loan.monthlyPayment * i);
		PrintLoop("-", loopLength, true);
		//Sleep(50);
	}
	PAUSE;
}
void loan_B_FAQ() {
	char title[] = " [ Interest & Monthly Payments -> FAQ ]\n\n";
	int numberOfQuestions = 3;
	for (;;) {
		char* questions[] =
		{ "How does this loan works?" ,
			"When should I use a loan like this?", 
			"\"I'm hungry for some Maths!\"" };
		char* answers[] =
		{ " The loan involves a monthly payment which is allocated for both the interest and the principal.\n\
 Thus, the principal decreases every month.\n\
 Coupling this with the fact that the monthly payment is constant, the interest also decreases every month and so more will be allocated for the principal.\n\
 Unlike the \"Interest Only\" loan, the interest is calculated from the balance at the particular time frame, and not the initial principal.", 
			" This model is generally used in various loans, including mortgages and general ones.\n\
 It is not really a question of \"Why\" but \"How\" one can choose a loan which is financially feasible."};
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
				OpenFAQ(l_B);
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
void loan_B_Display_Details() {
	int loopLength = (loan.duration.usesDate) ? 46 : 33;
	PrintLoop("-", loopLength, true);
	printf(" | Principal: %8.4lf\n | Annual interest rate: %8.4lf %%\n | Monthly payment: %8.4lf", loan.principal, loan.interest, loan.monthlyPayment);
	if (loan.duration.usesDate) {
		printf("\n | Interval: %9.9s %04d - %9.9s %04d ", IntToMonth(loan.duration.startDate.month), loan.duration.startDate.year, IntToMonth(loan.duration.endDate.month), loan.duration.endDate.year);
	}
	printf("\n | Number of months: %4d", loan.duration.months);
	printf("\n | Total interest: %8.4lf", loan.total - loan.principal);
	printf("\n | Net: %8.4lf \n", loan.total);
	PrintLoop("-", loopLength, true);
}
double loan_B_Compute_Graph(double months) {
	double balance = loan.principal;
	double interestPart = 0;
	double principalPart = 0;
	for (int i = 1; i <= months; i++) {
		interestPart = balance * loan.interest / 1200;
		principalPart = loan.monthlyPayment - interestPart;
		balance -= principalPart;
		if (balance < 0) {
			balance = 0;
		}
	}
	return balance;
}
double loan_B_Compute_MonthlyPayment(double principal, double annualInterest, double months) {
	double monthlyInterest = annualInterest / 1200.0;
	double power = pow(1 + monthlyInterest, months);
	return (principal * monthlyInterest * power) / (power - 1);
}
double loan_B_Compute_InterestRate(double principal, double months, double monthlyPayment, double start, int iterations) {
	//Function uses Newton-Rhapson's method to approximate the interest rate
	double r = start;//Iteration value
	for (int i = 0; i < iterations; i++) {
		double power = pow(1 + r, months);
		double power2 = pow(1 + r, months - 1);
		double differential = power / (power - 1) + r * ((power - 1) * months * power2 - power * months * power2) / pow(power - 1, 2.0);
		double function = (monthlyPayment / principal) - r * power / (power - 1);
		r = function / differential + r;
		//printf("%lf\n", r);
		//Sleep(100);
	}
	return r * 1200;
}
double loan_B_Compute_Duration(double principal, double monthlyPayment, double annualInterest) {
	double monthlyInterest = annualInterest / 1200;
	//Round to the nearest upper bound
	return ceil((log(monthlyPayment) - log(monthlyPayment - principal * monthlyInterest)) / log(1 + monthlyInterest));
}
//Car loan
void loan_Car() {
	for (;;) {
		CLEAR;
		PeriodicPrintf(DEFAULT_PRINTSPEED, "[ General Loan -> CAR LOAN ]\n");
		PrintLoop("-", 50, false);
		printf("\n[  OPTIONS  ][ <0> |            Back             ]");
		printf("\n             [ <1> |  Calculate Monthly Payment  ]");
		printf("\n             [ <2> |  Calculate Annual Interest  ]");
		printf("\n             [ <3> |     Calculate Duration      ]");
		printf("\n             [ <4> |             FAQ             ]\n");
		PrintLoop("-", 50, true);
		printf(">> ");
		char option = getchar();
		FLUSH;
		if (option == '0') {
			return;
		}
		else if (option == '1') {
			loan_Car_Results(1);
		}
		else if (option == '2') {
			loan_Car_Results(2);
		}
		else if (option == '3') {
			loan_Car_Results(3);
		}
		else if (option == '4') {
			loan_Car_FAQ();
		}
		else {
			PrintInvalidInput();
		}
	}
}
void loan_Car_Results(int type) {
	char title[] = "[ Car Loan -> CALCULATE MONTHLY PAYMENT ]\n";
	switch (type) {
	case 1:
		if (!input_Double(title, "Principal amount?", &loan.principal, true)
			|| !input_Interest(title, &loan.interest)
			|| !input_Int(title, "Duration? (Years)", &loan.duration.years, true)) {
			return;
		}
		break;
	case 2:
		strcpy(title, "[ Car Loan -> CALCULATE INTEREST RATE ]\n");
		if (!input_Double(title, "Principal amount?", &loan.principal, true)
			|| !input_Double(title, "Monthly payment?", &loan.monthlyPayment, true)
			|| !input_Int(title, "Duration? (Years)", &loan.duration.years, true)) {
			return;
		}
		break;
	case 3:
		strcpy(title, "[ Car Loan -> CALCULATE DURATION ]\n");
		if (!input_Double(title, "Principal amount?", &loan.principal, true)
			|| !input_Interest(title, &loan.interest)
			|| !input_Double(title, "Monthly payment?", &loan.monthlyPayment, true)) {
			return;
		}
		break;
	}
	bool showDetails = false;
	for (;;) {
		CLEAR;
		printf(title);
		PrintLoop("-", 42, false);
		printf("\n[  OPTIONS  ][ <0> |        Back         ]");
		if (showDetails) {
			printf("\n             [ <1> |  Hide Loan Details  ]");
		}
		else {
			printf("\n             [ <1> |  Show Loan Details  ]");
		}
		printf("\n             [ <2> |       Forecast      ]");
		printf("\n             [ <3> |        Graph        ]");
		printf("\n             [ <4> |        Change       ]\n");
		PrintLoop("-", 42, true);
		if (showDetails) {
			loan_Car_Display_Details();
		}
		switch (type) {
		case 1:
			loan.monthlyPayment = loan.principal * (1 + loan.interest / 100 * loan.duration.years) / (loan.duration.years * 12);
			printf("Monthly payment: %.4lf\n>> ", loan.monthlyPayment);
			break;
		case 2:
			loan.interest = 100 * (loan.monthlyPayment * loan.duration.years * 12 / loan.principal - 1) / loan.duration.years;
			if (loan.interest <= 0) {
				CLEAR;
				printf(title);
				loan_B_Display_Details();
				PeriodicPrintf(DEFAULT_PRINTSPEED, "The calculated interest is negative.\n");
				PAUSE;
				return;
			}
			printf("Annual interest rate: %.4lf %%\n>> ", loan.interest);
			break;
		case 3:
			loan.duration.years = 100 * loan.principal / (1200 * loan.monthlyPayment - loan.interest * loan.principal);
			if (loan.duration.years <= 0) {
				CLEAR;
				printf(title);
				loan_B_Display_Details();
				printf("Your monthly payment must be higher than %.4lf.\n", loan.principal * loan.interest / 1200);
				PAUSE;
				return;
			}
			printf("Duration: %4d years\n>> ", loan.duration.years);
			break;
		}
		loan.duration.months = loan.duration.years * 12;
		loan.total = loan.monthlyPayment * loan.duration.months;
		char option = getchar();
		FLUSH;
		if (option == '0') {
			return;
		}
		else if (option == '1') {
			showDetails = !showDetails;
		}
		else if (option == '2') {
			loan_Car_Forecast(title);
		}
		else if (option == '3') {
			DrawGraph(title, &loan_Car_Display_Details, &loan_Car_Compute_Graph, loan.duration.months, "Months", 0, loan.principal, "Balance");
		}
		else if (option == '4') {
			loan_Car_Results(type);
			break;
		}
		else {
			PrintInvalidInput();
		}
	}
}
void loan_Car_Forecast(char title[]) {
	int loopLength = 112;
	CLEAR;
	printf(title);
	loan_Car_Display_Details();
	NEWLINE;
	PrintLoop("-", loopLength, true);
	printf("[ %5.5s | %15.15s | %14.14s | %13.13s | %19.19s | %12.12s | %12.12s ]\n",
		"Month", "Monthly Payment", "Principal Part", "Interest Part", "Cumulative Interest", "Balance", "Net");
	PrintLoop("-", loopLength, true);
	double balance = loan.principal;
	double interestPart = loan.principal * loan.interest /1200;
	double principalPart = loan.monthlyPayment - interestPart;
	for (int i = 1; i <= loan.duration.months; i++) {
		balance -= principalPart;
		if (balance < 0) {
			balance = 0;
		}
		printf("[ %5d | %15.4lf | %14.4lf | %13.4lf | %19.4lf | %12.4lf | %12.4lf ]\n",
			i, loan.monthlyPayment, principalPart, interestPart, loan.monthlyPayment * i - (loan.principal - balance), balance, loan.monthlyPayment * i);
		PrintLoop("-", loopLength, true);
		//Sleep(50);
	}
	PAUSE;
}
void loan_Car_FAQ() {
	char title[] = " [ Car Loan -> FAQ ]\n\n";
	int numberOfQuestions = 2;
	for (;;) {
		char* questions[] =
		{ "How does this loan works?",
			"\"I'm hungry for some Maths!\"" };
		char* answers[] =
		{ " Unlike the other loans, the interest is charged yearly, not monthly.\n\
 Car loans are quite rudimentary in that the interest is constant and is computed from the initial principal.\n\
 The monthly payment is made constant by dividing the sum of the principal and cumulative interest by the duration(in months).\n\
 Although similar to interest-only loans, part of the monthly payment is allocated for the principal.\n\
 Hence, the loan will be fully paid at the end of the duration." };
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
				OpenFAQ(l_C);
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
void loan_Car_Display_Details() {
	int loopLength = 33;
	PrintLoop("-", loopLength, true);
	printf(" | Principal: %8.4lf\n | Annual interest rate: %8.4lf %%\n | Monthly payment: %8.4lf", loan.principal, loan.interest, loan.monthlyPayment);
	printf("\n | Number of years: %4d years", loan.duration.years);
	printf("\n | Total interest: %8.4lf", loan.total - loan.principal);
	printf("\n | Net: %8.4lf \n", loan.total);
	PrintLoop("-", loopLength, true);
}
double loan_Car_Compute_Graph(double months) {
	//Computes balance for graphing purposes
	double balance = loan.principal;
	double interestPart = loan.principal * loan.interest / 1200;
	double principalPart = loan.monthlyPayment - interestPart;
	for (int i = 1; i <= months; i++) {
		balance -= principalPart;
		if (balance < 0) {
			balance = 0;
		}
	}
	return balance;
}