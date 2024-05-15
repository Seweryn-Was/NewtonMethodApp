#include "./app.h"

App::App(){
	floatPoint = true;
	floatPointToInterval = false;
	interval = false; 
	input = 1.0;
	eps = 0.001;
	fgl = 1.0, fgr = 1.0;
	inputInt = {1.0, 1.0};
	max_itr = 1;

	std::vector<double> coeffFx = {1.0};
	std::vector<Interval<double>> coeffInt = {{1.0, 1.1}}; 
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

int App::update(){
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
	ImGui::Text("it = %d", output.it); 
	ImGui::Text("[%.20f, %.20f]", output.result.a, output.result.b); 
	
	ImGui::End(); 

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(size);
	ImGui::Begin("Newton Method", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

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
		floatPoint = true; 
		interval = false; 
		floatPointToInterval = false; 
		fgr = input;
		fgl = input;
	}

	ImGui::SameLine();
	if (ImGui::Checkbox("float Point to Interval", &floatPointToInterval)) {
		floatPoint = false; 
		interval = false; 
		floatPointToInterval = true; 
		Interval<double> temp = floatPointToInter(input); 
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
			this->output.result = Newton({fgl, fgr}, max_itr, eps, tempF, this->output.st, this->output.it);
		}

		if(floatPointToInterval || interval){
			std::vector<Interval<double>> tempF = reverseVecInt(coeffInt);
			this->output.result = Newton({fgl, fgr}, max_itr, eps, tempF, this->output.st, this->output.it);
		}

	}
	ImGui::End();
}

int App::termnate(){
    printf("Terminating app\n"); 
    ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
    return 0; 
}