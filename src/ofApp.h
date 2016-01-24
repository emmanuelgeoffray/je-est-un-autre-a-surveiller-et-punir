#pragma once

#include "ofMain.h"
#include "ofxCv.h"

#include "ofxFaceTracker.h"
#include "ofxImageSequenceRecorder.h"
#include "ofxImageSequence.h"
#ifdef __arm__
  #include "ofxCvPiCam.h"
#endif

using namespace cv;
using namespace ofxCv;

class ofxImageSequenceCustom : public ofxImageSequence {
  public:
  void push_back(ofPixels& pixels){
    sequence.push_back(pixels);
		loadFailed.push_back(false);
    loaded = true;
  }
};

class ofApp : public ofBaseApp {
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
	
	Mat grayFloat;

	ofVec2f position;
	float scale;
	ofMatrix4x4 rotationMatrix;
	
  ofxImageSequenceRecorder recorder;    
	ofxImageSequenceCustom sequence;


  Mat frame, frameR;
  float camWidth, camHeight;
  ofRectangle roi;
  ofPixels pixels;
  ofTexture cameraDebug;

  bool isDebug;
};
