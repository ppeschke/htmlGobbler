#pragma once
#include <string>
using namespace std;
#include "SelectorTagRelationshipRequirement.h"

class Selector
{
public:
	Selector(string);
	~Selector();

	bool match(DOMElement* de) const;

private:
	SelectorTagRelationshipRequirement* target;
};

