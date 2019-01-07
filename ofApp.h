#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "ofxOpenCv.h"
#include <vector>

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
		ofxCvColorImage getBackground();
		void setHistogramm(ofxCvGrayscaleImage im);
		int getThresholdIsodata();
		ofRectangle* checkROI();

		ofVideoGrabber camera;
		ofxCvColorImage colorImage;
		ofxCvGrayscaleImage grayImage;
		ofxCvGrayscaleImage background;
		ofxCvGrayscaleImage binImage;
		ofTexture videoTexture;

		int camWidth;
		int camHeight;
		int adaptiveThreshold;
		bool starten;
		int zaehler;
		int imgHistogram[256];
		vector<ofRectangle *> rectVec;
		ofRectangle* rightROI;
		ofRectangle* leftROI;
};
