#include "SelectorTagRequirement.h"
#include <algorithm>

SelectorTagRequirement::SelectorTagRequirement()
{
	reqName = "";
}

SelectorTagRequirement::SelectorTagRequirement(string name)
{
	reqName = name;
}

SelectorTagRequirement::~SelectorTagRequirement()
{
}

bool SelectorTagRequirement::eval(DOMElement * e)
{
	if(e == nullptr)
		return false;
	transform(reqName.begin(), reqName.end(), reqName.begin(), toupper);
	bool nameIsGood = reqName == "" || e->name == reqName;
	bool attributesAreGood = true;
	unsigned int size = reqAttributes.size();
	if(size > 0)
	{
		bool* attributeRequirementsMet = new bool[size];
		for(unsigned int i = 0; i < size; ++i)
			attributeRequirementsMet[i] = false;	//start with false assumption
		for(unsigned int i = 0; i < size; ++i)
		{
			for(auto& a : e->attributes)
			{
				if(reqAttributes[i].eval(&a))
					attributeRequirementsMet[i] = true;
			}
		}
		for(unsigned int i = 0; attributesAreGood && i < size; ++i)
		{
			if(!attributeRequirementsMet[i])
				attributesAreGood = false;
		}
		delete[] attributeRequirementsMet;
	}
	return nameIsGood && attributesAreGood;
}

void SelectorTagRequirement::addAttributeRequirement(SelectorAttributeRequirement r)
{
	reqAttributes.push_back(r);
}
