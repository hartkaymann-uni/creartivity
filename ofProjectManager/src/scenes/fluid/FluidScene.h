#pragma once

#include "ccFluid.h"
#include "ccScene.h"

/*
	Fluid Scene
	Simulation of an incompressible, homogenous fluid using the navier-stokes equaution.
	Right now the simulation is two-dimensional, but three-dimensional movement could be added in the future.
*/

class FluidScene : public ccScene
{
public:
	FluidScene();
	void setup();
	void update();
	void draw();

	void keyPressed( int key );
	void keyReleased( int key );

private:

	float time;
	bool debug;
	bool step;

	ofParameterGroup groupGeneral;
	ofParameterGroup groupGrid;
	ofParameterGroup groupViscosity;
	ofParameterGroup groupVorticity;
	ofParameterGroup groupSolver;
	ofParameterGroup groupGravity;
	ofParameterGroup groupView;

	ofParameter<bool> p_Sequences;
	ofParameter<bool> p_DebugView;
	ofParameter<bool> p_Bounds;
	ofParameter<bool> p_ApplyViscosity;
	ofParameter<bool> p_ApplyVorticity;
	ofParameter<bool> p_ApplyGravity;
	ofParameter<int> p_JacobiIterations;
	ofParameter<float> p_Curl;
	ofParameter<float> p_Scale;
	ofParameter<float> p_Epsilon;
	ofParameter<float> p_Timestep;
	ofParameter<float> p_Viscosity;
	ofParameter<float> p_SplatRadius;
	ofParameter<float> p_Dissipation;
	ofParameter<float> p_GravityStrength;
	ofParameter<glm::vec2> p_GravityDirection;
	ofParameter<ofFloatColor> p_SplatColor;

	fluid::ccSolver solver;

	ofShader displayScalar;
	ofShader displayVector;
	ofShader displayLines;
	ofShader displayVelocity;
	ofShader displayTexture;
	enum ShadingType {
		DEFAULT,
		PIXELS,
		VELOCITY,
		TEXTURE
	};
	const int NUM_SHADING = 4;
	ShadingType shading;
	void changeShading();

	void drawDefault();
	void drawDebug();
	void drawScalarField(ofFbo* const field, int x, int y, int w, int h);
	void drawPixelated();
	void drawVelocity();
	void drawTexture();
	fluid::Field texture;
	ofPlanePrimitive plane;

	// Members for sequences
	enum class SequenceName {
		Default,
		Smoke,
		Fast,
		Red,
		FastSmoke,
		Viscous,
		Stop,
		Empty
	};
	const int NUM_SEQ = 7;

	struct SequenceParameters {
		float timestep;
		float scale;
		float splat;
		float dissipation;
		bool applyVorticity;
		float curl;
		bool applyViscosity;
		float viscosity;
		glm::vec3 color;
	};

	SequenceName lastSequene;
	SequenceName currentSequence;
	float lastSequenceTime;
	float sequenceDuration;
	float sequenceTransitionDuration;
	map<SequenceName, SequenceParameters> sequenceMap;

	void setupSequences();
	void updateSequence();
	void setSequence( SequenceName name );
	void updateParameters();
	SequenceName randSequence();

	float SceneIntro();
	float SceneOutro();

	inline void handleCurlChanged( float& c ) { solver.setCurl( c ); }
	inline void handleBoundsChanged( bool& b ) { solver.setBounds( b ); }
	inline void handleScaleChanged( float& s ) { solver.setScale( s ); }
	inline void handleEpsilonChanged( float& e ) { solver.setEpsilon( e ); }
	inline void handleTimestepChanged( float& t ) { solver.setTimestep( t ); }
	inline void handleViscosityChanged( float& v ) { solver.setViscosity( v ); }
	inline void handleDissipationChanged( float& d ) { solver.setDissipation( d ); }
	inline void handleSplatRadiusChanged( float& r ) { solver.setSplatRadius( r ); }
	inline void handleApplyGravityChanged( bool& g ) { solver.setApplyGravity( g ); }
	inline void handleApplyViscosityChanged( bool& v ) { solver.setApplyViscosity( v ); }
	inline void handleApplyVorticityChanged( bool& v ) { solver.setApplyVorticity( v ); }
	inline void handleSplatColorChanged( ofFloatColor& c ) { solver.setSplatColor( c ); }
	inline void handleJacobiIterationsChanged( int& i ) { solver.setJacobiIterations( i ); }
	inline void handleGravityStrengthChanged( float& g ) { solver.setGravityStrength( g ); }
	inline void handleGravityDirectionChanged( glm::vec2& d ) { solver.setGravityDirection( d ); }
};
