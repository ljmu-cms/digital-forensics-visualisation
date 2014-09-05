#ifndef __DigitalForensicsVisualisation_h_
#define __DigitalForensicsVisualisation_h_



#include "MovableText.h"
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

struct GUIElements
{

	//GUI Elements
	CEGUI::Window *sheet;
	//static text fields
	CEGUI::Tooltip* tt1;
	CEGUI::Tooltip* tt2;
	//editboxes for text inputs
	CEGUI::Editbox* d1;
	CEGUI::Editbox* d2;
	//radio buttons
	CEGUI::RadioButton* rb1;
	CEGUI::RadioButton* rb2;
	CEGUI::RadioButton* rb3;
	//check box
	CEGUI::ToggleButton* tb;
	//used as button
	CEGUI::Window* visualise_button;

	CEGUI::ProgressBar* progress_bar;

		

};

class DigitalForensicsVisualisation : public BaseApplication
{

private:

	BST <ColorMap> colorTree;
	Ogre::SceneNode* bonesArr[20]; //array that stores the addresses of the bones
	Ogre::MovableText* textArr[5000000];
	unsigned long int textArrIndex;
	SampleListener leapSampleListener;
	Controller leapController;
	bool updateFrame(const Ogre::FrameEvent& evt);
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

	Ogre::ManualObject* const rectangle();
	


	GUIElements gui;

	void initGUIElements();

	void beginProgress();
	void endProgress();

	std::string parseDateInput(const char*);
	const std::string buildQuery();
	const char* const orderIn();
	const char selectedRb();

public:

	Entity e;
    DigitalForensicsVisualisation(void);
    virtual ~DigitalForensicsVisualisation(void);
	unsigned long long int rectCount;
	unsigned long long int cubeCount;
	unsigned long long int pyramidCount;
	unsigned long long int cylinderCount;
	std::string dbg2;
	//OgreBites::Label* mInfoLabel;

protected:

    virtual void createScene(void);
	virtual void createViewports(void);

	CEGUI::OgreRenderer* mRenderer;
 
   
	//virtual void createFrameListener(void);
 
 
    // Ogre::FrameListener
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
 
    // OIS::KeyListener and CEGUI listener
    virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );
    //// OIS::MouseListener
    virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	
    bool visualise(const CEGUI::EventArgs &e);
	bool rb1StateChanged(const CEGUI::EventArgs &e);
	bool rb2StateChanged(const CEGUI::EventArgs &e);
	bool rb3StateChanged(const CEGUI::EventArgs &e);
	
};

#endif // #ifndef __DigitalForensicsVisualisation_h_
