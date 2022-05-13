#pragma once

#include "ccScene.h"
#include "utils/ccPingPong.h"

class ContourLinesScene :public ccScene
{
public:
	ContourLinesScene(int w = 128, int h = 80);
	void setup();	
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);

private:
	float time;

	ofShader contourLineShader;
	bool wireframeShading = false;
	void changeShading();
		
	glm::vec2 grid;
	ofPlanePrimitive plane;

	ofParameterGroup terrainUniforms;
	ofParameter<float> p_Speed;
	ofParameter<float> p_Scale;
	ofParameter<float> p_Amplitude;
	ofParameter<float> p_Lacunarity;
	ofParameter<float> p_Persistance;
	ofParameter<float> p_Thickness;
	
	ofParameterGroup mouseUniforms;
	ofParameter<float> p_MouseRadius;
	ofParameter<float> p_MouseStrength;

	ofParameterGroup lightUniforms;
	ofParameter<bool> p_MoveLight;

	PingPong interaction;
	ofShader splatShader;
	ofShader subtractShader;

	void splat( glm::vec3 point );
};

