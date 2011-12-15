/* ***************************************************************************
 Author: Hang Qian
 Last Modified date : Dec/14/2011
 
 Force Repulsion
 
 This file is part of the final project of CSC630-Interactive Computer Graphic
 instructed by Dr. Wei, Saint Joseph's University 2011 fall.
 
 *********************************************************************************/

/*
 Force Repulsion defines the repulsion force between particles.
 
 The inline document can be found at  ForceRepulsion.cpp
 
 */
#pragma once

#include "Force.h"
#include "cinder/Vector.h"
#include "Particle.h"
#include "cinder/Perlin.h"
#include <list>

class ForceRepulsion : public Force {
 public:
	ForceRepulsion( int type, float aMagnitude );
	void apply( std::list<Particle> &aParticles, int aCounter );
};
