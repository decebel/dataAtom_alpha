#include "stroke.h"


void stroke::clear(){
	bDone = false;
	originalLine.clear();
	animateTo.clear();
	animated.clear();
	bFlip = false;
}

void stroke::done(){


	if (originalLine.getVertices().size() > 1){
		ofPoint ptA = originalLine.getVertices()[0];
		ofPoint ptB = originalLine.getVertices()[originalLine.getVertices().size()-1];



		ofPoint ptC;
		for (int i = 0; i < originalLine.getVertices().size(); i++){
			ptC += originalLine.getVertices()[i];
		}



		ptC /= (float)originalLine.getVertices().size();

		//d1 = ptC;


		float len = originalLine.getPerimeter();

		//n1 = (ptB - ptA).normalize() * len/2;

		newPta = ptC - (ptB - ptA).normalize() * (len/2);
		newPtb = ptC + (ptB - ptA).normalize() * (len/2);
		float distance = (newPta - newPtb).length();

		for (int i = 0; i < originalLine.getVertices().size(); i++){

			ofPoint pt = newPta + ((newPtb - newPta)/ originalLine.getVertices().size()) * i;
			animateTo.addVertex(pt.x, pt.y);
		}

		animated = originalLine;

		animatedTarget = animateTo;

		bDone = true;

		bFlip = false;
	}

}

void stroke::update(){

	if (bDone == true){
		for (int i = 0; i < animated.getVertices().size(); i++){
			animated.getVertices()[i] = 0.99f *  animated.getVertices()[i]  + 
				0.01f * animatedTarget.getVertices()[i];

		}

		float diff = 0;
		for (int i = 0; i < animated.getVertices().size(); i++){
			diff += (animated.getVertices()[i] - animatedTarget.getVertices()[i]).length();
		}
		//cout << diff << endl;
		if (diff < 2){
			if (bFlip == false){
				animatedTarget = originalLine;
			} else {
				animatedTarget = animateTo;
			}
			bFlip = !bFlip;
		}
	}
}

void stroke::draw(){
	if (bDone == true){
		animated.draw();
	} else {
		originalLine.draw();

	}
}
