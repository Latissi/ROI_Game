#include "ofApp.h"

using namespace cv;

//--------------------------------------------------------------
void ofApp::setup(){
	camWidth = 320;
	camHeight = 240;
	camera.setDeviceID(0);
	camera.setDesiredFrameRate(30);
	zaehler = 0;
	adaptiveThreshold = 30;
	camera.initGrabber(camWidth, camHeight);
	colorImage.allocate(camWidth, camHeight);
	background.allocate(camWidth, camHeight);
	grayDiff.allocate(camWidth, camHeight);

}

//--------------------------------------------------------------
void ofApp::update(){

	//---------------- Webcam grabben--------------------//
	camera.update();
	if (camera.isFrameNew()) {
		colorImage.setFromPixels(camera.getPixels());	                // Pixel von der Webcam in das ColorImage speichern
		colorImage.mirror(0, 1);										// camera horizontal spiegeln
		grayImage = colorImage;

		//---------------- Hintergrundsubstraktion------------//
		if (starten == true)
		{
			background = grayImage;				// Aktuelle grayImage wird als Hintergrund in grayBackground gespeichert
			//adaptiveThreshold = getThresholdIsodata(getHistogramm(grayImage));
			starten = false;
		}
		grayDiff.absDiff(background, grayImage);	                // Hintergrundsubtraktion Pixel - Pixel
		if (zaehler % 20 == 0) {
			adaptiveThreshold = getThresholdIsodata(getHistogramm(grayImage));
			cout << "Threshold: " << adaptiveThreshold << endl;
		}

		grayDiff.threshold(adaptiveThreshold);						// Schwelle für das Schwarz/Weiß Bild


	}

	
	//camera.update();

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetWindowTitle("WEBCAM");
	
	grayDiff.draw(0, 0, 640, 480);
	colorImage.draw(0, 480, 640, 480);
	background.draw(640, 0, 640, 480);
	//camera.draw(640, 480, 640*2, 480*2);
	//videoTexture.draw(20 + camWidth, 20, camWidth, camHeight);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == OF_KEY_RETURN)
		starten = true;

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

ofxCvColorImage ofApp::getBackground() {
	camera.update();
	colorImage.setFromPixels(camera.getPixels());
	colorImage.mirror(0, 1);
	return colorImage; 
}

int* ofApp::getHistogramm(ofxCvGrayscaleImage im)
{
	Mat matImg = im.getCvImage();
	int imgHistogram[256];
	for(int i = 0; i<76800; i+=4){
		//cout << "p: " << (int)(im.getPixels()[i])<< endl;
		++imgHistogram[(int)(im.getPixels()[i])];
	}

	return imgHistogram;
}

int ofApp::getThresholdIsodata(int* imageHistogram) {
	int theThreshold = 127; // our output

	//if (input != NULL) { // sanity check
	int thresh = theThreshold;
	int tnew = thresh;
	int thr = 0;
	int sum = 0;
	int mean1, mean2;
	int ntries = 0;

	do {
		thr = tnew;
		sum = mean1 = mean2 = 0;

		for (int i = 0; i < thr; i++) {
			mean1 += (imageHistogram[i] * i);
			sum += (imageHistogram[i]);
		}
		if (sum != 0) { mean1 = mean1 / sum; }

		sum = 0;
		for (int i = thr; i < 255; i++) {
			mean2 += (imageHistogram[i] * i);
			sum += (imageHistogram[i]);
		}

		if (sum != 0) { mean2 = mean2 / sum; }
		tnew = (mean1 + mean2) / 2;
		ntries++;

	} while ((tnew != thr) && (ntries < 64));
	theThreshold = tnew;
	//}

	return (theThreshold<30) ? 30 : theThreshold;
}


