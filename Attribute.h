#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <string>
using namespace std;

class Attribute;

//defines a pointer to an Attribute member function as AttrFuncPtr
typedef bool (Attribute::*AttrFuncPtr)(string);	//I know you're going to forget this syntax, Phillip, but please don't forget it.


class Attribute
{
public:
	Attribute();
	Attribute(string name, string value);
	~Attribute();
	bool attributeEquals(string v);			//=
	bool attributeStartsWith(string v);		//^=
	bool attributeStartsWithWord(string v);	//|=
	bool attributeEndsWith(string v);		//$=
	bool attributeContains(string v);		//*=
	bool attributeContainsWord(string v);	//~=

	static AttrFuncPtr decideOp(string op);

	void clear();

	string name;
	string value;
};

#endif