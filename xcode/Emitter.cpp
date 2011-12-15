#include "Emitter.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "cinder/CinderMath.h"

using namespace ci;

Emitter::Emitter()
{
//	mLoc		= Vec3f::zero();
	mLoc		= Vec3f(100.0f,0.0f,100.0f);
	mVel		= Vec3f::zero();
	mAngle      = 0.0f;
	mRotateVel  = 0.1f;
	mRadius		= 50.0f;
	mRadiusDest = 800.0f;
	mHeat		= 0.9f;
	mSpinSpeed	= 0.4f;
}

Emitter::Emitter(Vec3f loc, Vec3f vel,float angle, float rvel, float radius, float heat, float spinspeed)
{
	mLoc		= loc;
	mVel		= vel;
	mAngle		= angle;
	mRotateVel  = rvel;
	mRadius		= radius;
	mRadiusDest = 800.0f;
	mHeat		= heat;
	mSpinSpeed	= spinspeed;
}



void Emitter::update()
{	
	
	if(mLoc.y == 0.0f){ // only planet on y==0 plane can rotate
		mAngle += mRotateVel;
		if (mAngle > 2 * 3.1416) {
			mAngle -= 2 * 3.1416;
		}
		else if(mAngle < -2 * 3.1416){
			mAngle += 2* 3.1416;
		}
		float dist = sqrt(mLoc.x * mLoc.x + mLoc.z * mLoc.z);
		mLoc = Vec3f(dist * sin(mAngle),0.0f,dist * cos(mAngle));
	}
}


void Emitter::render(float alpha )
{
	//mRadius = radius - mSpinSpeed * 30.0f + 70.0f * ( 1.0f - mSpinSpeed );
	glColor4f( 1, 1, 1, alpha );
	gl::drawSphere( mLoc, mRadius, 32 );
}

