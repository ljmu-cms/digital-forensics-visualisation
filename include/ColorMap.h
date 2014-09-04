#include <iostream>

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

	if (e == "")
	{
		r = g = b = .10;
		return;
	}

	int ir, ig, ib;
	ig = ib = ir = toInt(e) * 5791;

	ir = (ir % 45) + 25;
	ig = (ig % 43) + 25;
	ib = (ib % 44) + 25;
	r = (float) ir / 100;
	g = (float) ig / 100;
	b = (float) ib / 100;

	extension = e;
}
