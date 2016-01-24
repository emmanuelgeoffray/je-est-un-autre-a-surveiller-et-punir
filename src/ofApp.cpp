#include "ofApp.h"


using namespace ofxCv;

bool isTracking = false;
bool isRecording = false;

void ofApp::setup() {
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
  float scaleCam = 1;
  camWidth = 640*scaleCam;
  camHeight = 480*scaleCam;
  //camWidth = 640/2;
  //camHeight = 480/2;
  //camWidth = 1024*2;
  //camHeight = 768*2;
	cam.setup(camWidth, camHeight, true);
	tracker.setup();
  tracker.setIterations(1);
  tracker.setAttempts(1);
  // TO DO: add GUI to control frame skip and get a satisfactional framerate
  // check if frameskip is working good
  tracker.setFrameSkip(120*2);
	float trackerWidth = 640*0.2;
  //tracker.setRescale(trackerWidth/camWidth);
  tracker.setRescale(0.2/scaleCam);

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

void ofApp::update() {

#ifdef __arm__
  frame = cam.grab();
  if(!frame.empty()) {
#else
  cam.update();
  if(cam.isFrameNew()) {
    frame = toCv(cam);
#endif
    if (isTracking){
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

        if (isRecording){
          // record
          toOf(frameR, pixels);
          recorder.addFrame(pixels);
          //sequence.loadSequence("frames");
          sequence.push_back(pixels);
        }
      }
    }
  }
}

    // special case for copying into ofTexture
    template <class S>
    void copy(S& src, ofTexture& tex) {
        imitate(tex, src);
        int w = tex.getWidth(), h = tex.getHeight();
        int glType = tex.getTextureData().glInternalFormat;
        Mat mat = toCv(src);
		tex.loadData(mat.ptr(), w, h, glType);
    }

void ofApp::draw() {
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
    if(sequence.isLoading()){
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

  if (isDebug){
      ofPushMatrix();
      scale = ofGetWidth()/camWidth*0.3;
      ofTranslate(scale*camWidth/2, scale*camHeight/2);
      ofScale(scale, scale, 1);
      ofTranslate(-camWidth/2, -camHeight/2);
        //ofSetColor(100);
        //ofFill();
        //ofRect(0,0,camWidth, camHeight);

        //frame = toCv(cameraDebug);
        //toOf(frame, cameraDebug);
        //cameraDebug.update();
        copy(frame,cameraDebug);
        cameraDebug.draw(0,0, camWidth,camHeight);
      //drawMat(frame,0,0);
      if (isTracking && tracker.getFound()){
        tracker.getImageFeature(ofxFaceTracker::LEFT_EYE).draw();
        ofNoFill();
        //ofDrawRectangle(roi);
        ofRect(roi);
      }
      ofPopMatrix();

  }

      ofSetColor(255);
      string msg = "fps: " + ofToString(ofGetFrameRate(), 2);
      ofDrawBitmapString(msg, 10, 20);



}

void ofApp::keyPressed(int key) {
	if(key == ' ') {
		tracker.reset();
	}
	else if(key == 'd') {
    isDebug = !isDebug;
	}
	else if(key == 't') {
    isTracking = !isTracking;
	}
	else if(key == 'r') {
    isRecording = !isRecording;
	}
}
