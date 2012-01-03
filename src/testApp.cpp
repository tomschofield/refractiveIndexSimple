#include "testApp.h"


//----ON THE QUESTION OF VIDEO LAG ------------------//

/* 
 http://www.cycling74.com/forums/topic.php?id=32247
 
 You can use the Pinnacle Dazzle RCA/Composite / USB 2.0 adaptor which gives you uncompressed NTSC/PAL over USB 2.0.
 
 Daystar XTraview, also uncompressed NTSC/PAL over USB 2.0
 
 Aditionally you can use similar and cheaper devices that work with the Video Glide Driver
 
 (IIRC the Xtraview comes with a license of VideoGlide driver). All the video glide devices are Uncompressed, USB 2.0.
 
 Here is an image of the capture devices for size.
 http://i.imgur.com/CKLbM.jpg
 
 Note, the Diamond Multimedia device shows up in the driver list, but I've not gotten it to work. I would stay away from it if on the Mac. The Pinnacle and the XLR8 work just fine, and show up in Jitter and other Quicktime compatible applications.
 
 Here is the Jitter list:
 http://i.imgur.com/m0IA7.jpg
 
 I've tested all 3 of the above, and they all work about the same (albeit the Diamond on PC only). I suspect they use the same chipset or same family of device, with small changes to provide vendor lockout (Diamond, im looking at you).
 
 These devices are cheap, but they work. Latency is I want to say around 3-4 frames (been a while since I tested). Better than DV (which usually 7, although some folks say 4 on new machines/OS'es, ive never seen 4 frames personally), and I think thats about on par with the DFG-1394.
 
 SO IT'S NEVER GOING TO BE CONSISTENT... OR "REAL TIME" - jamie... \
 
 */


//--------------------------------------------------------------
void testApp::setup(){
    
    //changed for version control test
    camHeight=48023434;
    showGraphLine=false;
    //switch for controlling all analysis etc V important variable
    menuState=0;
    //some change
    camWidth=640;
    camHeight=480;
    desFrameRate=-111;// the camera framerate
    
    //set gui defaults moved to start of setup because we need these values to set sliders in gui   
    masterAnalysis.setGUIDefaults();
    //ofSetFrameRate(60);   // this produces 10 or 11 frames of latency frames in the camera feed 
    
    //30/12/11 added this as a variable becuase we will need it to calculate maxNumberOfFramesForSynthesis from guiSecs later
    frameRateWeSet=30;
    
    ofSetFrameRate(frameRateWeSet);   // this produces 5 or 6 frames of latency frames in the camera feed 
    //ofSetFrameRate(15);     // this produces 2 or 3 frames of latency frames in the camera feed 
    //ofSetFrameRate(5);   // this produces  or 1 or 2 frames of latency
    //ofSetFrameRate(1);   // this produces?  i'm not patient enough to figure this out...
    // i.e.:  the frame lag seems to be ofGetFrameRate()/6 or thereabouts.
    
    ofEnableSmoothing();
    ofSetVerticalSync(TRUE);
    //set default codec
    codecChooser=0;
    
    
    if (!mainFileHelper.doesDirectoryExist("IMAGES/")){
        mainFileHelper.makeDirectory("IMAGES/");
    }
    
    
    //default location
    locationChooser="MIDDLESBROUGH";   //
    
    //default analysis
    analysisChooser="H_SHADOWSCAPES";   
    
    //its easier to initialise the camera with default settings than mess around with bad access errors when you try and draw it;(
    setupCamera(camWidth, camHeight, 2, 120,true);    
    //set initial report
    camStatus="Camera not setup";  //    //WHY say this if we've just set up the camera?
    
    keyControlMessage1="Use 1-9 & q+w+e+r to launch the analyses";
    keyControlMessage2="'c'-cursor        'v'-video input\n'f'-fullscreen   'g'-gui    'z'-to start";
    
    font.loadFont("MONACO.TTF", 10);
    
    //AT the moment - this WILL COMPILE for 10.5 if we remove the below image loaders - problem with POCO library - see here:
    // http://forum.openframeworks.cc/index.php?topic=6351.0
    
    startImage.loadImage("resourceimages/refractiveindexstart.jpg");
    endImage.loadImage("resourceimages/refractiveindexend.jpg");
    
    /*  compiling for 10.5 - we get a bunch of POCO errors
     
     SOLUTION:  http://forum.openframeworks.cc/index.php?topic=6351.0
     
     https://github.com/openframeworks/openFrameworks/issues/387
     
     http://forum.openframeworks.cc/index.php?topic=7549.0
     
     following error appears when i try to load an image; commenting out the call to loadImage("...") makes the error go away.
     
     Undefined symbols:
     "std::basic_ostream<char, std::char_traits<char> >& std::__ostream_insert<char, std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*, int)", 
     referenced from:
     Poco::Net::HTTPClientSession::proxyAuthenticateImpl(Poco::Net::HTTPRequest&)in PocoNet.a(HTTPClientSession.o)
     Poco::Net::HTTPClientSession::proxyAuthenticateImpl(Poco::Net::HTTPRequest&)in PocoNet.a(HTTPClientSession.o)
     Poco::Net::HTTPRequest::write(std::basic_ostream<char, std::char_traits<char> >&) constin PocoNet.a(HTTPRequest.o)
     Poco::Net::HTTPRequest::write(std::basic_ostream<char, std::char_traits<char> >&) constin PocoNet.a(HTTPRequest.o)
     Poco::Net::HTTPRequest::write(std::basic_ostream<char, std::char_traits<char> >&) constin PocoNet.a(HTTPRequest.o)
     Poco::Net::MessageHeader::write(std::basic_ostream<char, std::char_traits<char> >&) constin PocoNet.a(MessageHeader.o)
     Poco::Net::MessageHeader::write(std::basic_ostream<char, std::char_traits<char> >&) constin PocoNet.a(MessageHeader.o)
     Poco::Net::HTTPResponse::write(std::basic_ostream<char, std::char_traits<char> >&) constin PocoNet.a(HTTPResponse.o)
     Poco::Net::HTTPResponse::write(std::basic_ostream<char, std::char_traits<char> >&) constin PocoNet.a(HTTPResponse.o)
     ld: symbol(s) not found
     */
    
    showGui=FALSE;
    showCursor=FALSE;
    showCameraInput=FALSE;
    
    //talk to me about your troubles openframeworks
    ofSetLogLevel(OF_LOG_VERBOSE);  
    
    ////////////GUI SETUP STUFF////////////////
    
    // RI PURPLE  #716e88 =     113, 110, 136
    ofxControlPanel::setBackgroundColor(simpleColor(113, 110, 136, 200));
    ofxControlPanel::setTextColor(simpleColor(255, 255, 255, 255));
    ofxControlPanel::setOutlineColor(simpleColor(255, 255, 255, 255));   
    ofxControlPanel::setForegroundColor(simpleColor(50, 50, 50, 255));
    
    //
	gui.loadFont("MONACO.TTF", 8);		
	gui.setup("Refractive Index", 0, 0, ofGetWidth(), ofGetHeight());
	
    //FIRST PANEL HOLDS CAMERA CONTROLS
    gui.addPanel("Camera Settings", 4, false);
    gui.setWhichPanel(0);
    gui.setWhichColumn(0);
    gui.addToggle("set up camera input", "CAM_IS_GO", 0);
    gui.addButtonSlider("camera width", "CAM_WIDTH", camWidth, 1.0, 1920, true);
	gui.addButtonSlider("camera height", "CAM_HEIGHT", camHeight, 1.0, 1080, true);
    gui.addButtonSlider("desired frame rate", "FRAME_RATE", 1., 1.0, 30, true);
	gui.setWhichColumn(1);
    gui.addTextInput("text message", "input text here", 250 );
    
    //GET THE INPUT NAMES FROM THE QT VIDEO GRABBER
    vidGrabber.listDevices();
    gui.addToggle("more cam settings", "SETTINGS", 0);
    
    //A NEW PANEL
    
    //removed by Tom 27/12/11
    // gui.addPanel("Analyses", 4, false);
    
    gui.setWhichPanel(0);
    gui.setWhichColumn(1);
    
    vector<string>locationNames;
    locationNames.push_back("MIDDLESBROUGH");
    locationNames.push_back("BRADFORD");
    locationNames.push_back("BIRMINGHAM");
    locationNames.push_back("SWANSEA");
    locationNames.push_back("BRISTOL");
    locationNames.push_back("WOOLWICH");
    locationNames.push_back("DOVER");
    locationNames.push_back("BELFAST");
    locationNames.push_back("EDINRBURGH");
    locationNames.push_back("MANCHESTER");
    locationNames.push_back("LIVERPOOL");
    
    gui.addTextDropDown("location", "LOCATION", 130, locationNames);
    gui.setWhichColumn(2);
    
    //will handle which analysis we are doing this time
    //TomS 22.11.11 vector<string>analysisNames; moved to h file - this needs to be global as its now used to get the panel number when switching panel in the gui event
    
    analysisNames.push_back("H_SHADOWSCAPES");
    analysisNames.push_back("V_SHADOWSCAPES");
    analysisNames.push_back("D_SHADOWSCAPES");
    analysisNames.push_back("RELAXRATE");
    analysisNames.push_back("I_RESPONSE");
    analysisNames.push_back("SHAPE_SHADING");
    analysisNames.push_back("M_CODE");
    analysisNames.push_back("CAM_FRAMERATE");
    analysisNames.push_back("CAM_NOISE");
    analysisNames.push_back("COLOR_SINGLE");
    analysisNames.push_back("LATENCY_TEST");
    analysisNames.push_back("COLOR_MULTI");
    analysisNames.push_back("DIFF_NOISE");
    
    gui.addTextDropDown("choose analysis", "ANALYSIS_TYPE", 130, analysisNames);
    gui.setWhichColumn(3);
    /*  float maxResultA; 
     float maxNumberOfFramesForSynthesis; 
     float divisionsA; 
     bool showGraphA;*/
    
    //////ADD ONE PANEL FOR EACH ANALYSIS///// --> why did you do this again?  i.e.: why not have one Panel with a row of parameters for each analysis? so much clicking :P  JA
    for(int i = 0;i< analysisNames.size();i++){
        gui.addPanel(analysisNames[i], 4, false);
    }
    
    ///TOM 21/11/11 NOW SETTING THE CORRECT GUI ELEMENTS FOR EACH ANALYSIS, PAINFULLY, ONE BY ONE
    ///Note counterMax in the analysis class seem to do exactly the same job as maxNumberOfFramesForSynthesis which is currently connected to gui, show we merge these variables? 
    
    //This is a good effeciency, but there's a need for the default values for each of these to be different, and if one gets changed, we don't want the rest of them to have the value of that one... so is there another way to deal with it that is both efficient and allows each analysis to 'keep its state'?  - JA 11 Dec 
    
    
    //local variable for setting gui from numberofframestolastfor
    float maxFramesDividedByFrameRate = masterAnalysis.maxNumberOfFramesForSynthesis/frameRateWeSet;
    
    
    //addButtonSlider(string sliderName, string xmlName, float value , float min, float max, bool isInt);
    //H_SHADOWSCAPES GUI OPTIONS
    gui.setWhichPanel(1);
    gui.setWhichColumn(0);
    gui.addToggle("GO", "GO", 0);
    gui.addButtonSlider("scan line width", "SCAN_LINE_WIDTH", masterAnalysis.scanLineWidth, 1, ofGetWidth(), TRUE);
    gui.addButtonSlider("scan line speed", "SCAN_LINE_SPEED", masterAnalysis.scanLineSpeed, 1, 100, TRUE);
    
    //V_SHADOWSCAPES GUI OPTIONS
    gui.setWhichPanel(2);
    gui.setWhichColumn(0);
    gui.addToggle("GO", "GO", 0);
    gui.addButtonSlider("scan line width", "SCAN_LINE_WIDTH", masterAnalysis.scanLineWidth, 1, ofGetHeight(), TRUE);
    gui.addButtonSlider("scan line speed", "SCAN_LINE_SPEED", masterAnalysis.scanLineSpeed, 1, 100, TRUE);
    
    //D_SHADOWSCAPES GUI OPTIONS
    gui.setWhichPanel(3);
    gui.setWhichColumn(0);
    gui.addToggle("GO", "GO", 0);
    gui.addButtonSlider("scan line width", "SCAN_LINE_WIDTH", masterAnalysis.scanLineWidth, 1, 200, TRUE);
    gui.addButtonSlider("scan line speed", "SCAN_LINE_SPEED", masterAnalysis.scanLineSpeed, 1, 100, TRUE);
    
    //RELAXRATE GUI OPTIONS
    gui.setWhichPanel(4);
    gui.setWhichColumn(0);
    gui.addToggle("GO", "GO", 0);
    gui.addButtonSlider("max white level to ramp to", "GRAPH_MAX_RESULT", masterAnalysis.maxResultA, 1, 255, TRUE);
    gui.addButtonSlider("num of seconds to last for", "GRAPH_MAX_TIME",  maxFramesDividedByFrameRate, 1, 50, TRUE);
    gui.addButtonSlider("num of impulses", "GRAPH_NUM_DIVISIONS",  masterAnalysis.divisionsA, 1, 10, TRUE);
    gui.addToggle("show graph outlines", "SHOW_GRAPH_OUTLINE", masterAnalysis.showGraphA);
    
    vector<string>graphNames;
    graphNames.push_back("LINEAR");
    graphNames.push_back("EXPONENTIAL");
    graphNames.push_back("SQUARE_WAVE");
    graphNames.push_back("QUADRATIC");
    
    gui.addTextDropDown("RELAXRATE response curve", "GRAPH_TYPE", 130, graphNames);
    
    
    /*
     //for colour multi
     fadeTime = 20;
     everyNthFrameCaptureImage = 20; //the lower this number the more camera images we get per white level shown
     numberOfGreyLevels = 10;   //the number of grey levels we want to look at
     currentGreyLevel = numberOfGreyLevels;*/
    
    
    
    //I_RESPONSE GUI OPTIONS
    
    gui.setWhichPanel(5);
    gui.setWhichColumn(0);
    gui.addToggle("GO", "GO", 0);
    gui.addButtonSlider("num of seconds to last for", "GRAPH_MAX_TIME",  maxFramesDividedByFrameRate, 1, 50, TRUE);
    gui.addButtonSlider("number of grey levels", "ANIMATION_TIME_LIMIT", masterAnalysis.numberOfGreyLevels, 1, 20, TRUE);
    gui.addButtonSlider("fadeTime", "ANIMATION_TIME_LIMIT", masterAnalysis.fadeTime, 1, 30, TRUE);
    gui.addButtonSlider("number of frames to grab", "ANIMATION_TIME_LIMIT", masterAnalysis.numOfFramesToGrab, 1, 500, TRUE);
    
    //SHAPE_SHADING GUI OPTIONS
    gui.setWhichPanel(6);
    gui.setWhichColumn(0);
    gui.addToggle("GO", "GO", 0);
    gui.addButtonSlider("num of seconds to last for", "GRAPH_MAX_TIME",  maxFramesDividedByFrameRate, 1, 50, TRUE);
    gui.addButtonSlider("num of frames to grab per quadrant", "fpq",  masterAnalysis.framesPerQuadrant , 1, 150, TRUE);
    
    //M_CODE GUI OPTIONS
    gui.setWhichPanel(7);
    gui.setWhichColumn(0);
    gui.addButtonSlider("morse pause time", "MORSE_SPEED", masterAnalysis.speed, 1, 25, TRUE);
    //gui.addTextInput("morse output", "input morse here", 250 );
    //nasty hack for getting text back
    tl=gui.addTextInput("morse output", "USE_UNDERSCORES_AND_CAPS", 250 );
    gui.addToggle("GO", "GO", 0);
    
    //CAM_FRAMERATE GUI OPTIONS
    gui.setWhichPanel(8);
    gui.setWhichColumn(0);
    gui.addToggle("GO", "GO", 0);
    
    //CAM_NOISE GUI OPTIONS
    gui.setWhichPanel(9);
    gui.setWhichColumn(0);
    gui.addToggle("GO", "GO", 0);
    
    //COLOR_SINGLE GUI OPTIONS
    gui.setWhichPanel(10);
    gui.setWhichColumn(0);
    gui.addToggle("GO", "GO", 0);
    
    //LATENCY_TEST GUI OPTIONS
    gui.setWhichPanel(11);
    gui.setWhichColumn(0);
    gui.addToggle("GO", "GO", 0);
    
    //COLOR_MULTI GUI OPTIONS
    gui.setWhichPanel(12);
    cout<<gui.getWhichPanel()<<" whichPanel \n";
    gui.setWhichColumn(0);
    gui.addButtonSlider("color multi overall length", "COLORMULTI_SPEED", masterAnalysis.counterMaxColorMulti, 1, 2000, TRUE);
    gui.addToggle("GO", "GO", 0);
    
    //Tom S 21.11.11 colour multi now fades through the whole spectrum so doesn't need these any more
    //gui.addButtonSlider("red level", "RED_LEVEL", 10, 1, 255, TRUE);
    //gui.addButtonSlider("green level", "GREEN_LEVEL", 10, 1, 255, TRUE);
    //gui.addButtonSlider("blue level", "BLUE_LEVEL", 10, 1, 255, TRUE);
    
    //DIFF_NOISE GUI OPTIONS
    gui.setWhichPanel(13);
    gui.setWhichColumn(0);
    gui.addToggle("GO", "GO", 0);
    
    gui.setWhichPanel(0);
    gui.setWhichColumn(1);
    
    vector<string> names=vidGrabber.returnDeviceNames();
    cout<<names.size()<<" number of inputs found\n";
    //    masterAnalysis.setupAnalysis(camWidth, camHeight, 100, analysisChooser, locationChooser, codecChooser);//, vidGrabber);    
    
    
    //get list of codecs from movie object in analysis class
    returnedCodecNames=masterAnalysis.movieFromCamera.returnCodecNames();
    
    //CURRENTLY UNUSED
    cout<<names[names.size()-1]<<" names at 2\n";
    gui.addTextDropDown("inputs", "INPUTS", 130, names);
    gui.setWhichColumn(2);
    gui.addTextDropDown("codecs", "CODECS", 130, returnedCodecNames);
    
    //dont forget to actually set this stuff in action!
    gui.setupEvents();
	gui.enableEvents();
    //  -- this gives you back an ofEvent for all events in this control panel object
	ofAddListener(gui.guiEvent, this, &testApp::eventsIn);
    
    ////////////END OF GUI SETUP STUFF////////////////
    
    
    currentPanel=gui.getSelectedPanel();
    
    /*
     
     
     */
    
}

//--------------------------------------------------------------
void testApp::update(){  
    ofBackground(0, 0, 0);
    
    if(gui.getSelectedPanel()!=currentPanel&&gui.getSelectedPanel()>=1){
        //set the current analysis (the subtraction by 2 is because the first panel are not types of analysis
        analysisChooser=analysisNames[gui.getSelectedPanel()-1];
        
        currentPanel=gui.getSelectedPanel();
        cout<<" PANEL CHANGED TO "<<gui.getSelectedPanel()<< " CURRENT ANALYSIS IS NOW "<<analysisChooser;
        
    }
    
    string str = "framerate is "; 						
    str += ofToString(ofGetFrameRate(), 2)+"fps"; 
    ofSetWindowTitle(str);
    //set the window title to "framerate"
    
    
    // kylemcdonald's solution for getting the most recent frame from here: 
    // http://forum.openframeworks.cc/index.php?topic=7237.0
    /*
     int frames = 0;  
     while(frames < 4) {  
     vidGrabber.grabFrame();  
     if(vidGrabber.isFrameNew()) {  
     frames++;  
     }  
     ofSleepMillis(2);  
     }  
     */
    
    vidGrabber.grabFrame();  // go get frame from OS
    //vidGrabber.update();  // go get frame from OS
    
    //neutral, do nothing
    if(menuState==0){
        
        //before analysis - wait state
    }
    
    //setup analysis
    if(menuState==1){
        
        cout<<analysisChooser<<" menu state is 1, setting up saver and analyis\n";
        
        masterAnalysis.morseMessage= tl->getValueText();
        masterAnalysis.setupAnalysis(camWidth, camHeight, 100, analysisChooser, locationChooser, codecChooser);//, vidGrabber);
        
        //now we are setup lets analyse
        menuState=2;
    }
    
    //play the synthesized stuff to screen 
    if(menuState==2){
        
    }
    
    //run analysis on the movie we've recorded during 'synthDraw'
    if(menuState==3){
        // after 
    }
    
    //display analysis
    if(menuState==4){
        //masterAnalysis.setupMovie();
        //masterAnalysis.updatePlayer();  //playing the recorded movie file
    }
    
    gui.update();
    
}


//--------------------------------------------------------------
void testApp::draw(){
    
    //ofSetColor(0);
    //if no analysis, draw grabber
    
    if(menuState==0){
        ofSetColor(255,255,255);
        startImage.draw(0,0,ofGetWidth(), ofGetHeight());
        font.drawString("Refractive Index", ofGetWidth()-250, ofGetHeight()-70);
        font.drawString("Jamie Allen", ofGetWidth()-250, ofGetHeight()-50);
        font.drawString("refractiveindex.cc", ofGetWidth()-250, ofGetHeight()-30);
    }
    
    //continue to draw grabber in setup phase
    if(menuState==1){
        
    }
    
    //drawing synthesized impulses, etc., to the screen
    if(menuState==2){
        
        if(!masterAnalysis.synthesisComplete){
            
            //cout<<masterAnalysis.synthesisComplete<<"masterAnalysis.synthesisComplete \n";
            //cout<<"in draw loop menuState 2 \n";
            if (vidGrabber.isFrameNew())
            {   
                camPixels = vidGrabber.getPixelsRef();
            } 
            
            masterAnalysis.synthDrawCamRecord(camPixels);    
            
        } else {
            
            menuState = 3;
            
        }
        
    }
    
    //menustate  draw results of analysis
    if(menuState==3){
        
        ofSetColor(255,255,255);
        endImage.draw(0,0,ofGetWidth(), ofGetHeight());
        font.drawString("Refractive Index", ofGetWidth()-250, ofGetHeight()-70);
        font.drawString("Jamie Allen", ofGetWidth()-250, ofGetHeight()-50);
        font.drawString("refractiveindex.cc", ofGetWidth()-250, ofGetHeight()-30);
        
        //cout<<" delete [] camPixels; \n";
        //cout<<"in draw loop menuState 3 \n";
    }
    
    if(menuState==4){
        //masterAnalysis.displayResult();
    }
    
    
    if(showCameraInput){
        vidGrabber.draw(0, 0, ofGetWidth(), ofGetHeight());
    }
    
    // always show gui on top of everything else (if shown) 
    if(showGui){
        showCursor=TRUE;
        gui.draw();
        font.drawString(camStatus,30, ofGetHeight()-80);
        font.drawString(keyControlMessage1, 30, ofGetHeight()-60);
        font.drawString(keyControlMessage2, 30, ofGetHeight()-40);
    }
    
    if(showCursor)
    {
        ofShowCursor();
    } else {
        ofHideCursor();
    }
    
}

void testApp::exit(){
    
}

//---------------  CALLED FROM INSIDE EVENT FUNCTION WHEN CAMERA SETTINGS HAVE BEEN SELECTED ---------------------------------------
void testApp::setupCamera(int w, int h, int whichSource,int desiredFrameRate, bool firstSetup){
    cout<<whichSource<<" this source\n";
    
    vidGrabber.close();
    camWidth = w;
    camHeight =h;
    
    vidGrabber.setVerbose(true);
    vidGrabber.setDeviceID(whichSource);
    vidGrabber.initGrabber(camWidth,camHeight);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    //Tom S added key press launcher for all analyses with default values
    //use g as gui toggle
    
    //use z as return to beginning of program toggle
    if(key=='z'){
        menuState=0;
    }
    
    //use c as cursor toggle
    if(key=='c'){
        showCursor=!showCursor;
    }
    
    //use v as video toggle
    if(key=='v'){
        showCameraInput=!showCameraInput;
    }
    
    //use f as gui toggle
    if( key =='f')
    {
        ofToggleFullscreen(); 
    }
    
    //use g as gui toggle
    if(key=='g'){
        showGui=!showGui;
        showCursor=!showCursor;
    }
    
    if(key=='1'){
        analysisChooser="H_SHADOWSCAPES";
        showGui=false;
        showCursor=false;
        menuState=1;
    }
    if(key=='2'){
        analysisChooser="V_SHADOWSCAPES";
        showGui=false;
        showCursor=false;
        menuState=1;
    }
    if(key=='3'){
        analysisChooser="D_SHADOWSCAPES";
        showGui=false;
        showCursor=false;
        menuState=1;
    }
    if(key=='4'){
        analysisChooser="RELAXRATE";
        showGui=false;
        showCursor=false;
        menuState=1;
    }
    if(key=='5'){
        analysisChooser="I_RESPONSE";
        showGui=false;
        showCursor=false;
        menuState=1;
    }
    if(key=='6'){
        analysisChooser="SHAPE_SHADING";
        showGui=false;
        showCursor=false;
        menuState=1;
    }
    if(key=='7'){
        analysisChooser="M_CODE";
        showGui=false;
        showCursor=false;
        menuState=1;
    }
    if(key=='8'){
        analysisChooser="CAM_FRAMERATE";
        showGui=false;
        showCursor=false;
        menuState=1;
    }
    if(key=='9'){
        analysisChooser="CAM_NOISE";
        showGui=false;
        showCursor=false;
        menuState=1;
    }
    if(key=='q'){
        analysisChooser="COLOR_SINGLE";
        showGui=false;
        showCursor=false;
        menuState=1;
    }
    if(key=='w'){
        analysisChooser="LATENCY_TEST";
        showGui=false;
        showCursor=false;
        menuState=1;
    }
    if(key=='e'){
        analysisChooser="COLOR_MULTI";
        showGui=false;
        showCursor=false;
        menuState=1;
    }
    if(key=='r'){
        analysisChooser="DIFF_NOISE";
        showGui=false;
        showCursor=false;
        menuState=1;
    }
    
    else {
        
        if(showGui){
            bool control_panel_ate_key = gui.keyPressed( key );
        }
        else {
        }
    }
}
//----------------------NOT CURRENTLY USED----------------------------------------
void testApp::grabBackgroundEvent(guiCallbackData & data){
	
	//we use the event callback to capture the background - we then set the toggle value back to its previous value
	if( data.isElement( "GRAB_BACKGROUND" ) && data.getInt(0) == 1 ){
        //	bgExample.captureBackground();
		gui.setValueB("GRAB_BACKGROUND", false);
	}
}

//this captures all our control panel events - unless its setup differently in testApp::setup
//--------------------------------------------------------------
void testApp::eventsIn(guiCallbackData & data){
    
    //TODO: would like to 'checkHit' for each GUI element and use that to set the analysisChooser (this would be proper behavior for the GUI)
    
    //check whether we are still on the same panel
    /*
     analysisChooser
     gui.getSelectedPanel();
     */
    
    
    
	//lets send all events to our logger
	if( !data.isElement( "events logger" ) ){
		string logStr = data.getXmlName();
		
		for(int k = 0; k < data.getNumValues(); k++){
			logStr += " - " + data.getString(k);
		}
		
		logger.log(OF_LOG_NOTICE, "event - %s", logStr.c_str());
	}
	
	// print to terminal if you want to 
	//this code prints out the name of the events coming in and all the variables passed
    
    //SETUP CAMERA
	printf("testApp::eventsIn - name is %s - \n", data.getXmlName().c_str());
    if( data.getDisplayName() == "set up camera input" ){
        //vidGrabber.close();
        setupCamera(camWidth,camHeight,camInput,desFrameRate,false);
        
        //menuState=1;
        cout<<"cam is setup\n"; 
    }
    
    string thisName=data.getDisplayName();
    cout<<thisName<<" this Name\n";
    
    //START ANALYSIS IS NOW FROM A BUTTON
    if( thisName == "GO" ){
        
        showGui=false;
        showCursor=false;
        menuState=1;
    }
    
    //animation time limit
    if( thisName == "animation time limit" ){
        
        for(int k = 0; k < data.getNumValues(); k++){
            if( data.getType(k) == CB_VALUE_FLOAT ){
                masterAnalysis.maxNumberOfFramesForSynthesis=data.getFloat(k);
                cout<<masterAnalysis.maxNumberOfFramesForSynthesis<<"masterAnalysis.maxNumberOfFramesForSynthesis \n";
            }
            else if( data.getType(k) == CB_VALUE_STRING ){
                printf("%i string value = %s \n", k, data.getString(k).c_str());
            }
        }
    }
    
    //number of frames to grab
    if( thisName == "number of frames to grab" ){
        
        for(int k = 0; k < data.getNumValues(); k++){
            if( data.getType(k) == CB_VALUE_FLOAT ){
                
                masterAnalysis.numOfFramesToGrab =data.getFloat(k);
                cout<<masterAnalysis.numOfFramesToGrab<<"masterAnalysis.numOfFramesToGrab \n";
            }
            else if( data.getType(k) == CB_VALUE_STRING ){
                printf("%i string value = %s \n", k, data.getString(k).c_str());
            }
        }
    }
    
    
    //added Tom 31/12/11
    // number of grey levels
    if( thisName == "number of grey levels" ){
        
        for(int k = 0; k < data.getNumValues(); k++){
            if( data.getType(k) == CB_VALUE_FLOAT ){
                masterAnalysis.numberOfGreyLevels =data.getFloat(k);
                masterAnalysis.currentGreyLevel=data.getFloat(k);
                cout<<masterAnalysis.numberOfGreyLevels<<"masterAnalysis.numberOfGreyLevels \n";
            }
            else if( data.getType(k) == CB_VALUE_STRING ){
                printf("%i string value = %s \n", k, data.getString(k).c_str());
            }
        }
    }
    //fadeTime
    if( thisName == "fadeTime" ){
        
        for(int k = 0; k < data.getNumValues(); k++){
            if( data.getType(k) == CB_VALUE_FLOAT ){
                masterAnalysis.fadeTime =data.getFloat(k);
                cout<<masterAnalysis.fadeTime<<"masterAnalysis.fadeTime \n";
            }
            else if( data.getType(k) == CB_VALUE_STRING ){
                printf("%i string value = %s \n", k, data.getString(k).c_str());
            }
        }
    }
    if( thisName == "scan line width" ){
        for(int k = 0; k < data.getNumValues(); k++){
            if( data.getType(k) == CB_VALUE_FLOAT ){
                
                masterAnalysis.scanLineWidth=data.getFloat(k);
                cout<<masterAnalysis.scanLineWidth<<"masterAnalysis.scanLineWidth \n";
            }
            else if( data.getType(k) == CB_VALUE_STRING ){
                printf("%i string value = %s \n", k, data.getString(k).c_str());
            }
        }
    }
    
    if( thisName == "scan line speed" ){
        
        for(int k = 0; k < data.getNumValues(); k++){
            if( data.getType(k) == CB_VALUE_FLOAT ){
                
                masterAnalysis.scanLineSpeed=data.getFloat(k);
                cout<<masterAnalysis.scanLineSpeed<<"masterAnalysis.scanLineSpeed \n";
            }
            else if( data.getType(k) == CB_VALUE_STRING ){
                printf("%i string value = %s \n", k, data.getString(k).c_str());
            }
        }
    }
    
    if( thisName == "max white level to ramp to" ){
        
        for(int k = 0; k < data.getNumValues(); k++){
            if( data.getType(k) == CB_VALUE_FLOAT ){
                
                masterAnalysis.maxResultA=data.getFloat(k);
                cout<<masterAnalysis.maxResultA<<"masterAnalysis.maxResultA \n";
            }
            else if( data.getType(k) == CB_VALUE_STRING ){
                printf("%i string value = %s \n", k, data.getString(k).c_str());
            }
        }
    }
    
    if( thisName == "num of seconds to last for" ){
        
        for(int k = 0; k < data.getNumValues(); k++){
            if( data.getType(k) == CB_VALUE_FLOAT ){
                //30/12/11 maxNumberOfFramesForSynthesis is now set by multiplying gui time in secs by frameRateWeSet
                
                //if the analysis is iResponse this number of frames is per level/cycle so must be divided by number of grey levels
                
                // if (currentPanel==6) {
                // masterAnalysis.maxNumberOfFramesForSynthesis= frameRateWeSet * data.getFloat(k) / masterAnalysis.numberOfGreyLevels;
                
                //  }
                //  else{
                masterAnalysis.maxNumberOfFramesForSynthesis= frameRateWeSet * data.getFloat(k);
                //  }
                cout<<masterAnalysis.maxNumberOfFramesForSynthesis<<"masterAnalysis.maxNumberOfFramesForSynthesis \n";
            }
            else if( data.getType(k) == CB_VALUE_STRING ){
                printf("%i string value = %s \n", k, data.getString(k).c_str());
            }
        }
    }
    
    if( thisName == "num of impulses" ){
        
        for(int k = 0; k < data.getNumValues(); k++){
            if( data.getType(k) == CB_VALUE_FLOAT ){
                
                masterAnalysis.divisionsA=(2*(int)data.getFloat(k));
                cout<<"masterAnalysis.divisionsA = "<<masterAnalysis.divisionsA<<"  \n";
            }
            else if( data.getType(k) == CB_VALUE_STRING ){
                printf("%i string value = %s \n", k, data.getString(k).c_str());
            }
        }
    }
    
    if( thisName == "show graph outline" ){
        showGraphLine=!showGraphLine;
        masterAnalysis.showGraphA=showGraphLine;
        
    }
    
    //Tom S 14 Nov 19:10somehow this function appears to have been repeated so have commented out
    /* if( thisName == "graph max time" ){
     
     for(int k = 0; k < data.getNumValues(); k++){
     if( data.getType(k) == CB_VALUE_FLOAT ){
     
     masterAnalysis.maxNumberOfFramesForSynthesis=data.getFloat(k);
     cout<<masterAnalysis.maxNumberOfFramesForSynthesis<<"masterAnalysis.maxNumberOfFramesForSynthesis \n";
     }
     else if( data.getType(k) == CB_VALUE_STRING ){
     printf("%i string value = %s \n", k, data.getString(k).c_str());
     }
     }
     }*/
    
    if( thisName == "morse pause time" ){
        
        for(int k = 0; k < data.getNumValues(); k++){
            if( data.getType(k) == CB_VALUE_FLOAT ){
                
                masterAnalysis.speed=data.getFloat(k);
                cout<<masterAnalysis.speed<<"masterAnalysis.speed \n";
            }
            else if( data.getType(k) == CB_VALUE_STRING ){
                printf("%i string value = %s \n", k, data.getString(k).c_str());
            }
        }
    }
    
    if( thisName == "red level" ){
        
        for(int k = 0; k < data.getNumValues(); k++){
            if( data.getType(k) == CB_VALUE_FLOAT ){
                
                masterAnalysis.red=data.getFloat(k);
                cout<<masterAnalysis.red<<" masterAnalysis.red \n";
            }
            else if( data.getType(k) == CB_VALUE_STRING ){
                printf("%i string value = %s \n", k, data.getString(k).c_str());
            }
        }
    }
    
    if( thisName == "green level" ){
        
        for(int k = 0; k < data.getNumValues(); k++){
            if( data.getType(k) == CB_VALUE_FLOAT ){
                
                masterAnalysis.green=data.getFloat(k);
                cout<<masterAnalysis.green<<" masterAnalysis.green \n";
            }
            else if( data.getType(k) == CB_VALUE_STRING ){
                printf("%i string value = %s \n", k, data.getString(k).c_str());
            }
        }
        
    }
    
    if( thisName == "red level" ){
        
        for(int k = 0; k < data.getNumValues(); k++){
            if( data.getType(k) == CB_VALUE_FLOAT ){
                
                masterAnalysis.blue=data.getFloat(k);
                cout<<masterAnalysis.blue<<" masterAnalysis.blue \n";
            }
            else if( data.getType(k) == CB_VALUE_STRING ){
                printf("%i string value = %s \n", k, data.getString(k).c_str());
            }
        }
        
    }
    
    if( thisName == "color multi overall length" ){
        
        for(int k = 0; k < data.getNumValues(); k++){
            if( data.getType(k) == CB_VALUE_FLOAT ){
                
                masterAnalysis.counterMaxColorMulti=data.getFloat(k);
                cout<<masterAnalysis.counterMaxColorMulti<<"masterAnalysis.maxNumberOfFramesForSynthesis \n";
            }
            else if( data.getType(k) == CB_VALUE_STRING ){
                printf("%i string value = %s \n", k, data.getString(k).c_str());
            }
        }
    }
    
    
    //more cam settings", "SETTINGS"
    if( data.getDisplayName() == "more cam settings" ){
        vidGrabber.videoSettings();
    }
    
    //LOCATION SELECTIO
    if( data.getDisplayName()== "location" ){
        
        for(int k = 0; k < data.getNumValues(); k++){
            if( data.getType(k) == CB_VALUE_STRING ){
                locationChooser=data.getString(k);
                cout<<data.getString(k)<<" <<-- is our location \n";    
            }
        }
    }
    
    
    //START ANALYSIS
    //Tom 22/11/11 changed this from starting analysis to choosing panel of gui
    if( data.getDisplayName()== "choose analysis" ){
        
        for(int k = 0; k < data.getNumValues(); k++){
            if( data.getType(k) == CB_VALUE_STRING ){
                analysisChooser=data.getString(k);
                cout<<data.getString(k)<<" "<<k<<" <<-- lets jump to this tab\n";   
                
                //Tom 22/11/11
                for(int i=0;i<analysisNames.size();i++){
                    //check if the clicked option in the dropdown matches this analysis name
                    if(analysisNames[i]==data.getString(k)){
                        //if it does, jump to the correct gui panel
                        //changed 2 to 1 to account for mixing first 2 panels together
                        gui.setSelectedPanel(i+1);
                        //exit for loop
                        exit();
                    }
                }
            }
        }
    }
    
    //WHAT KIND OF GRAPH
    if( data.getDisplayName()== "RELAXRATE response curve" ){
        
        for(int k = 0; k < data.getNumValues(); k++){
            if( data.getType(k) == CB_VALUE_STRING ){
                
                //copy string straight into class
                masterAnalysis.whichGraph=data.getString(k);
                cout<<data.getString(k)<<" <<-- lets run THIS graph\n";    
            }
        }
    }
    
    //CAM HEIGHT
    if( data.getDisplayName() == "camera height" ){
        for(int k = 0; k < data.getNumValues(); k++){
            if( data.getType(k) == CB_VALUE_FLOAT ){
                printf("%i float  value = %f \n", k, data.getFloat(k));
                camHeight=data.getFloat(k);
            }
            else if( data.getType(k) == CB_VALUE_INT ){
                printf("%i int    value = %i \n", k, data.getInt(k));
                camHeight=data.getInt(k);
            }
            else if( data.getType(k) == CB_VALUE_STRING ){
                printf("%i string value = %s \n", k, data.getString(k).c_str());
            }
        }
    }
    
    //FRAME RATE ISNT WORKING - I THINK I HACKED THE LIBRARY TODO SET THIS RIGHT
    if( data.getDisplayName() == "desired frame rate" ){
        for(int k = 0; k < data.getNumValues(); k++){
            if( data.getType(k) == CB_VALUE_FLOAT ){
                printf("%i float  value = %f \n", k, data.getFloat(k));
                desFrameRate=data.getFloat(k);
            }
            else if( data.getType(k) == CB_VALUE_INT ){
                printf("%i int    value = %i \n", k, data.getInt(k));
                desFrameRate=data.getInt(k);
            }
        }
    }
    
    //LIST OF CAM SOURCES
    if( data.getDisplayName() == "inputs" ){
        cout<<"input selected\n";
        for(int k = 0; k < data.getNumValues(); k++){
            if( data.getType(k) == CB_VALUE_FLOAT ){
                printf("%i float  value = %f \n", k, data.getFloat(k));
            }
            else if( data.getType(k) == CB_VALUE_INT ){
                printf("%i int    value = %i \n", k, data.getInt(k));
                camInput=data.getInt(k);
                cout<<camInput<<" camInput \n";
            }
            else if( data.getType(k) == CB_VALUE_STRING ){
                printf("%i string value = %s \n", k, data.getString(k).c_str());
                camInputName=data.getString(k).c_str();
            }
        }
    }
    
    // TOM - doesn't this refer to the morse text INPUT on the GUI? 
    if( data.getDisplayName() == "morse output" ){
        cout<<"getting morse message\r";
    }
    
    //LIST OF CODECS
    
    if( data.getDisplayName() == "codecs" ){
        cout<<"input selected\n";
        for(int k = 0; k < data.getNumValues(); k++){
            
            if( data.getType(k) == CB_VALUE_STRING ){
                printf("%i string value = %s \n", k, data.getString(k).c_str());
                codecName=data.getString(k).c_str();
                cout<<codecName;
                //compare the string returned from the gui to get the number so we can pass this into the analysis setup
                for(int i=0;i<returnedCodecNames.size();i++){
                    
                    if(returnedCodecNames[i]==codecName){
                        codecChooser=i;
                    }
                }
            }
        }
    }
    
    //TODO:  are any of these camera settings actually working or updating?
    //
    //UPDATE THE REPORT STRING FOR CURRENT SETTINGS
    camStatus="CAMERA STATUS:: width :"+ofToString(camWidth)+" height :"+ofToString(camHeight)+"  frame rate :"+ofToString(desFrameRate)+"  source :"+camInputName+"codec: "+codecName;
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    //  mouseX=x;
    // mouseY=y;
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    gui.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    gui.mousePressed(x, y, button); 
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    gui.mouseReleased();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}