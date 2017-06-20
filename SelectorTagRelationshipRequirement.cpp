#include "SelectorTagRelationshipRequirement.h"
#include <iostream>
using namespace std;

SelectorTagRelationshipRequirement::SelectorTagRelationshipRequirement()
{
	relOp = nullptr;
	ancestry = nullptr;
}

SelectorTagRelationshipRequirement::SelectorTagRelationshipRequirement(SelectorTagRequirement target, bool(SelectorTagRelationshipRequirement::*op)(DOMElement*), SelectorTagRelationshipRequirement * anc) : targetTag(target)
{
	relOp = op;
	ancestry = anc;
}

SelectorTagRelationshipRequirement::~SelectorTagRelationshipRequirement()
{
	if(ancestry != nullptr)
		delete ancestry;
}

bool SelectorTagRelationshipRequirement::or(DOMElement* targ)
{
	return ancestry->eval(targ);
}

bool SelectorTagRelationshipRequirement::hasAncestor(DOMElement* targ)
{
	bool found = false;
	while(!found && targ->parent != nullptr)
	{
		found = ancestry->eval(targ->parent);
		targ = targ->parent;
	}
	return found;
}

bool SelectorTagRelationshipRequirement::hasDirectAncestor(DOMElement* targ)
{
	if(targ->parent != nullptr)
		return ancestry->eval(targ->parent);
	return false;
}

bool SelectorTagRelationshipRequirement::followedBy(DOMElement* targ)
{
	if(targ->parent)
	{
		list<DOMElement*> children = targ->parent->children;
		//find ourselves again
		list<DOMElement*>::iterator i;
		for(i = children.begin(); i != children.end(); ++i)
		{
			if((*i) == targ)
			{
				++i;
				return ancestry->eval((*i));
			}
		}
	}
	return false;
}

bool SelectorTagRelationshipRequirement::preceededBy(DOMElement* targ)
{
	if(targ->parent)
	{
		list<DOMElement*> children = targ->parent->children;
		//find ourselves again
		list<DOMElement*>::iterator i;
		for(i = children.begin(); i != children.end(); ++i)
		{
			if((*i) == targ)
			{
				--i;
				return ancestry->eval((*i));
			}
		}
	}
	return false;
}

SelTagRelFuncPtr SelectorTagRelationshipRequirement::decideOp(string o)
{
	if(o == " ")
		return &hasAncestor;
	else if(o == ",")
		return &or ;
	else if(o == ">")
		return &hasDirectAncestor;
	else if(o == "+")
		return &followedBy;
	else if(o == "~")
		return &preceededBy;
	return nullptr;
}

bool SelectorTagRelationshipRequirement::eval(DOMElement* e)
{
	bool currentTagIsGood = targetTag.eval(e);	//Selector Tag Requirement's eval function
	bool ancestryIsGood = true;
	if(currentTagIsGood && relOp == &SelectorTagRelationshipRequirement::or)	//optimization: skip evaluation of ancestry if relOp is or and current tag is good
		return true;
	if(currentTagIsGood && relOp != nullptr)	//optimization: only evaluate relational ancestry if current tag is good
		ancestryIsGood = (this->*relOp)(e);
	return currentTagIsGood && ancestryIsGood;
}