#include "Particle.h"
#include "cinder/Rand.h"
#include "cinder/gl/gl.h"


using namespace ci;

Particle::Particle( Vec3f aLoc, Vec3f aVel )
{
	
	mLen			= 15; // the trace length
	mInvLen			= 1.0f / (float)mLen;
	for( int i=0; i<mLen; ++i ) {
		// need to store the trace
		// so I init all mLen trace with the origin position
		mLoc.push_back( aLoc );
	}

	mVel			= aVel;
	mAcc			= Vec3f::zero();
	
	mRadius			= Rand::randFloat( 1.0f, 3.0f );
	if( Rand::randFloat() < 0.01f )
		mRadius		*= 4.0f;
	mMass			= mRadius;
	mInvMass		= 1.0f/mMass;
	mCharge			= Rand::randFloat( 0.35f, 0.75f );
	
	mIsBouncing		= false;
	mIsDying		= false;
	mIsDead			= false;
	mAge			= 0.0f;
	mLifeSpan		= Rand::randFloat( 5.0f, 170.0f );
	
	if( Rand::randFloat() < 0.15f )
		mLifeSpan += 200.0f;
		
	mAgePer			= 1.0f;
}

// move the particle
void Particle::move()
{
	if( mVel != Vec3f::zero() )
		mVel += mAcc;

	
	for( int i=mLen-1; i>0; i-- ) {
		// move the location one step furthur
		// it's a little bit tricky.
		mLoc[i] = mLoc[i-1];
	}
	mLoc[0] += mVel;
}

// update the particle 
void Particle::update()
{
	mVel *= 0.975f;// slow it down little by little
	mAcc.set( 0, 0, 0 );
	
	mAge ++;
	if( mAge > mLifeSpan ){
		// if it's over it's life span
		mIsDead = true;
	} else {
		/*
		 the particle color dims at a changing speed.
		 */
		mAgePer = 1.0f - mAge/mLifeSpan;
	}
}


void Particle::render( float heat )
{
	if( mIsDying && mVel == Vec3f::zero() ){
		// dying particle have different color
		glColor4f( mAgePer * 0.5f, mAgePer * 0.35f, 1.0f - mAgePer, mAgePer + Rand::randFloat( 0.25f ) );
	} else {
		glColor4f( mAgePer, mAgePer * 0.75f, 1.0f - mAgePer + 0.15f, mAgePer + Rand::randFloat( 0.5f ) );
	}
	
	float radius = mRadius;
	if( mAge == 1 && ! mIsDying && heat > 0.3f )
		radius *= 20.0f * heat;
	
	glTexCoord2f( 0, 0 );
	glVertex3f( mLoc[0].x - radius, mLoc[0].y - radius, mLoc[0].z );
	
	glTexCoord2f( 1, 0 );
	glVertex3f( mLoc[0].x + radius, mLoc[0].y - radius, mLoc[0].z );
	
	glTexCoord2f( 1, 1 );
	glVertex3f( mLoc[0].x + radius, mLoc[0].y + radius, mLoc[0].z );
	
	glTexCoord2f( 0, 1 );
	glVertex3f( mLoc[0].x - radius, mLoc[0].y + radius, mLoc[0].z );
}


void Particle::finish()
{
	mIsBouncing = false;
}


void Particle::renderLineStripTrail()
{
	float max = mAge;
	if( max > mLen )
		max = (float)mLen; // avoid access to null value if the particle dies at a very young age
		
	glBegin( GL_LINE_STRIP );
	for( int i=0; i<max; i++ ){
		float per = 1.0f - ( mInvLen * (float)i );

		glColor4f( per, 0.2f, 0.0f, per * mAgePer * 0.2f );
		glVertex3fv( mLoc[i] );
	}
	glEnd();
}


// render traces
void Particle::renderQuadStripTrail()
{
	glBegin( GL_QUAD_STRIP );
	for( int i=0; i<mLen-2; i++ ){
		float per	= i / (float)(mLen-1);
		/*
		 I did a little bit of matrix calculate here.
		 the direction vector cross the Z axis can get
		 a shadow on x-y plane
		 then cross it with direction again can have the
		 real the location of the current drawing position.
		 */
		Vec3f perp0	= Vec3f( mLoc[i].x, mLoc[i].y, 0.0f ) - Vec3f( mLoc[i+1].x, mLoc[i+1].y, 0.0f );
		Vec3f perp1	= perp0.cross( Vec3f::zAxis() );
		Vec3f perp2	= perp0.cross( perp1 );
			  perp1	= perp0.cross( perp2 ).normalized();
			  
		Vec3f off	= perp1 * ( mRadius * ( 1.0f - per ) * 0.25f  );

		glColor4f( ( 1.0f - per ) * 0.75f, 0.15f, per * 0.5f, ( 1.0f - per ) * mAgePer * 0.25f );
		glVertex3fv( mLoc[i] - off );
		glVertex3fv( mLoc[i] + off );
	}

	glEnd();
}






