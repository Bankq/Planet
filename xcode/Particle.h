/* ***************************************************************************
 Author: Hang Qian
 Last Modified date : Dec/14/2011
 
 Particle
 
 This file is part of the final project of CSC630-Interactive Computer Graphic
 instructed by Dr. Wei, Saint Joseph's University 2011 fall.
 
 *********************************************************************************/

/*
 Class Particle defines the property and method of the particle.
 It decides how a certain particle moves, updates, renders and finishes.
 The Particle has lifespan, current age and radius of moving.
 
 The inline document can be found at Particle.cpp
 
 */

#pragma once

#include "cinder/Vector.h"
#include "cinder/Color.h"

#include <vector>

class Particle {
 public:
	Particle( ci::Vec3f aLoc, ci::Vec3f aVel );
	void move();
	void update();
	void render( float heat );
	void renderLineStripTrail();
	void renderQuadStripTrail();
	void finish();
	
	int			mLen;
	float		mInvLen;
	std::vector<ci::Vec3f> mLoc;
	ci::Vec3f	mVel;
	ci::Vec3f	mAcc;

	float		mRadius;
	float		mMass;
	float		mInvMass;
	float		mCharge;
	
	float		mAge;
	float		mLifeSpan;
	float		mAgePer;
	
	bool		mIsBouncing;
	bool		mIsDying;
	bool		mIsDead;
};
