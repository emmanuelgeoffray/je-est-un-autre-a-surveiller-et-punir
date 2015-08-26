#include "testApp.h"


using namespace ofxCv;


void testApp::setup() {
  ofHideCursor();
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
  camWidth = 1024*2;
  camHeight = 768*2;
	cam.setup(camWidth, camHeight, true);
	float trackerWidth = 640*0.2;
	tracker.setup();
  tracker.setRescale(trackerWidth/camWidth);

  // player
	//sequence.enableThreadedLoad(true);
	//sequence.loadSequence("frame", "jpg", 1, 11, 8);
	sequence.loadSequence("frames");
  sequence.preloadAllFrames();
	sequence.setFrameRate(5); 

  // recorder
  string folder = "frames";
  string prefix = "frame";
  string ext = "jpg";
  
  recorder.setPrefix(folder+"/"+prefix); // this directory must already exist
  recorder.setFormat(ext); // png is really slow but high res, bmp is fast but big, jpg is just right
  recorder.setNumberWidth(8);
  
  // set initial number to avoid overwrite of previous sessions
  ofDirectory dir;
	dir.allowExt(ext);
	int numFiles = dir.listDir(folder);
  recorder.setCounter(numFiles);

  recorder.startThread(false, true);   


  isDebug = false;
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
      if (roi.getWidth() > 0 && roi.getWidth() < frame.cols && roi.getHeight() > 0 && roi.getHeight() < frame.rows){
        frame(toCv(roi)).copyTo(frameR);
      }

      // record
      toOf(frameR, pixels);
      recorder.addFrame(pixels);
      //sequence.loadSequence("frames");
      sequence.push_back(pixels);
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
    if (isDebug && !frame.empty()){
      drawMat(frame,0,0);
    }
    else if(sequence.isLoading()){
      ofBackground(255,0,0);
    }
    else{
      ofBackground(0);
      ofTexture img = sequence.getTextureForTime(ofGetElapsedTimef());
      //ofLog() << "tex w " << img.getWidth();
      
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



}

void testApp::keyPressed(int key) {
	if(key == 'r') {
		tracker.reset();
	}
	else if(key == 'd') {
    isDebug = !isDebug;
	}
}
