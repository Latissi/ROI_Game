#include "ofApp.h"

using namespace cv;

//--------------------------------------------------------------
void ofApp::setup(){
	//Variablen vorinitialisieren-------------------------------
	backgroundset = false;
	zaehler = 0;
	threshold = 30;

	//Initialisierien der Kamera--------------------------------
	camera.setDeviceID(0);
	camera.setDesiredFrameRate(30);
	camera.initGrabber(camWidth, camHeight);
	camera.videoSettings();

	//Bildergroesse festlegen-----------------------------------
	colorImage.allocate(camWidth, camHeight);
	background.allocate(camWidth, camHeight);
	binImage.allocate(camWidth, camHeight);

	//Initialisieren der Regions Of Interest (ROI)--------------
	//RoiData(name, x, y, breite, hoehe, key)
	leftHandROI = 
		new RoiData("linke Hand",0, camHeight / 3,camWidth / 4,
			camHeight / 3, buttonLeftHandROI, CONTINOUS_MODE_LEFT);
	rightHandROI =
		new RoiData("rechte Hand", camWidth / 4 * 3, camHeight / 3,
			camWidth / 4, camHeight / 3, buttonRightHandROI, CONTINOUS_MODE_RIGHT);
	topROI =
		new RoiData("oben",camWidth / 4, 0, camWidth / 2, camHeight / 5,
			buttonTopROI, CONTINOUS_MODE_TOP);
	vecROI.push_back(leftHandROI);
	vecROI.push_back(rightHandROI);
	vecROI.push_back(topROI);

	//Initialisierung des virtuellen keyboards
	keyboardIn.type = INPUT_KEYBOARD;
	keyboardIn.ki.wScan = 0;
	keyboardIn.ki.time = 0;
	keyboardIn.ki.dwExtraInfo = 0;


	//INFO:
	cout << ">>INFO<<" << endl;
	cout << "Adaptive Thresholding: " << ADAPTIVE_THRESHOLDING << endl;
	cout << "ROI trigger Scaler: " << scaleROItrigger << endl;
	cout << "Tastenbelegung: " << endl;
	cout << "Taste linkes ROI: " << buttonLeftHandROI << endl;
	cout << "Taste rechtes ROI: " << buttonRightHandROI << endl;
	cout << "Taste oberes ROI: " << buttonTopROI << endl;
}

//--------------------------------------------------------------
void ofApp::update(){

	//---------------- Webcam grabben--------------------//
	camera.update();
	if (camera.isFrameNew()) {
		colorImage.setFromPixels(camera.getPixels());
		colorImage.mirror(0, 1);										
		grayImage = colorImage;

		//---------------- Hintergrundsubstraktion------------//
		binImage.absDiff(background, grayImage);	                // Hintergrundsubtraktion Pixel - Pixel

		//adaptive Thresholding
		if ((zaehler % 20 == 0) && ADAPTIVE_THRESHOLDING) {
			setHistogramm(grayImage);
			threshold = getThresholdIsodata();
		}

		//Einstellen der Schwelle fuer das Schwarz-Weiss Bild
		binImage.threshold(threshold);

		//Ueberpruefen ob Aenderung in den ROIs stattgefunden haben
		if (zaehler % 300 == 0 && backgroundset)
			checkROI();

	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetWindowTitle("WEBCAM");
	
	binImage.draw(0, 0, scaleWindow*camWidth, scaleWindow*camHeight);
	colorImage.draw(0, scaleWindow*camHeight, scaleWindow*camWidth,
		scaleWindow*camHeight);
	background.draw(scaleWindow*camWidth, 0, scaleWindow*camWidth, scaleWindow*camHeight);

	for (RoiData *roi : vecROI) {
		ofNoFill();
		ofSetColor(200, 0, 0);
		ofDrawRectangle(scaleWindow*(roi->rect->x), scaleWindow*(roi->rect->y),
			scaleWindow*(roi->rect->width), scaleWindow*(roi->rect->height));
	}
	ofSetColor(255, 255, 255);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == OF_KEY_RETURN)
		getBackground();

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

/*getBackground: 
  Funktion, die ein Bild (des Hintergrunds) macht
  und zurückgibt*/
void ofApp::getBackground() {
	background = grayImage;
	backgroundset = true;
	cout << "Set background" << endl;
}

/*setHistogramm: 
  Funktion, die ein Histogramm zu einem Bild anfertigt*/
void ofApp::setHistogramm(ofxCvGrayscaleImage im)
{
	for(int i = 0; i<76800; i+=4){
		++imgHistogram[(int)(im.getPixels()[i])];
	}
}

/*getThresholdIsodata:
  Funktion die das Histogramm auswertet und
  darauf basierend den adaptiven Schwellenwert zurückgibt*/
int ofApp::getThresholdIsodata() {
	int theThreshold = 127; // our output

	if (imgHistogram != NULL) { // sanity check
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
			mean1 += (imgHistogram[i] * i);
			sum += (imgHistogram[i]);
		}
		if (sum != 0) { mean1 = mean1 / sum; }

		sum = 0;
		for (int i = thr; i < 255; i++) {
			mean2 += (imgHistogram[i] * i);
			sum += (imgHistogram[i]);
		}

		if (sum != 0) { mean2 = mean2 / sum; }
		tnew = (mean1 + mean2) / 2;
		ntries++;

	} while ((tnew != thr) && (ntries < 64));
	theThreshold = tnew;
	}

	return (theThreshold<30) ? 30 : theThreshold;
}

/*checkROI:
  Funktion, die überprüft, ob eine Änderung in einer 
  der ROIs stattgefunden hat.
  Gibt diese Regions of interest zurück*/
void ofApp::checkROI()
{
	for (RoiData *roi : vecROI)
	{
		if (binImage.countNonZeroInRegion(roi->rect->x, roi->rect->y, roi->rect->width, roi->rect->height)
	> (roi->rect->width * roi->rect->height) / scaleROItrigger)
			handleROI(roi, true);
		else
			handleROI(roi, false);
	}
}

void ofApp::handleROI(RoiData* r, bool recognized)
{
	if (recognized && !(r->triggered)) {
		cout << "ROI " << r->name <<" getriggert"<< endl;
		r->triggered = true;
		keyboardIn.ki.wVk = r->key;
		//"0" für Taste gedrückt:
		keyboardIn.ki.dwFlags = KEYEVENTF_EXTENDEDKEY;
		SendInput(1, &keyboardIn, sizeof(INPUT));
	}
	else if (!recognized && r->triggered) {
		cout << "ROI " << r->name << " deaktiviert" << endl;
		r->triggered = false;
		keyboardIn.ki.wVk = r->key;
		//Taste "loslassen"
		keyboardIn.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &keyboardIn, sizeof(INPUT));
	}
	else if (recognized && r->triggered && r->CONTINOUS_MODE) {
		SendInput(1, &keyboardIn, sizeof(INPUT));
	}
}

RoiData::RoiData(string name, int x, int y, int width, int height,
	int key, bool CONTINOUS_MODE)
{
	this->name = name;
	this->rect = new ofRectangle(x, y, width, height);
	this->key = key;
	this->triggered = false;
	this->CONTINOUS_MODE = CONTINOUS_MODE;
}
