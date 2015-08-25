#include "testApp.h"


using namespace ofxCv;


void testApp::setup() {
	ofSetFrameRate(60);
  ofSetFullscreen(true);
	ofSetVerticalSync(true);
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
  camWidth = 2592;
  camHeight = 1944;
  camWidth = 1920;
  camHeight = 1080;
  camWidth = 1296;
  camHeight = 730;
  camWidth = 640;
  camHeight = 480;
	cam.setup(camWidth, camHeight, true);
	trackerWidth = 640*0.2;
	tracker.setup();
  tracker.setRescale(trackerWidth/camWidth);

  recorder.setPrefix("frame"); // this directory must already exist
  recorder.setFormat("jpg"); // png is really slow but high res, bmp is fast but big, jpg is just right
  recorder.setNumberWidth(8); // png is really slow but high res, bmp is fast but big, jpg is just right
  recorder.startThread(false, true);   

}

void testApp::update() {
  frame = cam.grab();
  if(!frame.empty()) {
		tracker.update(toCv(frame));
		position = tracker.getPosition();
		scale = tracker.getScale();
		rotationMatrix = tracker.getRotationMatrix();
		
		if(tracker.getFound()) {
      roi = tracker.getImageFeature(ofxFaceTracker::LEFT_EYE).getBoundingBox();
      roi.scaleFromCenter(2);
      // TODO: avoid copy
      frame(toCv(roi)).copyTo(frameR);

      // record
      toOf(frameR, pixels);
      recorder.addFrame(pixels);
		}
	}
}

void testApp::draw() {
  ofSetColor(255);

  if(tracker.getFound() && !frame.empty()){
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    scale = ofGetWidth()/roi.getWidth();
    ofScale(scale, scale, 1);
    ofTranslate(-roi.getWidth()/2, -roi.getHeight()/2);
    drawMat(frameR,0,0);
    ofPopMatrix();
  }
  else {
    drawMat(frame,0,0);
    tracker.draw();
  }



}

void testApp::keyPressed(int key) {
	if(key == 'r') {
		tracker.reset();
	}
}
