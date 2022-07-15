#pragma once

#include "ccScene.h"
#include "utils/ccPingPong.h"

namespace contour {


	/// <summary>
	/// Contour lines scene. Imitates stylized moving topology maps.
	/// </summary>
	class ContourLinesScene :public ccScene
	{
	public:
		/// <summary>
		/// Create a new scene. Optionally specify the resolution of the grid upon which the topology map is based on.
		/// </summary>
		/// <param name="w">Width of the grid</param>
		/// <param name="h">Height of the grid</param>
		ContourLinesScene( int w = 128, int h = 80 );
		void setup();
		void update();
		void draw();

		void keyPressed( int key );
		void keyReleased( int key );
		void windowResized( int w, int h);

	private:
		/// <summary>
		/// Real time, used to determine when new sequences should start or scene should be changed.
		/// </summary>
		float time;
		/// <summary>
		/// Time used in shaders. Impacts movement speed and rate of change of the map.
		/// </summary>
		float sceneTime;

		// Grid parameters
		glm::vec2 grid;
		ofPlanePrimitive plane;

		// Shader parameters
		ofShader contourLineShader;
		bool wireframeShading = false;
		void changeShading();

		ofParameterGroup terrainUniforms;
		ofParameter<float> p_Speed;
		ofParameter<float> p_Scale;
		ofParameter<float> p_Amplitude;
		ofParameter<float> p_Thickness;
		ofParameter<float> p_Lacunarity;
		ofParameter<float> p_Persistance;
		ofParameter<bool> p_Sequences;

		ofParameterGroup lightUniforms;
		ofParameter<bool> p_MoveLight;

		// Interaction parameters
		PingPong interaction;
		ofShader splatShader;
		ofShader subtractShader;
		void splat( glm::vec3 point );

		// Mouse strength and radius, also used for all other types of interaction
		ofParameterGroup mouseUniforms;
		ofParameter<float> p_MouseRadius;
		ofParameter<float> p_MouseStrength;

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