#ifndef DOMELEMENT_H
#define DOMELEMENT_H

#include <vector>
#include "Attribute.h"

class DOMElement
{
public:
	DOMElement(string name, DOMElement* parent);
	~DOMElement();
	DOMElement* AddChild(string name, DOMElement* parent);
	void AddChild(DOMElement*);
	void AddChildren(vector<DOMElement*> children);
	void decideSelfClose();
	Attribute* findAttribute(string name);
	bool isTag(string n);

	friend ostream& operator<<(ostream& out, const DOMElement& elem);

	string name;
	vector<Attribute> attributes;
	DOMElement* parent;
	bool selfClose;

	vector<DOMElement*> children;
};

#endif