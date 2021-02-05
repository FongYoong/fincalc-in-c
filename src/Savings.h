#ifndef SAVINGS_H
#define SAVINGS_H
#include "DataManager.h"
struct Savings {
	double principal;
	double interest;//Annual interest rate
	double monthlyDeposit;
	struct Duration duration;
	double total;//The target
	double actualTotal;
} savings; //A variable to store temporary values.
void OpenSavings();
//Fixed Deposits
void save_Fixed();
void save_Fixed_Results(int type);
void save_Fixed_Forecast(char title[]);
void save_Fixed_FAQ();
void save_Fixed_Display_Details();
double save_Fixed_Compute_Graph(double months);
double save_Fixed_Compute_Specific_Interest(int iteration);
//Deposit Goals
void save_Goals();
void save_Goals_Results(int type);
void save_Goals_Forecast(char title[]);
void save_Goals_FAQ();
void save_Goals_Display_Details();
double save_Goals_Compute_Graph(double months);
double save_Goals_Compute_Total(double principal, double annualInterest, double monthlyPayment, int months);
double save_Goals_Compute_MonthlyDeposit(double principal, double total, double annualInterest, int months);
double save_Goals_Compute_InterestRate(double principal, double total, double monthlyPayment, int months, double start, int iterations);
int save_Goals_Compute_Duration(double principal, double total, double annualInterest, double monthlyPayment);
#endif