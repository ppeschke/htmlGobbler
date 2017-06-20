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

list<DOMElement*> DOMWalker::find(string css)
{
	Selector sel(css);
	return find(sel);
}

list<DOMElement*> DOMWalker::find(const Selector& s)
{
	list<DOMElement*> elements;

	if(!dom)
		return elements;
	for(auto & r : dom->roots)
		elements.splice(elements.end(), WalkTree(r, &s));

	return elements;
}

void DOMWalker::setDOM(DocumentObjectModel * d)
{
	dom = d;
}

list<DOMElement*> DOMWalker::WalkTree(DOMElement * e, const Selector* s)
{
	list<DOMElement*> elements;
	if(s->match(e))					//evaluates tag, ancestry, and other
		elements.push_back(e);
	for(auto & c : e->children)
		elements.splice(elements.end(), WalkTree(c, s));
	return elements;
}
