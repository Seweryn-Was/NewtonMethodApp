#pragma once

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <limits>
#include <vector>

#include "../imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include<GLFW/glfw3.h>

#include"../Interval.h"
#include "./newtonMethod.h"

#define BUFFER_SIZE 100



class App{
public:
	App(); 
    int init(); 
    int run(); 
    int termnate(); 
	int update(); 


private: 
    GLFWwindow* window;

    bool floatPoint = true, floatPointToInterval = false, interval = false; 
	double input = 1.0;
	double eps = 0.001;
	//double fgl = 1.0, fgr = 1.0;
	Interval<double> inputInt = {1.0, 1.0};
	char epsilonstr[100] = "0.001000";
	int max_itr = 1;

	std::vector<double> coeffFx = {1.0};
	std::vector<Interval<double>> coeffInt = {{1.0, 1.1}}; 
 

	std::vector<char> inputPointBuffer = std::vector<char>(BUFFER_SIZE); 

	std::vector<vector<char>> inputbuffer = {std::vector<char>(BUFFER_SIZE)}; 
	std::vector<vector<char>> inputbufferInt = {std::vector<char>(BUFFER_SIZE), std::vector<char>(BUFFER_SIZE)}; 

	newtonMethodOutput output; 

};