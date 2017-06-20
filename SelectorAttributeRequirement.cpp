#include "SelectorAttributeRequirement.h"


SelectorAttributeRequirement::SelectorAttributeRequirement(string n, bool(Attribute::* o)(string), string v)
{
	reqName = n;
	op = o;
	reqValue = v;
}

SelectorAttributeRequirement::~SelectorAttributeRequirement()
{
}

bool SelectorAttributeRequirement::eval(Attribute* a)
{
	bool nameIsGood = a->name == reqName;
	bool valueIsGood = true;
	if(op != nullptr)	//reqValue must be there if op is good
	{
		valueIsGood = (a->*op)(reqValue);	//pointer to class function. Phillip, please don't forget how to do this.
	}
	return nameIsGood && valueIsGood;
}
