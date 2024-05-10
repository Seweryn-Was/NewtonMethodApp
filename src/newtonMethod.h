#pragma once

#include "../Interval.h"
#include <limits>
#include <vector>


using namespace interval_arithmetic;

void NewtonMethod(double fgl, double fgr, double eps, int max_itr, std::vector<double> coeffArr, int *st );
Interval<double> f(Interval<double> x, std::vector<double> coeffArrfx);
bool shouldNewtonMethodEnd(Interval<double>x1Inter, Interval<double>x0Inter, double eps);
bool epsilionCondition(double eps, double x1, double x0);

Interval<double> floatPointToInter(double num); 

std::string doubleToString(double value);
std::string doubleToStringNoZeros(double value); 

struct newtonMethodOutput { 
	double xl, xr; 
	std::vector<double> coeff; 
	std::vector<Interval<double>> coeffInt; 
	int st; 
	int max_itr; 
	double eps; 
};