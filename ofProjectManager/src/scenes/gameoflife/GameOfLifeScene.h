#pragma once

#include "ccScene.h"

#include <map>

#include "ofxUbo.h"

namespace gol {
	struct pingPongBuffer {
	public:
		void allocate( int _width, int _height, int _internalformat = GL_RGBA ) {
			// Allocate
			for (int i = 0; i < 2; i++) {
				FBOs[i].allocate( _width, _height, _internalformat );
				FBOs[i].getTexture().setTextureMinMagFilter( GL_NEAREST, GL_NEAREST );
			}

			//Assign
			src = &FBOs[0];
			dst = &FBOs[1];

			// Clean
			clear();
		}

		void swap() {
			std::swap( src, dst );
		}

		void clear() {
			for (int i = 0; i < 2; i++) {
				FBOs[i].begin();
				ofClear( 0, 255 );
				FBOs[i].end();
			}
		}

		ofFbo& operator[]( int n ) { return FBOs[n]; }
		ofFbo* src;       // Source       ->  Ping
		ofFbo* dst;       // Destination  ->  Pong

	private:
		ofFbo   FBOs[2];    // Real addresses of ping/pong FBO«s
	};

	class GameOfLifeScene : public ccScene {

	public:

		GameOfLifeScene( int cells_x = 102, int cells_y = 77 );
		void setup();
		void update();
		void draw();
		void reset();

		void windowResized( int w, int h );

		void handleSphereResolutionChanged( int& sphereRes );
		void handleDimensionsChanged( ofVec2f& value );

	private:
		int n_cells_x;
		int n_cells_y;
		float cellOffset;

		float time;

		ofShader logicShader;
		ofShader instancedShader;
		ofShader outlineShader;

		pingPongBuffer cellPingPong;

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

		float calculateSphereRadius( ofVec2f dim );
		void allocateCellBuffer( int rows, int cols );

		void drawOutlined( ofVboMesh& mesh, ofShader& instance, ofShader& outline );

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
		void setSequence( SequenceName name );
		GameOfLifeScene::SequenceName randSequence();
		void updateParameters();

		float SceneIntro();
		float SceneOutro();

	};
}