#include "demoParticle.h"

ofParameter<float> demoParticle::scaleMod{ "Size", 10, 0, 20 };

//------------------------------------------------------------------
demoParticle::demoParticle() {
	attractPoints = NULL;
}

//------------------------------------------------------------------
void demoParticle::setMode(particleMode newMode) {
	mode = newMode;

	if (mode == PARTICLE_MODE_ATTRACT) {
		particleColor = ofColor(255, 63, 180);
		spawnVelocityMod = 4.0f;
		spawnScaleMod = 0.5f;
	}
	else if (mode == PARTICLE_MODE_REPEL) {
		particleColor = ofColor(208, 255, 63);
		spawnVelocityMod = 4.0f;
		spawnScaleMod = 0.5f;
	}
	else if (mode == PARTICLE_MODE_NOISE) {
		particleColor = ofColor(99, 63, 255);
		spawnVelocityMod = 4.0f;
		spawnScaleMod = 0.5f;
	}
	else if (mode == PARTICLE_MODE_NEAREST_POINTS) {
		particleColor = ofColor(103, 160, 237);
		spawnVelocityMod = 4.0f;
		spawnScaleMod = 0.5f;
	}
	else if (mode == PARICLE_MODE_FLOW_1) {
		particleColor = ofColor(103, 160, 237, 75);
		spawnVelocityMod = 0.05f;
		spawnScaleMod = 1.5f;
	}
}

//------------------------------------------------------------------
void demoParticle::setAttractPoints(vector <glm::vec3>* attract) {
	attractPoints = attract;
}

//------------------------------------------------------------------
void demoParticle::ColorExplosion(ofColor color, glm::vec2 source) {
	float dist = glm::length(source - pos);

	if (dist < 80) {
		particleColor = color;
	}

}

//------------------------------------------------------------------
void demoParticle::Impulse(glm::vec2 direction, glm::vec2 source) {
	float dist = glm::length(source - pos);
	direction = glm::normalize(direction);
	float impulseRadius = 100;

	if (dist < impulseRadius) {
		float distPerc = 1 - (dist / impulseRadius);
		vel += direction * 3 * distPerc + (ofRandom(-1, 1));
	}

}

//------------------------------------------------------------------
void demoParticle::reset() {
	//the unique val allows us to set properties slightly differently for each particle
	uniqueVal = ofRandom(-10000, 10000);

	pos.x = ofRandomWidth();
	pos.y = ofRandomHeight();
	/*pos.z = 0;*/

	vel.x = ofRandom(-1, 1) * spawnVelocityMod;
	vel.y = ofRandom(-1, 1) * spawnVelocityMod;
	/*vel.z = 0;*/

	frc = glm::vec3(0, 0, 0);

	scale = ofRandom(1, 2) * spawnScaleMod;

	if (mode == PARTICLE_MODE_NOISE) {
		drag = ofRandom(0.97, 0.99);
		vel.y = fabs(vel.y) * 3.0; //make the particles all be going down
	}
	else {
		drag = ofRandom(0.95, 0.998);
	}
}

//------------------------------------------------------------------
void demoParticle::update() {

	//1 - APPLY THE FORCES BASED ON WHICH MODE WE ARE IN 

	if (mode == PARTICLE_MODE_ATTRACT) {
		glm::vec2 attractPt(ofGetMouseX(), ofGetMouseY());
		frc = attractPt - pos; // we get the attraction force/vector by looking at the mouse pos relative to our pos
		frc = glm::normalize(frc); //by normalizing we disregard how close the particle is to the attraction point

		vel *= drag; //apply drag
		vel += frc * 0.6; //apply force
	}
	else if (mode == PARTICLE_MODE_REPEL) {
		glm::vec2 attractPt(ofGetMouseX(), ofGetMouseY());
		frc = attractPt - pos;

		//let get the distance and only repel points close to the mouse
		float dist = glm::length(frc);
		frc = glm::normalize(frc);

		vel *= drag;
		if (dist < 150) {
			vel += -frc * 0.6; //notice the frc is negative 
		}
		else {
			//if the particles are not close to us, lets add a little bit of random movement using noise. this is where uniqueVal comes in handy. 			
			frc.x = ofSignedNoise(uniqueVal, pos.y * 0.01, ofGetElapsedTimef() * 0.2);
			frc.y = ofSignedNoise(uniqueVal, pos.x * 0.01, ofGetElapsedTimef() * 0.2);
			vel += frc * 0.04;
		}
	}
	else if (mode == PARTICLE_MODE_NOISE) {
		//lets simulate falling snow 
		//the fake wind is meant to add a shift to the particles based on where in x they are
		//we add pos.y as an arg so to prevent obvious vertical banding around x values - try removing the pos.y * 0.006 to see the banding
		float fakeWindX = ofSignedNoise(pos.x * 0.003, pos.y * 0.006, ofGetElapsedTimef() * 0.6);

		frc.x = fakeWindX * 0.25 + ofSignedNoise(uniqueVal, pos.y * 0.04) * 0.6;
		frc.y = ofSignedNoise(uniqueVal, pos.x * 0.006, ofGetElapsedTimef() * 0.2) * 0.09 + 0.18;

		vel *= drag;
		vel += frc * 0.4;

		//we do this so as to skip the bounds check for the bottom and make the particles go back to the top of the screen
		if (pos.y + vel.y > ofGetHeight()) {
			pos.y -= ofGetHeight();
		}
	}
	else if (mode == PARTICLE_MODE_NEAREST_POINTS) {

		if (attractPoints) {

			//1 - find closest attractPoint 
			glm::vec3 closestPt;
			int closest = -1;
			float closestDist = 9999999;

			for (unsigned int i = 0; i < attractPoints->size(); i++) {
				float lenSq = glm::length2(attractPoints->at(i) - pos);
				if (lenSq < closestDist) {
					closestDist = lenSq;
					closest = i;
				}
			}

			//2 - if we have a closest point - lets calcuate the force towards it
			if (closest != -1) {
				closestPt = attractPoints->at(closest);
				float dist = sqrt(closestDist);

				//in this case we don't normalize as we want to have the force proportional to distance 
				frc = closestPt - pos;

				vel *= drag;

				//lets also limit our attraction to a certain distance and don't apply if 'f' key is pressed
				if (dist < 300 && dist > 40 && !ofGetKeyPressed('f')) {
					vel += frc * 0.003;
				}
				else {
					//if the particles are not close to us, lets add a little bit of random movement using noise. this is where uniqueVal comes in handy. 			
					frc.x = ofSignedNoise(uniqueVal, pos.y * 0.01, ofGetElapsedTimef() * 0.2);
					frc.y = ofSignedNoise(uniqueVal, pos.x * 0.01, ofGetElapsedTimef() * 0.2);
					vel += frc * 0.4;
				}

			}

		}

	} 
	else if(mode == PARICLE_MODE_FLOW_1) {
		
	}


	//2 - UPDATE OUR POSITION

	pos += vel;


	//3 - (optional) LIMIT THE PARTICLES TO STAY ON SCREEN 
	//we could also pass in bounds to check - or alternatively do this at the ofApp level
	if (pos.x > ofGetWidth()) {
		pos.x = ofGetWidth();
		vel.x *= -1.0;
	}
	else if (pos.x < 0) {
		pos.x = 0;
		vel.x *= -1.0;
	}
	if (pos.y > ofGetHeight()) {
		pos.y = ofGetHeight();
		vel.y *= -1.0;
	}
	else if (pos.y < 0) {
		pos.y = 0;
		vel.y *= -1.0;
	}

}

//------------------------------------------------------------------
void demoParticle::draw() {
	ofSetColor(particleColor);
	ofDrawCircle(pos.x, pos.y, scale + demoParticle::scaleMod);
}

