#pragma once

#include "ofMain.h"
#include "ofxCv.h"

#include "ofxFaceTracker.h"
#include "ofxImageSequenceRecorder.h"
#include "ofxImageSequence.h"
#ifdef __arm__
  #include "ofxCvPiCam.h"
#endif

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	
#ifdef __arm__
  ofxCvPiCam cam;
#else
  ofVideoGrabber cam;
#endif
	ofxFaceTracker tracker;
	
	cv::Mat grayFloat;

	ofVec2f position;
	float scale;
	ofMatrix4x4 rotationMatrix;
	
  ofxImageSequenceRecorder recorder;    
	ofxImageSequence sequence;


  ofxCv::Mat frame, frameR;
  float camWidth, camHeight;
  ofRectangle roi;
  ofPixels pixels;

  bool isDebug;
};
