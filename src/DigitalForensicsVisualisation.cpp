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




MYSQL *mysqlPtr; // Create a pointer to the MySQL instance

DigitalForensicsVisualisation app;  // creating application object here!!




void mysqlConn ()
{
	
    mysqlPtr = mysql_init(NULL); // Initialise the instance
    /* This If is irrelevant and you don't need to show it. I kept it in for Fault Testing.*/
    if(!mysqlPtr)    /* If instance didn't initialize say so and exit with fault.*/
    {
        fprintf(stderr,"MySQL Initialization Failed");
        
    }
    /* Now we will actually connect to the specific database.*/
 
    mysqlPtr=mysql_real_connect(mysqlPtr,SERVER,USER,PASSWORD,DATABASE,0,NULL,0);
 
    //if(mysqlPtr){
    //    printf("Connection Succeeded\n");
    //}
    //else{
    //    printf("Connection Failed!\n");
    //}

    
}

MYSQL_RES* mysqlExecute(const char* query)
{
	//mysql_query(mysqlPtr, "INSERT INTO `test`.`table1` (`a`, `b`, `c`) VALUES ('5', 't', 'y');");
	mysql_query(mysqlPtr,query);

    return mysql_store_result(mysqlPtr); /* Receive the result and store it in res_set */
 
}


void mysqlDisconnect()
{
	mysql_close(mysqlPtr);
}

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
			Date d;
			
			app.e.name = pent->d_name;
			app.e.directory = directory;
			app.e.size = st.st_size;
			app.e.full_name = fileName;

			app.e.write_permission = (st.st_mode & 00200) ? 1 : 0;
			app.e.access_permission = (st.st_mode & 00100) ? 1 : 0;
			
			app.e.creation_time = ctime(&st.st_ctime);
			app.e.access_time = ctime(&st.st_atime);
			app.e.modification_time = ctime(&st.st_mtime);
			
			
			app.e.c = app.e.parseDate(app.e.creation_time);
			app.e.a = app.e.parseDate(app.e.access_time);
			app.e.m = app.e.parseDate(app.e.modification_time);

			app.e.setExtension();

			


			std::stringstream ss;
			ss << "INSERT INTO `test`.`file` ( `name`, `directory`, `size`, `extension`, `write_permission`,`access_permission`, `c_time`,`a_time`,`m_time`,`c`,`a`,`m`) VALUES ("<< "'"
				<< app.e.name << "', " << "'"
				<< app.e.directory << "', " << "'" << app.e.size << "', " << "'" << app.e.extension << "', " << "'" 
				<< app.e.write_permission << "', " << "'" << app.e.access_permission << "', " << "'" 
				<< app.e.creation_time << "', " << "'" << app.e.access_time << "', " << "'" << app.e.modification_time << "', " << "'" << app.e.c << "', " << "'" << app.e.a << "', " << "'" << app.e.m  <<"'); ";

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
Ogre::ManualObject* const DigitalForensicsVisualisation::cylinder(ColorMap cm)
{
	Ogre::ColourValue cv;
	cv.r = cm.r; cv.g = cm.g; cv.b = cm.b; cv.a = 0.65;




	
	
	char* name = (char*) malloc (32);
	sprintf(name, "cylinder%d", app.cylinderCount++);
	Ogre::ManualObject* cylinder = mSceneMgr->createManualObject(name);

	cylinder->begin("MyMaterial1", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	const float accuracy = 20;
	const float radius = 50;
	unsigned int index = 2;
	
	
	cylinder->position(50,0,-50);
	cylinder->colour(cv);
	cylinder->normal(50,90,-50);

	cylinder->position(50,-90,-50);
	cylinder->colour(cv);
	cylinder->normal(50,-91,-50);


	for (float theta = 0; theta <= Ogre::Math::PI * 2; theta += Ogre::Math::PI / (accuracy)) 
	{
		// TL: top-left, BR: bottom-right
 /*TL*/ cylinder->position(radius * cos(theta) + 50, 0, radius * sin(theta) - 50);
		cylinder->colour(cv); 
		cylinder->normal(radius * cos(theta) + 50, 90, radius * sin(theta) - 50); 
 /*TR*/ cylinder->position(radius * cos(theta - Ogre::Math::PI / accuracy) + 50, 0, radius * sin(theta - Ogre::Math::PI / accuracy) - 50);
		cylinder->colour(cv);
		cylinder->normal(radius * cos(theta - Ogre::Math::PI / accuracy) + 50, 90, radius * sin(theta - Ogre::Math::PI / accuracy) - 50);
	
 /*TL*/ cylinder->position(radius * cos(theta) + 50, -90, radius * sin(theta) - 50); 
		cylinder->colour(cv);
		cylinder->normal(radius * cos(theta) + 50, -91, radius * sin(theta) - 50); 
 /*TR*/ cylinder->position(radius * cos(theta - Ogre::Math::PI / accuracy) + 50, -90, radius * sin(theta - Ogre::Math::PI / accuracy) - 50);
		cylinder->colour(cv);
		cylinder->normal(radius * cos(theta - Ogre::Math::PI / accuracy) + 50, -91, radius * sin(theta - Ogre::Math::PI / accuracy) - 50);

		
		cylinder->triangle(index, index + 1, 0);
		cylinder->triangle(1, index + 3, index + 2);
		cylinder->triangle(index, index + 2, index + 3);
		cylinder->triangle(index + 3, index + 1, index);

		index += 4;

	}

	cylinder->end();
	
	free(name);
	return cylinder;



}
//-------------------------------------------------------------------------------------
Ogre::ManualObject* const DigitalForensicsVisualisation::pyramid(ColorMap cm)
{
	Ogre::ColourValue cv;
	cv.r = cm.r; cv.g = cm.g; cv.b = cm.b; cv.a = 0.65;
	

	char* name = (char*) malloc (32);
	sprintf(name, "pyramid%d", app.pyramidCount++);
	Ogre::ManualObject* pyramid = mSceneMgr->createManualObject(name);

	pyramid->begin("MyMaterial1", Ogre::RenderOperation::OT_TRIANGLE_LIST);

	Ogre::Vector3 normal;

	pyramid->position(0,0,0);    // 0
	pyramid->colour(cv);
	normal.x = normal.y = normal.z = -50;
	normal.normalise();
	pyramid->normal(normal);

	pyramid->position(100,0,0);  // 1
	pyramid->colour(cv);
	normal.x = 50; normal.y = normal.z = -50; 
	normal.normalise();
	pyramid->normal(normal);

	pyramid->position(50,0,100); // 2
	pyramid->colour(cv);
	normal.x = 0; normal.y = -50; normal.z = 50;
	normal.normalise();
	pyramid->normal(normal);

	pyramid->position(0,100,0);  // 3
	pyramid->colour(cv);
	normal.x = normal.z = -50;
	normal.y = 50;
	normal.normalise();
	pyramid->normal(normal);

	pyramid->position(100,100,0);  // 4
	pyramid->colour(cv);
	normal.x = normal.y = 50; normal.z = -50; 
	normal.normalise();
	pyramid->normal(normal);

	pyramid->position(50,100,100); // 5
	pyramid->colour(cv);
	normal.x = 0; normal.y = normal.z = 50;
	normal.normalise();
	pyramid->normal(normal);

	pyramid->triangle(0,1,2);
	pyramid->triangle(5,4,3);
	
	pyramid->triangle(3,1,0);
	pyramid->triangle(1,3,4);

	pyramid->triangle(0,2,3);
	pyramid->triangle(5,3,2);

	pyramid->triangle(4,2,1);
	pyramid->triangle(2,4,5);

	

	pyramid->end();
	free (name);
	return pyramid;




}

//-------------------------------------------------------------------------------------
Ogre::ManualObject* const DigitalForensicsVisualisation::cube (bool isFrustum, ColorMap cm)
{
	Ogre::ColourValue cv;
	cv.r = cm.r; cv.g = cm.g; cv.b = cm.b; cv.a = 0.65;


	char* name = (char*) malloc (32);
	sprintf(name, "cube%d", app.cubeCount++);
	Ogre::ManualObject* cube = mSceneMgr->createManualObject(name);

	cube->begin("MyMaterial1", Ogre::RenderOperation::OT_TRIANGLE_LIST);

	Ogre::Vector3 centre(50, 50, 50);
	Ogre::Vector3 position;
	//bottom points
	//0
	position.z = position. y = 0; 
	position.x = 0;
	Ogre::Vector3 normal = (position - centre);	normal.normalise();
	cube->position(position);
	cube->colour(cv);
	cube->normal(normal);
	//1
	position.x = 100; // 100, 0, 0
	normal = (position - centre);	normal.normalise();
	cube->position(position);
	cube->colour(cv);
	cube->normal(normal);
	//2
	position.z = 100; position.x = 0; 
	normal = (position - centre);	normal.normalise();
	cube->position(position);
	cube->colour(cv);
	cube->normal(normal);
	////3
	position.x = 100;
	normal = (position - centre); normal.normalise();
	cube->position(position);
	cube->colour(cv);
	cube->normal(normal);
	////top points
	////4
	position.y = 100;
	position.x = isFrustum ? 25 : 0;
	position.z = isFrustum ? 25 : 0; 
	normal = (position - centre);	normal.normalise();
	cube->position(position);
	cube->colour(cv);
	cube->normal(normal);
	////5
	position.x = isFrustum ? 75 : 100;
	normal = (position - centre);	normal.normalise();
	cube->position(position);
	cube->colour(cv);
	cube->normal(normal);
	////6
	position.x = isFrustum ? 25 : 0; position.z = isFrustum ? 75 : 100;
	normal = (position - centre);	normal.normalise();
	cube->position(position);
	cube->colour(cv);
	cube->normal(normal);
	////7
	position.x = isFrustum ? 75 : 100;
	normal = (position - centre); normal.normalise();
	cube->position(position);
	cube->colour(cv);
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
	// to establish mysql connection
	mysqlConn();
	previousPosition.x = previousFrameYaw = previousFrameRoll = 0;
	previousPosition.y = -300;
	previousPosition.z = -500;
	handOrientationFlag = false;
	cubeCount = pyramidCount = cylinderCount = 0;
	textArrIndex = 0;


	
	
	
	
}
//-------------------------------------------------------------------------------------
DigitalForensicsVisualisation::~DigitalForensicsVisualisation(void)
{
	mysqlDisconnect();
}

//-------------------------------------------------------------------------------------
void DigitalForensicsVisualisation::createScene(void)
{
	


#pragma region objects


	// leap controller and listener objects
	leapController.addListener(leapSampleListener);
	leapController.setPolicyFlags(Leap::Controller::POLICY_BACKGROUND_FRAMES);
	leapSampleListener.onFrame(leapController);
	// end leap



	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.05, 0.05, 0.05));
	
	//hand
	handNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("HandNode");
	handNode->setPosition (0, -300, -500);
	
	// palm
	Ogre::Entity* palm = mSceneMgr->createEntity("sphere","cube.mesh");
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
		bone = mSceneMgr->createEntity(str,"sphere.mesh");
		bonesArr[i] = fingersNode->createChildSceneNode(str);
		bonesArr[i]->attachObject(bone);
		bonesArr[i]->scale(.1,.1,.1);

	
	
	}


 
    pointLight = mSceneMgr->createLight("pointLight");
	pointLight->setType(Ogre::Light::LT_POINT);
    pointLight->setPosition(Ogre::Vector3(250, 150, 250));
    pointLight->setDiffuseColour(Ogre::ColourValue::White);
    pointLight->setSpecularColour(Ogre::ColourValue::White);
	
	handNode->setVisible(false);
	
#pragma endregion objects


	


#pragma region surface
	//	Ogre::ManualObject* Circle = mSceneMgr->createManualObject("Circle");
	//Ogre::SceneNode* CircleNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("CircleNode");

	//Circle->begin("BaseWhite", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	//
	//
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
	//CircleNode->setPosition (0,-400,-500);

#pragma endregion surface


	//const char* dir = "C:/";
	//scan(dir);

#pragma region initialise_gui_elements

	mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();

	CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");	

	CEGUI::SchemeManager::getSingleton().createFromFile("AlfiskoSkin.scheme");
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("AlfiskoSkin/MouseArrow");
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setVisible( true );

	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	sheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");
	
	
	tt1 = static_cast<CEGUI::Tooltip*> (wmgr.createWindow("AlfiskoSkin/Label","tt1"));
	tt1->setPosition(CEGUI::UVector2(CEGUI::UDim(-0.021,0),CEGUI::UDim(0.075,0)));
	tt1->setSize(CEGUI::USize(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.025, 0)));
	tt1->setText("Accessed files between:");
	sheet->addChild(tt1);

	d1 = static_cast<CEGUI::Editbox*> (wmgr.createWindow("AlfiskoSkin/Editbox","eb1"));
	d1->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0),CEGUI::UDim(0.105,0)));
	d1->setSize(CEGUI::USize(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.025, 0)));
	d1->setText("02-04-2011");
	sheet->addChild(d1);

	d2 = static_cast<CEGUI::Editbox*> (wmgr.createWindow("AlfiskoSkin/Editbox","eb2"));
	d2->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0),CEGUI::UDim(0.135,0)));
	d2->setSize(CEGUI::USize(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.025, 0)));
	d2->setText("08-12-2014");
	sheet->addChild(d2);

	tt2 = static_cast<CEGUI::Tooltip*> (wmgr.createWindow("AlfiskoSkin/Label","tt2"));
	tt2->setPosition(CEGUI::UVector2(CEGUI::UDim(-0.071,0),CEGUI::UDim(0.170,0)));
	tt2->setSize(CEGUI::USize(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.025, 0)));
	tt2->setText("Sort files by:");
	sheet->addChild(tt2);

	rb1 = static_cast<CEGUI::RadioButton*> (wmgr.createWindow("AlfiskoSkin/RadioButton","rb1"));
	rb1->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0),CEGUI::UDim(0.2,0)));
	rb1->setSize(CEGUI::USize(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.025, 0)));
	rb1->setText("Last Access Date");
	rb1->setGroupID(1);
	rb1->setID(1);
	rb1->setSelected(true);
	rb1->setVisible(true);
	sheet->addChild(rb1);

	rb2 = static_cast<CEGUI::RadioButton*> (wmgr.createWindow("AlfiskoSkin/RadioButton","rb2"));
	rb2->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0),CEGUI::UDim(0.225,0)));
	rb2->setSize(CEGUI::USize(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.025, 0)));
	rb2->setText("Last Modification Date");
	rb2->setGroupID(1);
	rb2->setID(2);
	rb2->setSelected(false);
	rb2->setVisible(true);
	sheet->addChild(rb2);

	rb3 = static_cast<CEGUI::RadioButton*> (wmgr.createWindow("AlfiskoSkin/RadioButton","rb3"));
	rb3->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0),CEGUI::UDim(0.250,0)));
	rb3->setSize(CEGUI::USize(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.025, 0)));
	rb3->setText("Creation Date");
	rb3->setGroupID(1);
	rb3->setID(3);
	rb3->setSelected(false);
	rb3->setVisible(true);
	sheet->addChild(rb3);

	//toggle button 
	tb = static_cast<CEGUI::ToggleButton*> (wmgr.createWindow("AlfiskoSkin/Checkbox","toggle"));
	tb->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0),CEGUI::UDim(0.3,0)));
	tb->setSize(CEGUI::USize(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.025, 0)));
	tb->setText("Sort in descending order");
	sheet->addChild(tb);

	//visualise button
	visualise_button = wmgr.createWindow("AlfiskoSkin/Button", "visualise_button");
	visualise_button->setText("Visualise!");
	visualise_button->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0),CEGUI::UDim(0.4,0)));
	visualise_button->setSize(CEGUI::USize(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.05, 0)));
	visualise_button->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&DigitalForensicsVisualisation::visualise, this));
	sheet->addChild(visualise_button);

	progress_bar = static_cast<CEGUI::ProgressBar*> (wmgr.createWindow("AlfiskoSkin/ProgressBar","progress_bar"));
	progress_bar->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5,0),CEGUI::UDim(0.4,0)));
	progress_bar->setSize(CEGUI::USize(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.1, 0)));
	sheet->addChild(progress_bar);
	progress_bar->setVisible(false);

	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

#pragma endregion initialise_gui_elements
	

	
}

//---------------------------------------------------------------------------------------
void DigitalForensicsVisualisation::beginProgress()
{
	progress_bar->setVisible(true);
	progress_bar->activate();
	progress_bar->setProgress(0.25f);
	visualise_button->deactivate();
}


//---------------------------------------------------------------------------------------
void DigitalForensicsVisualisation::endProgress()
{
	progress_bar->deactivate();
	progress_bar->setVisible(false);
	visualise_button->activate();
}

//---------------------------------------------------------------------------------------
std::string DigitalForensicsVisualisation::parseDateInput(const char* date)
{
	char *day, *month, *year;
	char *value = (char*) malloc (strlen (date) +2 );
	sprintf(value, "%s", date);
	char *dummy;
	dummy = strtok (value, "-"); // day
	day = dummy;
	dummy = strtok (NULL, "-"); // month
	month = dummy;
	dummy = strtok (NULL, " "); // year
	year = dummy;

	std::stringstream parsedDate;
	parsedDate << year << month << day;

	free(value);
	day = month = year = value = NULL;

	return parsedDate.str();
}

//---------------------------------------------------------------------------------------
const char DigitalForensicsVisualisation::orderBy()
{
	return (rb1->isSelected() ? 'a' : rb2->isSelected() ? 'm' : 'c');
}

//---------------------------------------------------------------------------------------
const char* const DigitalForensicsVisualisation::orderIn()
{
	return (tb->isSelected() ? "desc" : " ");
}

//---------------------------------------------------------------------------------------
const std::string DigitalForensicsVisualisation::buildQuery ()
{
	std::stringstream queryBuilder;

	queryBuilder << "SELECT * FROM file where ( a > " << parseDateInput(d1->getText().c_str()) << " and a < " 
		<< parseDateInput(d2->getText().c_str()) << " ) order by " << orderBy() << " " << orderIn() << " ;" ; 

	return queryBuilder.str();
}

//---------------------------------------------------------------------------------------
bool DigitalForensicsVisualisation::visualise(const CEGUI::EventArgs &e)
{
	try
	{
		mSceneMgr->getRootSceneNode()->removeAndDestroyChild("FilesNode");
		mSceneMgr->destroyAllMovableObjects(); //does not destroy movable texts, they are needed to be destroyed manually
		mSceneMgr->destroyAllManualObjects();
		cubeCount = pyramidCount = cylinderCount = 0;

		for(unsigned long int i = 0; i < textArrIndex; ++i)
		{
			delete textArr[textArrIndex];
			textArr[textArrIndex] = NULL;
		}

		textArrIndex = 0;
	}
	catch(std::exception& e)
	{
		//do nothing
		OutputDebugString("DID NOTHING\nn\nn\nn\nn\nn\nn\nn\nn\nn\nn\nn\nnREALLYDIDNOTHING\nn");
	}

	beginProgress();

	const float radius = 2000;
	const float thickness = 1955;
	

	try
	{

		MYSQL_RES *res_set; /* Create a pointer to recieve the return value.*/
		MYSQL_ROW row;  /* Assign variable for rows. */
 
		OutputDebugString(buildQuery().c_str());
		res_set = mysqlExecute(buildQuery().c_str());
		unsigned long long int numrows = mysql_num_rows(res_set); /* Create the count to print all rows */
 

		float distFromCentre = radius - thickness;
		unsigned long long int itemIndex = 0;
		char containerName[50]; //for container
		char fileName[50]; //for fileName
		char fontName[50]; //for fontName

		filesNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("FilesNode");
		filesNode->setPosition (0, -300, -500);

		srand(time(NULL));

	
		for (float y = radius - thickness; y <= radius; y += Ogre::Math::PI * 6)
		{

			//progress_bar->setProgress(progress_bar->getProgress() + ( 1/ (Ogre::Math::PI / (thickness * thickness))));

			for (float theta = 0; theta <= Ogre::Math::PI * 1.8; theta += Ogre::Math::PI / (distFromCentre / 6)) 
			{
				if ((row = mysql_fetch_row(res_set)) == NULL)
					break;

			
				try
				{	

					app.e.name = row[0];
					app.e.directory = row[1];
					app.e.size = std::stoull(row[2]); //for unsigned long long
					std::string ext = row[3];
			
					for (unsigned int i = 0; i < ext.length(); ++i)
					{
						ext[i] = std::toupper(ext[i]);
					}

					app.e.extension = ext;

					app.e.write_permission = std::stoi(row[4]);
					app.e.access_permission = std::stoi(row[5]);
					app.e.creation_time = row[6];
					app.e.access_time = row[7];
					app.e.modification_time = row[8];
					app.e.c = std::stoi(row[9]);
					app.e.a = std::stoi(row[10]);
					app.e.m = std::stoi(row[11]);
			
				}
				catch(std::exception& e)
				{	
					OutputDebugString(e.what());
					OutputDebugString("SIZEEXCEPTIONNN\nn\n\nn\nn\nn\nn\nn\nn\nn\nn\nn\nn\nn\nn\nn\nn\nn\n\nn\nn\nn\nn\nnn\nn\nSIZEEXCEPTIONNN\nn\n");
					app.e.creation_time = app.e.modification_time = app.e.access_time = app.e.name = "exception occured";
				
				}

				sprintf(containerName,"container_%d",itemIndex);
				sprintf(fontName,"font_%d", itemIndex);
				sprintf(fileName, "file_%d", itemIndex++);

				Ogre::SceneNode* container = filesNode->createChildSceneNode(containerName);
				Ogre::SceneNode* fsn = container->createChildSceneNode(fileName);
				Ogre::SceneNode* fontNode = container->createChildSceneNode(fontName);

			

				ColorMap cm(app.e.extension);
			

				if (colorTree.search(cm) == NULL)
					colorTree.insert(cm);
				else
					cm = colorTree.search(cm)->data;
				

		

			

				if ((app.e.write_permission == 1) && (app.e.access_permission == 1))
					fsn ->attachObject(cube(true, cm));
				else if ((app.e.write_permission == 1) && (app.e.access_permission == 0))
					fsn->attachObject(cube(false, cm));
				else if ((app.e.write_permission == 0) && (app.e.access_permission == 1))
				{
					fsn -> attachObject(pyramid(cm));
					fsn -> scale (1.3, 1.3, 1.3);
					fsn ->setPosition(Ogre::Vector3(fsn->getPosition().x + 1,fsn->getPosition().y,fsn->getPosition().z + 1));
				}
				else
				{
					fsn->attachObject(cylinder(cm));
					const Ogre::Vector3 pos =fsn->getPosition();
					fsn->pitch((Ogre::Radian) Ogre::Math::PI);
					fsn->setPosition(pos);
				}
				std::stringstream ss;
				ss << "file name: " << app.e.name << "\n\nlast access time: " << app.e.access_time << "\nmodification time: " 
					<< app.e.modification_time << "\ncreation time: " << app.e.creation_time;
				std::string s = ss.str();
			
				textArr[textArrIndex] = new Ogre::MovableText("tayyar", (s));
				textArr[textArrIndex] ->setTextAlignment(Ogre::MovableText::H_CENTER, Ogre::MovableText::V_CENTER); // Center horizontally and display above the node	
				textArr[textArrIndex] ->setColor(Ogre::ColourValue(1,1,1,.65));

				fontNode->attachObject(textArr[textArrIndex++]);

				fsn->setPosition(y * cos(theta), 0, y * sin(theta));
				fontNode->setPosition(fsn->getPosition().x + 6, fsn->getPosition().y, fsn->getPosition().z + 3.75);
			

				fsn->scale(.09,((float) app.e.size / 20000000 + 0.1),.09);

		

				OutputDebugString(fileName);
				OutputDebugString("\n");
			
			}

			distFromCentre += Ogre::Math::PI * 6;
		
		}

	}
	catch(std::exception& e)
	{
		OutputDebugString(e.what());
		OutputDebugString("EXCEPTIONNN\nn\n\nn\nn\nn\nn\nn\nn\nn\nn\nn\nn\nn\nn\nn\nn\nn\n\nn\nn\nn\nn\nnn\nn\nEXCEPTIONNN\nn\n");
					
	}
	
	endProgress();
	return true;
}



//-------------------------------------------------------------------------------------
void DigitalForensicsVisualisation::createViewports(void)
{
	    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(.0,.0,.0));
    // Alter the camera aspect ratio to match the viewport
	mCamera->setFarClipDistance(1500);
	
	mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));  

}

//-------------------------------------------------------------------------------------
bool DigitalForensicsVisualisation::keyPressed( const OIS::KeyEvent &arg )
{

	if (arg.key == OIS::KC_H)   // toggle visibility of advanced frame stats
    {
		sheet->setVisible((sheet->isVisible() ? false : true));
    }
    
    else if (arg.key == OIS::KC_R)   // cycle polygon rendering mode
    {
        Ogre::String newVal;
        Ogre::PolygonMode pm;

        switch (mCamera->getPolygonMode())
        {
        case Ogre::PM_SOLID:
            newVal = "Wireframe";
            pm = Ogre::PM_WIREFRAME;
            break;
        case Ogre::PM_WIREFRAME:
            newVal = "Points";
            pm = Ogre::PM_POINTS;
            break;
        default:
            newVal = "Solid";
            pm = Ogre::PM_SOLID;
        }

        mCamera->setPolygonMode(pm);
    }
    else if(arg.key == OIS::KC_F5)   // refresh all textures
    {
        Ogre::TextureManager::getSingleton().reloadAll();
    }
    else if (arg.key == OIS::KC_SYSRQ)   // take a screenshot
    {
        mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
    }
    else if (arg.key == OIS::KC_ESCAPE)
    {
        mShutDown = true;
    }

	CEGUI::System &sys = CEGUI::System::getSingleton();
	sys.getDefaultGUIContext().injectKeyDown((CEGUI::Key::Scan) arg.key);
	sys.getDefaultGUIContext().injectChar(arg.text);
    mCameraMan->injectKeyDown(arg);
    return true;



}

//-------------------------------------------------------------------------------------
bool DigitalForensicsVisualisation::keyReleased( const OIS::KeyEvent &arg )
{
	CEGUI::System &sys = CEGUI::System::getSingleton();
	sys.getDefaultGUIContext().injectKeyUp((CEGUI::Key::Scan) arg.key);
    mCameraMan->injectKeyUp(arg);
    return true;
}


//-------------------------------------------------------------------------------------
bool DigitalForensicsVisualisation::mouseMoved( const OIS::MouseEvent &arg )
{
  
    if (false) {
	mCameraMan->injectMouseMove(arg);
	}
	else {
	CEGUI::System &sys = CEGUI::System::getSingleton();
	sys.getDefaultGUIContext().injectMouseMove(arg.state.X.rel, arg.state.Y.rel);
	// Scroll wheel.
	if (arg.state.Z.rel)
		sys.getDefaultGUIContext().injectMouseWheelChange(arg.state.Z.rel / 120.0f);
	}

	return true;
}

//-------------------------------------------------------------------------------------
bool DigitalForensicsVisualisation::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	CEGUI::System &sys = CEGUI::System::getSingleton();
	sys.getDefaultGUIContext().injectMouseButtonDown(BaseApplication::convertButton(id));
    return true;
}

//-------------------------------------------------------------------------------------
bool DigitalForensicsVisualisation::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    
	CEGUI::System &sys = CEGUI::System::getSingleton();
	sys.getDefaultGUIContext().injectMouseButtonUp(BaseApplication::convertButton(id));
    mCameraMan->injectMouseUp(arg, id);
    return true;
}

//-------------------------------------------------------------------------------------
bool DigitalForensicsVisualisation::processUnbufferedInput(const Ogre::FrameEvent& evt)
{
	
	pointLight->setPosition(mCamera->getPosition());

	//leap
	static Frame frame;
	frame = leapController.frame();
	

	
	Leap::Hand rightMost = frame.hands().rightmost();

	
	
	
	
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
		
		float pitchValue = rightMost.direction().pitch() * RAD_TO_DEG;
		float rollValue = rightMost.palmNormal().roll() * RAD_TO_DEG;
		float yawValue = rightMost.direction().yaw() * RAD_TO_DEG;

		// to detect open and closed hand
		Ogre::Vector3 indexTip = toVector(rightMost.fingers()[1].tipPosition());
		Ogre::Vector3 pinkyTip = toVector(rightMost.fingers()[3].tipPosition());

	
		float angle = std::abs(indexTip.x - pinkyTip.x);
		angle /= rightMost.fingers()[1].length(); // to normalise 

		if (rightMost.grabStrength() == 1)
		{
			/*char* dummy = (char*) malloc(8);
			sprintf (dummy, "%f\n", rightMost.grabStrength()); 
			OutputDebugString(dummy);
			free (dummy);*/
			mCamera->pitch((Ogre::Radian) (rightMost.palmPosition().y - previousPosition.y) / 80);
			//mCamera->yaw((Ogre::Radian) (rightMost.palmPosition().x - previousPosition.x) / -80);
			//mCamera->roll((Ogre::Radian) (rollValue - previousFrameRoll) / 30);
			
		}
		else if (angle > 0.8)
		{
			
			palmNode->setPosition(toVector(frame.hands().rightmost().palmPosition())); // between 100 and 250	
			mCamera->setPosition(mCamera->getPosition() + (palmNode->getPosition() - previousPosition)*2 );
			
			
		}
		//else
		//{
		//	mCamera->yaw((Ogre::Radian) (yawValue - previousFrameYaw) / -10);
		//}

		previousPosition = toVector(frame.hands().rightmost().palmPosition());
		


		palmNode->pitch((Ogre::Radian) (pitchValue - previousFramePitch) );

		
		palmNode->roll((Ogre::Radian) (rollValue - previousFrameRoll) );

		
		palmNode->yaw((Ogre::Radian) (yawValue - previousFrameYaw) );

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
		//Ogre::Vector3 indexTip = toVector(rightMost.fingers()[1].tipPosition());
		//Ogre::Vector3 pinkyTip = toVector(rightMost.fingers()[3].tipPosition());

	
		//float angle = std::abs(indexTip.x - pinkyTip.x);
		//angle /= rightMost.fingers()[1].length(); // to normalise 
		//
		//char* dummy = (char*) malloc(8);
		//sprintf (dummy, "%f\n", angle); 
		//OutputDebugString(dummy);
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