#include "ofAppGlutWindow.h"
#include "testApp.h"
#include "RefractiveIndex.h"

int main() {
	ofAppGlutWindow window;
    //window.setGlutDisplayString("rgba double samples >= 8");
	ofSetupOpenGL(&window, 1024, 768, OF_WINDOW);
	
    //David's new code structure
    ofRunApp(new RefractiveIndex());

    //Old test app code
   // ofRunApp(new testApp());
}
