#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //ofSetDataPathRoot("../Resources/data/");
    ofSetWindowShape(640, 480);

    sendReal = false;
    sendScreen = true;
    
    kinect.setup();
    //kinect.setupFromONI("/Users/gene/Downloads/hometest_single.oni");
    kinect.addDepthGenerator();
    kinect.addUserGenerator();
    kinect.setMaxNumUsers(1);
    kinect.start();
    
    oscAddressRoot = OSC_ADDRESS_ROOT_DEFAULT;
    oscPort = OSC_PORT_DEFAULT;
    osc.setup("localhost", oscPort);
    
    gui.setName("KinectOSC");
    gui.addToggle("send real-world coordinated", &sendReal);
    gui.addToggle("send screen coordinates", &sendScreen);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    
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

    // draw skeleton
    kinect.drawSkeleton(0);
    
    
    // send screen coordinates
    if (sendScreen) {
        ofxOscMessage msg;
        msg.setAddress(oscAddressRoot+"/screen");
        
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
        msg.setAddress(oscAddressRoot+"/realworld");
        
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
