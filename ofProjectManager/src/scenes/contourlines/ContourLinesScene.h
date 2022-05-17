#pragma once

#include "ccScene.h"
#include "utils/ccPingPong.h"

namespace contour {

	class ContourLinesScene :public ccScene
	{
	public:
		ContourLinesScene( int w = 128, int h = 80 );
		void setup();
		void update();
		void draw();

		void keyPressed( int key );
		void keyReleased( int key );

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
		ofParameter<float> p_Thickness;
		ofParameter<float> p_Lacunarity;
		ofParameter<float> p_Persistance;
		ofParameter<bool> p_Sequences;

		ofParameterGroup mouseUniforms;
		ofParameter<float> p_MouseRadius;
		ofParameter<float> p_MouseStrength;

		ofParameterGroup lightUniforms;
		ofParameter<bool> p_MoveLight;

		PingPong interaction;
		ofShader splatShader;
		ofShader subtractShader;

		void splat( glm::vec3 point );

		// Members for sequences
		enum class SequenceName {
			Default,
			Fast,
			Big,
			HighAmplitude,
			Rough,
			Empty
		};
		const int NUM_SEQ = 5;

		struct SequenceParameters {
			float speed;
			float scale;
			float amplitude;
			float thickness;
			float lacunarity;
			float persistance;
		};

		SequenceName lastSequene;
		SequenceName currentSequence;
		float lastSequenceTime;
		float sequenceDuration;
		float sequenceTransitionDuration;
		map<SequenceName, SequenceParameters> sequenceMap;

		void initSequences();
		void updateSequence();
		void setSequence( SequenceName name );
		void updateParameters();
		SequenceName randSequence();

		float SceneIntro();
		float SceneOutro();
	};
}