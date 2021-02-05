#include "Invest.h"

void OpenInvest() {
	for (;;) {
		CLEAR;
		PeriodicPrintf(DEFAULT_PRINTSPEED, "[ Menu -> INVESTMENTS ]\n");
		PrintLoop("-", 35, false);
		printf("\n[  OPTIONS  ][ <0> |    Menu     ]");
		printf("\n             [ <1> |    CAGR     ]");
		printf("\n             [ <2> |  Prospects  ]\n");
		PrintLoop("-", 35, true);
		printf(">> ");
		char option = getchar();
		FLUSH;
		if (option == '0') {
			return;
		}
		else if (option == '1') {
			invest_C();
		}
		else if (option == '2') {
			invest_P();
		}
		else {
			PrintInvalidInput();
		}
	}
}
//CAGR
void invest_C() {
	for (;;) {
		CLEAR;
		PeriodicPrintf(DEFAULT_PRINTSPEED, "[ Investments -> CAGR (Compound Annual Growth Rate) ]\n");
		PrintLoop("-", 48, false);
		printf("\n[  OPTIONS  ][ <0> |            Back           ]");
		printf("\n             [ <1> |       Calculate Total     ]");
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
			invest_C_Results(1);
		}
		else if (option == '2') {
			invest_C_Results(2);
		}
		else if (option == '3') {
			invest_C_Results(3);
		}
		else if (option == '4') {
			invest_C_FAQ();
		}
		else {
			PrintInvalidInput();
		}
	}
}
void invest_C_Results(int type) {
	char title[48] = "[ CARG -> CALCULATE TOTAL ]\n";
	switch (type) {
	case 1:
		if (!input_Double(title, "Principal amount?", &invest.principal, true)
			|| !input_Interest(title, &invest.interest)
			|| !input_Int(title, "Duration? (Years)", &invest.duration.years, true)) {
			return;
		}
		break;
	case 2:
		strcpy(title, "[ CARG -> CALCULATE INTEREST RATE ]\n");
		if (!input_Double(title, "Principal amount?", &invest.principal, true)
			|| !input_Double(title, "Target / Final Balance?", &invest.total, true)
			|| !input_Int(title, "Duration? (Years)", &invest.duration.years, true)) {
			return;
		}
		break;
	case 3:
		strcpy(title, "[ CARG -> CALCULATE DURATION ]\n");
		if (!input_Double(title, "Principal amount?", &invest.principal, true)
			|| !input_Double(title, "Target / Final Balance?", &invest.total, true)
			|| !input_Interest(title, &invest.interest)) {
			return;
		}
		break;
	}
	bool showDetails = false;
	for (;;) {
		CLEAR;
		printf(title);
		PrintLoop("-", 48, false);
		printf("\n[  OPTIONS  ][ <0> |            Back           ]");
		if (showDetails) {
			printf("\n             [ <1> |  Hide Investment Details  ]");
		}
		else {
			printf("\n             [ <1> |  Show Investment Details  ]");
		}
		printf("\n             [ <2> |          Forecast         ]");
		printf("\n             [ <3> |           Graph           ]");
		printf("\n             [ <4> |           Change          ]\n");
		PrintLoop("-", 48, true);
		if (showDetails) {
			invest_C_Display_Details();
		}
		switch (type) {
		case 1:
			invest.total = invest.principal * pow(1 + invest.interest /100, invest.duration.years);
			printf("Total / Final balance: %.4lf\n>> ", invest.total);
			break;
		case 2:
			invest.interest = 100 * (pow(invest.total / invest.principal, 1.0 / invest.duration.years) - 1);
			if (invest.interest <= 0) {
				CLEAR;
				printf(title);
				invest_C_Display_Details();
				PeriodicPrintf(DEFAULT_PRINTSPEED, "The calculated interest is negative.\n");
				printf("Your total (%.4lf) must be higher than the principal (%.4lf).\n", invest.total, invest.principal);
				PAUSE;
				return;
			}
			printf("Annual interest rate: %.4lf %%\n>> ", invest.interest);
			break;
		case 3:
			invest.duration.years = ceil(log(invest.total / invest.principal) / log(1 + invest.interest / 100));
			if (invest.duration.years <= 0) {
				CLEAR;
				printf(title);
				invest_C_Display_Details();
				PeriodicPrintf(DEFAULT_PRINTSPEED, "The calculated duration is negative.\n");
				printf("Your total (%.4lf) must be higher than the principal (%.4lf).\n", invest.total, invest.principal);
				PAUSE;
				return;
			}
			printf("Duration: %4d years\n>> ", invest.duration.years);
			break;
		}
		invest.total = invest.principal * pow(1 + invest.interest / 100, invest.duration.years);
		char option = getchar();
		FLUSH;
		if (option == '0') {
			return;
		}
		else if (option == '1') {
			showDetails = !showDetails;
		}
		else if (option == '2') {
			invest_C_Forecast(title);
		}
		else if (option == '3') {
			DrawGraph(title, &invest_C_Display_Details, &invest_C_Compute_Graph, invest.duration.years, "Years", invest.principal, invest.total, "Balance");
		}
		else if (option == '4') {
			invest_C_Results(type);
			break;
		}
		else {
			PrintInvalidInput();
		}
	}
}
void invest_C_Forecast(char title[]) {
	int loopLength = 63;
	CLEAR;
	printf(title);
	invest_C_Display_Details();
	NEWLINE;
	PrintLoop("-", loopLength, true);
	printf("[ %4.4s | %15.15s | %19.19s | %12.12s ]\n",
		"Year", "Yearly Earnings", "Cumulative Earnings", "Net");
	PrintLoop("-", loopLength, true);
	double total = invest.principal;
	double interest = 0;
	for (int i = 1; i <= invest.duration.years; i++) {
		interest = total * invest.interest / 100;
		total += interest;
		printf("[ %4d | %15.4lf | %19.4lf | %12.4lf ]\n",
			i, interest, total - invest.principal, total);
		PrintLoop("-", loopLength, true);
		//Sleep(50);
	}
	PAUSE;
}
void invest_C_FAQ() {
	char title[] = " [ CAGR -> FAQ ]\n\n";
	int numberOfQuestions = 2;
	for (;;) {
		char* questions[] =
		{ "How does this investment model works?",
			"\"I'm hungry for some Maths!\"" };
		char* answers[] =
		{ " Compound annual growth rate (CAGR) refers to the mean annual growth rate\n\
 of an investment over a certain period of time, usually longer than one year.\n\
 It is a measure of growth of an investment based on the assumption\n\
 that the investment grows in terms of value on a steady rate, compounded annually.\n\
 The comparison of various CAGR measures may help reveal an investment's strengths and weaknesses." };
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
				OpenFAQ(i_C);
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
void invest_C_Display_Details() {
	int loopLength = 33;
	PrintLoop("-", loopLength, true);
	printf(" | Principal: %8.4lf\n | Annual interest rate: %8.4lf %%", invest.principal, invest.interest);
	printf("\n | Number of years: %4d years", invest.duration.years);
	printf("\n | Total earnings: %8.4lf", invest.total - invest.principal);
	printf("\n | Final balance: %8.4lf \n", invest.total);
	PrintLoop("-", loopLength, true);
}
double invest_C_Compute_Graph(double years) {
	//Computes balance for graphing purposes
	double total = invest.principal;
	double interest = 0;
	for (int i = 1; i <= years; i++) {
		interest = total * invest.interest / 100;
		total += interest;
	}
	return total;
}
//Prospects
void invest_P() {
	for (;;) {
		CLEAR;
		PeriodicPrintf(DEFAULT_PRINTSPEED, "[ Investments -> PROSPECTS\n");
		PrintLoop("-", 38, false);
		printf("\n[  OPTIONS  ][ <0> |       Back      ]");
		printf("\n             [ <1> |  Calculate NPV  ]");
		printf("\n             [ <2> |  Calculate IRR  ]");
		printf("\n             [ <3> |       FAQ       ]\n");
		PrintLoop("-", 38, true);
		printf(">> ");
		char option = getchar();
		FLUSH;
		if (option == '0') {
			return;
		}
		else if (option == '1') {
			invest_P_Results(1);
		}
		else if (option == '2') {
			invest_P_Results(2);
		}
		else if (option == '3') {
			invest_P_FAQ();
		}
		else {
			PrintInvalidInput();
		}
	}
}
void invest_P_Results(int type) {
	char title[48] = "[ Prospects -> CALCULATE NPV ]\n";
	switch (type) {
	case 1:
		if (!input_Double(title, "Principal amount?", &investP.principal, true)
			|| !input_Interest(title, &investP.interest) || !invest_P_InputList(title)) {
			return;
		}
		investP.npv = invest_P_Compute_NPV(investP.list, investP.principal, investP.interest);
		break;
	case 2:
		strcpy(title, "[ Prospects -> CALCULATE IRR ]\n");
		if (!input_Double(title, "Principal amount?", &investP.principal, true)) {
			return;
		}
		if (!invest_P_InputList(title)) {
			return;
		}
		break;
	}
	investP.irr = invest_P_Compute_IRR(investP.list, investP.principal);
	bool showDetails = false;
	for (;;) {
		CLEAR;
		printf(title);
		PrintLoop("-", 48, false);
		printf("\n[  OPTIONS  ][ <0> |            Back           ]");
		if (showDetails) {
			printf("\n             [ <1> |  Hide Investment Details  ]");
		}
		else {
			printf("\n             [ <1> |  Show Investment Details  ]");
		}
		printf("\n             [ <2> |           Change          ]\n");
		PrintLoop("-", 48, true);
		if (showDetails) {
			invest_P_Display_Details();
		}
		switch (type) {
		case 1:
			printf("Net present value (NPV): %.4lf\n", investP.npv);
			if (investP.npv < 0) {
				printf("This investment is relatively unprofitable.");
			}
			printf("\n>> ");
			break;
		case 2:
			printf("Internal rate of return: %.4lf %%\n>> ", investP.irr);
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
			invest_P_Results(type);
			break;
		}
		else {
			PrintInvalidInput();
		}
	}
}
void invest_P_FAQ() {
	char title[] = " [ Prospects -> FAQ ]\n\n";
	int numberOfQuestions = 3;
	for (;;) {
		char* questions[] =
		{ "What is an NPV?", 
			"What is an IRR?", 
			"\"I'm hungry for some Maths!\"" };
		char* answers[] =
		{ " >> Net present value (NPV) is the difference between the present value of cash inflows and the present value of cash outflows over a period of time.\n\
 NPV is used in capital budgeting and investment planning to analyze the profitability of a projected investment or project.\n\
 A positive NPV indicates that the projected earnings generated by a project or investment (in present dollars) exceeds the anticipated costs (also in present dollars).\n\
 It is assumed that an investment with a positive NPV will be profitable, and an investment with a negative NPV will result in a net loss.", 
		" >> Internal rate of return (IRR) is a metric used in capital budgeting to estimate the profitability of potential investments.\n\
 It is a discount rate that makes the NPV of all cash flows from a particular project equal to zero.\n\
 IRR calculations rely on the same formula as NPV does.\n\
 Generally speaking, the higher a project's IRR, the more desirable it is to undertake."};
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
				OpenFAQ(i_P);
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
void invest_P_Display_Details() {
	int loopLength =29;
	PrintLoop("-", loopLength, true);
	printf(" | Principal: %8.4lf\n | Interest rate: %8.4lf %%\n\n", investP.principal, investP.interest);
	int i = 1;
	struct Invest_P_List* current = investP.list;
	while (current->next != NULL) {
		current = current->next;
		printf(" | Payment %d: %lf\n", i, current->in);
		i++;
	}
	printf("\n | NPV: %8.4lf", investP.npv);
	printf("\n | IRR: %8.4lf %%\n", investP.irr);
	PrintLoop("-", loopLength, true);
}
int invest_P_InputList(char title[]) {
	if (investP.list != NULL) {
		free(investP.list);
	}
	investP.list = malloc(sizeof(struct Invest_P_List));
	investP.list->next = NULL;
	int i = 1;
	for (;;) {
		char query[12];
		sprintf(query, "Payment %d?", i);
		double value;
		int result = input_Double_List(title, query, &value, true);
		if (result == false) {
			return false;
		}
		else if (result == -1) {
			break;
		}
		invest_P_AddToList(investP.list, value);
		i++;
	}
	if (i == 1) {
		return false;
	}
	return true;
}
void invest_P_AddToList(struct Invest_P_List* head, double in) {
	struct Invest_P_List* current = head;
	while (current->next != NULL) {
		current = current->next;
	}
	current->next = malloc(sizeof(struct Invest_P_List));
	current->next->in = in;
	current->next->next = NULL;
}
double invest_P_Compute_NPV(struct Invest_P_List* head, double principal, double interest) {
	double i = 1.0;
	double npv = -principal;
	struct Invest_P_List* current = head;
	while (current->next != NULL) {
		current = current->next;
		npv += current->in / pow(1 + interest / 100, i);
		i++;
	}
	return npv;
}
double invest_P_Compute_IRR(struct Invest_P_List* head, double principal) {
	CLEAR;
	printf("Estimating...");
	double r = 0;
	double errorMargin = 0.01;
	double interval = 0.01;
	double previousNPV = 0;
	for (;;) {
		double i = 1.0;
		double npv = -principal;
		struct Invest_P_List* current = head;
		while (current->next != NULL) {
			current = current->next;
			npv += current->in / pow(1 + r / 100, i);
			i++;
		}
		//Check if values are alternating between positives and negatives
		if (npv > 0 && previousNPV < 0 || npv < 0 && previousNPV >0) {
			return r;
		}
		//Verify accuracy via error margins
		if (npv > errorMargin) {
			r += interval;
		}
		else if (npv < -errorMargin) {
			r -= interval;
		}
		else {
			return r;
		}
		previousNPV = npv;
	}
}