#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <time.h>

struct Date
{
	unsigned short year,month,day;
	unsigned long date;
};



class Entity
{


public:

	
	const char* full_name;
	char* name;
	const char* directory;
	unsigned long size;
	std::string extension;
	unsigned short write_permission;
	unsigned short access_permission;
	char* creation_time;
	char* access_time;
	char* modification_time;
	unsigned long c, a, m;  //create, access, modification

	//Methods

	Entity();
	~Entity();
	int monthNumber (std::string);
	void setExtension();
	bool isTextureFile();

	unsigned long parseDate (char* time)
	{
		Date date;
		char* value = (char*) malloc (strlen (time) +2 );
		sprintf(value, "%s", time);
		char* dummy;
		dummy = strtok (value, " "); // day (mon, tue, wed etc. (ignored)
		dummy = strtok (NULL, " "); // month
		date.month = monthNumber(dummy);
		dummy = strtok (NULL, " "); // day (integer)
		date.day = atoi (dummy);
		dummy = strtok (NULL, " "); // hour minute second (ignored, already in creation_time)
		dummy = strtok (NULL, " "); // year
		date.year = atoi (dummy);

		date.date = (date.year * 10000) + (date.month * 100) + date.day;
		free (value);
		value = dummy = NULL;
		return date.date;
	}

};

bool Entity::isTextureFile()
{

	if (this->extension == "PNG" || this->extension == "JPG" || this->extension == "JPEG" || this->extension == "GIF" || this->extension == "ICO")
		return true;
	else
		return false;
}

int Entity::monthNumber (std::string month)
{
	return ((month == "Jan") ? 1 : (month ==  "Feb") ? 2 : (month == "Mar") ? 3 : (month == "Apr") ? 4 :
		(month == "May") ? 5 : (month == "Jun") ? 6 : (month == "Jul") ? 7 : (month == "Aug") ? 8 :
		(month == "Sep") ? 9 : (month == "Oct") ? 10 : (month == "Nov") ? 11 : 12);
}

Entity::Entity()
{


	size = c = a = m = 0;
	write_permission = access_permission = 0;

}

Entity::~Entity()
{
	name = NULL;
	
}

void Entity::setExtension()
{

	int lastDotIndex = 0;

	std::string str(name);

	for (int i = strlen(name) - 1; i > 0; i--)
	{
		if (name[i] == '.')
		{
			lastDotIndex = i;
			
			goto found;
		}	
	}

	// not found statement
	extension = "";
	return;

	found:
	std::string str2 = "";

	for (int i = lastDotIndex; i < (strlen(name) -1) ; i++)
	{
		str2 += str[i+1];
	}

	extension = str2;
	
}