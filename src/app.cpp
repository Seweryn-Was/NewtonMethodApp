#include "./app.h"

App::App(){
	floatPoint = true;
	floatPointToInterval = false;
	interval = false; 
	input = 1.0;
	eps = 0.000000000000001;
	//fgl = 1.0, fgr = 1.0;
	inputInt = {1.0, 1.0};
	max_itr = 100;
	output.st = -1; 
	output.max_itr = max_itr; 

	std::vector<double> coeffFx = {};
	std::vector<Interval<double>> coeffInt = {}; 

	// Interval<double> test2 = IntRead<double>(std::to_string(3.0)); 

	// printf("%.310f\n", test2.a); 
	// printf("%.310f\n", test2.b); 
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

		//app code:
		update(); 

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glfwSwapBuffers(window);
		
	}
	return 0; 
}

void App::OutputWindow(){
		ImVec2 size = ImGui::GetIO().DisplaySize;
	size.x /= 2; 
	ImGui::SetNextWindowPos(ImVec2(size.x, 0));
	ImGui::SetNextWindowSize(size);
	ImGui::Begin("output", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
	ImGui::Text("Output"); 

	ImGui::Text("Dane:");
	if(floatPoint){
		ImGui::Text("x0 = %.20f", output.xl);
	}
	else{
		ImGui::Text("x0 = [%.20f, %.20f] ", output.xl, output.xr);
	}
	
	
	ImGui::Text("eps = %.30f", output.eps);
	ImGui::Text("max_itr = %d", output.max_itr);
	for(int i = 0; i<output.coeff.size(); i++){
		ImGui::Text("x^%d * %.30f",i, output.coeff[i]);
	}
	for(int i = 0; i<output.coeffInt.size(); i++){
		//ImGui::Text("x^%d * [%.40f, %.30f]",i, output.coeffInt[i].a, output.coeffInt[i].b);
		std::string a, b;  
		output.coeffInt[i].IEndsToStrings(a, b); 
		ImGui::Text("x^%d [%s, %s]", i, a.c_str(), b.c_str());
	}
	ImGui::Text("Wynik");
	ImGui::Text("st = %d", output.st); 
	ImGui::Text("it = %d", output.it); 

	if(floatPoint){
		ImGui::Text("x = %.30f ", output.result.a); 

	}else{
		std::string a, b;  
		output.result.IEndsToStrings(a, b); 
		ImGui::Text("x = [%s, %s]", a.c_str(), b.c_str());
		ImGui::Text("szerokosc = %.3e", IntWidth(output.result));
	}

	
	ImGui::End(); 
}

int App::update(){
	//my code:

	this->OutputWindow(); 
	ImVec2 size = ImGui::GetIO().DisplaySize;
	size.x /= 2;
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(size);
	ImGui::Begin("Newton Method", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

	std::string function; 

	if (ImGui::Checkbox("Interval", &interval)) {
		//printf("switching to interval\n");
		floatPoint = false; 
		interval = true; 
		floatPointToInterval = false; 
		for (int i = 0; i < coeffInt.size(); i++) {
			
			coeffInt[i].a = LeftRead<double>(std::string(inputbufferInt[i*2].data())); 

			coeffInt[i].b = RightRead<double>(std::string(inputbufferInt[i*2+1].data())); 

		}
	}

	ImGui::SameLine();
	if (ImGui::Checkbox("float Point", &floatPoint)) {
		floatPoint = true; 
		interval = false; 
		floatPointToInterval = false; 
	}

	ImGui::SameLine();
	if (ImGui::Checkbox("float Point to Interval", &floatPointToInterval)) {
		floatPoint = false; 
		interval = false; 
		floatPointToInterval = true; 
		Interval<double> temp = floatPointToInter(input); 
		for(int i =0; i<coeffFx.size(); i++){
			coeffInt[i] = IntRead<double>(std::string(inputbuffer[i].data())); 
		}

	}


	if(floatPoint || floatPointToInterval) {
		ImGui::Text("Point input:");
		if(ImGui::InputText("x",inputPointBuffer.data(), BUFFER_SIZE)){
				std::string t = std::string(inputPointBuffer.data());
				if(t.length() > 1 || ( t.length() == 1  && t[0] != '-') ) 
				{	
					input = LeftRead<double>(std::string(inputPointBuffer.data())); //std::stod(inputPointBuffer.data());
					inputInt = IntRead<double>(std::string(inputPointBuffer.data())); 
					
				}else{
					input = 0.0; 
					inputInt = IntRead<double>(std::string("0.0")); 
				}
			}
	}

	if (interval) {
		ImGui::Text("Interval input[xl, xr]:");
		ImGui::SetNextItemWidth((int)(size.x/2-12));
		ImGui::InputDouble("##x left", &inputInt.a);
		ImGui::SameLine();
		ImGui::SetNextItemWidth((int)(size.x/2-12));
		ImGui::InputDouble("##x right", &inputInt.b);
	
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
			eps = LeftRead<double>(std::string(&epsilonstr[0])); //std::stod(epsilonstr);
		}
		else {
			epsilonstr[0] = '0';
		}
	}

	ImGui::Text("Function a_i:");
	ImGui::SameLine();
	if (ImGui::Button("[+] Add coefficient")) {
		coeffFx.push_back(0.0);
		coeffInt.push_back({0.0, 0.0});

		std::string t = "0.0"; 

		inputbuffer.push_back(vector<char>(BUFFER_SIZE));
		std::copy(t.begin(), t.end(), inputbuffer[inputbuffer.size()-1].begin()); 
		
		inputbufferInt.push_back(vector<char>(BUFFER_SIZE));
		std::copy(t.begin(), t.end(), inputbufferInt[inputbufferInt.size()-1].begin()); 

		inputbufferInt.push_back(vector<char>(BUFFER_SIZE)); 
		std::copy(t.begin(), t.end(), inputbufferInt[inputbufferInt.size()-1].begin()); 


	}
	ImGui::SameLine();
	if (ImGui::Button("[-] Delete coefficient")) {
		if (coeffFx.size() > 0) {
			coeffFx.pop_back();
		}
		if (coeffInt.size() > 0) {
			coeffInt.pop_back(); 
			inputbuffer.pop_back(); 
			inputbufferInt.pop_back();
		}

	}

	if(floatPoint || floatPointToInterval){
		for (int i = 0; i < coeffFx.size(); i++) {
			std::string label = "x^" + std::to_string(i);
			ImGui::Text(label.c_str());
			ImGui::SameLine(); 
			label = "##"+label; 
			ImGui::SetNextItemWidth((int)(size.x/2-25));
			if(ImGui::InputText(label.c_str(), inputbuffer[i].data(), BUFFER_SIZE)){
				std::string t = std::string(inputbuffer[i].data());
				if(t.length() > 1 || ( t.length() == 1  && t[0] != '-') ) 
				{	
					coeffFx[i] = std::stod(inputbuffer[i].data());
					coeffInt[i] = IntRead<double>(std::string(inputbuffer[i].data())); 

					std::string temp = std::to_string(coeffInt[i].a);  
					std::copy(temp.begin(), temp.end(), inputbufferInt[i*2].begin()); 

					temp = std::to_string(coeffInt[i].b);  
					std::copy(temp.begin(), temp.end(), inputbufferInt[i*2+1].begin()); 

				}else{
					coeffFx[i] = 0.0; 
					coeffInt[i] = IntRead<double>(std::string("0.0")); 
				}
			}
		}
	}

	if(interval){
		for (int i = 0; i < coeffInt.size(); i++) {
			std::string label = "x^" + std::to_string(i);
			ImGui::Text(label.c_str());
			ImGui::SameLine();
			label = "##"+label; 
			ImGui::SetNextItemWidth((int)(size.x/2-25)); 
			if(ImGui::InputText(label.c_str(), inputbufferInt[i*2].data(), BUFFER_SIZE)){
				std::string t = std::string(inputbufferInt[i*2].data());
				//printf("x[%d] = %s \n", i*2, t.c_str()); 
				if(t.length() > 1 || ( t.length() == 1  && t[0] != '-') ) 
				{	
					coeffInt[i].a = LeftRead<double>(std::string(inputbufferInt[i*2].data())); 
				}else{
					coeffInt[i].a = LeftRead<double>(std::string("0.0")); 
				}
			}

			label = label + "sec";
			ImGui::SameLine(); 
			ImGui::SetNextItemWidth((int)(size.x/2-25)); 
			if(ImGui::InputText(label.c_str(), inputbufferInt[i*2+1].data(), BUFFER_SIZE)){
				std::string t = std::string(inputbufferInt[i*2+1].data());
				
				if(t.length() > 1 || ( t.length() == 1  && t[0] != '-') ) 
				{	
					coeffInt[i].b = RightRead<double>(std::string(inputbufferInt[i*2+1].data())); 
				}else{
					coeffInt[i].b = RightRead<double>(std::string("0.0")); 
				}
			}
		}
	}

	if (coeffFx.empty() || coeffInt.empty()) {
		ImGui::Text("NO COEFFICIENTS"); 
	}

	if(ImGui::Button("Calculate")){ 

		this->output.xl = inputInt.a; 
		this->output.xr = inputInt.b; 
		this->output.eps = eps; 
		this->output.max_itr  = max_itr; 
		this->output.it = 0; 
		double eps1 = eps ;//<1e-15 ? 1e-14 : eps;

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
			this->output.result.a = Newton(input, max_itr, eps1, tempF, this->output.st, this->output.it);
		}

		if(floatPointToInterval || interval){
			std::vector<Interval<double>> tempF = reverseVecInt(coeffInt);
			this->output.result = Newton(inputInt, max_itr, eps1, tempF, this->output.st, this->output.it);
		}

	}
	ImGui::End();
}

int App::termnate(){
    printf("Terminating app1\n"); 
    ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();

    return 0; 
}