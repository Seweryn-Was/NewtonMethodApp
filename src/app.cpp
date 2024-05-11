#include "./app.h"

App::App(){

}



int App::init(){
	if (!glfwInit())
		return 1;

	this->window = glfwCreateWindow(1280, 720, "Newton Method", nullptr, nullptr);
	if (window == nullptr)
		return 1;

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init();
    return 0; 
}

int App::run(){
    while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//my code:
		ImVec2 size = ImGui::GetIO().DisplaySize;
		size.x /= 2; 
		ImGui::SetNextWindowPos(ImVec2(size.x, 0));
		ImGui::SetNextWindowSize(size);
		ImGui::Begin("output", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
		ImGui::Text("Output"); 

		ImGui::Text("Dane:");
		ImGui::Text("[%.20f, %.20f] ", output.xl, output.xr);
        ImGui::Text("eps = %.30f", output.eps);
        ImGui::Text("max_itr = %d", output.max_itr);
        for(int i = 0; i<output.coeff.size(); i++){
            ImGui::Text("x^%d * %.30f",i, output.coeff[i]);
        }
        for(int i = 0; i<output.coeffInt.size(); i++){
            ImGui::Text("x^%d * [%.30f, %.30f]",i, output.coeffInt[i].a, output.coeffInt[i].b);
        }
        ImGui::Text("Wynik");
		ImGui::Text("st = %d", output.st); 
		ImGui::Text("[%.20f, %.20f]", output.result.a, output.result.b); 
		
		ImGui::End(); 

		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(size);
		ImGui::Begin("Newton Method", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

		//ImGui::Text("saved epsilon value: %.40f", eps);

		std::string function; 

		if (ImGui::Checkbox("Interval", &interval)) {
			//printf("switching to interval\n");
			floatPoint = false; 
			interval = true; 
			floatPointToInterval = false; 

			fgr = inputInt.b; 
			fgl = inputInt.a; 

		}

		ImGui::SameLine();
		if (ImGui::Checkbox("float Point", &floatPoint)) {
			//printf("switching to float point\n");
			floatPoint = true; 
			interval = false; 
			floatPointToInterval = false; 
			fgr = input;
			fgl = input;
		}

		ImGui::SameLine();
		if (ImGui::Checkbox("float Point to Interval", &floatPointToInterval)) {
			//printf("switching float point to interval\n");
			floatPoint = false; 
			interval = false; 
			floatPointToInterval = true; 
			Interval<double> temp = floatPointToInter(input); 
			//printf("t: %.30f, %.30f \n", temp.a, temp.b); 
			fgl = temp.a; 
			fgr = temp.b; 
		}


		if(floatPoint) {
			ImGui::Text("Point input:");
			if (ImGui::InputDouble("x", &input)) {
				fgr = input;
				fgl = input;
			}
		}
		if(floatPointToInterval){
			ImGui::Text("Point input:");
			if (ImGui::InputDouble("x", &input)) {
				Interval<double> temp = floatPointToInter(input); 
				
				fgr = temp.b; 
				fgl = temp.a; 
			}
		}

		if (interval) {
			ImGui::Text("Interval input[xl, xr]:");
			ImGui::SetNextItemWidth((int)(size.x/2-12));
			ImGui::InputDouble("##x left", &inputInt.a);
			ImGui::SameLine();
			ImGui::SetNextItemWidth((int)(size.x/2-12));
			ImGui::InputDouble("##x right", &inputInt.b);
			fgr = inputInt.b; 
			fgl = inputInt.a; 
		}


		ImGui::Text("maximum Iterations");
		if (ImGui::InputInt("##max iterations", &max_itr)) {
			if (max_itr < 1) {
				max_itr = 1;
			}
		}
		ImGui::Text("Epsilon"); 
		if (ImGui::InputText("##epsilon", epsilonstr, 100)) {
			if (epsilonstr[0] != '\0')
			{
				//std::cout << std::stod(epsilonstr) << "\n";
				eps = std::stod(epsilonstr);
			}
			else {
				epsilonstr[0] = '0';
			}
		}

		ImGui::Text("Function a_i:");
		ImGui::SameLine();
		if (ImGui::Button("[+] Add coefficient")) {
			coeffFx.push_back(1.0);
			coeffInt.push_back({1.0, 1.1}); 
		}
		ImGui::SameLine();
		if (ImGui::Button("[-] Delete coefficient")) {
			if (coeffFx.size() > 0) {
				coeffFx.pop_back();
			}
			if (coeffInt.size() > 0) {
				coeffInt.pop_back();
			}

		}
		//TO DO 
		//wypisywanei funkcji dla przdziaw etc
		// function = "W(x)=";
		// if(floatPoint){
		// 	for (int i = coeffFx.size()-1; i >= 0; i--) {
		// 		if (coeffFx[i] != 0) 
		// 		if (coeffFx[i] != 1) {
		// 			function += doubleToStringNoZeros(coeffFx[i]) + "*x^" + std::to_string(i) + " + ";
		// 		}
		// 		else {	
		// 			function += "x^" + std::to_string(i) + " + ";
		// 		}
		// 	}			
		// }else{
		// 	for (int i = coeffInt.size()-1; i >= 0; i--) {
		// 		function += "[" + doubleToStringNoZeros(coeffInt[i].a)+", "+doubleToStringNoZeros(coeffInt[i].b) + "]*x^" + std::to_string(i) + " + ";	
		// 	}
		// }


		// if (coeffFx.size() > 0) {
		// 	function[function.length() - 2] = ' ';	
		// }
		// else {
		// 	function += "null"; 
		// }
		// ImGui::Text(function.c_str()); 

		if(floatPoint){
			for (int i = 0; i < coeffFx.size(); i++) {
				std::string label = "x^" + std::to_string(i);
                ImGui::Text(label.c_str());
                ImGui::SameLine(); 
                label = "##"+label; 
                ImGui::SetNextItemWidth((int)(size.x/2-25));
				ImGui::InputDouble(label.c_str(), &coeffFx[i]);
			}
		}

		if(floatPointToInterval){
			for (int i = 0; i < coeffFx.size(); i++) {
				//printf("%d\n", i); 
				std::string label = "x^" + std::to_string(i);
                ImGui::Text(label.c_str());
                ImGui::SameLine();
                label = "##"+label; 
                ImGui::SetNextItemWidth((int)(size.x/2-25));
				ImGui::InputDouble(label.c_str(), &coeffFx[i]);
				coeffInt[i] = floatPointToInter(coeffFx[i]); 
			}
		}

		if(interval){
			for (int i = 0; i < coeffInt.size(); i++) {
				std::string label = "x^" + std::to_string(i);
                ImGui::Text(label.c_str());
                ImGui::SameLine();
                label = "##"+label; 
                ImGui::SetNextItemWidth((int)(size.x/2-27));
				ImGui::InputDouble(label.c_str(), &coeffInt[i].a);
                label = label + "sec";
				ImGui::SameLine(); 
                ImGui::SetNextItemWidth(size.x/2-27);
				ImGui::InputDouble(label.c_str(), &coeffInt[i].b);
			}
		}

		if (coeffFx.empty() || coeffInt.empty()) {
			ImGui::Text("NO COEFFICIENTS"); 
		}
		
		//TO DO zapisywanie do outputu wyniku 
		// i zrobi poprawne wypisywanie 
		// stworzenie funckji NEWTOMETHOD dla przedzialu
		if(ImGui::Button("Calculate")){ 
	
			this->output.xl = fgl; 
			this->output.xr = fgr; 
            this->output.eps = eps; 
            this->output.max_itr  = max_itr; 
            //this->output.st = 1;
            if(interval || floatPointToInterval){
				this->output.coeffInt = coeffInt;
			}else{ 
                this->output.coeffInt = {}; 
			}

            if(floatPoint){
                this->output.coeff = coeffFx; 
			}else{
                this->output.coeff = {}; 
			}

			if(floatPoint){
				std::vector<double> tempF = reverseVec(coeffFx); 
				this->output.result = NewtonMethod(fgl, fgr, eps, max_itr, tempF, &this->output.st); 
			}

			if(floatPointToInterval || interval){
				//printf("newton method interval\n"); 
				//reverseVecInt(coeffInt);
				std::vector<Interval<double>> tempF = reverseVecInt(coeffInt);

				this->output.result = NewtonMethod(fgl, fgr, eps, max_itr, tempF,&this->output.st);
				//printf("END newton method interval\n"); 
			}

		}

		ImGui::End();

		//end of my code
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glfwSwapBuffers(window);
		
	}
	return 0; 
}

int App::termnate(){
    printf("Terminating app\n"); 
    ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
    return 0; 
}