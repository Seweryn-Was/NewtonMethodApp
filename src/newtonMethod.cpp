#include "./newtonMethod.h"


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

void f(mpfr_t result, const mpfr_t x, const std::vector<mpfr_t>& coefficients){
	mpfr_t fx, term; 
	mpfr_init(fx);
	mpfr_init(term); 

	mpfr_set(fx, coefficients[0], MPFR_RNDN); 

	for(int i = 1; i<coefficients.size(); i++){
		mpfr_set(term,fx , MPFR_RNDN);
		mpfr_mul(term, term  ,x, MPFR_RNDN); 
		mpfr_add(term, term, coefficients[i], MPFR_RNDN); 
		mpfr_set(fx, term, MPFR_RNDN); 
	}
	
	mpfr_set(result, fx, MPFR_RNDN); 

	mpfr_clear(fx); 
	mpfr_clear(term); 
}





double Newton(double x, int mit, double eps,std::vector<double>& coefx, int& st, int& it) {

    double dfatx,xh;
	double v, w;
	
	int exponent = coefx.size()-1;

	if(exponent <= 0){
		st = 2; 
		
		return 0.0;
	}

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
            if (dfatx== 0){
                st = 2;
				break;
			}
            else {
                
                w = std::abs(x);
				xh = x;
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
			 
        }
    }

    if (st == 0 || st == 3) {
        return x;
 	}

    return 0.0; // W przypadku, gdy st nie jest ani 0 ani 3, zwracam 0 dla przykładu
}

double NewtonMPFR(double ix, int mit, double ieps,std::vector<double>& icoefx, int& st, int& it) {

	mpfr_t x, eps, dfx, xh, fx, w, v, temp;
	mpfr_inits(x, eps, dfx, xh, fx, w, v, temp, (mpfr_ptr)0);  
	
	mpfr_set_d(x, ix, MPFR_RNDN); 
	mpfr_set_d(eps, ieps, MPFR_RNDN); 

	std::vector<mpfr_t> coefficients(icoefx.size()); 
	for(int i = 0; i<icoefx.size(); i++){
		mpfr_init_set_d(coefficients[i], icoefx[i], MPFR_RNDN); 
	}

	int exponent = icoefx.size()-1;

	if(exponent <= 0){
		st = 2; 

		for (auto& coef : coefficients) {
			mpfr_clear(coef);
		}
		mpfr_clears(x, eps, dfx, xh, fx, w, v, temp, (mpfr_ptr)0);
		return 0.0;
	}

	std::vector<mpfr_t> coefficientsderiv(icoefx.size()-1); 
	for(int i = 0; i<icoefx.size(); i++){
		mpfr_init(coefficientsderiv[i]); 
		mpfr_mul_ui(coefficientsderiv[i], coefficients[i], exponent-i, MPFR_RNDN);
	}

    if (mit < 1)
        st = 1;
    else {
        st = 3;
        it = 0;
        while ((it < mit) && st ==3){
            it++;
			f(dfx, x, coefficientsderiv); 
            
            if (mpfr_zero_p(dfx)){
                st = 2;
				break;
			}
            else {
                f(fx, x, coefficients); 
                mpfr_abs(w, x, MPFR_RNDN); 
				mpfr_set(xh, x, MPFR_RNDN);
				mpfr_div(temp, fx, dfx, MPFR_RNDN); 
				mpfr_sub(x, x, temp, MPFR_RNDN);
				
				printf("x = %.30f\n", mpfr_get_d(x, MPFR_RNDN)); 
   
                mpfr_abs(v, x, MPFR_RNDN); 
                if (mpfr_less_p(v, w)); 
                    mpfr_set(v, w, MPFR_RNDN);

                if (mpfr_zero_p(v)){
					printf("v = 0\n"); 
                    st = 0;
					break;
				}
                else {
					mpfr_sub(temp, x, xh, MPFR_RNDN); 
					mpfr_abs(temp, temp, MPFR_RNDN); 
					mpfr_div(temp, temp, v, MPFR_RNDN); 
					if (mpfr_cmp(temp, eps)<=0){
						st = 0;
						break;
					}
				}
            }
			 
        }
    }
	
	printf("clean\n"); 
	for (int i =0; i< coefficientsderiv.size(); i++) {
        mpfr_clear(coefficientsderiv[i]);
    }
	
    for (int i =0 ; i< coefficients.size(); i++) {
        mpfr_clear(coefficients[i]);
    }
	
    if (st == 0 || st == 3) {
		ix = mpfr_get_d(x, MPFR_RNDN);
		mpfr_clears(x, eps, dfx, xh, fx, w, v, temp, (mpfr_ptr)0);
        return ix; 
 	}

	mpfr_clears(x, eps, dfx, xh, fx, w, v, temp, (mpfr_ptr)0);
    return 0.0; // W przypadku, gdy st nie jest ani 0 ani 3, zwracam 0 dla przykładu
	
}


Interval<double> Newton(Interval<double> x, int mit, double eps,std::vector<Interval<double>>& coefx, int& st, int& it) {
    Interval<double> dfatx,xh = x;
	Interval<double> v, w;

	int exponent = coefx.size()-1;

	if(exponent <= 0){
		st = 2; 
		printf("exponent<=0"); 
		return {0.0, 0.0};
	}

	std::vector<Interval<double>> coedfx(exponent);
	for(unsigned int i = 0; i< coefx.size()-1; i++){
		coedfx[i] = coefx[i]*exponent;
	
		exponent--;
	}

	Interval<double> fx = f(x, coefx); 
    if (mit < 1)
        st = 1;
    else {
        st = 3;
        it = 0;
        while ((it < mit) && st == 3){
			it++;
            dfatx = f(x, coedfx);
            if (dfatx.a*dfatx.b <= 0){
                st = 2;
				break;
			}
            else {
                xh = x;
                w = IAbs(xh);
				Interval<double> t = fx / dfatx;

                x = x - t;

				//printf("fx/fdx = [%.20f, %.20f] ", t.a, t.b);
				printf("it = %d [%.20f, %.20f]\n",it, x.a, x.b);

				Interval<double> fxh = f(xh,coefx);
				if (fxh.a * fxh.b <=0 && (IntWidth(xh)<= eps || IntWidth(xh) < IntWidth(x))){
                    st = 0;
					break;
				}
				fx = f(x, coefx);

            }
			
        }
    }

    if (st == 0 || st == 3) {
        return xh;
 	}

    return xh; // W przypadku, gdy st nie jest ani 0 ani 3, zwracam 0 dla przykładu
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