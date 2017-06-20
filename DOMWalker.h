#pragma once
#include <string>
#include <list>
using namespace std;

#include "Selector.h"
class DOMElement;
class DocumentObjectModel;

class DOMWalker
{
public:
	DOMWalker();
	~DOMWalker();

	list<DOMElement*> find(string css);
	list<DOMElement*> find(const Selector& s);
	void setDOM(DocumentObjectModel* d);
private:
	list<DOMElement*> WalkTree(DOMElement* e, const Selector* s);
	DocumentObjectModel* dom;
};

