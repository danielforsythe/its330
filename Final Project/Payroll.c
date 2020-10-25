#define _CRT_SECURE_NO_WARNINGS
#include <stdio.H>
#include <stdlib.H>
#include <string.H>
#include "payrolldata.h"

//This is a sample function to see how to search a table.  It is not used in the program.
float getEmployeePay(int x) {
	for (int i = 0; i < sizeof(employees) / sizeof(employees[0]); i++) {
		if (employees[i].employee_id == x) return employees[i].rate;
	}
	return 0.0;
}

//Receives exemptions, returns allowance amount.
float getFedAllowance(int x) {
	float allowance;
	for (int i = 0; i < 11; i++)
	{
		if (x == federal[i].exemptions)
		{
			allowance = federal[i].allowance_amount;
		}
	}
	return allowance;
};

//Receives exemptions, returns allowance amount.
float getStateAllowance(int x) {
	float allowance;
	for (int i = 0; i < 11; i++)
	{
		if (x == state[i].exemptions)
		{
			allowance = state[i].allowance_amount;
		}
	}
	return allowance;
};

//Receives an amount, returns the base for the bracket it fits in.
float getMarriedBase(float x) {
	float base;
	for (int i = 0; i < 7; i++)
	{
		if (x >= married[i].minimum && x <= married[i].maximum)
		{
			base = married[i].base;
		}
	}
	return base;
};

//Receives an amount, returns the base for the bracket it fits in.
float getSingleBase(float x) {
	float base;
	for (int i = 0; i < 7; i++)
	{
		if (x >= single[i].minimum && x <= single[i].maximum)
		{
			base = single[i].base;
		}
	}
	return base;
};

//Receives an amount, returns the minimum for the bracket it fits in.
float getMarriedMinimum(float x) {
	float minimum;
	for (int i = 0; i < 7; i++)
	{
		if (x >= married[i].minimum && x <= married[i].maximum)
		{
			minimum = married[i].minimum;
		}
	}
	return minimum;
};

//Receives an amount, returns the minimum for the bracket it fits in.
float getSingleMinimum(float x) {
	float minimum;
	for (int i = 0; i < 7; i++)
	{
		if (x >= single[i].minimum && x <= single[i].maximum)
		{
			minimum = single[i].minimum;
		}
	}
	return minimum;
};

//Receives an amount, returns the percent for the bracket it fits in.
float getMarriedPercent(float x) {
	float percent;
	for (int i = 0; i < 7; i++)
	{
		if (x >= married[i].minimum && x <= married[i].maximum)
		{
			percent = married[i].percent;
		}
	}
	return percent;
};

//Receives an amount, returns the percent for the bracket it fits in.
float getSinglePercent(float x) {
	float percent;
	for (int i = 0; i < 7; i++)
	{
		if (x >= single[i].minimum && x <= single[i].maximum)
		{
			percent = single[i].percent;
		}
	}
	return percent;
};

//Receives hours, rate, and salaried code, returns the gross pay.
float calcGross(float hours, float rate, char salaried) {
	float grossPay, otPay;
	if (salaried == 'S')
	{
		grossPay = (rate / 26) * (hours / 80);
	}
	else
	{
		if (hours > 80)
		{
			grossPay = 80 * rate;
			otPay = (hours - 80) * (rate + (rate / 2));
			grossPay = grossPay + otPay;
		}
		else
		{
			grossPay = hours * rate;
		}
	}
	return grossPay;
};

//Receives marital status, gross pay, and exemptions, returns federal tax.
//Must call getFederalAllowances and getStateAllowances.
float calcFedTax(char maritalStatus, float grossPay, int exemptions) {
	float taxablePay, federalTax;
	taxablePay = grossPay - getFedAllowance(exemptions);

	if (taxablePay < 0)
	{
		taxablePay = 0;
	}
	if (maritalStatus == 'M')
	{
		federalTax = getMarriedBase(taxablePay) + (getMarriedPercent(taxablePay) * (taxablePay - getMarriedMinimum(taxablePay)));
	}
	if (maritalStatus == 'S')
	{
		federalTax = getSingleBase(taxablePay) + getSinglePercent(taxablePay) * (taxablePay - getSingleMinimum(taxablePay));
	}
	return federalTax;
};

//Receives gross pay, returns the social security tax.
float calcFICA(float grossPay) {
	float FICA;

	FICA = .042 * grossPay;
	if (FICA > 4107.69 / 26)
	{
		FICA = .042 * 4107.69;
	}
	return FICA;
};

//Receives gross pay and exemptions, returns state tax.
//Must call getFederalAllowances and get StateAllowances.
float calcState(float grossPay, float exemptions) {
	float taxablePay, stateTax;
	taxablePay = grossPay - getStateAllowance(exemptions);

	if (taxablePay < 0)
	{
		taxablePay = 0;
	}
	stateTax = taxablePay * .034;

	return stateTax;
};

//Receives gross pay, exemptions, and county, returns county tax.
float calcCounty(float grossPay, float exemptions, char county) {
	float taxablePay, countyTax;
	taxablePay = grossPay - getStateAllowance(exemptions);

	if (taxablePay < 0)
	{
		taxablePay = 0;
	}
	if (county == 'L')
	{
		countyTax = .0095 * taxablePay;
	}
	if (county != 'L')
	{
		countyTax = .007 * taxablePay;
	}

	return countyTax;
};
int main()
{
	//Remove the following line and uncomment out the other code once you see how searching works.

	printf("Employee_ID Gross Pay  Federal    FICA      State    County    Net Pay\n");
	printf("----------- --------- --------- --------- --------- --------- ---------\n");
	for (int i = 0; i< sizeof(employees)/sizeof(employees[0]); i++){
	//for (int i = 1; i< 2; i++){
	float grossPay, federalTax, FICA, stateTax, countyTax, netPay;


	grossPay = calcGross(employees[i].hours, employees[i].rate, employees[i].salaried);
	federalTax = calcFedTax(employees[i].marital_status, grossPay, employees[i].exemptions);
	FICA = calcFICA(grossPay);
	stateTax = calcState(grossPay, employees[i].exemptions);
	countyTax = calcCounty(grossPay, employees[i].exemptions, employees[i].county);
	netPay = grossPay - federalTax - FICA - stateTax - countyTax;

	printf("   %d      %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f\n", employees[i].employee_id, grossPay, federalTax, FICA, stateTax, countyTax, netPay);
	}
	getchar();
	return 0;
}