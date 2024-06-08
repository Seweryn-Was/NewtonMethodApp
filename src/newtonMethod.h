#pragma once

#include "../Interval.h"
#include <limits>
#include <vector>

using namespace interval_arithmetic;

Interval<double> Newton(Interval<double> x, int mit, double eps,std::vector<Interval<double>>& coefx, int& st, int& it);
Interval<double> Newton(Interval<double> x, int mit, double eps,std::vector<double>& coefx, int& st, int& it);
double Newton(double x, int mit, double eps,std::vector<double>& coefx, int& st, int& it);
double NewtonMPFR(double x, int mit, double eps,std::vector<double>& coefx, int& st, int& it);



Interval<double> f(Interval<double> x, std::vector<Interval<double>> coeffArrfx);
Interval<double> f(Interval<double> x, std::vector<double> coeffArrfx);
double f(double x, std::vector<double> coeffArrfx);

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
	int it; 
	double eps; 
	Interval<double> result; 
};