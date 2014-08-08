#include <iostream>
#include <string.h>
#include <stdlib.h>

typedef struct Date
{
	unsigned short year,month,day;
	unsigned int date;
}; 




class Entity
{


public:

	
	const char* full_name;
	char* name;
	const char* directory;
	unsigned long size;
	char* extension;
	unsigned short read_permission;
	unsigned short write_permission;
	unsigned short execute_permission;
	char* creation_time;
	char* execution_time;
	char* modification_time;

	//Methods

	Entity();
	~Entity();
	int monthNumber (char*);
	void setExtension();

	void parseDate (Date &date, char* time)
	{
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
	}

};


int Entity::monthNumber (char* month)
{
	return (month == ("jan")) ? 1 : (month ==  ("feb")) ? 2 : (month == ("mar")) ? 3 : (month == ("apr")) ? 4 :
		(month == ("may")) ? 5 : (month == ("jun")) ? 6 : (month == ("jul")) ? 7 : (month == ( "aug")) ? 8 :
		(month == ("sep")) ? 9 : (month == ("oct")) ? 10 : (month == ("nov") ? 11 : 12);
}

Entity::Entity()
{


	size = 0;
	read_permission = write_permission = execute_permission = 0;

}

Entity::~Entity()
{
	name = NULL;
	free (extension);
}

void Entity::setExtension()
{

	int lastDotIndex;

	std::string str(name);

	for (int i = strlen(name); i > 0; i--)
	{
		if (name[i] == '.')
		{
			lastDotIndex = i;
			break;
		}	
	}

	std::string str2 = "";

	for (int i = lastDotIndex; i < (strlen(name) -1) ; i++)
	{
		str2 += str[i+1];
	}

	extension = (char*) malloc (strlen(name) - lastDotIndex + 1);
	sprintf(extension, "%s",str2);
	
}