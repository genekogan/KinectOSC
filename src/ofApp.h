#pragma once

#include "ofMain.h"
#include "ofxOpenNI.h"
#include "ofxOsc.h"
#include "ofxControl.h"


#define OSC_DESTINATION_DEFAULT "localhost"
#define OSC_ADDRESS_ROOT_DEFAULT "/kinect"
#define OSC_PORT_DEFAULT 6448

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

    ofxOpenNI kinect;
    bool hadUsers;
    
    ofxOscSender osc;
    string oscDestination;
    string oscAddressRoot;
    int oscPort;
    string oscMessageString;
    string realWorldAddress, screenAddress;

    bool sendReal, sendScreen;
    ofxControlWidget gui;
};
