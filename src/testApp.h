#pragma once

#include "ofMain.h"
#include "ofxCv.h"

#include "ofxFaceTracker.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	
	ofVideoGrabber cam;
	ofxFaceTracker tracker;
	
	cv::Mat grayFloat;

	ofVec2f position;
	float scale;
	ofMatrix4x4 rotationMatrix;
	
};
