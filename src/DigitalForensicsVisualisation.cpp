#include "DigitalForensicsVisualisation.h"
#include <sstream>
#include <math.h>
//mysql
#include "mysql.h"
#include "my_global.h"


#define SERVER "localhost"
#define USER "root"
#define PASSWORD "081293"
#define DATABASE "test"
//end mysql

//file system
#include "dirent.h" 
#include <sys/stat.h>
#include <fstream>
#include <time.h>
#define is_regular 0100000
//end filesystem

using std::cout;
using std::endl;

char* query;

unsigned long long count;


MYSQL *mysqlPtr; // Create a pointer to the MySQL instance

DigitalForensicsVisualisation app;  // creating application object here!!

void scan (const char* directory)
{
	DIR* pdir = opendir (directory); 
    
    dirent *pent = NULL;

    

    while (pent = readdir (pdir))
    {
		
		if (pent->d_name[0] == '.')
			continue;
		
        //cout << pent->d_name << " - " << pent->d_namlen << endl;
        char* fileName = (char*) malloc(strlen(directory) + strlen(pent->d_name) + 2);
        strcpy_s(fileName, strlen(directory) + 1 ,directory);
        strcat(fileName,  "/");
        strcat(fileName, pent->d_name);

        struct stat st;
        stat(fileName, &st);
        
		if (st.st_mode & is_regular)
		{
			//cout << count << fileName << endl; //get the file

			//OutputDebugString(fileName);
			//OutputDebugString("\n");

			
			app.e.name = pent->d_name;
			app.e.directory = directory;
			app.e.size = st.st_size;
			app.e.full_name = fileName;

			app.e.read_permission = (st.st_mode & 00400) ? 1 : 0;
			app.e.write_permission = (st.st_mode & 00200) ? 1 : 0;
			app.e.execute_permission = (st.st_mode & 00100) ? 1 : 0;
			
			app.e.creation_time = ctime(&st.st_ctime);
			app.e.modification_time = ctime(&st.st_mtime);
			app.e.execution_time = ctime(&st.st_atime);
			
			app.e.setExtension();

			count++;
			//mysqlPtr = mysql_init(NULL); // Initialise the instance
			//mysqlPtr=mysql_real_connect(mysqlPtr,SERVER,USER,PASSWORD,DATABASE,0,NULL,0);
			////
			//*query = NULL;

			std::stringstream ss;
			ss << "INSERT INTO `test`.`file` ( `name`, `directory`, `size`, `extension`, `read_permission`, `write_permission`,`execute_permission`, `c_time`,`a_time`,`m_time`) VALUES ("<< "'"
				<< app.e.name << "', " << "'"
				<< app.e.directory << "', " << "'" << app.e.size << "', " << "'" << app.e.extension << "', " << "'"
				<< app.e.read_permission << "', " << "'" << app.e.write_permission << "', " << "'" << app.e.execute_permission << "', " << "'" 
				<< app.e.creation_time << "', " << "'" << app.e.execution_time << "', " << "'" << app.e.modification_time << "'); ";

			std::string insertQuery = ss.str();

			mysql_query(mysqlPtr, insertQuery.c_str());
			
		}
		else
		{
			scan (fileName);
		}

		free (fileName);
        
    }
}


Ogre::Vector3 toVector (Vector leapVector)
{
	return Ogre::Vector3(leapVector.x, leapVector.y, leapVector.z);
}

	
	

//-------------------------------------------------------------------------------------
Ogre::ManualObject* const DigitalForensicsVisualisation::cube (bool isFrustum = false)
{
	char* name = (char*) malloc (32);
	sprintf(name, "cube%d", app.cubeCount++);
	Ogre::ManualObject* cube = mSceneMgr->createManualObject(name);

	cube->begin("BaseWhite", Ogre::RenderOperation::OT_TRIANGLE_LIST);

	Ogre::Vector3 centre(50, 50, 50);
	Ogre::Vector3 position;
	//bottom points
	//0
	position.z = position. y = 0; 
	position.x = 0;
	Ogre::Vector3 normal = (position - centre);	normal.normalise();
	cube->position(position);
	cube->normal(normal);
	//1
	position.x = 100; // 100, 0, 0
	normal = (position - centre);	normal.normalise();
	cube->position(position);
	cube->normal(normal);
	//2
	position.z = 100; position.x = 0; 
	normal = (position - centre);	normal.normalise();
	cube->position(position);
	cube->normal(normal);
	////3
	position.x = 100;
	normal = (position - centre); normal.normalise();
	cube->position(position);
	cube->normal(normal);
	////top points
	////4
	position.y = 100;
	position.x = isFrustum ? 25 : 0;
	position.z = isFrustum ? 25 : 0; 
	normal = (position - centre);	normal.normalise();
	cube->position(position);
	cube->normal(normal);
	////5
	position.x = isFrustum ? 75 : 100;
	normal = (position - centre);	normal.normalise();
	cube->position(position);
	cube->normal(normal);
	////6
	position.x = isFrustum ? 25 : 0; position.z = isFrustum ? 75 : 100;
	normal = (position - centre);	normal.normalise();
	cube->position(position);
	cube->normal(normal);
	////7
	position.x = isFrustum ? 75 : 100;
	normal = (position - centre); normal.normalise();
	cube->position(position);
	cube->normal(normal);

	
	cube->triangle(0,1,2);//NEGATIVE
	cube->triangle(3,2,1);

	cube->triangle(6,5,4);//POSITIVE
	cube->triangle(5,6,7);

	cube->triangle(0,2,4);
	cube->triangle(6,4,2);
	
	cube->triangle(5,3,1); 
	cube->triangle(3,5,7);
	
	cube->triangle(2,3,6);
	cube->triangle(7,6,3);
	
	cube->triangle(4,1,0);
	cube->triangle(1,4,5);


	cube->end();
	free (name);
	return cube;


}

//-------------------------------------------------------------------------------------
DigitalForensicsVisualisation::DigitalForensicsVisualisation(void)
{
	previousFramePitch = previousFrameYaw = previousFrameRoll = 0;
	handOrientationFlag = false;
	cubeCount = 0;
	
	
	
}
//-------------------------------------------------------------------------------------
DigitalForensicsVisualisation::~DigitalForensicsVisualisation(void)
{
	
}

//-------------------------------------------------------------------------------------
void DigitalForensicsVisualisation::createScene(void)
{
	query = (char*) malloc (4096);
	
	//MYSQL *connect; // Create a pointer to the MySQL instance
    mysqlPtr = mysql_init(NULL); // Initialise the instance
    /* This If is irrelevant and you don't need to show it. I kept it in for Fault Testing.*/
    if(!mysqlPtr)    /* If instance didn't initialize say so and exit with fault.*/
    {
        fprintf(stderr,"MySQL Initialization Failed");
        
    }
    /* Now we will actually connect to the specific database.*/
 
    mysqlPtr=mysql_real_connect(mysqlPtr,SERVER,USER,PASSWORD,DATABASE,0,NULL,0);
    /* Following if statements are unneeded too, but it's worth it to show on your
    first app, so that if your database is empty or the query didn't return anything it
    will at least let you know that the connection to the mysql server was established. */
 
    //if(mysqlPtr){
    //    printf("Connection Succeeded\n");
    //}
    //else{
    //    printf("Connection Failed!\n");
    //}
 //   MYSQL_RES *res_set; /* Create a pointer to recieve the return value.*/
 //   MYSQL_ROW row;  /* Assign variable for rows. */
 //   //mysql_query(mysqlPtr, "INSERT INTO `test`.`table1` (`a`, `b`, `c`) VALUES ('5', 't', 'y');");
	//mysql_query(mysqlPtr,"SELECT * FROM file");

 //   /* Send a query to the database. */
 //   unsigned int i = 0; /* Create a counter for the rows */
 //
 //   res_set = mysql_store_result(mysqlPtr); /* Receive the result and store it in res_set */
 //
 //   unsigned int numrows = mysql_num_rows(res_set); /* Create the count to print all rows */
 //
 //   /* This while is to print all rows and not just the first row found, */
	//char* debug = (char*) malloc (32);
 //   while ((row = mysql_fetch_row(res_set)) != NULL){
 //       sprintf(debug, "!!%s!!%s!!%s\n",row[i+1] != NULL ?
	//		row[i+1] : "NULL", row[i+2], row[i+3]);
	//	OutputDebugString(debug);
	//	/* Print the row data */
 //   }
	//free (debug);
 //   

	//mysql_close(mysqlPtr);   /* Close and shutdown */



	// leap controller and listener objects
	leapController.addListener(leapSampleListener);
	leapController.setPolicyFlags(Leap::Controller::POLICY_BACKGROUND_FRAMES);
	leapSampleListener.onFrame(leapController);
	// end leap



	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.25, 0.25, 0.25));
 
	//hand
	handNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("HandNode");
	handNode->setPosition (0, -300, -500);
	// palm
	Ogre::Entity* palm = mSceneMgr->createEntity("palm","cube.mesh");
	palmNode = handNode->createChildSceneNode("PalmNode");
	
	palmNode->attachObject(palm);
	palmNode->scale(.1,.1,.1);
	
	
	// fingers
	//Ogre::Entity* fingers = new Ogre::Entity[5];
	fingersNode = handNode->createChildSceneNode("FingersNode");
	
	Ogre::Entity* bone;
	char str[3];

	for (int i = 0; i < 20; ++i)
	{
		sprintf(str, "%d", i);
		bone = mSceneMgr->createEntity(str,"cube.mesh");
		bonesArr[i] = fingersNode->createChildSceneNode(str);
		bonesArr[i]->attachObject(bone);
		bonesArr[i]->scale(.1,.1,.1);

	
	
	}


 
    Ogre::Light* pointLight = mSceneMgr->createLight("pointLight");
    pointLight->setType(Ogre::Light::LT_POINT);
    pointLight->setPosition(Ogre::Vector3(250, 150, 250));
    pointLight->setDiffuseColour(Ogre::ColourValue::White);
    pointLight->setSpecularColour(Ogre::ColourValue::White);



	//Ogre::ManualObject* Circle = mSceneMgr->createManualObject("Circle");
	//Ogre::SceneNode* CircleNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("CircleNode");

	//Circle->begin("BaseWhite", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	const float accuracy = 45;
	const float radius = 2000;
	const float thickness = 1955;
	unsigned int index = 0;
	
	//
	//for (float theta = 0; theta <= Ogre::Math::PI * 1.8; theta += Ogre::Math::PI / (accuracy)) 
	//{
	//	// TL: top-left, BR: bottom-right
 ///*TL*/ Circle->position(radius * cos(theta), 0, radius * sin(theta)); 
	//	Circle->normal(radius * cos(theta), 90, radius * sin(theta)); 
 ///*TR*/ Circle->position(radius * cos(theta - Ogre::Math::PI / accuracy),0, radius * sin(theta - Ogre::Math::PI / accuracy));
	//	Circle->normal(radius * cos(theta - Ogre::Math::PI / accuracy),90, radius * sin(theta - Ogre::Math::PI / accuracy));
 ///*BR*/ Circle->position((radius - thickness) * cos(theta - Ogre::Math::PI / accuracy), 0, (radius - thickness) * sin(theta - Ogre::Math::PI / accuracy));
	//	Circle->normal((radius - thickness) * cos(theta - Ogre::Math::PI / accuracy), 90, (radius - thickness) * sin(theta - Ogre::Math::PI / accuracy));
 ///*BL*/ Circle->position((radius - thickness) * cos(theta), 0, (radius - thickness) * sin(theta));
	//	Circle->normal((radius - thickness) * cos(theta), 90, (radius - thickness) * sin(theta));

	//	
	//	//Circle->triangle(index, index + 1, index + 3);
	//	//Circle->triangle(index + 1, index + 2, index + 3);

	//	Circle->index (index);
	//	Circle->index (index + 1);
	//	Circle->index (index + 3);
	//	Circle->index (index + 1);
	//	Circle->index (index + 2);
	//	Circle->index (index + 3);

	//	//in order to make object both-side visible, indexes of each triangle also must be defined in inverse order
	//	Circle->index (index + 2);
	//	Circle->index (index + 1);
	//	Circle->index (index + 3);
	//	Circle->index (index + 1);
	//	Circle->index (index);
	//	Circle->index (index + 3);

	//	//triangle and quad are just easier shortcuts of index		

	//	//Circle->quad(index, index + 1, index + 2, index + 3); 
	//	index += 4;
	//	//break;





	//}
	//Circle->end();
	//CircleNode->attachObject(Circle);
	//CircleNode->setPosition (0,-300,-500);


	float distFromCentre = radius - thickness;
	int itemIndex = 0;
	char str2[50];
	filesNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("FilesNode");
	filesNode->setPosition (0, -300, -500);

	for (float y = radius - thickness; y <= radius; y += Ogre::Math::PI * 6)
	{
			


		for (float theta = 0; theta <= Ogre::Math::PI * 1.8; theta += Ogre::Math::PI / (distFromCentre / 6)) 
		{
				
			sprintf(str2, "file%d", itemIndex++);
			Ogre::SceneNode* fsn = filesNode->createChildSceneNode(str2);
			fsn->attachObject(cube(true));
			fsn->setPosition(y * cos(theta), 0, y * sin(theta));
			fsn->scale(.09,.09,.09);
			OutputDebugString(str2);
			OutputDebugString("\n");
		}

		distFromCentre += Ogre::Math::PI * 6;

	}
		
	

	count = 0;
	const char* dir = "C:/";
	//scan(dir);


	


	char* dummy = (char*) malloc (64);
	sprintf(dummy,"%d",count);
	OutputDebugString(dummy);
	free (query);
	mysql_close(mysqlPtr); 
}

//-------------------------------------------------------------------------------------
void DigitalForensicsVisualisation::createViewports(void)
{
	    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(.2,.1,.3));
    // Alter the camera aspect ratio to match the viewport
	mCamera->setFarClipDistance(1000);
	
	mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));  
}
 


//-------------------------------------------------------------------------------------

bool DigitalForensicsVisualisation::processUnbufferedInput(const Ogre::FrameEvent& evt)
{


	//leap
	static Frame frame;
	frame = leapController.frame();
	

	
	Leap::Hand rightMost = frame.hands().rightmost();

		
		
	previousFramePitch = rightMost.direction().pitch() * RAD_TO_DEG;
	previousFrameYaw = rightMost.direction().yaw() * RAD_TO_DEG;
	previousFrameRoll = rightMost.palmNormal().roll() * RAD_TO_DEG;

	
	
	
	if (!frame.hands().isEmpty() && !handOrientationFlag) 
	{
		
		palmNode->resetOrientation();
		handOrientationFlag = true;	
	}
	else if (handOrientationFlag && frame.hands().isEmpty() )
	{
		handOrientationFlag = false;
	}

	if (!frame.hands().isEmpty())
	{
		

		Leap::Hand rightMost = frame.hands().rightmost();
		palmNode->setPosition(rightMost.palmPosition().x, rightMost.palmPosition().y, rightMost.palmPosition().z); // between 100 and 250
		
		

		float pitchValue = rightMost.direction().pitch() * RAD_TO_DEG;
		palmNode->pitch((Ogre::Radian) (pitchValue - previousFramePitch) );

		float rollValue = rightMost.palmNormal().roll() * RAD_TO_DEG;
		palmNode->roll((Ogre::Radian) (rollValue - previousFrameRoll) );

		float yawValue = rightMost.direction().yaw() * RAD_TO_DEG;
		palmNode->yaw((Ogre::Radian) (yawValue - previousFrameYaw) );

		/*char* dummy = (char*) malloc(64);
		sprintf (dummy, "pitch: %f, yaw: %f, roll: %f\n", pitchValue, yawValue, rollValue); 
		OutputDebugString (dummy);
		free(dummy);*/
 
		previousFramePitch = rightMost.direction().pitch() * RAD_TO_DEG;
		previousFrameYaw = rightMost.direction().yaw() * RAD_TO_DEG;
		previousFrameRoll = rightMost.palmNormal().roll() * RAD_TO_DEG;


		// Get fingers
		static FingerList fingers; 
		fingers = rightMost.fingers();
		int i = 0; //between 0 and 19 (finger bones)
		
		for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) 
		{
			
			static Finger finger;
			finger = *fl;
			
			
		  /*char* dummy = (char*) malloc(128);
			sprintf (dummy, "finger id: %d, length: %f, width: %f\n", finger.id(), finger.length(), finger.width()); 
			OutputDebugString (dummy);
			free(dummy);*/

			// Get finger bones
			for (int b = 0; b < 4; ++b) 
			{
				static Bone::Type boneType;
				boneType = static_cast<Bone::Type>(b);
				static Bone bone;
				bone = finger.bone(boneType);
				bonesArr[i++]->setPosition(bone.center().x, bone.center().y, bone.center().z);

			}
		}

		// to detect fist
		/*char* dummy = (char*) malloc(8);
		sprintf (dummy, "%f\n", rightMost.grabStrength()); 
		OutputDebugString(dummy);*/


		// to detect open and closed hand
		Ogre::Vector3 indexTip = toVector(rightMost.fingers()[1].tipPosition());
		Ogre::Vector3 pinkyTip = toVector(rightMost.fingers()[3].tipPosition());

	
		float angle = std::abs(indexTip.x - pinkyTip.x);
		angle /= rightMost.fingers()[1].length(); // to normalise 
		
		char* dummy = (char*) malloc(8);
		sprintf (dummy, "%f\n", angle); 
		OutputDebugString(dummy);
	}



 
   

    return true;
}
//-------------------------------------------------------------------------------------
bool DigitalForensicsVisualisation::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    bool ret = BaseApplication::frameRenderingQueued(evt);
 
    if (!processUnbufferedInput(evt)) return false;
 
    return ret;
}
//-------------------------------------------------------------------------------------


#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        //#################### application object is used to be created here!!!!!!!!!!!!!!!!!

        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif