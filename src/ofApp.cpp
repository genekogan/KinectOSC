#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //ofSetDataPathRoot("../Resources/data/");
    ofSetWindowShape(640, 480);

    // default settings
    sendReal = false;
    sendScreen = true;
    oscDestination = OSC_DESTINATION_DEFAULT;
    oscAddressRoot = OSC_ADDRESS_ROOT_DEFAULT;
    oscPort = OSC_PORT_DEFAULT;

    // load settings from file
    ofXml xml;
    xml.load("settings.xml");
    xml.setTo("KinectOSC");
    oscDestination = xml.getValue("ip");
    oscPort = ofToInt(xml.getValue("port"));
    oscAddressRoot = xml.getValue("address");

    // setup kinect
    kinect.setup();
    kinect.addDepthGenerator();
    kinect.addUserGenerator();
    kinect.setMaxNumUsers(1);
    kinect.start();
    
    // addresses + setup osc
    realWorldAddress = oscAddressRoot+"/realworld";
    screenAddress = oscAddressRoot+"/screen";
    osc.setup(oscDestination, oscPort);
    oscMessageString = "Sending OSC to "+oscDestination+", port "+ofToString(oscPort);
    
    // setup gui
    gui.setName("KinectOSC");
    gui.addToggle(realWorldAddress, &sendReal);
    gui.addToggle(screenAddress, &sendScreen);
}

//--------------------------------------------------------------
void ofApp::update(){
    kinect.update();
    if (!kinect.isNewFrame())   return;
    
    // trick to reset user generator if no tracked users found
    if (kinect.getNumTrackedUsers() > 0) {
        hadUsers = true;
    }
    else if (kinect.getNumTrackedUsers() == 0 && hadUsers){
        hadUsers = false;
        kinect.setPaused(true);
        kinect.removeUserGenerator();
        kinect.addUserGenerator();
        kinect.setPaused(false);
    }
    
    if (kinect.getNumTrackedUsers() == 0)   return;
    
    // send screen coordinates
    if (sendScreen) {
        ofxOscMessage msg;
        msg.setAddress(screenAddress);
        
        ofxOpenNIUser & user = kinect.getTrackedUser(0);
        for (int j=0; j<user.getNumJoints(); j++) {
            ofxOpenNIJoint & joint = user.getJoint((Joint) j);
            ofPoint p = joint.getProjectivePosition();
            msg.addFloatArg(p.x);
            msg.addFloatArg(p.y);
        }
        osc.sendMessage(msg);
    }
    
    // send real world coordinates
    if (sendReal) {
        ofxOscMessage msg;
        msg.setAddress(realWorldAddress);
        
        ofxOpenNIUser & user = kinect.getTrackedUser(0);
        for (int j=0; j<user.getNumJoints(); j++) {
            ofxOpenNIJoint & joint = user.getJoint((Joint) j);
            ofPoint w = joint.getWorldPosition();
            msg.addFloatArg(w.x);
            msg.addFloatArg(w.y);
            msg.addFloatArg(w.z);
        }
        osc.sendMessage(msg);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    
    // draw skeleton
    ofSetColor(255);
    if (kinect.getNumTrackedUsers() > 0) {
        kinect.drawSkeleton(0);
    }
    ofDrawBitmapString(oscMessageString, 15, ofGetHeight() - 4);
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
