#include "DocumentObjectModel.h"

DocumentObjectModel::DocumentObjectModel()
{
}

DocumentObjectModel::DocumentObjectModel(DocumentObjectModel && rvalue)
{
	this->roots = rvalue.roots;
	rvalue.roots.clear();	//will not call destructor on the tree
}


DocumentObjectModel::~DocumentObjectModel()
{
	for(auto & r : roots)
	{
		delete r;
	}
}

DocumentObjectModel & DocumentObjectModel::operator=(DocumentObjectModel && rvalue)
{
	if(this != &rvalue)
	{
		this->roots = rvalue.roots;	//will clear other roots in lvalue tree
		rvalue.roots.clear();	//will not call destructor on the tree
	}
	return *this;
}

DOMElement* DocumentObjectModel::AddRoot(string name)
{
	DOMElement* de = new DOMElement(name, nullptr);
	roots.push_back(de);

	return de;
}

ostream & operator<<(ostream & out, const DocumentObjectModel & dom)
{
	for(auto & r : dom.roots)
		out << (*r);
	return out;
}
