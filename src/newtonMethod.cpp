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


double f(double x, std::vector<double> coeffArrfx){
	double fx = coeffArrfx[0];
	for(unsigned int i = 1; i<coeffArrfx.size(); i++){
		fx = fx * x + coeffArrfx[i];
	}
	return fx;
}

//double fgl, double fgr, double eps, int max_itr, std::vector<Interval<double>>& coeffArrfx, int* st
double Newton(double x, int mit, double eps,std::vector<double>& coefx, int& st, int& it) {
    double dfatx,xh;
	double v, w;

	int exponent = coefx.size()-1;
	std::vector<double> coedfx(exponent);
	for(unsigned int i = 0; i< coefx.size()-1; i++, exponent--){
		coedfx[i] = coefx[i]*exponent;
	}

    if (mit < 1)
        st = 1;
    else {
        st = 3;
        it = 0;
        while ((it < mit) && st ==3){
            it++;
            dfatx = f(x, coedfx); 
			//printf("fdx = %.5f  ", dfatx); 
            if (dfatx== 0){
                st = 2;
				break;
			}
            else {
                
                w = std::abs(x);
				//printf("f(x, coefx) = %.5f ", f(x, coefx));
                x = x - f(x, coefx) / dfatx;
                v = std::abs(x);
                if (v < w)
                    v = w;

                if (v ==0){
                    st = 0;
					break;
				}
                else if ((std::abs(x - xh)/v) <= eps){
                    st = 0;
					break;
				}
            }
			printf("x = %.20f\n",x); 
        }
    }

    if (st == 0 || st == 3) {
        return x;
 	}

    return 0.0; // W przypadku, gdy st nie jest ani 0 ani 3, zwracam 0 dla przykładu
}

Interval<double> Newton(Interval<double> x, int mit, double eps,std::vector<double>& coefx, int& st, int& it) {
    Interval<double> dfatx,xh;
	Interval<double> v, w;

	int exponent = coefx.size()-1;
	std::vector<double> coedfx(exponent);
	for(unsigned int i = 0; i< coefx.size()-1; i++){
		coedfx[i] = coefx[i]*exponent;
	
		exponent--;
	}

	printf("mit = %d\n", mit); 
    if (mit < 1)
        st = 1;
    else {
        st = 3;
        it = 0;
        while ((it < mit)){
            it++;
            dfatx = f(x, coedfx);
            if (dfatx.a*dfatx.b <= 0){
                st = 2;
				break;
			}
            else {
                xh = x;
                w = IAbs(xh);
                x = x - f(x, coefx) / dfatx;
                v = IAbs(x);
				Interval<double> temp = v; 
                if (v.a < w.a)
                    temp.a = w.a;
				if (v.b < w.b)
                    temp.b = w.b;

                if (temp.a * temp.b <=0){
                    st = 0;
					break;
				}
                else if ((std::abs(x.a - xh.a)/temp.a) <= eps && (std::abs(x.b - xh.b)/temp.b) <= eps){
                    st = 0;
					break;
				}
            }
			printf("it = %d [%.20f, %.20f]\n",it, x.a, x.b);
        }
    }

    if (st == 0 || st == 3) {
        return x;
 	}

    return {0.0, 0.0}; // W przypadku, gdy st nie jest ani 0 ani 3, zwracam 0 dla przykładu
}



Interval<double> Newton(Interval<double> x, int mit, double eps,std::vector<Interval<double>>& coefx, int& st, int& it) {
    Interval<double> dfatx,xh;
	Interval<double> v, w;

	int exponent = coefx.size()-1;
	std::vector<Interval<double>> coedfx(exponent);
	for(unsigned int i = 0; i< coefx.size()-1; i++){
		coedfx[i] = coefx[i]*exponent;
	
		exponent--;
	}

	printf("mit = %d\n", mit); 
    if (mit < 1)
        st = 1;
    else {
        st = 3;
        it = 0;
        while ((it < mit)){
            it++;
            dfatx = f(x, coedfx);
            if (dfatx.a*dfatx.b <= 0){
                st = 2;
				break;
			}
            else {
                xh = x;
                w = IAbs(xh);
                x = x - f(x, coefx) / dfatx;
                v = IAbs(x);
				Interval<double> temp = v; 
                if (v.a < w.a)
                    temp.a = w.a;
				if (v.b < w.b)
                    temp.b = w.b;

                if (temp.a * temp.b <=0){
                    st = 0;
					break;
				}
                else if ((std::abs(x.a - xh.a)/temp.a) <= eps && (std::abs(x.b - xh.b)/temp.b) <= eps){
                    st = 0;
					break;
				}
            }
			printf("it = %d [%.20f, %.20f]\n",it, x.a, x.b);
        }
    }

    if (st == 0 || st == 3) {
        return x;
 	}

    return {0.0, 0.0}; // W przypadku, gdy st nie jest ani 0 ani 3, zwracam 0 dla przykładu
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