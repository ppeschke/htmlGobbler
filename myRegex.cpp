#include "myRegex.h"

myRegex::myRegex(const string s)
{
	testPattern = s;
	match = "";
	pos = -1;
}

myRegex::myRegex(const myRegex & orig)
{
	pos = orig.pos;
	match = orig.match;
	testPattern = orig.testPattern;
}

myRegex::~myRegex()
{
}

bool myRegex::search(const string& html, unsigned int startPos)
{
	unsigned int rindex = 0;
	for(unsigned int i = startPos; rindex < testPattern.length() && i < html.length(); ++i)
	{
		if(html[i] != testPattern[rindex])
			return false;
		++rindex;
	}
	if(rindex == testPattern.length())
	{
		pos = 0;
		match = testPattern;
		return true;
	}
	return false;
}
