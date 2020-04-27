#ifndef LoanInterest_H
#define LoanInterest_H
#include "DataManager.h"
//It is assumed that the payment is made monthly.
struct Loan {
	double principal;
	double interest;//Annual interest rate
	double monthlyPayment;
	struct Duration duration;
	double total;//The net, which combines both the interest and the principal
} loan; //A variable to store temporary values.
//Functions
void OpenLoans();
//Interest only
void loan_I();
void loan_I_Results(int type);
void loan_I_Forecast(char title[]);
void loan_I_FAQ();
void loan_I_Display_Details();
double loan_I_Compute_Graph(double months);
//Interest and monthly payments
void loan_B();
void loan_B_Results(int type);
void loan_B_Forecast(char title[]);
void loan_B_FAQ();
void loan_B_Display_Details();
double loan_B_Compute_Graph(double months);
double loan_B_Compute_MonthlyPayment(double principal, double annualInterest, double months);
double loan_B_Compute_InterestRate(double principal, double months, double monthlyPayment, double start, int iterations);
double loan_B_Compute_Duration(double principal, double monthlyPayment, double annualInterest);
//Car loan
void loan_Car();
void loan_Car_Results(int type);
void loan_Car_Forecast(char title[]);
void loan_Car_FAQ();
void loan_Car_Display_Details();
double loan_Car_Compute_Graph(double months);
#endif