#include "DOMWalker.h"
#include "DOMElement.h"
#include "Selector.h"
#include "DocumentObjectModel.h"

DOMWalker::DOMWalker()
{
	dom = nullptr;
}


DOMWalker::~DOMWalker()
{
}

vector<DOMElement*> DOMWalker::find(string css)
{
	Selector sel(css);
	return find(sel);
}

vector<DOMElement*> DOMWalker::find(const Selector& s)
{
	vector<DOMElement*> elements;

	if(!dom)
		return elements;
	for(auto & r : dom->roots)
	{
		vector<DOMElement*> a = WalkTree(r, &s);
		elements.insert(elements.end(), a.begin(), a.end());
	}

	return elements;
}

void DOMWalker::setDOM(DocumentObjectModel * d)
{
	dom = d;
}

vector<DOMElement*> DOMWalker::WalkTree(DOMElement * e, const Selector* s)
{
	vector<DOMElement*> elements;
	if(s->match(e))					//evaluates tag, ancestry, and other
		elements.push_back(e);
	for(auto & c : e->children)
	{
		vector<DOMElement*> a = WalkTree(c, s);
		elements.insert(elements.end(), a.begin(), a.end());
	}
	return elements;
}
