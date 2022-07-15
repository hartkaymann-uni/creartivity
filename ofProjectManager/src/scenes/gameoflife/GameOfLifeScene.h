#pragma once

#include "ccScene.h"

#include <map>

#include "ofxUbo.h"
#include "utils/ccPingPong.h"

namespace gol {

	class GameOfLifeScene : public ccScene {

	public:

		GameOfLifeScene(int cells_x = 102, int cells_y = 77);
		void setup();
		void update();
		void draw();
		void reset();

		void keyPressed(int key);
		void windowResized(int w, int h);

		void handleSphereResolutionChanged(int& sphereRes);
		void handleDimensionsChanged(ofVec2f& value);

	private:
		int n_cells_x;
		int n_cells_y;
		float cellOffset;

		float time;

		void step();
		void addInteraction(glm::vec2 point);

		enum ShadingType {
			OUTLINE,
			METABALL
		};
		ShadingType shading;
		void changeShading();

		ofShader logicShader;
		ofShader splatShader;
		ofShader instancedShader;
		ofShader outlineShader;
		ofShader metaballShader;

		PingPong cellPingPong;

		ofVboMesh vboSphere;

		ofParameterGroup shaderUniforms;
		ofParameter<ofVec2f> dimensions;
		ofParameter<int> sphereResolution;
		ofParameter<float> sphereRadius;
		ofParameter<float> evolutionFactor;
		ofParameter<float> dataSrcSize;
		ofParameter<float> mouseRadius;
		ofParameter<float> mouseStrength;
		ofParameter<float> jiggleFactor;
		ofParameter<bool> runSequences;

		float calculateSphereRadius(ofVec2f dim);
		void allocateCellBuffer(int rows, int cols);

		void drawOutlined(ofVboMesh& mesh, ofShader& instance, ofShader& outline);

		void drawMetaballs(ofShader& metaballs);


		// Members for sequences
		enum class SequenceName {
			Default,
			NoJiggle,
			BigCells,
			SmallCells,
			FastEvolution,
			SlowEvolution,
			Empty
		};
		const int NUM_SEQ = 5;

		struct SequenceParameters {
			float evolution;
			float radius;
			float jiggle;
		};

		SequenceName lastSequene;
		SequenceName currentSequence;
		float lastSequenceTime;
		float sequenceDuration;
		float sequenceTransitionDuration;
		map<SequenceName, SequenceParameters> sequenceMap;

		void initSequences();
		void updateSequence();
		void setSequence(SequenceName name);
		void updateParameters();
		SequenceName randSequence();

		float SceneIntro();
		float SceneOutro();
	};
}
