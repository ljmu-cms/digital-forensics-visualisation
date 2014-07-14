#ifndef __DigitalForensicsVisualisation_h_
#define __DigitalForensicsVisualisation_h_


#include "BaseApplication.h"
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "../res/resource.h"
#endif


class DigitalForensicsVisualisation : public BaseApplication
{

private:

	bool processUnbufferedInput(const Ogre::FrameEvent& evt);	

public:
    DigitalForensicsVisualisation(void);
    virtual ~DigitalForensicsVisualisation(void);
	//OgreBites::Label* mInfoLabel;

protected:
    virtual void createScene(void);
	virtual void createViewports(void);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	
};

#endif // #ifndef __DigitalForensicsVisualisation_h_
