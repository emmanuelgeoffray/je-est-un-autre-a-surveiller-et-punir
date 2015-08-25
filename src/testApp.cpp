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
	cam.initGrabber(640, 480);
	trackerWidth = 640*0.2;
	tracker.setup();
  tracker.setRescale(trackerWidth/camWidth);

  // recorder
  recorder.setPrefix("frame"); // this directory must already exist
  recorder.setFormat("jpg"); // png is really slow but high res, bmp is fast but big, jpg is just right
  recorder.setNumberWidth(8); // png is really slow but high res, bmp is fast but big, jpg is just right
  recorder.startThread(false, true);   

  // player
	sequence.enableThreadedLoad(true);
	//sequence.loadSequence("frame", "jpg", 1, 11, 8);
	sequence.loadSequence(".");
	sequence.setFrameRate(10); 

}

void testApp::update() {

#ifdef __arm__
  frame = cam.grab();
  if(!frame.empty()) {
#else
	cam.update();
	if(cam.isFrameNew()) {
    frame = toCv(cam);
#endif
		tracker.update(toCv(frame));
		position = tracker.getPosition();
		scale = tracker.getScale();
		rotationMatrix = tracker.getRotationMatrix();
		
		if(tracker.getFound()) {
      roi = tracker.getImageFeature(ofxFaceTracker::LEFT_EYE).getBoundingBox();
      roi.scaleFromCenter(3);
      roi.scaleFromCenter(16/9.*roi.getHeight()/roi.getWidth(), 1);
      // TODO: avoid copy
      frame(toCv(roi)).copyTo(frameR);

      // record
      toOf(frameR, pixels);
      recorder.addFrame(pixels);
		}
	}
}

void testApp::draw() {
		ofBackground(0);
  ofSetColor(255);

  if(tracker.getFound() && !frame.empty()){
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    scale = ofGetWidth()/roi.getWidth()*1.2;
    ofScale(scale, scale, 1);
    ofTranslate(-roi.getWidth()/2, -roi.getHeight()/2);
    drawMat(frameR,0,0);
    ofPopMatrix();
  }
  else {
    ofTexture img = sequence.getTextureForTime(ofGetElapsedTimef());
    ofLog() << "tex w " << img.getWidth();
    
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    scale = ofGetWidth()/img.getWidth()*1.2;
    ofScale(scale, scale, 1);
    ofTranslate(-img.getWidth()/2, -img.getHeight()/2);
    img.draw(0,0);
    ofPopMatrix();
    
    //img.draw(0,0);
  }



}

void testApp::keyPressed(int key) {
	if(key == 'r') {
		tracker.reset();
	}
}
