#pragma once
#include <vector>
using namespace std;

#include "SelectorAttributeRequirement.h"
#include "DOMElement.h"

class SelectorTagRequirement
{
public:
	SelectorTagRequirement();
	SelectorTagRequirement(string name);
	~SelectorTagRequirement();

	string reqName;
	vector<SelectorAttributeRequirement> reqAttributes;

	bool eval(DOMElement* e);
	void addAttributeRequirement(SelectorAttributeRequirement r);
};

