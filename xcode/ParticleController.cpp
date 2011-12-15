#include "ParticleController.h"
#include "Constraint.h"
#include "ForcePerlin.h"
#include "ForceGravity.h"
#include "ForceRepulsion.h"
#include "cinder/Rand.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

using namespace ci;
using std::list;
using std::vector;
using std::cout;


ParticleController::ParticleController()
{	
	/* there are 3 basic forces on particles, gravity, perlin noise and repulsion*/
	
	FORCEGRAVITY		= 0;
	FORCEPERLIN			= 1;
	FORCEREPULSION		= 2;
	
	
	mEnableQuad			= true; // enable the particle rendering
	mEnableTail			= true; // enable the particle tail rendering
	mEnablePerlin		= true; // enable the perlin noise effect
	mEnableRepulsion	= false; // enable the repulsion effect
	mEnableGravity		= false; // enable the gravity effect
	mEnableConstraints	= false; // enable the contraints
	
	adjustForcesOfType( FORCEPERLIN, mEnablePerlin );
	adjustForcesOfType( FORCEGRAVITY, mEnableGravity );
	adjustForcesOfType(FORCEREPULSION, mEnableRepulsion);
}

void ParticleController::createConstraints( Vec2f windowDim )
{
	/* initialize a new constraints */
	mConstraints.clear();
	mConstraints.push_back( new Constraint( Vec3f( 0, 1, 0 ), -1000.0f, windowDim.y * 0.625f ) );
}




void ParticleController::update( Emitter &emitter, int counter )
{
	// update Forces
	// apply each enabled force 
	for( vector<Force*>::iterator forceIt = mForces.begin(); forceIt != mForces.end(); ++forceIt ){
		if( (*forceIt)->mEnabled )
			(*forceIt)->apply( mParticles, counter );
	}
	
	// update Particles
	for( list<Particle>::iterator particleIt = mParticles.begin(); particleIt != mParticles.end(); ) {
		if( ! particleIt->mIsDead ) {
			// the particle should be alive
			if( particleIt->mIsBouncing ){
				// the particle is "healthy" then give it a chance to kill it
				if( Rand::randFloat() < 0.025f && !particleIt->mIsDying ){
					// the chance is really small .025/1
					mParticles.push_back( Particle( particleIt->mLoc[0], Vec3f::zero() ) );
					mParticles.back().mIsDying = true;
					particleIt->mIsDying = true;
				}
			}
			
			particleIt->move();
			particleIt->update();
			particleIt->finish();
			++particleIt;
		}
		else {
			particleIt = mParticles.erase( particleIt );
			// erase the particle from the list
		}
	}
	
	// apply Constraints;
	for( vector<Constraint*>::iterator constraintIt = mConstraints.begin(); constraintIt != mConstraints.end(); ++constraintIt ){
		if( mEnableConstraints ){
			(*constraintIt)->apply( mParticles );
			(*constraintIt)->apply( emitter );
		}
	}
	
}



// render the particles based on the given heat
// the heat usually comes fromt the emitter heat
// but you can do whatever you want.
void ParticleController::render( float heat )
{
	if( mEnableQuad ){
		renderParticles( heat );
	}

	if( mEnableTail ){
		glDisable( GL_TEXTURE_2D );
		//Trails is not 2D. 
		renderTrails();
	}
}

// render particle
// use particle textures
void ParticleController::renderParticles( float heat )
{
	glColor4f( 1, 1, 1, 1 );
	glBegin( GL_QUADS );
	for( list<Particle>::iterator particleIt = mParticles.begin(); particleIt != mParticles.end(); ++particleIt ) {
		// do not draw those particles with tiny radius because you can't see it either.
		// it can save solid amount of cpu.
		if( particleIt->mRadius > 0.1f ) 
			particleIt->render( heat );
	}
	glEnd();
}

// render trails
void ParticleController::renderTrails()
{
	for( list<Particle>::iterator particleIt = mParticles.begin(); particleIt != mParticles.end(); ++particleIt ) {
		particleIt->renderQuadStripTrail();
	}
}


// add more particles based on give amount, location, velocity, heat and radius
void ParticleController::addParticles( int amt, Vec3f loc, Vec3f vel, float heat, float radius )
{
	for( int i = 0; i < amt; i++ ) {
		/* 
		 It needs to be random process to make it real.
		 the offset, velocity are both kind of randomized.
		 but still based ont the given heat and radius.
		 Basicly, the hotter and bigger planet(Emitter) can
		 emit a more powerful and wide spread particle because
		 it has more energy.
		 */
		Vec3f lOffset = Rand::randVec3f();
		Vec3f l = loc + lOffset * radius * 0.45f;
		Vec3f v = -vel + lOffset * Rand::randFloat( 6.0f, 10.5f ) * ( heat + 0.75f ) + Rand::randVec3f() * Rand::randFloat( 1.0f, 2.0f );
		v.y *= 0.65f;
		mParticles.push_back( Particle( l, v ) );
	}
}


// modify the forces list so user can toggle the forces. 
void ParticleController::adjustForcesOfType( int type, bool enable )
{
	if( enable ){
		if( type == FORCEPERLIN ){
			mForces.push_back( new ForcePerlin( FORCEPERLIN, 0.5f ) );
		}
		else if( type == FORCEGRAVITY ){
			mForces.push_back( new ForceGravity( FORCEGRAVITY, Vec3f( 0.0f, 0.1f, 0.0f ), 1.0f ) );
		}
		else if( type == FORCEREPULSION ){
			/* I don't recommend use repulsion because it can be really cpu-consuming but
			   the reason why is the particle's mass is too small and velocity is large so the repulsion
			   effect is not that obvious.
			 */
			mForces.push_back( new ForceRepulsion( FORCEREPULSION, 10.0f ) ); 
		}
	} else {
		for( vector<Force*>::iterator forceIt = mForces.begin(); forceIt != mForces.end(); ){
			if( (*forceIt)->mType == type ){
				forceIt = mForces.erase( forceIt );
			} else {
				++forceIt;
			}
		}
	}
}

