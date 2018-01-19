#pragma once

#include <string>
using namespace std;

class myRegex
{
public:
	myRegex(const string s);
	myRegex(const myRegex& orig);
	~myRegex();
	unsigned int getMatchPosition()
	{
		return pos;
	}
	string getLastMatch()
	{
		return match;
	}

	virtual bool search(const string& s, unsigned int startPos);

protected:
	unsigned int pos;
	string match;
	string testPattern;
};

