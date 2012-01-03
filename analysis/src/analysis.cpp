#include "analysis.h"
#include <ctime>
#include "time.h"

/* TIME OF DAY CODE WILL BE DIFFERENT ON WINDOWS>>>.
 #include "windows.h"
 SYSTEMTIME time;
 GetSystemTime(&time);
 WORD millis = (time.wSeconds * 1000) + time.wMilliseconds;
 */

/*
 * daktronics  - WINDOWS 7 
 - ATI Radeon HD 3450 - is this the capture card 
 - Pinnacle card 	
 - AMCAp - software for monitoring the input 
 
 * other sites - WINDOWS XP 
 - might have a different card 
 
 * mini to DVI out of the lapop 
 * make a compile for 10.5.8 system - 1.83GHZ Intel Core 2 Duo - 1GB 6667 MHz DDR2 SDRAM
 */

////////////////////////---------/////////////////////////////////////
//SETUP THE CLASS
//Composed of things this class will always need, i.e.: a way of synthesizing the outputs, recording images
//from the camera, analysing the recorded data and outputting resulting info to the screen.

////////////////////////---------/////////////////////////////////////
void analysis::setupAnalysis(int camW, int camH, int analasisTimePass, string whichAnalysisPass, string whichLocationPass, int whichCodec){//, ofVideoGrabber &grabber){
    //i included an argument which is the pointer to the grabber in case this is better than passing in pixel array? not currently used
    check=0;
    whichAnalysis = whichAnalysisPass;
    analysisTime = analasisTimePass;
    whichLocation = whichLocationPass;  
    
    /*
     time_t rawtime;
     struct tm * timeinfo;
     time(&rawtime);
     timeinfo = localtime ( &rawtime );
     string time = asctime(&rawtime);
     cout<< time << " <---  time \n";
     */
    
    time_t rawtime;
    struct tm * timeinfo;
    
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    string time = asctime(timeinfo);
    //printf ( "Current local time and date: %s", asctime (timeinfo) );
    
    imageSaveFolderPath = "IMAGES/"+whichLocation+"/"+whichAnalysis+"/"+time+"/";
    
    if (!myFileHelper.doesDirectoryExist("IMAGES/"+whichLocation)){
        myFileHelper.makeDirectory("IMAGES/"+whichLocation);
    }
    
    if (!myFileHelper.doesDirectoryExist("IMAGES/"+whichLocation+"/"+whichAnalysis)){
        myFileHelper.makeDirectory("IMAGES/"+whichLocation+"/"+whichAnalysis);
    }
    
    if (!myFileHelper.doesDirectoryExist("IMAGES/"+whichLocation+"/"+whichAnalysis+"/"+time)){
        myFileHelper.makeDirectory("IMAGES/"+whichLocation+"/"+whichAnalysis+"/"+time);
    }
    
    if (!myFileHelper.doesDirectoryExist(imageSaveFolderPath)){
        myFileHelper.makeDirectory(imageSaveFolderPath);
    }
    
    // add one here that does time as a subfolder?
    /*
     if (!myFileHelper.doesDirectoryExist(whichLocation+"/"+whichAnalysis+"/"+ofToString(multipleAnalysisCounter))){
     ofxFileHelper::makeDirectory(whichLocation+"/"+whichAnalysis+"/"+ofToString(multipleAnalysisCounter));
     }
     */
    
    
    
    //dataPathName = "/Users/jamieallen/Projects/newcastle/projects/RefractiveIndexLaptop/openframeworks/refractiveindex/apps/myApps/refractiveIndex/bin/data/MEDIA/";
    //dataPathName=ofToDataPath("")+"MEDIA/";
    
    //cout<< dataPathName << " <<-- data Path \n";
    
    //    myFileHelper.makeDirectory(whichLocation+"/"+whichAnalysis+"/"+ofToString(multipleAnalysisCounter));
    //    string whereToSaveAnalysisOutputFiles;
    //    whereToSaveAnalysisOutputFiles = whichLocation+"/"+whichAnalysis+"/"+ofToString(multipleAnalysisCounter);
    //    cout<<whereToSaveAnalysisOutputFiles<< " <<---whereToSaveAnalysisOutputFiles \n";
    //    ofSetDataPathRoot(whereToSaveAnalysisOutputFiles);
    
    
    
    // probably good in future to have one of these for each analysis - and then have the folder name increment for each 'run' of the analysis at a given site
    
    // TOM-> JAMIE dod you mean have a different folder? if so why don't we just use the analysis names
    // as the folder names we could also set the site name in the gui if you like and use this? we could have a folder for each site with subfolders for each analysis
    
    //this is the short cut to get the data path (ie location of "data folder") ofToDataPath("") so ofToDataPath("")+"MEDIA" is prob what we want here
    
    latencyFrameCounter = -1;
    noOfLatencyFrames = 1+ofGetFrameRate()/6;
    gotAllLatencyFrames = FALSE;
    
    // set this to TRUE to turn off the latency frame collector at the beginning of synthDrawCamRecord - 
    // i don't think it's really working anyway... JA - nov 15 01:22 
    nowDoAnalyses = TRUE;
    //nowDoAnalyses = FALSE;
    
    //cout<<whichAnalysis<<" WHICH ANALYSIS AT SETUP\n";
    //cout<<"chosen codec number "<<whichCodec<<"\r";
    camHeight=camH;
    camWidth=camW;
    check=0;
    
    synthesisComplete=FALSE;   
    
    //Setups for the specific analyses as needed...    
    if (whichAnalysis=="H_SHADOWSCAPES") {
        counter = 0;
        scanLinePosition= -scanLineWidth;  // initialising this way keeps the line off the screen during the first frame
        // scanLineSpeed is now set in gui for all shadowscapes
    }
    
    if (whichAnalysis=="V_SHADOWSCAPES") {
        counter = 0;
        scanLinePosition= -scanLineWidth; // initialising this way keeps the line off the screen during the first frame
        //scanLineSpeed = 10;
    } 
    
    if (whichAnalysis=="D_SHADOWSCAPES") {
        counter = 0;
        scanLinePosition = 0; // initialising this way keeps the line off the screen during the first frame
        //scanLineSpeed = 10;
    } 
    
    if (whichAnalysis=="RELAXRATE") {
        counter = 0;
        //SETUP VIDEOSAVER
        //this function initalises on the variables for the graphs
        setupGraphs();
    } 
    
    if (whichAnalysis=="I_RESPONSE") {
        counter = 0;
        
        lastTime = ofGetElapsedTimeMillis();
        
        //amimationTimeLimit is now set in GUI
        //maxNumberOfFramesForSynthesis = 3000;  //milliseconds
        
        timeDiff = 0;
        counter = 0;
        frameCounter = 0;
        level = 0;
        
        //TODO: Set fadeTime in GUI
        
        
        //TODO: Set fadeTime in GUI
        
        //fadeTime , everyNthFrameCaptureImage , numberOfGreyLevels moved to set gui defaults
        
    }
    
    if (whichAnalysis=="SHAPE_SHADING") {
        counter=0;
        ofSetCircleResolution(100);
        ofEnableSmoothing();
        gotAllLocalFrames1=FALSE;
        gotAllLocalFrames2=FALSE;
        gotAllLocalFrames3=FALSE;
        gotAllLocalFrames4=FALSE;
        localFrameCounter=0;
        //TOm 30.12.11 moved to setGuiDefaults
        // framesPerQuadrant = 100;
    } 
    
    if (whichAnalysis=="CAM_NOISE") {
        counter = 0;
        //counterMax = 500;
        //counterMax is now replaced by maxNumberOfFramesForSynthesis
        frameCounter = 0;
        localFrameCounter = 0;
        framesPerGreyValue = 500;
        localFrameCounter = 0;
        
        newFrame = true;
        numberOfGreyLevels = 10.0;  //this number sets the number of grey levels that are shown  for each noise measurement
        k=1;// need to get my frames
    } 
    
    if (whichAnalysis=="COLOR_SINGLE") {
        counter = 0;
        frameCounter = 0;
        localFrameCounter = 0;
        aColour.r=0;
        aColour.g=0;
        aColour.b=0;
        framesPerColourValue = 30;
        
        gotAllLocalFrames1 = FALSE;  //should generalise these based on the # of colors we want to use
        gotAllLocalFrames2 = FALSE;
        gotAllLocalFrames3 = FALSE;
        //  counterMax = 500;
        //counterMax is now replaced by maxNumberOfFramesForSynthesis
        
    } 
    
    if (whichAnalysis=="M_CODE") {
        counter = 0;
        //im going to borrow some of these variables
        setupGraphs();
        onCounter=0;
        offCounter=0;
        pauseBetween=0;
        //load morse code from text file 
        loadMorse();
        morseComplete = FALSE;
        morseMessage=translateToMorse(morseMessage);
    }
    
    //LATENCY TEST
    if (whichAnalysis=="LATENCY_TEST") {
        counter = 0;
        num = 0;
        aColour.r=num;
        aColour.g=num;
        aColour.b=num;
        // counterMax = 250;  //how long to run for
        //counterMax is now replaced by maxNumberOfFramesForSynthesis
        
    }
    
    if (whichAnalysis=="CAM_FRAMERATE") {
        counter = 0;
        setupGraphs();
        strobeCounter = 0;
        synthesisComplete=FALSE;
        strobeToggleCounter = 0.0000001;
        currentSRate = 0;
        strobeComplete = FALSE;
    } 
    
    if (whichAnalysis=="COLOR_MULTI") {
        counter = 0;
        aColour.setHsb(0, 0, 0); 
        ofSetColor(aColour); 
        oldHue = 0;
        newHue = 0;
        howDifferentHuesNeedToBeBeforeFrameSaved = 1.0;
    }
    
    if (whichAnalysis=="DIFF_NOISE") {
        counter = 0;
        // counterMax = 500;
        
    }
    
}

////////////////////////---------/////////////////////////////////////
//SYNTH METHODS
//all methods function called "synth" are about playing initial impulses, etc., to the screen
////////////////////////---------/////////////////////////////////////

void analysis::synthUpdate(){
    
}

/*
 1 analysisNames.push_back("H_SHADOWSCAPES");
 2 analysisNames.push_back("V_SHADOWSCAPES");
 3 analysisNames.push_back("D_SHADOWSCAPES");
 4 analysisNames.push_back("RELAXRATE");
 5 analysisNames.push_back("I_RESPONSE");
 6 analysisNames.push_back("SHAPE_SHADING");
 7 analysisNames.push_back("M_CODE");
 8 analysisNames.push_back("CAM_FRAMERATE");
 9 analysisNames.push_back("CAM_NOISE");
 10 analysisNames.push_back("COLOR_SINGLE");
 11 analysisNames.push_back("LATENCY_TEST");
 12 analysisNames.push_back("COLOR_MULTI");
 13 analysisNames.push_back("DIFF_NOISE");
 
 */

void analysis::synthDrawCamRecord(ofPixels pixels){
    
    /* notes on the latency - 
     
     - setting the frame rate LOWER on the ofApp in main.cpp DECREASES the number of latency frames we seem to be getting before the color changes - in COLOR_SINGLE at least 
     - setting the frame rate HIGHER on the ofApp in main.cpp INCREASES the number of latency frames we are getting before the color changes - in COLOR_SINGLE at least 
     ** this means that the drawing of the app is perhaps lagging behind the camera 'new frame' coming in?  i.e.: if we slow the drawing rate down, we're allowing the drawing to catch up with the camera?     
     */
    
    /*
     //GRAB THE INITAL FRAME #DELAY HERE
     if((latencyFrameCounter < noOfLatencyFrames) && (!gotAllLatencyFrames) && (!nowDoAnalyses))
     {
     vectorOfPixels.push_back(pixels);
     latencyFrameCounter++;
     nowDoAnalyses = FALSE;
     cout<<"getting latency frames into vector of ofPixels \n";
     } else if ((latencyFrameCounter >= noOfLatencyFrames) && (!nowDoAnalyses)) 
     {
     string filename;
     for (i = 0; i < vectorOfPixels.size(); i++)  
     {   
     //cout<<i<<"< i LATENCY FRAMES \n";
     
     //THIS needs to be fixed with the folder saving paths
     fileName = imageSaveFolderPath+"latencyFrame_"+ofToString(i)+".jpg";
     ofSaveImage(vectorOfPixels[i], fileName, OF_IMAGE_QUALITY_BEST);
     
     //ofSaveImage(vectorOfPixels[i], "latencyFrame_"+ofToString(i)+".jpg", OF_IMAGE_QUALITY_BEST);
     }
     vectorOfPixels.clear();
     gotAllLatencyFrames = TRUE;
     nowDoAnalyses = TRUE;
     cout<<"nowDoAnalyses = TRUE;\n";
     }
     */
    
    if (nowDoAnalyses == TRUE)
    {
        if(whichAnalysis=="H_SHADOWSCAPES"){ 
            //SAVES AN IMAGE EVERY FRAME
            // cout<<"if(whichAnalysis=="H_SHADOWSCAPES") \n";
            if(synthesisComplete==FALSE){    
                
                latencyFrameCounter++;
                //skip looking at the first 'noOfLatencyFrames' frames
                if (latencyFrameCounter < noOfLatencyFrames){
                    
                    //do nothing
                } 
                else {
                    ofSetColor(255, 255, 255);            
                    ofRect(scanLinePosition, 0, scanLineWidth, ofGetWidth());
                    vectorOfPixels.push_back(pixels);
                    
                    scanLinePosition += scanLineSpeed;
                    
                    if(scanLinePosition > (ofGetWidth()+(scanLineSpeed+scanLineWidth)+noOfLatencyFrames)) {
                        string fileName; 
                        
                        for (i = 0; i < vectorOfPixels.size(); i++)  
                        {   
                            //cout<<i<<"< i in H_SHADOWSCAPES ** frames being written to images \n";
                            fileName = imageSaveFolderPath+whichAnalysis+"_"+ofToString(i)+"_"+ofToString(100.0*i*scanLineSpeed/ofGetWidth(),2)+"%"+".jpg";
                            ofSaveImage(vectorOfPixels[i], fileName, OF_IMAGE_QUALITY_BEST);
                            
                            //ofSaveImage(vectorOfPixels[i], whichAnalysis+"_"+"_"+ofToString(i)+".jpg", OF_IMAGE_QUALITY_BEST);
                        }
                        
                        vectorOfPixels.clear(); //empty out the vector
                        counter = 0;
                        latencyFrameCounter = 0;
                        scanLinePosition=0;
                        synthesisComplete=TRUE; 
                        cout<<whichAnalysis<<"<<-- synthesis and recording complete: \n";
                    }   
                    
                }
                
            } 
            else { 
                cout<<"couldn't synth / record - either not ready or something else it wrong...\n";
            }
        }
        
        //The V_SHADOWSCAPES synth 
        
        if(whichAnalysis=="V_SHADOWSCAPES"){
            //SAVES AN IMAGE EVERY FRAME        
            if(synthesisComplete==FALSE){    
                
                latencyFrameCounter++;
                //skip looking at the first 'noOfLatencyFrames' frames
                if (latencyFrameCounter < noOfLatencyFrames){
                    
                    //do nothing
                } else {
                    
                    //draw the scanning bar
                    ofSetColor(255, 255, 255);            
                    ofRect(0, scanLinePosition, ofGetWidth(),scanLineWidth);
                    
                    vectorOfPixels.push_back(pixels);
                    scanLinePosition += scanLineSpeed;
                    
                    if(scanLinePosition > (ofGetHeight()+(scanLineSpeed+scanLineWidth)+noOfLatencyFrames))
                    {
                        string fileName;
                        
                        for (i = 0; i < vectorOfPixels.size(); i++)  
                        {   
                            //cout<<i<<"< i in V_SHADOWSCAPES ** frames being written to images \n";
                            fileName = imageSaveFolderPath+whichAnalysis+"_"+ofToString(100.0*i*scanLineSpeed/ofGetHeight(),2)+"%_"+ofToString(i)+".jpg";
                            ofSaveImage(vectorOfPixels[i], fileName, OF_IMAGE_QUALITY_BEST);
                        }
                        
                        vectorOfPixels.clear(); //empty out the vector
                        counter = 0;
                        latencyFrameCounter = 0;
                        scanLinePosition=0;
                        synthesisComplete=TRUE; 
                        cout<<whichAnalysis<<"<<-- synthesis and recording complete: \n";               
                    }
                }
            } else {
                cout<<"couldn't synth / record - either not ready or something else it wrong...\n";
            }
        }
        
        //The D_SHADOWSCAPES synth and record method below should load the images as an array of in-RAM pixels -
        //i.e.: let's see if we can speed up the render by holding the camera data in memory
        //something like - ofImageRecorderGrid example or tracerPix_src_v1.0
        if(whichAnalysis=="D_SHADOWSCAPES"){
            //SAVES AN IMAGE EVERY FRAME
            if(synthesisComplete==FALSE){    
                
                latencyFrameCounter++;
                //skip looking at the first 'noOfLatencyFrames' frames
                
                if (latencyFrameCounter < noOfLatencyFrames){
                    //do nothing
                } 
                else {    
                    //draw the scanning bar
                    ofSetColor(255, 255, 255); 
                    ofBeginShape();
                    ofVertex(0,0+scanLinePosition);
                    ofVertex(0,-scanLineWidth+scanLinePosition);
                    ofVertex(ofGetWidth(),-ofGetHeight()+scanLinePosition);
                    ofVertex(ofGetWidth(),-ofGetHeight()+scanLineWidth+scanLinePosition);
                    ofEndShape();
                    scanLinePosition += scanLineSpeed;
                    //cout<<scanLinePosition<<" <-- D_SHADOWSCAPES scanLinePosition: \n";
                    
                    vectorOfPixels.push_back(pixels);
                    
                    if(scanLinePosition > (2*ofGetHeight()+(scanLineSpeed+scanLineWidth)+noOfLatencyFrames)) {
                        //cout<<scanLinePosition<<" <-- scanLinePosition \n";
                        //cout<<ofGetHeight()<<" <-- ofGetHeight() \n";
                        //cout<<" ** scanLinePosition > ofGetHeight() \n";
                        string fileName;
                        
                        for (i = 0; i < vectorOfPixels.size(); i++)
                        {
                            fileName = imageSaveFolderPath+whichAnalysis+"_"+ofToString(i)+"_"+ofToString((100.0*i*scanLineSpeed)/(2*ofGetHeight()),2)+"%"+".jpg";
                            ofSaveImage(vectorOfPixels[i], fileName, OF_IMAGE_QUALITY_BEST);                
                        }
                        
                        vectorOfPixels.clear(); //empty out the vector
                        counter = 0;
                        latencyFrameCounter = 0;
                        scanLinePosition=0;
                        synthesisComplete=TRUE; 
                        cout<<whichAnalysis<<"<<-- synthesis and recording complete: \n";   
                    }
                }
            } else {
                cout<<"couldn't synth / record - either not ready or something else it wrong...\n";
            }
        }
        
        //The analysis as a whole going to require some kind of "wait state" for us to be able to keep something on the screen 
        //for long enough to get an average overall light reading?  i.e: cosmic latte?
        
        //*************  I THINK THIS IS THE MOST PROMISING WAY TO DO THE FILE SAVING - although it could be heavily RAM dependent?! **********//
        if(whichAnalysis=="RELAXRATE"){
            //SAVES AN IMAGE EVERY FRAME
            if(synthesisComplete==FALSE){ 
                
                latencyFrameCounter++;
                //skip looking at the first 'noOfLatencyFrames' frames
                if (latencyFrameCounter < noOfLatencyFrames){
                    
                    //do nothing
                } 
                else {  
                    
                    float lightLevel = 0;
                    //CURVE RELAXES
                    //cout<<"max result and maxtime "<<maxResultA<<" "<< maxNumberOfFramesForSynthesis<<" "<<divisionsA<<"\r";
                    
                    if(whichGraph=="LINEAR"){
                        lightLevel = linear(maxResultA, divisionsA, showGraphA);
                    }
                    
                    // TODO:  TOM - could you hook up 'division' to the GUI for exponential?  and also i don't think the math is exactly right in the function - division is hardcoded for now
                    if(whichGraph=="EXPONENTIAL"){
                        lightLevel = exponential(maxResultA, divisionsA, showGraphA);
                    }
                    
                    // TODO:  the timing on the square wave doesn't seem to be writing to files in an even manner
                    // TODO:  make it so the square wave levels each (i.e.: HIGH/LOW) each write an equal number of files to the disk  
                    
                    if(whichGraph=="SQUARE_WAVE"){
                        lightLevel = 255*squareWave(maxResultA, divisionsA, showGraphA);
                    }
                    //TODO: NEED TO REMOVE THIS from functions and GUI
                    /*
                     if(whichGraph=="QUADRATIC"){
                     quadratic(maxResultA, maxNumberOfFramesForSynthesis, divisionsA, showGraphA);
                     }
                     */
                    
                    //ofFill();
                    ofSetColor(lightLevel);
                    ofRect(0, 0, ofGetWidth(), ofGetHeight());
                    vectorOfPixels.push_back(pixels);            
                    lightLevels.push_back(lightLevel);
                    
                    //frameCounter++;
                    //cout<<frameCounter<<" <<-- frameCounter in RELAXRATE \n";
                    
                    if (finishedGraph) {
                        string fileName; 
                        //cout << vectorOfPixels.size()<< "<< -- vectorOfPixels.size() \n";
                        for (i = 0; i < vectorOfPixels.size(); i++)
                        {
                            //cout<<lightLevels[i]<<"< lightLevels[i] in RELAXRATE \n";
                            fileName = imageSaveFolderPath+whichAnalysis+"_"+whichGraph+"_"+ofToString(i)+"_"+ofToString(lightLevels[i],2)+".jpg";
                            ofSaveImage(vectorOfPixels[i], fileName, OF_IMAGE_QUALITY_BEST);  
                        }
                        
                        vectorOfPixels.clear(); //empty out the vector
                        lightLevels.clear();
                        latencyFrameCounter=0;
                        frameCounter = 0;
                        scanLinePosition=0;
                        synthesisComplete=TRUE; 
                        cout<<whichAnalysis<<"<<-- synthesis and recording complete: \n";   
                    }
                }   
                
            } else {
                cout<<"couldn't synth / record - either not ready or something else it wrong...\n";
            }
        }
        
        //The I_RESPONSE - see google docs
        if(whichAnalysis=="I_RESPONSE"){
            //TOM 31/12/11 THIS HAS BEEN CHANGED TO RUN FROM SYSTEM MILLIS TO maxNumberOfFramesForSynthesis THE TIMING DOESN'T QUITE MATCH THE GUI SETTINGS THOUGH BUT I'VE PLAYED AND PLAYED WITH IT AND CAN'T WORK OUT WHY :( 
            
            
            //THIS ANALYSIS NUMBER OF FRAMES SAVED IS ( numberOfGreyLevels * maxNumberOfFramesForSynthesis ) / everyNthFrameCaptureImage
            //float maxNumberOfFramesForSynthesisPerGreyLevel =  (maxNumberOfFramesForSynthesis- (fadeTime* numberOfGreyLevels))/numberOfGreyLevels;
            float maxNumberOfFramesForSynthesisPerGreyLevel = (maxNumberOfFramesForSynthesis/numberOfGreyLevels)-fadeTime;
            
            if(synthesisComplete == FALSE ){     //other tests of file readyness, etc. go here 
                /*
                 //the below takes in the pixel as raw unsigned chars from the camera, 
                 //stores these in a vector, until the on-screen synthesis is finished 
                 //then the whole set of buffered images is written 
                 //TO A SET OF INDIVIDUAL JPGS
                 //(this is pretty much the last way i can think of doing this file saving bit...) 
                 
                 //cout<<counter<<" <-- RELAXRATE COUNTER: \n";
                 
                 // white rectangle that stays on the screen for 5000 ms, then fades out over 2000 ms 
                 // take 5 images over the first 5 seconds from the camera, but then none during the fade out
                 
                 // want to hold the programme here - take a few images while the screen light level 
                 // is at held constant, then fade what's on the screen away (this isn't for analytical purposes... just so it looks nice 
                 // then we 'hold' at these constant levels of 255, 254, 253 
                 */
                
                latencyFrameCounter++;
                //skip looking at the first 'noOfLatencyFrames' frames
                if (latencyFrameCounter < noOfLatencyFrames){
                    
                    //do nothing
                } 
                else { 
                    everyNthFrameCaptureImage = ( numberOfGreyLevels * maxNumberOfFramesForSynthesis ) / numOfFramesToGrab;
                    cout<<maxNumberOfFramesForSynthesisPerGreyLevel<<" "<<ofGetFrameRate()<<" "<< everyNthFrameCaptureImage<< " "<<numberOfGreyLevels<<" "<<maxNumberOfFramesForSynthesis<<" "<<numOfFramesToGrab<<" data \n";
                    if (currentGreyLevel > 0) {
                        
                        //TOM 31/12/11 timeDiff etc replaced with counter in line with other analyses
                        
                        // thisTime = ofGetElapsedTimeMillis(); 
                        //timeDiff = thisTime-lastTime; 
                        //cout<<timeDiff<<"<-- timeDiff \n";
                        //cout<<counter2<<"<-- counter2 \n";
                        
                        
                        ///////////THIS SECTION IS JUST FOR PUSHING THE PIXELS INTO A VECTOR EVERY NTH FRAME////////////
                        
                        if (counter < maxNumberOfFramesForSynthesisPerGreyLevel + noOfLatencyFrames) {
                            //if (timeDiff < maxNumberOfFramesForSynthesisPerGreyLevel) {
                            //currentGreyLevel and numberOfGreyLevels are the same at the start so dividing one by the other is going to give progress as a percentage
                            level = 255.0*(currentGreyLevel/numberOfGreyLevels), 255.0*(currentGreyLevel/numberOfGreyLevels), 255.0*(currentGreyLevel/numberOfGreyLevels);
                            ofSetColor(level);            
                            ofRect(0, 0, ofGetWidth(), ofGetHeight());
                            
                            
                            
                            if (counter%everyNthFrameCaptureImage == 0)  
                            {
                                vectorOfPixels.push_back(pixels);
                                lightLevels.push_back(level); 
                                cout<<level<<" "<<check<<" level in 1st if (counter%everyNthFrameCaptureImage == 0) \n";  
                                //frameCounter++;
                                check++;
                                //cout<<frameCounter<<" <<-- frameCounter in I_RESPONSE \n";
                            }
                        } 
                        
                        
                        ///IF THE COUNTER IS OVER THE TIME LIMIT BUT STILL FADING THEN REDUCE LEVEL
                        if ((counter >= maxNumberOfFramesForSynthesisPerGreyLevel) && (counter <= maxNumberOfFramesForSynthesisPerGreyLevel+fadeTime) ){
                            cout<<"fading \n";
                            level = (255.0*(currentGreyLevel/numberOfGreyLevels))-(255.0*(currentGreyLevel/numberOfGreyLevels)*(counter-maxNumberOfFramesForSynthesisPerGreyLevel)/(fadeTime));                               
                            ofSetColor(level);                     
                            ofRect(0, 0, ofGetWidth(), ofGetHeight());                    
                        } 
                        
                        counter++;
                        
                        ///IF THE COUNTER IS FINISHED FADING THEN SAVE EVERYTHING OUT THEN RESET COUNTER AND REDUCE GREY LEVEL BY 1, START AGAIN FROM THE TOP
                        
                        if (counter > (maxNumberOfFramesForSynthesisPerGreyLevel+fadeTime)) {
                            cout<<"<-- inside second else if \n";
                            ofSetColor(0, 0, 0);               
                            ofRect(0, 0, ofGetWidth(), ofGetHeight());
                            
                            string fileName; 
                            for (i = 0; i < vectorOfPixels.size(); i++)
                            {
                                fileName = imageSaveFolderPath+whichAnalysis+"_"+"_"+ofToString(i)+"_"+ofToString(lightLevels[i],2)+".jpg";
                                ofSaveImage(vectorOfPixels[i], fileName, OF_IMAGE_QUALITY_BEST);
                                //cout<<i<<"< i in I_RESPONSE ** frames being written to images \n";
                                //cout<<ofToString(lightLevels[i],2)<<"<<-- ofToString(lightLevels[i],2) \n";
                            }
                            
                            vectorOfPixels.clear(); //empty out the vector
                            lightLevels.clear();
                            frameCounter = 0;
                            latencyFrameCounter=0;
                            counter=0;
                            scanLinePosition=0;
                            currentGreyLevel--;
                            check=0;
                            // lastTime = ofGetElapsedTimeMillis();
                        } 
                        
                    } 
                    else {
                        synthesisComplete=TRUE; 
                        cout<<whichAnalysis<<"<<-- synthesis and recording complete: \n"; 
                    }     
                }     
                
            } 
            else {
                cout<<"couldn't synth / record - either not ready or something else it wrong...\n";
            }
        }       
        
        //skkpping this one for the moment...  This is going to be complicated - hoping for help from DAVID G
        
        //TODO: THIS IS ONLY SAVING THE 3RD SET?! 
        
        //Tom S added copy of shapes from "Trimensional" iphone app. 
        
        if(whichAnalysis=="SHAPE_SHADING") {
            //IN THIS ANALYSIS NUMBER OF FRAMES SAVED IS framesPerQuadrant * 4
            int xPos;
            int yPos;
            float circleDia=ofGetWidth()*0.33;
            float smallCircleDia=ofGetWidth()*0.3;
            string fileName;
            
            if(synthesisComplete == FALSE ) { 
                
                latencyFrameCounter++;
                //skip looking at the first 'noOfLatencyFrames' frames
                
                if (latencyFrameCounter < noOfLatencyFrames){
                    
                    //do nothing
                    
                } else {     
                    
                    if (counter < maxNumberOfFramesForSynthesis) {
                        
                        //draw top middle
                        if(counter < maxNumberOfFramesForSynthesis/4) {
                            xPos = ofGetWidth()*0.5;
                            yPos = 0;
                            ofSetColor(155);
                            ofCircle(xPos-circleDia/2, yPos, smallCircleDia);
                            ofCircle(xPos+circleDia/2, yPos, smallCircleDia);
                            ofSetColor(255);
                            ofCircle(xPos, yPos, circleDia);
                            
                            if ((localFrameCounter < (framesPerQuadrant+noOfLatencyFrames)) && (!gotAllLocalFrames1))
                            {
                                localFrameCounter++;                        
                                vectorOfPixels.push_back(pixels);
                            } 
                            else {
                                string fileName;
                                for (i = 0; i<vectorOfPixels.size(); i++)
                                {
                                    fileName = imageSaveFolderPath+whichAnalysis+"_"+"_"+ofToString(i)+"_"+"Q1"+".jpg";
                                    ofSaveImage(vectorOfPixels[i], fileName, OF_IMAGE_QUALITY_BEST);
                                }
                                vectorOfPixels.clear();
                                gotAllLocalFrames1 = TRUE;
                                localFrameCounter = 0;
                            }
                        }
                        
                        //TODO:  put in CROSS FADES, ETC§E 
                        
                        //draw right
                        if(counter >= maxNumberOfFramesForSynthesis/4 && counter < maxNumberOfFramesForSynthesis/2){
                            xPos= ofGetWidth();
                            yPos=ofGetHeight()*0.5;
                            ofSetColor(155);
                            ofCircle(xPos, yPos-circleDia/2, smallCircleDia);
                            ofCircle(xPos, yPos+circleDia/2, smallCircleDia);
                            ofSetColor(255);
                            ofCircle(xPos, yPos, circleDia);
                            
                            if ((localFrameCounter < (framesPerQuadrant+noOfLatencyFrames)) && (!gotAllLocalFrames2))
                            {
                                localFrameCounter++;                        
                                vectorOfPixels.push_back(pixels);
                            } 
                            else {
                                string fileName;
                                for (i = 0; i<vectorOfPixels.size(); i++)
                                {
                                    fileName = imageSaveFolderPath+whichAnalysis+"_"+"_"+ofToString(i)+"_"+"Q2"+".jpg";
                                    ofSaveImage(vectorOfPixels[i], fileName, OF_IMAGE_QUALITY_BEST);
                                }
                                vectorOfPixels.clear();
                                gotAllLocalFrames2 = TRUE;
                                localFrameCounter = 0;
                            }
                        }
                        
                        //draw bottom middle
                        if(counter >= maxNumberOfFramesForSynthesis/2 && counter < 3*maxNumberOfFramesForSynthesis/4) {
                            
                            xPos= ofGetWidth()*0.5;
                            yPos=ofGetHeight();
                            ofSetColor(155);
                            ofCircle(xPos-circleDia/2, yPos, smallCircleDia);
                            ofCircle(xPos+circleDia/2, yPos, smallCircleDia);
                            ofSetColor(255);
                            ofCircle(xPos, yPos, circleDia);
                            
                            
                            if ((localFrameCounter < (framesPerQuadrant+noOfLatencyFrames)) && (!gotAllLocalFrames3))
                            {
                                localFrameCounter++;                        
                                vectorOfPixels.push_back(pixels);
                            } 
                            else {
                                string fileName;
                                for (i = 0; i<vectorOfPixels.size(); i++)
                                {
                                    fileName = imageSaveFolderPath+whichAnalysis+"_"+"_"+ofToString(i)+"_"+"Q3"+".jpg";
                                    ofSaveImage(vectorOfPixels[i], fileName, OF_IMAGE_QUALITY_BEST);
                                }
                                vectorOfPixels.clear();
                                gotAllLocalFrames3 = TRUE;
                                localFrameCounter = 0;
                            }
                        }
                        
                        //draw left
                        if(counter>= 3*maxNumberOfFramesForSynthesis/4 && counter<maxNumberOfFramesForSynthesis){
                            xPos= 0;
                            yPos=ofGetHeight()*0.5;
                            ofSetColor(155);
                            ofCircle(xPos, yPos-circleDia/2, smallCircleDia);
                            ofCircle(xPos, yPos+circleDia/2, smallCircleDia);
                            ofSetColor(255);
                            ofCircle(xPos, yPos, circleDia);
                            
                            if ((localFrameCounter < (framesPerQuadrant+noOfLatencyFrames)) && !gotAllLocalFrames4)
                            {
                                localFrameCounter++;                        
                                vectorOfPixels.push_back(pixels);
                            } 
                            else {
                                string fileName;
                                for (i = 0; i<vectorOfPixels.size(); i++)
                                {
                                    fileName = imageSaveFolderPath+whichAnalysis+"_"+"_"+ofToString(i)+"_"+"Q4"+".jpg";
                                    ofSaveImage(vectorOfPixels[i], fileName, OF_IMAGE_QUALITY_BEST);
                                }
                                vectorOfPixels.clear();
                                lightLevels.clear();
                                gotAllLocalFrames4 = TRUE;
                                localFrameCounter = 0;
                            }
                        }
                        
                        counter++;
                        
                    }
                    else{
                        synthesisComplete =TRUE;  
                        latencyFrameCounter=0;
                        localFrameCounter = 0;
                        counter=0;
                    }
                    
                }
                
            } else {
                cout<<"couldn't synth / record - either not ready or something else it wrong...\n";
            }
            
        }    
        
        if(whichAnalysis=="M_CODE"){
            //THIS ANALYSIS DOES NOT APPEAR TO BE SAVING OUT IMAGES AT ALL RIGHT NOW
            if(synthesisComplete == FALSE ){
                
                //cout<<"showing morse "<<morseMessage<<"\r";
                showMorse(morseMessage);
                
                
                //WHERE in here do we grab frames from the camera? 
                // have tried to grab one everytime through - see what we get... but its too many frames
                
                
                //vectorOfPixels.push_back(pixels);
                
                if (morseComplete == TRUE)
                {
                    string fileName; 
                    for (i = 0; i < vectorOfPixels.size(); i++)
                    {
                        fileName = imageSaveFolderPath+whichAnalysis+"_"+ofToString(i)+".jpg";
                        ofSaveImage(vectorOfPixels[i], fileName, OF_IMAGE_QUALITY_BEST);
                        //cout<<i<<"< i in M_CODE ** frames being written to images \n";
                    }
                    vectorOfPixels.clear(); //empty out the vector
                    lightLevels.clear();
                    counter = 0;
                    frameCounter = 0;
                    scanLinePosition=0;
                    synthesisComplete=TRUE; 
                    cout<<whichAnalysis<<"<<-- synthesis and recording complete: \n";
                }        
                
            } else {
                cout<<"couldn't synth / record - either not ready or something else it wrong...\n";
            }
            //  synthesisComplete =TRUE;
            
        }
        
        //CAM_FRAMERATE 
        
        // TODO: This is still a bit fucked up - the timing goes 'over' 30 and under 'zero' 
        // TODO: The frame rate should probably ramp as a gaussian (not linearly)
        
        
        // TODO:  How to save the output from this?  Perhaps as a video?... hmmm..
        if(whichAnalysis=="CAM_FRAMERATE"){
            //THIS ANALYSIS SAVES A FRAME EVERY TIME
            if(synthesisComplete == FALSE ){      
                
                strobe();
                
                // TODO:  
                //there should be a saving function - that takes frames in sync with the strobe (1 frame when on, 1 frame when off)  
                //i don't understand how to do this with the strobe() encapsulated in a function like this 
                
                //do we use strobe() elsewhere?  or could we just put the guts of the function here so we can save a frame on the up and downswing of the strobe? 
                
                //The basics for file saving are below... same problem as M_CODE - i.e.: how to save this to files that won't slow the strobe down
                
                vectorOfPixels.push_back(pixels);
                
                if (strobeComplete == TRUE)
                {
                    string fileName; 
                    for (i = 0; i < vectorOfPixels.size(); i++)
                    {
                        fileName = imageSaveFolderPath+whichAnalysis+"_"+ofToString(i)+".jpg";
                        ofSaveImage(vectorOfPixels[i], fileName, OF_IMAGE_QUALITY_BEST);
                        //cout<<i<<"< i in M_CODE ** frames being written to images \n";
                    }
                    
                    vectorOfPixels.clear(); //empty out the vector
                    counter = 0;
                    frameCounter = 0;
                    scanLinePosition=0;
                    synthesisComplete=TRUE; 
                    cout<<whichAnalysis<<"<<-- synthesis and recording complete: \n";
                    
                }        
                
            } else {
                cout<<"couldn't synth / record - either not ready or something else it wrong...\n";
            }
            
            
            //synthesisComplete =TRUE;
            
        }
        
        //Writes a given grey value to the screen, then grabs 'X' number of grames while that grey value is on the screen     
        //TODO:  THIS DOESN'T end with black... should increase the 
        
        if(whichAnalysis=="CAM_NOISE"){
            //IN THIS ANALYSIS NUMBER OF FRAMES SAVED IS framesPerGreyValue * numberOfGreyLevels
            if(synthesisComplete==FALSE){    
                
                // tries to gather a set frames to get rid of as they are occuring before the on-screen imagery is being writtne 
                latencyFrameCounter++;
                
                //put this outside the latency check so something gets written to the screen everytime
                // greyValue = 255.0 - ((255.0/numberOfGreyLevels)*(int)((numberOfGreyLevels)*(double)counter/255.0));
                
                //not really sure how you're getting this grey level                
                greyValue = maxNumberOfFramesForSynthesis - ((maxNumberOfFramesForSynthesis/numberOfGreyLevels)*(int)((numberOfGreyLevels)*(double)counter/maxNumberOfFramesForSynthesis));
                
                
                // successively more dim rectangle to the screen 
                
                ofSetColor(greyValue*255.0/maxNumberOfFramesForSynthesis);                   
                ofRect(0, 0, ofGetWidth(), ofGetHeight());
                
                if (latencyFrameCounter < noOfLatencyFrames) {
                    //do nothing
                    
                } else {  
                    
                    counter++;                    
                    
                    //cout << greyValue-oldGreyValue<<" <-- CAM_NOISE greyValue-oldGreyValue \n";
                    //cout << greyValue<<" <-- CAM_NOISE greyValue \n";
                    //cout<<counter<<" <-- CAM_NOISE COUNTER: \n";
                    //cout<<((255.0/counter2)*(int)((counter2+1)*(double)counter/255.0))<<" <-- CAM_NOISE math thing \n";
                    
                    //whenever the greyValue is the same, take X images 
                    
                    if (greyValue - oldGreyValue == 0)  {
                        
                        if ((localFrameCounter < (framesPerGreyValue+noOfLatencyFrames)) && (k==1))
                        {
                            vectorOfPixels.push_back(pixels); 
                            lightLevels.push_back(greyValue);
                            localFrameCounter++;
                            k=1;
                        } 
                        else {
                            localFrameCounter=0;
                            k=0;
                        }
                    }  
                    else {
                        k = 1; 
                    }
                    
                    oldGreyValue = greyValue;
                    
                    //once we've finished synthesis and capturing all the frames into RAM, we can then write the
                    //image vectors "imgs" backinto a quicktime movie...
                    
                    if(counter >= maxNumberOfFramesForSynthesis) {
                        
                        string fileName; 
                        
                        for (i = 0; i < vectorOfPixels.size(); i++)
                        {
                            //  cout<<i<<" <<-- i inside CAM_NOISE \n";
                            fileName = imageSaveFolderPath+whichAnalysis+"_"+ofToString(i)+"_"+ofToString(lightLevels[i],2)+".jpg";
                            ofSaveImage(vectorOfPixels[i], fileName, OF_IMAGE_QUALITY_BEST);
                        }
                        vectorOfPixels.clear(); //empty out the vector
                        lightLevels.clear();
                        counter = 0;
                        frameCounter = 0;
                        latencyFrameCounter=0;
                        counter=0;
                        synthesisComplete=TRUE; 
                        cout<<whichAnalysis<<"<<-- synthesis and recording complete: \n";
                        
                    }
                }
                
                
            } else {
                cout<<"couldn't synth / record - either not ready or something else it wrong...\n";
            }
        }
        
        
        //Writes a color to the screen and reads back X number of frames
        if(whichAnalysis=="COLOR_SINGLE"){
            //IN THIS ANALYSIS NUMBER OF FRAMES SAVED IS framesPerColourValue * numberOfGreyLevels
            
            
            
            // TODO - new strategy for latency - wait for X frames before grabbing, then stop grabbing Y frames before a color change  
            // The frame lag (empirically, using the isight camera) seems to be ofGetFrameRate()/6, so we'll wait that, plus 1
            
            if(synthesisComplete==FALSE){    ///may have to add a little thing in here that ensures the camera buffer (of frames) is empty
                
                counter++;
                //cout<<counter<<"<<-- counter COLOR_SINGLE \n";
                // cout<<maxNumberOfFramesForSynthesis/3<<"<<- maxNumberOfFramesForSynthesis/3 COLOR_SINGLE \n";
                
                ofSetColor(aColour);
                ofRect(0, 0, ofGetWidth(), ofGetHeight());
                
                if (0*maxNumberOfFramesForSynthesis <= counter && counter < maxNumberOfFramesForSynthesis/3)  {
                    //cout<<"counter < maxNumberOfFramesForSynthesis/3 \n";
                    //red
                    aColour.r=255;
                    aColour.g=0;
                    aColour.b=0;
                    
                    latencyFrameCounter++;
                    //skip looking at the first ofGetFrameRate()/6 frames
                    
                    if (latencyFrameCounter < noOfLatencyFrames){
                        //do nothing
                    } else {
                        
                        latencyFrameCounter = 0;
                        
                        if ((localFrameCounter < (framesPerColourValue+noOfLatencyFrames)) && (!gotAllLocalFrames1) )
                        {
                            vectorOfPixels.push_back(pixels); 
                            lightLevels.push_back(1);
                            localFrameCounter++;
                            //cout<<frameCounter<<"<-- frameCounter \n";
                            //cout<<localFrameCounter<<"<-- localFrameCounter RED \n";
                        } else {
                            gotAllLocalFrames1 = true;
                            localFrameCounter = 0;
                        }
                    }
                }
                
                if (maxNumberOfFramesForSynthesis/3 <= counter && counter < 2*maxNumberOfFramesForSynthesis/3) {
                    //cout<<"maxNumberOfFramesForSynthesis/3 <= counter <= 2*maxNumberOfFramesForSynthesis/3 \n";
                    // green
                    
                    aColour.r=0;
                    aColour.g=255;
                    aColour.b=0;
                    
                    latencyFrameCounter++;
                    //skip looking at the first ofGetFrameRate()/6 frames
                    if (latencyFrameCounter < noOfLatencyFrames){
                        //do nothing
                    } else {
                        
                        latencyFrameCounter = 0;
                        
                        if ((localFrameCounter < (framesPerColourValue+noOfLatencyFrames)) && (!gotAllLocalFrames2) )
                        {
                            vectorOfPixels.push_back(pixels); 
                            lightLevels.push_back(2);
                            localFrameCounter++;
                            //cout<<localFrameCounter<<"<-- localFrameCounter GREEN \n";
                        } else {
                            gotAllLocalFrames2 = TRUE;
                            localFrameCounter = 0;
                        }
                    }
                } 
                
                if (2*maxNumberOfFramesForSynthesis/3 <= counter && counter < maxNumberOfFramesForSynthesis) {
                    // blue
                    aColour.r=0;
                    aColour.g=0;
                    aColour.b=255;
                    
                    latencyFrameCounter++;
                    //skip looking at the first ofGetFrameRate()/6 frames
                    if (latencyFrameCounter < noOfLatencyFrames){
                        //do nothing
                    } else {
                        // get frames
                        latencyFrameCounter = 0;
                        
                        // the latencyFrames variable below causes us to get "latencyFrames" more frames than we normally would 
                        // - in order to compensate for the lateness of the 
                        if ((localFrameCounter < (framesPerColourValue+noOfLatencyFrames)) && (!gotAllLocalFrames3) )
                        {
                            vectorOfPixels.push_back(pixels); 
                            lightLevels.push_back(3);
                            localFrameCounter++;
                            //cout<<localFrameCounter<<"<-- localFrameCounter BLUE \n";
                        } else {
                            gotAllLocalFrames3 = TRUE;
                            localFrameCounter = 0;
                        }
                    }
                } 
                
                if(counter > maxNumberOfFramesForSynthesis) {
                    cout<<"counter >= maxNumberOfFramesForSynthesis in COLOR_SINGLE \n";
                    string fileName;
                    for (i = 0; i < vectorOfPixels.size(); i++)
                    {
                        fileName = imageSaveFolderPath+whichAnalysis+"_"+ofToString(i)+"_"+ofToString(lightLevels[i])+".jpg";
                        ofSaveImage(vectorOfPixels[i], fileName, OF_IMAGE_QUALITY_BEST);
                    }
                    vectorOfPixels.clear(); //empty out the vector
                    lightLevels.clear();
                    counter = 0;
                    frameCounter = 0;
                    localFrameCounter = 0;
                    synthesisComplete=TRUE; 
                    cout<<whichAnalysis<<"<<-- synthesis and recording complete: \n";
                }
            } else {
                cout<<"couldn't synth / record - either not ready or something else it wrong...\n";
            }
        }
        
        //TODO:  Repurpose this as a latency measurement function.  Measures the frame lag between the input (video camera) and the output (writing frames to screen)
        //i.e.: Play black to screen, start recording frames, play white to screen, average pixels to detect the change - calculate how many frames after the actual output change
        // NOte - this is currently set as  ofGetFramerate()/6, empirically
        
        if(whichAnalysis=="LATENCY_TEST"){
            //THIS ANALYSIS SAVES A FRAME EVERY TIME
            
            if(synthesisComplete == FALSE ){    
                
                ofSetColor(aColour);
                ofRect(0, 0, ofGetWidth(), ofGetHeight());
                aColour.r = num;
                aColour.g = num;
                aColour.b = num;
                
                counter++;
                //cout<<counter<<" <<-- counter\n";
                
                if((0 < counter) && (counter <= maxNumberOfFramesForSynthesis/3))
                {
                    cout<<" 0 <= counter < 30\n";
                    num = 0;
                    //  cout<<num<<" <<-- num\n";
                } 
                
                if((maxNumberOfFramesForSynthesis/3 < counter) && (counter <= 2*maxNumberOfFramesForSynthesis/3))  
                {
                    cout<<" 30<=counter < 60\n";
                    num = 255;
                    //  cout<<num<<" <<-- num\n";
                } 
                
                if((2*maxNumberOfFramesForSynthesis/3 < counter) &&  (counter < maxNumberOfFramesForSynthesis))
                {
                    cout<<" 60 <= counter < 90\n";
                    num = 0;
                    //  cout<<num<<" <<-- num\n";
                } 
                
                if(counter >= maxNumberOfFramesForSynthesis)
                {
                    string fileName;
                    for (i = 0; i < vectorOfPixels.size(); i++)
                    {
                        fileName = imageSaveFolderPath + whichAnalysis + "_" + ofToString(i) + "_" + ofToString(lightLevels[i]) + ".jpg";
                        ofSaveImage(vectorOfPixels[i], fileName, OF_IMAGE_QUALITY_BEST);
                    }
                    vectorOfPixels.clear(); //empty out the vector
                    lightLevels.clear();
                    counter = 0;
                    synthesisComplete=TRUE; 
                    cout<<whichAnalysis<<"<<-- synthesis and recording complete: \n";
                    
                    ofImage reLoadedImage;
                    for (i = 0; i < vectorOfPixels.size(); i++)
                    {
                        fileName = imageSaveFolderPath + whichAnalysis + "_" + ofToString(i) + "_" + ofToString(lightLevels[i]) + ".jpg";
                        
                        if (myFileHelper.doesFileExist(fileName)){
                            reLoadedImage.loadImage(fileName);
                        }
                    }
                }
                
                vectorOfPixels.push_back(pixels); 
                lightLevels.push_back(num);
                
            } else {
                cout<<"couldn't synth / record - either not ready or something else it wrong...\n";
            }
        }
        
        
        //This will record as many frames as possible at specific output light hues
        if(whichAnalysis=="COLOR_MULTI"){
            //IN THIS ANALYSIS THE NUMBER OF FRAMES SAVED = counterMaxColorMulti / howDifferentHuesNeedToBeBeforeFrameSaved
            //TODO - Fade up from black to not 'jar' the scene with initial color 
            
            ofRect(0, 0, ofGetWidth(), ofGetHeight());
            
            if(synthesisComplete == FALSE ){    
                
                counter++;
                //cout<<counter<<" <<-- counter in COLOR_MULTI \n";
                
                latencyFrameCounter++;
                //skip looking at the first ofGetFrameRate()/6 frames
                
                if (latencyFrameCounter < noOfLatencyFrames) {
                    //do nothing - skip these first frames to allow the camera to catch up
                } 
                else {
                    //so when counter == counterMaxColorMulti cHue will be 255
                    cHue = 255.0*(float)counter/(float)counterMaxColorMulti; 
                    //cout << cHue <<  "<<--- cHue in COLOR_MULTI \n";
                    
                    //ofColor.setHsb(float hue, float saturation, float brightness)
                    aColour.setHsb(cHue, 255, 255); 
                    ofSetColor(aColour); 
                    //Tom -> Jamie 30/12/11 why do we need to copy the value of cHue over to newHue here? 
                    newHue = cHue;
                    if (abs(newHue - oldHue) > howDifferentHuesNeedToBeBeforeFrameSaved)
                    {
                        //cout << "hue is different enough to bother saving a frame - COLOR_MULTI \n";
                        vectorOfPixels.push_back(pixels); 
                        lightLevels.push_back(cHue);
                        oldHue = newHue;
                    }
                    
                    if (counter > counterMaxColorMulti + noOfLatencyFrames) {  
                        
                        //cout << counter << " <<-- counter >= counterMax in COLOR_MULTI \n";
                        string fileName;
                        for (i = 0; i < vectorOfPixels.size(); i++)  
                        {   
                            fileName = imageSaveFolderPath+whichAnalysis+"_"+ofToString(i)+"_"+ofToString(lightLevels[i])+".jpg";
                            ofSaveImage(vectorOfPixels[i], fileName, OF_IMAGE_QUALITY_BEST);
                        }
                        
                        vectorOfPixels.clear(); //empty out the vector
                        lightLevels.clear();
                        counter = 0;
                        synthesisComplete=TRUE; 
                        latencyFrameCounter;
                        cout<<whichAnalysis<<"<<-- synthesis and recording complete: \n";
                    }
                    
                }
                
            } else {
                cout<<"couldn't synth / record - either not ready or something else it wrong...\n";
            }
        }
        
        
        if(whichAnalysis=="DIFF_NOISE") {
            //NUMBER OF FRAMES SAVED IN THIS ANALYSIS IS DEPENDENT ON A RANDOM VARIABLE IT SEEMS TO BE SET UP TO GRAB ABOUT HALF THE FRAMES
            if(synthesisComplete == FALSE) { 
                
                //noiseTexture.allocate(camWidth, camHeight, GL_RGB);
                
                //noisey.allocate(ofGetWidth(),ofGetHeight(), OF_IMAGE_COLOR_ALPHA);
                //cout<<noisey.getHeight()<<"noisey.getHeight() /n";
                //cout<<noisey.getWidth()<<"noisey.getWidth() /n";
                
                //noisey.draw(0,0);  
                
                counter++;
                //cout<<counter<<" <<-- counter in DIFF_NOISE \n";
                
                //ofColor.setHsb(float hue, float saturation, float brightness)
                rand255 = ofRandom(0,255);
                aColour.setHsb(rand255, 255, 255); 
                ofSetColor(aColour); 
                //cout<<aColour<<"<<-- aColour in DIFF_NOISE \n";
                ofRect(0, 0, ofGetWidth(), ofGetHeight());
                
                rand10 = ofRandom(0,10);
                
                if (rand10 > 5.0) {
                    cout << " we're randomly saving a frame - DIFF_NOISE \n";
                    vectorOfPixels.push_back(pixels); 
                    lightLevels.push_back(rand255);
                }
                
                if (counter > maxNumberOfFramesForSynthesis) {  
                    cout << counter << " <<-- counter >= maxNumberOfFramesForSynthesis in DIFF_NOISE \n";
                    string fileName;
                    for (i = 0; i < vectorOfPixels.size(); i++)  
                    {   
                        fileName = imageSaveFolderPath+whichAnalysis+"_"+ofToString(i)+"_"+ofToString(lightLevels[i])+".jpg";
                        ofSaveImage(vectorOfPixels[i], fileName, OF_IMAGE_QUALITY_BEST);
                    }
                    vectorOfPixels.clear(); //empty out the vector
                    counter = 0;
                    synthesisComplete=TRUE; 
                    cout<<whichAnalysis<<"<<-- synthesis and recording complete: \n";
                }
                
            } else {
                cout<<"couldn't synth / record - either not ready or something else it wrong...\n";
            }
        }
        
        
    }
}


////////////////////////---------/////////////////////////////////////
//ANALYSE VIDEO METHODS
// the below functions run analysis on the recorded video to extract whatever information we might want... 
// the output of this could be another video file, or a live rendering of the data
void analysis::analyseInput(unsigned char * pixels){
    
    
    /*
     by calling setUseTexture(false) on the ofImage before you load it, you'll stop openFrameworks from uploading the pixels to the graphics card and save a lot of space.
     */
    
    
    //Analysis for the specific analyses as needed...    
    if (whichAnalysis=="H_SHADOWSCAPES") {
        
    }
    
    if (whichAnalysis=="V_SHADOWSCAPES") {
        
    }     
    
    if (whichAnalysis=="D_SHADOWSCAPES") {
        
    } 
    
    if (whichAnalysis=="RELAXRATE") {
        
    } 
    
    if (whichAnalysis=="I_RESPONSE") {
        
    }
    
    if (whichAnalysis=="SHAPE_SHADING") {
        
    } 
    
    if (whichAnalysis=="M_CODE") {        
        
    } 
    
    if (whichAnalysis=="CAM_FRAMERATE") {
        
    } 
    
    if (whichAnalysis=="CAM_NOISE") {
        // load a first image - take the pixel value from each subsequent frame taken at each grey value
        
        // the differenced images - create z-map - overlay on the original images
        
    } 
    
    if (whichAnalysis=="COLOR_SINGLE") {
        
    } 
    
    if (whichAnalysis=="LATENCY_TEST") {
        
    }
    
    if (whichAnalysis=="COLOR_MULTI") {
        
    } 
    
    if (whichAnalysis=="DIFF_NOISE") {
        
    } 
    
}

////////////////////////---------/////////////////////////////////////
// DISPLAY THE RESULT OF THE ANALYSIS TO SCREEN
// the below functions run analysis on the recorded video to extract whatever information we might want... 
// the output of this could be another video file, or a live rendering of the data
void analysis::displayResult(){
    
    if(whichAnalysis=="H_SHADOWSCAPES"){ 
        player.draw(0, 0, camWidth, camHeight);
    }
    
    if(whichAnalysis=="V_SHADOWSCAPES"){ 
        player.draw(0, 0, camWidth, camHeight);
    }
    
    if(whichAnalysis=="D_SHADOWSCAPES"){ 
        player.draw(0, 0, camWidth, camHeight);
    }
}


//Setup the movie for playback
void analysis::setupMovie(){
    if(!player.isLoaded()){
        player.loadMovie(cameraMovieName);
        player.play();
    }
}

//below isn't being used - but could pull the 'setup' functions from analysis setup above into this we want...
void analysis::setupQuicktimeMovieRecord(int camH, int camW, int codec){
    movieFromCamera.setCodecType(codec);
    movieFromCamera.setup(camH, camW, cameraMovieName);
}

////////////////////////---------/////////////////////////////////////
void analysis::updatePlayer(){
    player.idleMovie();
}

////////////////////////---------/////////////////////////////////////
vector<ofImage> analysis::returnFrames(){
    //return buffer;
    
}

void analysis::setupGraphs(){
    showGraphA = FALSE;
    graphCounter = 0;
    dummyCounter = 0;
    intervalCounter = 0;
    intervalSpeed = 10;
    limiter = 0;
    on = false; 
    flip = 1;    
    level = 0;
    finishedGraph = FALSE;
}
//TOM 30/12/11 removed float maxTime which previously passed maxNumberOfFramesForSynthesis into graphing functions

//actually just to the power of a square
float analysis::exponential(float maxResult,float divisions, bool showGraph){
    //removed this and returned control of variable to gui
    // divisions = 10.0;
    float mappedLightLevel;
    float threshold=maxNumberOfFramesForSynthesis/divisions;
    
    if (limiter < maxNumberOfFramesForSynthesis + noOfLatencyFrames) {
        
        float yPos;
        float lightLevel=pow(level,2);
        // mappedLightLevel = ofMap(lightLevel, 0, pow(maxNumberOfFramesForSynthesis/2,2), 0,maxResult);
        mappedLightLevel = ofMap(lightLevel, 0, pow(maxNumberOfFramesForSynthesis/divisions,2), 0,maxResult);
        
        graphCounter++;
        limiter++;
        
        //cout<<graphCounter<<" <<-- graphCounter \n";
        
        if (graphCounter < maxNumberOfFramesForSynthesis/divisions) {
            level+=flip;
            //level++;
        } else {
            level-=flip;
            //level--;
        }
        
        if(graphCounter >= threshold){
            graphCounter = 0;
            cout<<"graphCounter set to zero \n";
            flip*=-1;
        }
        
        if(showGraph){
            float localCount=0;
            ofNoFill();
            ofSetColor(255,0,0);
            
            float xPos=ofMap(graphCounter, 0, maxNumberOfFramesForSynthesis, 0, ofGetWidth());
            ofLine(xPos, ofGetHeight()-1, xPos, ofGetHeight()-150);
            ofBeginShape();
            for(float i=0;i<maxNumberOfFramesForSynthesis;i++){
                yPos=pow(localCount,2); 
                float mappedY = ofMap(yPos,0,pow(maxNumberOfFramesForSynthesis/2,2), 0,maxResult);
                ofVertex( ofMap(i, 0, maxNumberOfFramesForSynthesis, 0, ofGetWidth()) , ofGetHeight()-mappedY);
                if (i<maxNumberOfFramesForSynthesis/2) {
                    localCount++;
                }
                else{
                    localCount--;
                }
            }
            ofEndShape();
        }   
    } else {
        finishedGraph=TRUE;
    }
    
    return mappedLightLevel;
}

//TOM 30/12/11 removed float maxTime which previously passed maxNumberOfFramesForSynthesis into graphing functions


void analysis::quadratic(float maxResult, float divisions, bool showGraph){
    
    if (limiter < maxNumberOfFramesForSynthesis) {
        ofNoFill();
        ofSetColor(255, 0, 0);
        ofBeginShape();
        
        for(float i=-maxNumberOfFramesForSynthesis/2; i<maxNumberOfFramesForSynthesis/2; i++) {
            
            float yPos;
            //makes it narrower
            float a=.2;
            
            float b=.2;
            
            //shifts up or down
            float c=0;
            
            float x=i;
            yPos= (a * pow(x,2)) + (b*x)+ c; 
            
            // ofVertex(ofMap(i+maxNumberOfFramesForSynthesis/2, 0,maxNumberOfFramesForSynthesis, 0, ofGetWidth()) , yPos);
            ofVertex(i+maxNumberOfFramesForSynthesis/2 , yPos);
            cout<<i+maxNumberOfFramesForSynthesis/2<<" "<<yPos<<" yPos \r";
        }
        ofEndShape();
    }
    else{
        finishedGraph=true;
    }
}

//TOM 30/12/11 removed float maxTime which previously passed maxNumberOfFramesForSynthesis into graphing functions


bool analysis::squareWave(float maxResult,float divisions, bool showGraph){
    
    bool myBoolean;
    float threshold=maxNumberOfFramesForSynthesis/(divisions+1);
    ofFill();
    
    if (limiter < maxNumberOfFramesForSynthesis + noOfLatencyFrames) {
        
        graphCounter++;
        limiter++;
        //reset graphCounter and flip boolean
        //maybe change this to a modulo
        
        myBoolean = on;
        
        if(graphCounter >= threshold){
            graphCounter=0;
            on=!on;
        }
        
        if(showGraph){
            int adjust=0;
            //this is just to prove it's making a square wave ;) //it has a gradient of one pixel/height hence the "adjust" variable
            int localCounter=0;
            bool localOn=false;
            ofBeginShape();
            ofNoFill();
            ofSetColor(255, 0, 0);
            float xPos=ofMap(limiter, 0, maxNumberOfFramesForSynthesis, 0, ofGetWidth());
            ofLine(xPos, ofGetHeight()-1, xPos, ofGetHeight()-150);
            for(int i=0;i<maxNumberOfFramesForSynthesis;i++){
                
                if(localOn){
                    ofVertex(ofMap(i+adjust, 0, maxNumberOfFramesForSynthesis, 0, ofGetWidth()) , ofGetHeight()-maxResult);
                }
                else{
                    ofVertex(ofMap(i+adjust, 0, maxNumberOfFramesForSynthesis, 0, ofGetWidth()) , ofGetHeight()-1);
                }
                localCounter++;
                adjust=0;
                if(localCounter>=threshold){
                    localCounter=0;
                    localOn=!localOn;
                    adjust=-1;
                }
            }  
            ofEndShape();
        }
        
    } else {
        finishedGraph=true;
    }
    
    return myBoolean;
}

//TOM 30/12/11 removed float maxTime which previously passed maxNumberOfFramesForSynthesis into graphing functions

float analysis::linear(float maxResult,  float divisions, bool showGraph){
    //showGraph = FALSE;
    //it should change direction at every peak or trough
    float threshold=maxNumberOfFramesForSynthesis/divisions;
    float adder=maxResult/threshold;
    
    level+=adder*flip;
    
    //cout<<limiter<<" <<-- limiter in linear \n";
    //cout<<maxNumberOfFramesForSynthesis<<" <<-- maxNumberOfFramesForSynthesis in linear \n";
    
    if (limiter < maxNumberOfFramesForSynthesis + noOfLatencyFrames) {
        
        //cout<<level<<" <<-- level in linear \n";
        graphCounter++;
        limiter++;
        
        if(graphCounter>=threshold){
            graphCounter=0;
            flip*=-1;
        }
        
        if(showGraph){
            ofSetColor(255, 0, 0);
            ofNoFill();
            float xPos=ofMap(limiter, 0, maxNumberOfFramesForSynthesis, 0, ofGetWidth());
            ofLine(xPos, ofGetHeight()-level+50, xPos, ofGetHeight()-level-50);
            int localFlip=1;
            int localCounter=0;
            float localLevel=0;
            
            ofBeginShape();
            
            for (int i=0; i<maxNumberOfFramesForSynthesis; i++) {
                cout<<adder<<" adder \r";
                localLevel+=adder*localFlip;
                
                ofVertex(ofMap(i, 0, maxNumberOfFramesForSynthesis, 0, ofGetWidth()), ofGetHeight()-localLevel);
                localCounter++;
                if(localCounter>=threshold){
                    localCounter=0;
                    localFlip*=-1;
                    
                }
            }
            ofEndShape();
        }
    }
    else{
        finishedGraph=true;
    }
    //cout<<level<<" <<-- level in linear \n";
    return level;
}


void analysis::loadMorse(){
    string line;
    const char* filePath=ofToDataPath("morse.txt").c_str();
    ifstream myfile (filePath);
    if (myfile.is_open())
    {
        while (myfile.good() )
        {
            getline (myfile,line);
            vector<string> twoHalves = ofSplitString(line, "\t");
            textTranslation.push_back(twoHalves[0]);
            morseCode.push_back(twoHalves[1]);
            // cout << line << endl;
        }
        myfile.close();
    }
    else{
        cout<<"can't open file \n";
    }
    for(int i=0;i<morseCode.size();i++){
        
        cout<<morseCode[i]<<" morse code is loaded\n";
    }
}

string analysis::translateToMorse(string messageToTranslate){
    string messageInMorse;
    cout<<messageToTranslate.length()<<" message length\r";
    for(int i=0;i<messageToTranslate.length();i++){
        //tedious casting from char to string
        stringstream ss;
        string s;
        char c = messageToTranslate.at(i);
        ss << c;
        ss >> s;
        //cout<<s<<" \r";
        //string thisChar=stringmessageToTranslate.at(i);
        if(s=="_"){
            cout<<"found space\r";
            //put a forward slash in to define word break as opposed to break between characters
            messageInMorse+="/";
        }
        else{
            for(int j=0;j<morseCode.size();j++){
                if(s==textTranslation[j]){
                    //add this morse character to the message
                    messageInMorse+=morseCode[j]+"*";   
                }
            }
        }     
    }
    cout<<messageInMorse<<" message in morse \r";
    
    return messageInMorse;
}


void analysis::showMorse(string message){
    
    if(counter<message.length()){
        // graphCounter++;
        
        if(pauseBetween<=0){
            //cout<<"pause bettween is "<< pauseBetween  <<" "<<message.at(counter)<< " COUNTER IS "<<counter<<" message at counter \r";
            
            //cout<<" on counter "<<onCounter<<" offCounter "<<offCounter<<"\r";
            //speed=5;
            int thresh;
            if(message.at(counter)=='.'){
                thresh=speed;
                onCounter++;
                on=true;
            }
            
            if(message.at(counter)=='-'){
                thresh=3*speed;
                onCounter++;
                on=true;
            }
            
            //char break
            if(message.at(counter)=='*'){
                thresh=2*speed;
                //cout<<"got star\r";
                offCounter++;
                on=false;
            }
            
            //word break
            if(message.at(counter)=='/'){
                thresh=6*speed;
                offCounter++;
                on=false;
                
            }
            
            if(onCounter>=thresh){
                
                onCounter=0;
                counter++;
                pauseBetween = speed;
                
            }
            if(offCounter>=thresh){
                
                offCounter=0;
                counter++;
                // pauseBetween=thresh;
            }
            //cout<<on<<" on\r";
            if(on){   
                ofSetColor(255);
            }
            else{
                ofSetColor(0);
            }
            ofRect(0, 0, ofGetWidth(), ofGetHeight());
        }
        
        //if we just finished one character pause for one beat
        else{
            //cout<<" pausing "<<pauseBetween<<"\r";
            pauseBetween--;
        }
    }
    else{
        cout<<"finished morse\r";
        morseComplete=true;
    }
}

float analysis::returnGaussian(float x, float spread, float height,   float centre, float range){
    float returnY;
    float e=2.718281828;
    float myPower=  - ( pow( (x-centre), 2) ) / (2* (spread*spread));
    returnY= - x* height * pow(e,myPower);
    return returnY/range;
}


// This should probably strobe upwards and downwards.. and give us a straightforward rate variable that can be set from a GUI - is that what howLongToShowEachFrameRateFor?
void analysis::strobe(){
    
    strobeCounter++;
    // int howLongToShowEachFrameRateFor=2*(31-currentSRate);
    
    int howLongToStrobeAtEachStrobeSpeed=1;
    
    //the total length of time each fR is shown for needs to be 
    // exactly divisible by that frame rate or it will do (for example) a frame and a half and be impossible to count
    
    if(strobeCounter > howLongToStrobeAtEachStrobeSpeed){
        //advance to next stroberate   
        
        currentSRate = intervalGenerator();
        
        //cout<<"new frame rate is "<<currentSRate<<"\r";
        
        graphCounter++;
        
        limiter++;
        
        strobeCounter=0;
    }
    
    // TODO:  TOM - could you work out the relationships here between the strobe // the # of ramp cycles // the frame numbers and the timing?
    
    //"limiter" appears to be the minimum length of each strobe square high-going pulse?
    
    if (limiter < 400) {
        
        //this speed needs calibration
        strobeToggleCounter+=1;
        
        //1/30;//1/speed
        //imagine a frame rate of 30fps
        
        if(strobeToggleCounter >= int(30-currentSRate)){
            on=!on;
            strobeToggleCounter=0;      
        }
        
        if(on){   
            ofSetColor(255);
        }
        else{
            ofSetColor(0);
        }
        
        ofRect(0, 0, ofGetWidth(), ofGetHeight());
    }
    
    else {
        
        strobeComplete=true;
    }    
}

float analysis::intervalGenerator(){
    float interval;
    float maxStrobeRate=ofGetFrameRate();  //TOM - why not make this equal to 'ofGetFrameRate()'?
    
    //cout<<ofGetFrameRate()<<" <<-- ofGetFrameRate() in Interval Generator \n";
    //it should change direction at every peak or trough
    float threshold=maxNumberOfFramesForSynthesis/2;
    
    //go up in steps which are the total distance divided by number of changes in direction
    float adder=maxStrobeRate/threshold;
    level+=adder*flip;    
    interval=level;
    intervalCounter++;
    
    if(intervalCounter >= threshold){
        intervalCounter=0;
        flip*=-1;
    }      
    return interval;
}



// TODO:  These defaults are not reflected at programme startup when the GUIs launch - i.e.:  These don't 'set' the GUI values at the beginning but should

void analysis::setGUIDefaults (){    
    // this is called in the main programme setup - in testApp.cpp - and should contain all the default values for the GUI sliders
    
    //Tom S - completed 14 nov 19:06;03 
    scanLineWidth = 200;
    scanLineSpeed = 2;
    
    //how many frames to take to do it
    maxNumberOfFramesForSynthesis = 500;
    //for iResponse
    // maxNumberOfFramesForSynthesis = 250;
    // float numFramesToGrab= (
    fadeTime = 20;
    everyNthFrameCaptureImage = 20; //the lower this number the more camera images we get per white level shown
    numberOfGreyLevels = 10;   //the number of grey levels we want to look at
    currentGreyLevel = numberOfGreyLevels; 
    numOfFramesToGrab = ( numberOfGreyLevels * maxNumberOfFramesForSynthesis ) / everyNthFrameCaptureImage;
    
    //max white value to go to
    maxResultA = 255;
    
    //for shape shading. how many frames to grab for each quadrant
    framesPerQuadrant = 100;
    
    //this counter limiting var has been left as seperate from maxNumberOfFramesForSynthesis because this analysis needs to be a lot longer than the others
    
    counterMaxColorMulti = 1500;  //the 'slowness' of the increments of color - the bigger this is, the longer this analysis process takes
    
    //how many times to change direction up or down - 2 divisions is one complete cycle
    //TODO -  why not call it 'numOfHalfCycles' and similarly name it in the GUI
    divisionsA = 2;
    showGraphA = false;
    
    //morse flash rate
    speed=3;
    
    red=0;
    green=0;
    blue=0;
    
    whichGraph="EXPONENTIAL";
    morseMessage="A";  //TO DO:  this doesn't seem to be working - not matter what the message played is the same length
    
}


//CODEC LISTING 
/*
 k32AlphaGrayCodecType
 k422YpCbCr10CodecType
 k422YpCbCr16CodecType
 k422YpCbCr8CodecType
 k4444YpCbCrA8CodecType
 k4444YpCbCrA8RCodecType
 k444YpCbCr10CodecType
 k444YpCbCr8CodecType
 k48RGBCodecType
 k64ARGBCodecType
 kAnimationCodecType
 kBaseCodecType
 kCinepakCodecType
 kCloudCodecType
 kCMYKCodecType
 kComponentVideoCodecType
 kComponentVideoSigned
 kComponentVideoUnsigned
 kDVCNTSCCodecType
 kDVCPALCodecType
 kDVCProNTSCCodecType
 kDVCProPALCodecType
 kFireCodecType
 kFLCCodecType
 kGIFCodecType
 kGraphicsCodecType
 kH261CodecType
 kH263CodecType
 kIndeo4CodecType
 kJPEGCodecType
 kMacPaintCodecType
 kMicrosoftVideo1CodecType
 kMotionJPEGACodecType
 kMotionJPEGBCodecType
 kMpegYUV420CodecType
 kPhotoCDCodecType
 kPlanarRGBCodecType
 kPNGCodecType
 kQuickDrawGXCodecType
 kRawCodecType
 kSGICodecType
 kSorenson3CodecType
 kSorensonCodecType
 kSorensonYUV9CodecType
 kTargaCodecType
 kTIFFCodecType
 kVideoCodecType
 kWaterRippleCodecType
 kYUV420CodecType
 
 */

