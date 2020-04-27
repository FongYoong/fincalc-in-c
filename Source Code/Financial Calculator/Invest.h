#ifndef INVEST_H
#define INVEST_H
#include "DataManager.h"
struct Invest {
	double principal;
	double interest;//Annual interest rate
	struct Duration duration;
	double total;
} invest; //A variable to store temporary values.
//Linked list
struct Invest_P_List {
	struct Invest_P_List* next;
	double in;//Positive cash flow
};
struct Invest_P {
	double principal;
	double interest;
	double npv; //Normal Present Value
	double irr; //Interna Rate of Return
	struct Invest_P_List* list; // The first element in the linked list will serve as a strating point. It will not be the actual first element in the list.
} investP; //A variable to store temporary values.
void OpenInvest();
void invest_C();
void invest_C_Results(int type);
void invest_C_Forecast(char title[]);
void invest_C_FAQ();
void invest_C_Display_Details();
double invest_C_Compute_Graph(double years);
void invest_P();
void invest_P_Results(int type);
void invest_P_FAQ();
void invest_P_Display_Details();
int invest_P_InputList(char title[]);
void invest_P_AddToList(struct Invest_P_List* head, double in);
double invest_P_Compute_NPV(struct Invest_P_List* head, double principal, double interest);
double invest_P_Compute_IRR(struct Invest_P_List* head, double principal);
#endif
