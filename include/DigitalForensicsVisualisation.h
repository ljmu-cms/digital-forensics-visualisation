#ifndef __DigitalForensicsVisualisation_h_
#define __DigitalForensicsVisualisation_h_


#include "BaseApplication.h"
#include "SampleListener.h"
#include <stdio.h>
#include <stdlib.h>
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>
#include "Entity.h"
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "../res/resource.h"
#endif


class DigitalForensicsVisualisation : public BaseApplication
{

private:

	Ogre::SceneNode* bonesArr[20]; //array that stores the addresses of the bones
	SampleListener leapSampleListener;
	Controller leapController;
	bool processUnbufferedInput(const Ogre::FrameEvent& evt);
	Ogre::SceneNode* handNode;
	Ogre::SceneNode *palmNode;
	Ogre::SceneNode* fingersNode;
	Ogre::SceneNode* filesNode;
	float previousFramePitch;
	float previousFrameYaw;
	float previousFrameRoll;
	bool handOrientationFlag;

	Ogre::ManualObject* const cube(bool isFrustum);
	Ogre::ManualObject* const pyramid();
	Ogre::ManualObject* const cylinder();
	

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
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	
};

#endif // #ifndef __DigitalForensicsVisualisation_h_
