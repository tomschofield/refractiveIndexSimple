#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "analysis.h"
#include "ofxControlPanel.h"
#include "ofxFileHelper.h"

//change
//MOVED TO ANALYSIS CLASS
#include "ofxQtVideoSaver.h"
//

class testApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    float returnGaussian(float x, float spread, float height, float max, float centre);
    
    ///other functions
    void setupCamera(int w, int h, int whichSource, int desiredFrameRate, bool firstSetup);
    void drawInstructions();
    //void updateVars(string whichVar);
    ofVideoGrabber vidGrabber;
    ofTexture mirrorTexture;
    
    analysis masterAnalysis;
    vector<ofImage> resultsOfAnalysis;
    
    ofImage startImage, endImage;
    
    string locationChooser;
    string analysisChooser;
    
    ofxFileHelper mainFileHelper;
    
    int codecChooser;
    int whichCodec;
    vector<string> returnedCodecNames;
    //vars which hold camera and saver setup information
    int camWidth;
    int camHeight;
    
    string codecName;
    int whichImage;
    int numOfInputs;
    int camInput;
    int desFrameRate;
    string camInputName;
    
    //report string for letting us know whats what with the camera settings
    string camStatus;
    string keyControlMessage1;
    string keyControlMessage2;
    
    bool showCursor;
    bool showGui;
    bool showCameraInput;
    int menuState;
    int mouseX, mouseY;
    
    //global gui variables 
    
    //pointer to kye input gui object - this is kind of a hack because the text input doesn't seem to be fully implemented
    guiTypeTextInput *tl;
    int scanLineWidth;
    string whichGraph;
    bool showGraphLine;
    int currentPanel;
    int frameRateWeSet;
    //menu state is the main control sequence variable should have the following modes
    /*
     1 setup analyis
     2 display and conduct analysis
     3 display results
     
     */
    bool record;
    ofTrueTypeFont font;
    
    ofVideoPlayer player;
    //for the gui
    
    void eventsIn(guiCallbackData & data);
    void grabBackgroundEvent(guiCallbackData & data);
    
    ofxControlPanel gui;
    simpleFileLister lister;
    simpleLogger logger;
    vector<string>analysisNames;
    ofPixels camPixels;
    
    
};

#endif
