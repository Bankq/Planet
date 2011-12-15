/* ***************************************************************************
 Author: Hang Qian
 Last Modified date : Dec/14/2011
 
 Force Perlin
 
 This file is part of the final project of CSC630-Interactive Computer Graphic
 instructed by Dr. Wei, Saint Joseph's University 2011 fall.
 
 *********************************************************************************/

/*
 Force Perlin defines the Perlin noise  between particles.
 
 
 Perlin noise is a procedural texture primitive, a type of gradient noise used by 
 visual effects artists to increase the appearance of realism in computer graphics. 
 The function has a pseudo-random appearance, yet all of its visual details are the 
 same size (see image). This property allows it to be readily controllable; multiple 
 scaled copies of Perlin noise can be inserted into mathematical expressions to 
 create a great variety of procedural textures. Synthetic textures using Perlin noise 
 are often used in CGI to make computer-generated visual elements - such as fire, smoke, 
 or clouds - appear more natural, by imitating the controlled random appearance of 
 textures of nature.It is also frequently used to generate textures when memory is 
 extremely limited, such as in demos, and is increasingly finding use in Graphics Processing 
 Units for real-time graphics in computer games.
 
 
 The introduction of perlin noise can be found here:
 http://en.wikipedia.org/wiki/Perlin_noise
 
 
 
 The inline document can be found at  ForcePerlin.cpp
 
 */
#pragma once

#include "Force.h"
#include "cinder/Vector.h"
#include "Particle.h"
#include "cinder/Perlin.h"
#include <list>

class ForcePerlin : public Force {
 public:
	ForcePerlin( int type, float magnitude );
	void apply( std::list<Particle> &particles, int counter );
	
	ci::Perlin mPerlin;
};
