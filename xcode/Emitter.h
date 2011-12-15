/* ***************************************************************************
 Author: Hang Qian
 Last Modified date : Dec/14/2011
 
 Emitter
 
 This file is part of the final project of CSC630-Interactive Computer Graphic
 instructed by Dr. Wei, Saint Joseph's University 2011 fall.
 
 *********************************************************************************/

/*
 Emitter is the planet and stars in the space. It is called emitter because it's able
 to emit particles. For some of the emitters they are orbiting around a point and for
 other part of them they stay in one place and only do self spinning.
 
 The inline document can be found at  Emitter.cpp
 
 */
#pragma once
#include "Particle.h"
#include "Force.h"
#include "cinder/Vector.h"
#include "cinder/Color.h"
#include "cinder/gl/Texture.h"
#include <list>
#include <vector>

class Emitter {
 public:
	Emitter();
	Emitter( ci::Vec3f loc, ci::Vec3f vel, float angle, float rvel, float radius, float heat, float spinspeed);
	void update();
	void render( float alpha );
	void renderFloorLighting( float floorLevel );
	
	ci::Vec3f mLoc;
	ci::Vec3f mVel;
	float mAngle;
	float mRotateVel;
	float mRadius;
	float mRadiusDest;
	ci::gl::Texture mTexture;
	float mHeat;
	float mSpinSpeed;
	bool mIsTouchingConstraint;
};