#pragma once
#include <string>
using namespace std;

#include "Attribute.h"

class SelectorAttributeRequirement
{
public:
	SelectorAttributeRequirement(string n, bool (Attribute::*op)(string), string v);
	~SelectorAttributeRequirement();

	string reqName;
	string reqValue;
	bool (Attribute::*op)(string);

	bool eval(Attribute* a);
};

