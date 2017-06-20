#pragma once
#include "SelectorTagRequirement.h"

class SelectorTagRelationshipRequirement;

typedef bool (SelectorTagRelationshipRequirement::*SelTagRelFuncPtr)(DOMElement*);

class SelectorTagRelationshipRequirement
{
public:
	SelectorTagRelationshipRequirement();
	SelectorTagRelationshipRequirement(SelectorTagRequirement target, bool (SelectorTagRelationshipRequirement::*op)(DOMElement*), SelectorTagRelationshipRequirement* anc);
	~SelectorTagRelationshipRequirement();

	SelectorTagRequirement targetTag;
	bool (SelectorTagRelationshipRequirement::*relOp)(DOMElement*);
	SelectorTagRelationshipRequirement* ancestry;

	bool eval(DOMElement* e);

	//relationship ops
	bool or(DOMElement*);					// ,
	bool hasAncestor(DOMElement*);			// (space)
	bool hasDirectAncestor(DOMElement*);	// >
	bool followedBy(DOMElement*);			// +
	bool preceededBy(DOMElement*);			// ~

	static SelTagRelFuncPtr decideOp(string o);
};

