/* ***************************************************************************
 Author: Hang Qian
 Last Modified date : Dec/14/2011
 
 Info Panel
 
 This file is part of the final project of CSC630-Interactive Computer Graphic
 instructed by Dr. Wei, Saint Joseph's University 2011 fall.
 
 *********************************************************************************/

/*
 Info panel defines a panel that introduces the basic usage of the the project
 
 The inline document can be found at InfoPanel.cpp
 
 */

#pragma once

#include "cinder/Vector.h"
#include "cinder/gl/Texture.h"

class InfoPanel {
 public:
	InfoPanel();
	void createTexture();
	void update( ci::Vec2f windowDim, float counter );
	void render( ci::Vec2f windowDim );
	void toggleState();
	
	ci::Vec2f			mLoc;
	
	float				mOpacity;
	float				mCounter;
	bool				mState;
	int					mFadeTime;
	ci::gl::Texture	mTexture;
};
