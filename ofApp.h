#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "ofxOpenCv.h"
#include <vector>


struct RoiData {
	ofRectangle* rect;
	string name;
	int key;
	bool triggered;
	bool CONTINOUS_MODE;
	RoiData(string name, int x, int y, int width, int height,
		int key, bool CONTINOUS_MODE);
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void getBackground();
		void setHistogramm(ofxCvGrayscaleImage im);
		int getThresholdIsodata();
		void checkROI();
		void handleROI(RoiData* r, bool recognized);

		bool backgroundset;
		int threshold;
		bool starten;
		int zaehler;
		int imgHistogram[256];
		INPUT keyboardIn;
		ofVideoGrabber camera;
		ofxCvColorImage colorImage;
		ofxCvGrayscaleImage grayImage;
		ofxCvGrayscaleImage background;
		ofxCvGrayscaleImage binImage;
		ofTexture videoTexture;
		vector<RoiData*> vecROI;
		RoiData* leftHandROI;
		RoiData* rightHandROI;
		RoiData* topROI;

		//HIER Parameter einstellen
		const int camWidth = 320;
		const int camHeight = 240;

		//CONTINOUS_MODE bestimmt, ob beim Ausloessen die Taste gehalten werden soll
		const bool CONTINOUS_MODE_RIGHT = true;
		const bool CONTINOUS_MODE_LEFT = true;
		const bool CONTINOUS_MODE_TOP = true;

		//scaleROItrigger bestimmt die Sensitivitaet der Ausloesung der ROI
		//Je groesser desto eher wird etwas in einem ROI erkannt
		const int scaleROItrigger = 4;

		//Adaptive Thresholding bestimmt dynamisch den geeigneten Threshold,
		//ist allerdings rechenaufwendig
		const bool ADAPTIVE_THRESHOLDING = false;

		//Zuweisen der ROIs zu den Tasten:
		//Alle Belegungen unter:
		//https://docs.microsoft.com/en-us/windows/desktop/inputdev/virtual-key-codes
		const int buttonLeftHandROI = 0x57;
		const int buttonRightHandROI = 0x45;
		const int buttonTopROI = 0x26;
};
