#include "testApp.h"


using namespace ofxCv;

Mat frame;
float camWidth;
void testApp::setup() {
	ofSetFrameRate(60);
  ofSetFullscreen(true);
	ofSetVerticalSync(true);
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
  camWidth = 640;
	cam.setup(camWidth, 480, true);
	
	tracker.setup();
}

void testApp::update() {
  frame = cam.grab();
  if(!frame.empty()) {
		tracker.update(toCv(frame));
		position = tracker.getPosition();
		scale = tracker.getScale();
		rotationMatrix = tracker.getRotationMatrix();
		
		if(tracker.getFound()) {
		}
	}
}

void testApp::draw() {
  ofSetColor(255);

  if(tracker.getFound() && !frame.empty()){
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    float leftEyeWidth = tracker.getImageFeature(ofxFaceTracker::LEFT_EYE).getBoundingBox().getWidth();
    //ofLog() << leftEyeWidth;
    scale = camWidth/leftEyeWidth;
    ofScale(scale, scale, 1);
    applyMatrix(rotationMatrix.getInverse());
    ofTranslate(-tracker.getImageFeature(ofxFaceTracker::LEFT_EYE).getCentroid2D());
    drawMat(frame,0,0);
    //cam.draw(0, 0);
    //tracker.draw();
    ofPopMatrix();
  }
  else {
    drawMat(frame,0,0);
    //cam.draw(0, 0);
    tracker.draw();
  }



}

void testApp::keyPressed(int key) {
	if(key == 'r') {
		tracker.reset();
	}
}
