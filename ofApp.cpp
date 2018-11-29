#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	camWidth = 640;
	camHeight = 480;
	camera.setDeviceID(0);
	camera.setDesiredFrameRate(60);
	camera.initGrabber(camWidth, camHeight);

}

//--------------------------------------------------------------
void ofApp::update(){
	camera.update();

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetWindowTitle("WEBCAM");
	camera.draw(0, 0, 640*2, 480*2);
	videoTexture.draw(20 + camWidth, 20, camWidth, camHeight);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
