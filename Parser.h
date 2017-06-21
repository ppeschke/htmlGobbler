#pragma once
#include <fstream>
#include "DocumentObjectModel.h"
#include "TokenStream.h"

class Parser
{
public:
	Parser();
	~Parser();

	DocumentObjectModel Parse(TokenStream tok);
private:
	ofstream fout;
	vector<Token> ts;
	vector<DOMElement*> parseElements(DOMElement* parent, unsigned int& index);
	Attribute parseAttribute(unsigned int& index);
	bool searchAncestryFor(DOMElement* parent, string tagname);
	bool whitespaceOnly(string s);
};

