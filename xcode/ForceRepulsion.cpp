#include "ForceRepulsion.h"

using namespace ci;
using std::list;

ForceRepulsion::ForceRepulsion( int type, float magnitude )
	: Force( type, magnitude )
{
}


void ForceRepulsion::apply( std::list<Particle> &particles, int counter )
{
	for( list<Particle>::iterator p1 = particles.begin(); p1 != particles.end(); ++p1 ){
		// particle have mass and charge
		// difines the repulsion effect
		float thisQTimesInvM = p1->mInvMass * p1->mCharge;
		
		for( list<Particle>::iterator p2 = p1; p2 != particles.end(); ++p2 ){
			// to particle p1, to each particle excluding itself
			if( p1 != p2 ){
				Vec3f dir = p1->mLoc[0] - p2->mLoc[0];
				float distSqrd = dir.lengthSquared();// get the length between 2 particles
				float radiusSum = 125.0f; // effect radius
				float radiusSqrd = radiusSum * radiusSum;
				
				if( distSqrd < radiusSqrd && distSqrd > 0.1f ) {
					// in effect area and not too close
					// Coulomb law
					// F = C * q1 * q2 / dist;
					float per = 1.0f - distSqrd/radiusSqrd;
					float E = p2->mCharge / distSqrd;
					float F = E * thisQTimesInvM;
					
					if( F > 15.0f ) // F cannot be too large
						F = 15.0f;
					
					dir.normalize();
					dir *= F * per * mMagnitude;
					
					p1->mAcc += dir;
					p2->mAcc -= dir;
				}
			}
		}
	}
	
}

