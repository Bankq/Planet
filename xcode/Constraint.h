/* ***************************************************************************
 Author: Hang Qian
 Last Modified date : Dec/14/2011
 
 Contraint.h
 
 This file is part of the final project of CSC630-Interactive Computer Graphic
 instructed by Dr. Wei, Saint Joseph's University 2011 fall.
 
 *********************************************************************************/

/*
  Contraint defines the volume contraint of the emitter and the particles.
 
 The inline document can be found at  Contraint.cpp
 
 */
#pragma once

#include "cinder/Vector.h"
#include "Emitter.h"
#include "Particle.h"
#include "cinder/gl/Texture.h"

#include <list>

class Constraint {
 public:
	Constraint( ci::Vec3f normal, float minValue, float maxValue );
	virtual void apply( Emitter &aEmitter );
	virtual void apply( std::list<Particle> &aParticles );
	ci::Vec3f mNormal;
	float mMinValue;
	float mMaxValue;
 protected:
};
