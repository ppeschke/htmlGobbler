#include "Attribute.h"

Attribute::Attribute()
{
}

Attribute::Attribute(string n, string v)
{
	name = n;
	value = v;
}


Attribute::~Attribute()
{
}

bool Attribute::attributeEquals(string v)
{
	return v == value;
}

bool Attribute::attributeStartsWith(string v)
{
	return value.find(v) == 0;
}

bool Attribute::attributeStartsWithWord(string v)
{
	return value.find(v + '-') == 0 || value == v;
}

bool Attribute::attributeEndsWith(string v)
{
	return value.find(v) == (value.size() - v.size());
}

bool Attribute::attributeContains(string v)
{
	return value.find(v) != -1;
}

bool Attribute::attributeContainsWord(string v)
{
	return value == v
		|| value.find(v + ' ') == 0
		|| value.find(' ' + v) != -1 && value.find(' ' + v) == (value.size() - (v.size() + 1))
		|| value.find(' ' + v + ' ') != -1;
}

AttrFuncPtr Attribute::decideOp(string op)
{
	if(op == "=")
		return &attributeEquals;
	else if(op == "^=")
		return &attributeStartsWith;
	else if(op == "|=")
		return &attributeStartsWithWord;
	else if(op == "$=")
		return &attributeEndsWith;
	else if(op == "*=")
		return &attributeContains;
	else if(op == "~=")
		return &attributeContainsWord;
	return nullptr;
}

void Attribute::clear()
{
	name = "";
	value = "";
}
