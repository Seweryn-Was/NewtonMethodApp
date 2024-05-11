#pragma once

#include "../Interval.h"
#include <limits>
#include <vector>


using namespace interval_arithmetic;

Interval<double> NewtonMethod(double fgl, double fgr, double eps, int max_itr, std::vector<double> coeffArr, int *st );
Interval<double> NewtonMethod(double fgl, double fgr, double eps, int max_itr, std::vector<Interval<double>> coeffArr, int *st );

Interval<double> f(Interval<double> x, std::vector<double> coeffArrfx);
bool shouldNewtonMethodEnd(Interval<double>x1Inter, Interval<double>x0Inter, double eps);
bool epsilionCondition(double eps, double x1, double x0);

Interval<double> floatPointToInter(double num); 

std::string doubleToString(double value);
std::string doubleToStringNoZeros(double value); 

std::vector<Interval<double>> reverseVecInt(std::vector<Interval<double>>& vec);
std::vector<double> reverseVec(std::vector<double>& vec);


struct newtonMethodOutput { 
	double xl, xr; 
	std::vector<double> coeff; 
	std::vector<Interval<double>> coeffInt; 
	int st; 
	int max_itr; 
	double eps; 
	Interval<double> result; 
};