#pragma once

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <vector>

#include "../imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include<GLFW/glfw3.h>

#include"../Interval.h"
#include "./newtonMethod.h"


class App{
public:
    App(); 
    int init(); 
    int run(); 
    int termnate(); 


private: 
    GLFWwindow* window;

    bool floatPoint = true, floatPointToInterval = false, interval = false; 
	double input = 1.0;
	double eps = 0.001;
	double fgl = 1.0, fgr = 1.0;
	Interval<double> inputInt = {1.0, 1.0};
	char epsilonstr[100] = "0.001000";
	int max_itr = 1;

	std::vector<double> coeffFx = {1.0};
	std::vector<Interval<double>> coeffInt = {{1.0, 1.1}}; 

	newtonMethodOutput output; 
    

}; 

