#include "DOMElement.h"
#include <algorithm>

vector<string> selfClosingList = {"!DOCTYPE", "META", "LINK", "IMG", "BR", "HR", "!--"};

DOMElement::DOMElement(string n, DOMElement* p)
{
	name = n;
	parent = p;
}


DOMElement::~DOMElement()
{
	for(auto & c : children)
	{
		delete c;
	}
}

DOMElement* DOMElement::AddChild(string name, DOMElement * parent)
{
	DOMElement* de = new DOMElement(name, parent);
	children.push_back(de);
	return de;
}

void DOMElement::AddChild(DOMElement * c)
{
	children.push_back(c);
}

void DOMElement::AddChildren(vector<DOMElement*> c)
{
	children.insert(children.end(), c.begin(), c.end());
}

void DOMElement::decideSelfClose()
{
	transform(name.begin(), name.end(), name.begin(), toupper);
	if(find(selfClosingList.begin(), selfClosingList.end(), this->name) != selfClosingList.end())
		selfClose = true;
	else
		selfClose = false;
}

Attribute * DOMElement::findAttribute(string name)
{
	Attribute* found = nullptr;
	for(vector<Attribute>::iterator index = attributes.begin(); !found && index != attributes.end(); ++index)
	{
		if(index->name == name)
			found = &(*index);	//change index from iterator to Attribute*
	}
	return found;
}

bool DOMElement::isTag(string n)
{
	transform(n.begin(), n.end(), n.begin(), toupper);
	return name == n;
}

ostream & operator<<(ostream & out, const DOMElement & elem)
{
	out << elem.name << ' ';
	for(auto & a : elem.attributes)
		out << '[' << a.name << (a.value != ""? ('=' + a.value):"") << ']' << ' ';
	out << endl;
	for(auto & c : elem.children)
		out << '-' << (*c);
	return out;
}
