#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef struct Date
{
	unsigned short year,month,day;
	unsigned long date;
}; 

class ColorMap
{

public:

	std::string extension;
	float r,g,b;

	ColorMap();
	ColorMap(std::string e);

	bool operator < (ColorMap c)
	{
		return toInt(this->extension) < toInt(c.extension);
	}
	bool operator > (ColorMap c)
	{
		return toInt(this->extension) > toInt(c.extension);
	}
	bool operator <= (ColorMap c)
	{
		return toInt(this->extension) <= toInt(c.extension);
	}
	bool operator >= (ColorMap c)
	{
		return toInt(this->extension) >= toInt(c.extension);
	}
	bool operator == (ColorMap c)
	{
		return toInt(this->extension) == toInt(c.extension);
	}
	bool operator != (ColorMap c)
	{
		return toInt(this->extension) != toInt(c.extension);
	}

	int toInt(std::string s)
	{
		int dec = 0;
		for(int i=0; i<s.length(); i++)
			dec = dec * 10 + ( s[i] - '0' );
		return dec;
	}
};

ColorMap::ColorMap()
{
	r = g = b = 1;
	extension = "";

}

ColorMap::ColorMap(std::string e)
{
	int ir = rand();
	int ig = rand();
	int ib = rand();

	ir = ir % 45 + 20;
	ig = ig % 45 + 20;
	ib = ib % 45 + 20;
	r = (float) ir / 100;
	g = (float) ig / 100;
	b = (float) ib / 100;

	extension = e;
}


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
		return date.date;
	}

};


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

	extension = str2;
	
}