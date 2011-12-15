/************************************************************************************
	Author: Hang Qian
    Last Modified date : Dec/14/2011
 
	Particle Controller
 
	This file is part of the final project of CSC630-Interactive Computer Graphic
	instructed by Dr. Wei, Saint Joseph's University.
 
**************************************************************************************/

/*
	Particle controller is the controller between a planet(Emitter) and its particles.
	It adds, renders and changes the particles of the given Emitter.
	
	The inline document can be found at ParticleController.cpp
*/


#pragma once
#include "Particle.h"
#include "Force.h"
#include "Constraint.h"
#include "Emitter.h"
#include "cinder/Vector.h"
#include "cinder/Color.h"
#include "cinder/gl/Texture.h"
#include <list>
#include <vector>

class ParticleController {
 public:
	ParticleController();
	void update( Emitter &emitter, int counter  );
	void render( float heat );
	void renderParticles( float heat );
	void renderTrails();

	void addParticles( int amt, ci::Vec3f loc, ci::Vec3f vel, float heat, float radius );
	void addGravity( ci::Vec3f loc, float radius );
	void createConstraints( ci::Vec2f windowDim );
	void adjustForcesOfType( int type, bool enable );
	
	std::list<Particle>			mParticles;
	std::vector<Force*>			mForces;
	std::vector<Constraint*>	mConstraints;
	
	int			FORCEGRAVITY;
	int			FORCEPERLIN;
	int			FORCEREPULSION;
	
	bool		mEnableConstraints;
	bool		mEnableGravity;
	bool		mEnablePerlin;
	bool		mEnableRepulsion;
	bool		mEnableTail;
	bool		mEnableQuad;
	
	ci::gl::Texture mParticleTexture;
};