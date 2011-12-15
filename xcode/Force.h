/* ***************************************************************************
 Author: Hang Qian
 Last Modified date : Dec/14/2011
 
 Force
 
 This file is part of the final project of CSC630-Interactive Computer Graphic
 instructed by Dr. Wei, Saint Joseph's University 2011 fall.
 
 *********************************************************************************/
#pragma once

#include "cinder/Vector.h"
#include "Particle.h"
#include "cinder/gl/Texture.h"

#include <list>

class Force {
 public:
	Force( int type, float mag );
	virtual void update();
	virtual void apply( std::list<Particle> &particles, int counter );
	virtual void enableIfType( int type, bool enable );
	
	void setMagnitude( float mag );

	int mType;
	bool mEnabled;
	
 protected:
	float mMagnitude;
};
