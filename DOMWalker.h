#pragma once
#include <string>
#include <vector>
using namespace std;

#include "Selector.h"
class DOMElement;
class DocumentObjectModel;

class DOMWalker
{
public:
	DOMWalker();
	~DOMWalker();

	vector<DOMElement*> find(string css);
	vector<DOMElement*> find(const Selector& s);
	void setDOM(DocumentObjectModel* d);
private:
	vector<DOMElement*> WalkTree(DOMElement* e, const Selector* s);
	DocumentObjectModel* dom;
};

