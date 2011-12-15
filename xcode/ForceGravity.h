/* ***************************************************************************
 Author: Hang Qian
 Last Modified date : Dec/14/2011
 
 Force Gravity
 
 This file is part of the final project of CSC630-Interactive Computer Graphic
 instructed by Dr. Wei, Saint Joseph's University 2011 fall.
 
 *********************************************************************************/

/*
 Force Graphic defines the gravity of the particles
 
 The inline document can be found at  ForceGravity.cpp
 
 */

#pragma once

#include "Force.h"
#include "cinder/Vector.h"
#include "Particle.h"
#include "cinder/Perlin.h"
#include "cinder/gl/Texture.h"
#include <list>

class ForceGravity : public Force {
 public:
	ForceGravity( int type, ci::Vec3f axis, float magnitude );
	void apply( std::list<Particle> &particles, int counter );
	
	ci::Vec3f mAxis;
};
