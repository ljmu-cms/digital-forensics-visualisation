#ifndef __DigitalForensicsVisualisation_h_
#define __DigitalForensicsVisualisation_h_




#include "BaseApplication.h"
#include "SampleListener.h"
#include <stdio.h>
#include <stdlib.h>
#include "BST.h"
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>
#include "Entity.h"
#include "ColorMap.h"
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "../res/resource.h"
#endif


class DigitalForensicsVisualisation : public BaseApplication
{

private:

	BST <ColorMap> colorTree;
	Ogre::SceneNode* bonesArr[20]; //array that stores the addresses of the bones
	SampleListener leapSampleListener;
	Controller leapController;
	bool processUnbufferedInput(const Ogre::FrameEvent& evt);
	Ogre::SceneNode* handNode;
	Ogre::SceneNode *palmNode;
	Ogre::SceneNode* fingersNode;
	Ogre::SceneNode* filesNode;
	Ogre::Vector3 previousPosition;
	float previousFramePitch;
	float previousFrameYaw;
	float previousFrameRoll;
	bool handOrientationFlag;

	Ogre::Light* pointLight;

	Ogre::ManualObject* const cube(bool isFrustum, ColorMap);
	Ogre::ManualObject* const pyramid(ColorMap);
	Ogre::ManualObject* const cylinder(ColorMap);
	


public:
	
	

	Entity e;
    DigitalForensicsVisualisation(void);
    virtual ~DigitalForensicsVisualisation(void);
	int cubeCount;
	int pyramidCount;
	int cylinderCount;
	//OgreBites::Label* mInfoLabel;

protected:

    virtual void createScene(void);
	virtual void createViewports(void);




	    CEGUI::OgreRenderer* mRenderer;
 
   
	//virtual void createFrameListener(void);
 
 
    // Ogre::FrameListener
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
 
    // OIS::KeyListener
    //virtual bool keyPressed( const OIS::KeyEvent &arg );
    //virtual bool keyReleased( const OIS::KeyEvent &arg );
    //// OIS::MouseListener
    //virtual bool mouseMoved( const OIS::MouseEvent &arg );
    //virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    //virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	
    bool visualise(const CEGUI::EventArgs &e);
    bool quit(const CEGUI::EventArgs &e);

	
};

#endif // #ifndef __DigitalForensicsVisualisation_h_
