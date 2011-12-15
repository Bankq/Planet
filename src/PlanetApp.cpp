#include "cinder/app/AppBasic.h"
#include "cinder/gl/Texture.h"
#include "cinder/Rand.h"
#include "cinder/ImageIo.h"
#include "cinder/Camera.h"
#include "cinder/Utilities.h"
#include "cinder/gl/GlslProg.h"

#include "ParticleController.h"
#include "InfoPanel.h"
#include "Emitter.h"
#include "Resources.h"

using namespace ci;
using namespace ci::app;
using std::list;

#include <list>
#include <sstream>

class PlanetApp : public AppBasic {
private:
	enum { TOP, BOT, LEF, RIG, NEA, FARP };
public:
	void prepareSettings( Settings *settings );
	void resize( ResizeEvent event );
	void setup();
	void update();
	void keyDown( KeyEvent event );
	void keyUp( KeyEvent event);
	void mouseMove( MouseEvent event );
	void mouseDrag( MouseEvent event );
	void mouseDown( MouseEvent event );
	void mouseUp( MouseEvent event );
	void draw();

	
	
	
	gl::GlslProg	                 mEmitterShader;
	Vec3f			                 mMouseLoc;
	Vec3f			                 mMousePastLoc;
	Vec3f			                 mMouseVel;
	
	int				                 mCounter;
	float			                 mMouseDownCounter;
	bool			                 mIsMouseDown;
	bool			                 mSaveFrames;
	
	gl::Texture		                 mEmitterTexture;
	gl::Texture		                 mParticleTexture;
	gl::Texture		                 mNormalTexture;
	gl::Texture		                 mHeightTexture;
	gl::Texture		                 mSpecTexture;
	
	ci::Vec3f						 mLightDir;
	
	std::list<Emitter*>				 mEmitters;
	std::list<ParticleController*>	 mParticleControllers;
	
	/* ************the camera settings ********************/
	bool                             mFrustumPlaneCached;
	
	CameraPersp                      mCam;
	CameraPersp                      mRenderCam;
	
	bool                             mResetFlag;
	
	float                            mFov, mNear, mFar;
	float                            mDecay;
	
	Vec3f                            mEye, mEyeDest, mCenter, mEyeNormal;
	float                            mDistance;
	float                            mAngle, mAngleDest;
	float                            mVerticalAngle, mVerticalAngleDest;
	
	Vec3f                            mREye, mRCenter;
	Vec3f                            mUp;
	
	bool                             mIsWatchingCam;
	/* ***********************************************/
	float                            mVelRatio;
	float                            mMaxVelRatio;
	
	InfoPanel			             mInfoPanel;
};


void PlanetApp::prepareSettings( Settings *settings )
{
	settings->setWindowSize( 1280, 768 );
	settings->setFrameRate( 60.0f );
	settings->setFullScreen( false );
}

// the initial routine//
void PlanetApp::setup()
{
	mFrustumPlaneCached =         false;
	
	mIsWatchingCam	    =         false;
	
	mFov		        =         60.0f;
	mNear		        =         10.0f;
	mFar		        =         2000000.0f;
	mDecay		        =         0.25f;
	// mClipCam Eye and Center
	mEye		        =		  Vec3f( 0.0f, 0.0f, 100.0f );
	mEyeDest			=		  Vec3f( 0.0f, 0.0f, 100.0f );
	mCenter		        =         Vec3f( 0.0f, 0.0f, -100.0f );
	mEyeNormal	        =         Vec3f::yAxis();
	
	mDistance	        =         500.0f;
	mAngle		        =         0.0f;
	mAngleDest	        =         0.0f;
	mVerticalAngle      =         0.0f;
	mVerticalAngleDest  =         0.0f;
	
	mResetFlag = false;
	// mRenderCam Eye and Center
	mREye		        =         Vec3f( 10.0f, 10.0f, 10.0f );
	mRCenter	        =         Vec3f::zero();
	
	mUp			        =         Vec3f::yAxis();
	
	mVelRatio           =         1.0f;
	mMaxVelRatio        =         5.0f;
	
	
	/*******************************************/
	
	mCounter			=         0;
	mIsMouseDown		=         false;
	
	mInfoPanel.createTexture();
	
	
	
	/* load the texture resources */
	mParticleTexture	=         gl::Texture( loadImage( loadResource( RES_PARTICLE ) ) );
	mEmitterTexture		=         gl::Texture( loadImage( loadResource( RES_EMITTER ) ) );
	mNormalTexture		=         gl::Texture( loadImage( loadResource( RES_NORMAL ) ) );
	mHeightTexture		=         gl::Texture( loadImage( loadResource( RES_BUMP ) ) );
	mSpecTexture		=         gl::Texture( loadImage( loadResource( RES_SPEC_EXPONENT ) ) );
	
	mEmitterTexture.setWrap( GL_REPEAT, GL_REPEAT );
	mNormalTexture.setWrap( GL_REPEAT, GL_REPEAT );
	mHeightTexture.setWrap( GL_REPEAT, GL_REPEAT );
	mSpecTexture.setWrap( GL_REPEAT, GL_REPEAT );
	
	
	try {
		mEmitterShader = gl::GlslProg( loadResource( RES_EMITTER_VERT ), loadResource( RES_EMITTER_FRAG ) );
	}
	catch( gl::GlslProgCompileExc &exc ) {
		std::cout << "Shader compile error: " << std::endl;
		std::cout << exc.what();
	}
	catch( ... ) {
		std::cout << "Unable to load shader" << std::endl;
	}
	
	mLightDir		= Vec3f( 0.0f, 0.25f, 1.0f );
	mLightDir.normalize();
	mMousePastLoc = mMouseLoc = mMouseVel = Vec3f::zero();
	
	
	
	/* Initialize planet here */
	//                      Emitter(Vec3f loc, Vec3f vel,float angle, float rvel, float radius, float heat, float spinspeed)
	mEmitters.push_back(new Emitter(Vec3f::zero(), Vec3f::zero(),0.0f,0.1f,300.0f,0.9f,0.1f));
	mEmitters.push_back(new Emitter(Vec3f(700.0f,0.0f,700.0f), Vec3f::zero(),0.0f,-0.1f,100.0f,0.4f,0.1f));
	mEmitters.push_back(new Emitter(Vec3f(300.0f,0.0f,300.0f),Vec3f::zero(),0.0f,0.05f,50.0f,0.3f,0.1f));
	mEmitters.push_back(new Emitter(Vec3f(240.0f,900.0f,50.0f),Vec3f::zero(),0.0f,0.0f,70.0f,0.8f,0.1f));
	
	for (int i = 0 ; i < mEmitters.size(); i++) {
		mParticleControllers.push_back(new ParticleController());
	}


	
	
	
}


void PlanetApp::resize( ResizeEvent event )
{
	AppBasic::resize( event );
	// We need to recalculate the constraints for all controllers
	for(list<ParticleController*>::iterator it = mParticleControllers.begin(); it != mParticleControllers.end(); it++){
		(*it)->createConstraints( Vec2f( getWindowWidth(), getWindowHeight() ) );
	};
}


void PlanetApp::keyUp( KeyEvent event){
	if ( event.getChar() == 'w' || event.getChar() == 'W' || event.getChar() == 's' || event.getChar() == 'S' ){
		// after you release the moving keys, it will delete the accelaration raito
		mVelRatio = 1.0f;
	}
}


void PlanetApp::keyDown( KeyEvent event )
{
	if( event.getChar() == 'f' || event.getChar() == 'F' ){
		setFullScreen( ! isFullScreen() );
	}
	else if( event.getChar() == '/' || event.getChar() == '?' ){
		mInfoPanel.toggleState();
	}

	
	for( list<ParticleController*>::iterator it = mParticleControllers.begin(); it != mParticleControllers.end(); it++){
		if( event.getChar() == 't' || event.getChar() == 'T' ) {
			(*it)->mEnableTail = ! (*it)->mEnableTail;
		}
		else if( event.getChar() == 'c' || event.getChar() == 'C' ) {
			(*it)->mEnableConstraints = ! (*it)->mEnableConstraints;
		}
		else if( event.getChar() == 'p' || event.getChar() == 'P' ) {
			(*it)->mEnablePerlin = ! (*it)->mEnablePerlin;
			(*it)->adjustForcesOfType( (*it)->FORCEPERLIN, (*it)->mEnablePerlin );
		}
		else if( event.getChar() == 'g' || event.getChar() == 'G' ) {
			(*it)->mEnableGravity = ! (*it)->mEnableGravity;
			(*it)->adjustForcesOfType( (*it)->FORCEGRAVITY, (*it)->mEnableGravity );
		}
		else if( event.getChar() == 'r' || event.getChar() == 'R' ) {
			(*it)->mEnableRepulsion = ! (*it)->mEnableRepulsion;
			(*it)->adjustForcesOfType( (*it)->FORCEREPULSION, (*it)->mEnableRepulsion );
		}
		else if( event.getChar() == 'q' || event.getChar() == 'Q' ) {
			(*it)->mEnableQuad = ! (*it)->mEnableQuad;
		}	
	
	}
	
	
	// toggle the cameras
	if( event.getChar() == 'b' || event.getChar() == 'B' ){
		mIsWatchingCam = ! mIsWatchingCam;
	}
	else if( event.getChar() == 'x' || event.getChar() == 'X'){
		mResetFlag = true;
	}
	else if( event.getChar() == 'w' || event.getChar() == 'W' ){
		mEyeDest += mVelRatio * mEyeNormal * 40.0f;
		mVelRatio = (mVelRatio > mMaxVelRatio)?mMaxVelRatio:(1.1f * mVelRatio);
	} else if( event.getChar() == 's' || event.getChar() == 'S' ){
		mEyeDest -= mVelRatio * mEyeNormal * 40.0f;
		mVelRatio = (mVelRatio > mMaxVelRatio)?mMaxVelRatio:(1.1f * mVelRatio);
	}
	
	if( event.getCode() == KeyEvent::KEY_UP ){
		mVerticalAngleDest += 0.1f;
	} else if( event.getCode() == KeyEvent::KEY_DOWN ){
		mVerticalAngleDest -= 0.1f;
	}
	
	if( event.getCode() == KeyEvent::KEY_LEFT ){
		mAngleDest += 0.1f;
	} else if( event.getCode() == KeyEvent::KEY_RIGHT ){
		mAngleDest -= 0.1f;
	}
	
	

}

void PlanetApp::mouseMove( MouseEvent event )
{
	mMouseLoc = Vec3f( event.getX(), event.getY(), 0.0f );
}

void PlanetApp::mouseDrag( MouseEvent event )
{
	mouseMove( event );
}

void PlanetApp::mouseDown( MouseEvent event )
{
	mIsMouseDown = true;
}

void PlanetApp::mouseUp( MouseEvent event )
{
	mIsMouseDown = false;
}
// update routine
void PlanetApp::update()
{
	mMousePastLoc -= ( mMousePastLoc - mMouseLoc ) * 0.01f;
	mMouseVel -= ( mMouseVel - ( mMousePastLoc - mMouseLoc ) ) * 0.01f;
	
	
	
	/* 
	 in update routine,we need to update the current emitters and particles and decide if needed
	 to add more particles
	 */
	list<Emitter*>::iterator eIt;
	list<ParticleController*>::iterator pIt;
	for(eIt = mEmitters.begin(),pIt = mParticleControllers.begin(); eIt != mEmitters.end(); eIt++, pIt++){
		// update emitter
		(*eIt)->update();
		// add particles 
		if( (*eIt)->mHeat > 0.5f ){
			// if emitter is hot enough
			float per = Rand::randFloat();
			Vec3f vel = (*eIt)->mVel * per;
			vel.y *= 0.02f;
			int numParticlesToSpawn = ( (*eIt)->mHeat - 0.5f ) * 250;
			if( Rand::randFloat() < 0.02f )
				// in case too small number
				numParticlesToSpawn *= 5;
			(*pIt)->addParticles( numParticlesToSpawn, (*eIt)->mLoc, (*eIt)->mVel * per, (*eIt)->mHeat, (*eIt)->mRadius );
		}

		(*pIt)->update( **eIt, mCounter );
	
	}
	
	
	
	
	/* camera */
	mAngle -= ( mAngle - mAngleDest ) * 0.1f;
	mVerticalAngle -= ( mVerticalAngle - mVerticalAngleDest ) * 0.1f;

	mEye -= ( mEye - mEyeDest ) * 0.2f;
	// x = r*sinA*cosB; y = r*sinB; z = r × cosA * sinB
	mEyeNormal = Vec3f( sin( mAngle ) * cos(mVerticalAngle), sin(mVerticalAngle), cos( mAngle ) * cos(mVerticalAngle) ); 
	
	mCenter = mEye + mEyeNormal * 50.0f;
	
	mCam.setPerspective( 25.0f, getWindowAspectRatio(), 100.0f, 350.0f );
	mCam.lookAt( mEye, mCenter, mUp );

	
	
	
	if( mIsWatchingCam ){
		mREye = Vec3f(5000.0f,5000.0f,5000.0f);
		mRCenter = Vec3f::zero();
		mFov -= ( mFov - 60.0f ) * mDecay;
		mNear -= ( mNear - 10.0f ) * mDecay;
		mFar -= ( mFar - 1500.0f ) * mDecay;
		
	} else {

		mREye.lerpEq( mDecay, mEye );
		if( mResetFlag){
			// if user reset the camera
			mRCenter = Vec3f::zero();
			mResetFlag = false;
		}
		else{
			mRCenter.lerpEq( mDecay, mCenter );
		}
		mFov -= ( mFov - mCam.getFov() ) * mDecay;
		mNear -= ( mNear - mCam.getNearClip() ) * mDecay;
		mFar -= ( mFar - mCam.getFarClip() ) * mDecay;
	}
	
	
	mRenderCam.setPerspective( mFov, getWindowAspectRatio(), 10.0f, 2000000.0f );
	mRenderCam.lookAt( mREye, mRCenter, mUp );
	
	gl::setMatrices( mRenderCam );
	
	
	
}


void PlanetApp::draw()
{
	glClearColor( 0.0025f, 0.0025f, 0.0025f, 1 );
	gl::enableDepthWrite( true );
	gl::enableDepthRead( true );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	/*
	 for each emitter and particle
	 bind it to the texture resource and shading language
	 */
	list<Emitter*>::iterator eIt;
	list<ParticleController*>::iterator pIt;
	for( eIt = mEmitters.begin(), pIt = mParticleControllers.begin(); eIt != mEmitters.end(); eIt++, pIt++){
		gl::enableAlphaBlending();
		glEnable( GL_TEXTURE_2D );
		mEmitterTexture.bind( 0 );
		mNormalTexture.bind( 1 );
		mHeightTexture.bind( 2 );
		mSpecTexture.bind( 3 );
		mEmitterShader.bind();
		mEmitterShader.uniform( "texDiffuse", 0 );
		mEmitterShader.uniform( "texNormal", 1 );
		mEmitterShader.uniform( "texHeight", 2 );
		mEmitterShader.uniform( "texSpec", 3 );
		if( mIsMouseDown )
			mEmitterShader.uniform( "isPressed", 1 );
		else
			mEmitterShader.uniform( "isPressed", 0 );
	

		mEmitterShader.uniform( "heat", (*eIt)->mHeat );
		mEmitterShader.uniform( "mouseVel", mMouseVel * 0.025f );
		mEmitterShader.uniform( "spinSpeed", (*eIt)->mSpinSpeed );
		mEmitterShader.uniform( "counter", (float)mCounter );
		mEmitterShader.uniform( "lightDir", mLightDir );
		mEmitterShader.uniform( "minHeight", 0 );
	
		
		(*eIt)->render(1.0f );
	
	
		gl::enableDepthWrite( false );
		glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
	
		mEmitterShader.unbind();
		gl::enableAdditiveBlending();
	
		mParticleTexture.bind();
		(*pIt)->render( (*eIt)->mHeat );
	}
	



	

	
	
	gl::enableDepthWrite( false );
	gl::setMatricesWindow( getWindowSize() );
	
	glEnable( GL_TEXTURE_2D );
	mInfoPanel.update( Vec2f( getWindowWidth(), getWindowHeight() ), mCounter );
	glDisable( GL_TEXTURE_2D );
	
	
	mCounter++;
}





CINDER_APP_BASIC( PlanetApp, RendererGl )