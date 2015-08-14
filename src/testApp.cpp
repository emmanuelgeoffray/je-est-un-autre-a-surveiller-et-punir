#include "testApp.h"


using namespace ofxCv;

void testApp::setup() {
	ofSetFrameRate(60);
  ofSetFullscreen(true);
	ofSetVerticalSync(true);
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
  cam.setDeviceID(1);
	cam.initGrabber(640, 480);
	
	tracker.setup();
}

void testApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		tracker.update(toCv(cam));
		position = tracker.getPosition();
		scale = tracker.getScale();
		rotationMatrix = tracker.getRotationMatrix();
		
		if(tracker.getFound()) {
		}
	}
}

void testApp::draw() {
  ofSetColor(255);

  if(tracker.getFound()) {
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    float leftEyeWidth = tracker.getImageFeature(ofxFaceTracker::LEFT_EYE).getBoundingBox().getWidth();
    //ofLog() << leftEyeWidth;
    scale = cam.getWidth()/leftEyeWidth;
    ofScale(scale, scale, 1);
    applyMatrix(rotationMatrix.getInverse());
    ofTranslate(-tracker.getImageFeature(ofxFaceTracker::LEFT_EYE).getCentroid2D());
    cam.draw(0, 0);
    //tracker.draw();
    ofPopMatrix();
  }
  else {
    cam.draw(0, 0);
    tracker.draw();
  }



}

void testApp::keyPressed(int key) {
	if(key == 'r') {
		tracker.reset();
	}
}
