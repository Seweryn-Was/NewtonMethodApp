#include "./newtonMethod.h"

Interval<double> f(Interval<double> x, std::vector<double> coeffArrfx){
	Interval<double> toReturn = {coeffArrfx[0],coeffArrfx[0]} ;
	for(unsigned int i = 1; i<coeffArrfx.size(); i++){
		toReturn = toReturn * x;
		Interval<double> add = {coeffArrfx[i], coeffArrfx[i]};
		toReturn = toReturn + add;

	}
	//std::cout<<"["<<toReturn.a<<" "<<toReturn.b<<"]\n";
	return toReturn;
}

Interval<double> f(Interval<double> x, std::vector<Interval<double>> coeffArrfx){
	Interval<double> toReturn = coeffArrfx[0];
	for(unsigned int i = 1; i<coeffArrfx.size(); i++){
		toReturn = toReturn * x;
		Interval<double> add = coeffArrfx[i];
		toReturn = toReturn + add;

	}
	//std::cout<<"["<<toReturn.a<<" "<<toReturn.b<<"]\n";
	return toReturn;
}


Interval<double> NewtonMethod(double fgl, double fgr, double eps, int max_itr, std::vector<double>& coeffArrfx, int* st){
	//printf("Newton Method call\n"); 
	if(max_itr<1){
		*st = 1; // maksymalne iteracje zostaly zle wproawdzone
		return {0.0, 0.0};
	}
	
	int exponent = coeffArrfx.size()-1;
	if(exponent <= 0){
		*st = 2; 
		return {0.0, 0.0}; 
	}

	/*
	 * 	f(x) = x^3 + 3x^2 - 4x + 2
	 * 	coeffArrfx = [1, 3, -4, 2]
	 *
	 *	f'(x) = 3x^2 + 6x - 4 + 0
	 *	coefArrfdx = [3, 6, 4]
	 */
	std::vector<double> coeffArrfdx(exponent);

	for(unsigned int i = 0; i<coeffArrfx.size(); i++){
		coeffArrfdx[i] = coeffArrfx[i]*exponent;
		exponent--;
	}
 

	Interval<double> searchInter = {fgl, fgr};	//search Interval
	Interval<double> x0Inter = {fgl, fgr};		// xi
	Interval<double> x1Inter = {fgl, fgr}; 		// xi+1
	Interval<double> fxInter;					//f(xi)
	Interval<double> fdxInter;					//f'(xi)
	int numberOfIterations = 0;



	while(true) {
		numberOfIterations ++;
		x0Inter = x1Inter;
		fdxInter = f(x0Inter, coeffArrfdx);
		fxInter = f(x0Inter, coeffArrfx);
		/* xi+1 = xi - (f(xi)/f'(xi)) --> x1Inter = x0Inter - (fxInter/fdxInter)*/
		if(fdxInter.a * fdxInter.b < 0){
			//std::cout<<"f'(x)==0";
			*st=2;
			//std::cout<<"st = "<<*st<<" : w pewnej iteracji uzyskano f'(x) = 0]\n";
			break;
		}
		x1Inter = x0Inter - (fxInter/fdxInter);

		/*
		 * |xi+1 - xi|/max(|xi+1|,  |xi|) < eps
		 * xi+1 = xi = 0
		 * step >
		 */
		if(shouldNewtonMethodEnd(x1Inter, x0Inter, eps)){
			*st = 0; //zakonczenie programu - poprwane zakonczenie
			break;
		}
		if(numberOfIterations >= max_itr){
			*st = 3; // osogianieto limit iteracji
			//std::cout<<"st = "<<*st<<" : osiagnieto limit iteracji\n";
			break;
		}
	}
	//std::cout<<"Koniec Algorytm\nliczba iteracji:"<<numberOfIterations<<"\nprzedial wynikowy: ["<<x1Inter.a<<", "<<x1Inter.b<<"]\n";
	return x1Inter; 
}


Interval<double> NewtonMethod(double fgl, double fgr, double eps, int max_itr, std::vector<Interval<double>>& coeffArrfx, int* st){
	//printf("Newton Method call\n"); 
	if(max_itr<1){
		*st = 1; // maksymalne iteracje zostaly zle wproawdzone
		return {0.0, 0.0};
	}
	
	int exponent = coeffArrfx.size()-1;
	if(exponent <= 0){
		*st = 2; 
		return {0.0, 0.0}; 
	}

	/*
	 * 	f(x) = x^3 + 3x^2 - 4x + 2
	 * 	coeffArrfx = [1, 3, -4, 2]
	 *
	 *	f'(x) = 3x^2 + 6x - 4 + 0
	 *	coefArrfdx = [3, 6, 4]
	 */
	std::vector<Interval<double>> coeffArrfdx(exponent);

	for(unsigned int i = 0; i<coeffArrfx.size(); i++){
		coeffArrfdx[i] = coeffArrfx[i]*exponent;
		//printf("%d [%.30f, %.30f]\n", i,coeffArrfdx[i].a, coeffArrfdx[i].b);
		exponent--;
	}

	Interval<double> searchInter = {fgl, fgr};	//search Interval
	Interval<double> x0Inter = {fgl, fgr};		// xi
	Interval<double> x1Inter = {fgl, fgr}; 		// xi+1
	Interval<double> fxInter;					//f(xi)
	Interval<double> fdxInter;					//f'(xi)
	int numberOfIterations = 0;
	//printf("while\n"); 
	while(true) {
		numberOfIterations ++; 
		x0Inter = x1Inter;

		fdxInter = f(x0Inter, coeffArrfdx);
		fxInter = f(x0Inter, coeffArrfx);

		if(fdxInter.a * fdxInter.b < 0){
			//std::cout<<"f'(x)==0\n";
			*st=2;
			break;
		}
		Interval<double> h = (fxInter/fdxInter);
		x1Inter = x0Inter - h; 

		if(fabs(h.a)<eps || fabs(h.b) <eps){
			//printf("--->eps %.20f, %.20f\n", h.a, h.b); 
			*st = 0;
			break; 
		}
		
		if(numberOfIterations > max_itr)
		{	
			//printf("max_itr\n");
			*st = 3; 
			break;
		}
		//printf("%d [%.20f, %.20f]\n", numberOfIterations, x1Inter.a, x1Inter.b); 
	}
	
	return x1Inter; 
}


bool shouldNewtonMethodEnd(Interval<double>x1Inter, Interval<double>x0Inter, double eps){
	if(epsilionCondition(eps,x1Inter.a, x0Inter.a) || epsilionCondition(eps,x1Inter.b, x0Inter.b)){
		return true;
	}
	if((x1Inter.a ==0 && x0Inter.a==0) || (x1Inter.b ==0 && x0Inter.b==0) ){
		return true;
	}
	return false;
}

bool epsilionCondition(double eps, double x1, double x0){
	return abs(x1-x0)/fmax(abs(x1), abs(x0))<eps;
}


Interval<double> floatPointToInter(double num){
	Interval<double> temp; 
	temp.a = std::nextafter(num , -std::numeric_limits<double>::max());  
	temp.b = std::nextafter(num , std::numeric_limits<double>::max()); 
	return temp; 
}

std::string doubleToString(double value) {
	std::stringstream ss;
	ss << std::setprecision(std::numeric_limits<double>::max_digits10) << value;
	return ss.str();
}

std::string doubleToStringNoZeros(double value) {
	std::string str = std::to_string(value); // Convert double to string

	// Remove trailing zeros after the decimal point
	size_t pos = str.find_last_not_of('0');
	if (pos != std::string::npos && str[pos] == '.') {
		pos--; // Keep the decimal point if it's the last character
	}
	str.erase(pos + 1, std::string::npos);

	return str;
}


std::vector<double> reverseVec(std::vector<double>& vec){
	std::vector<double> result = vec; 
	std::reverse(result.begin(), result.end());
	return result; 
}

std::vector<Interval<double>> reverseVecInt(std::vector<Interval<double>>& vec){
	std::vector<Interval<double>> result = vec; 
	std::reverse(result.begin(), result.end());
	return result; 
}