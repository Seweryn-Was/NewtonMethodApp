#include "app.h"

int main() {
	App app;
	if(app.init())
		return 1; 
	app.run(); 
	app.termnate();  
	return 0;
}