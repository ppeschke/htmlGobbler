#pragma once

#include <fstream>
#include "TokenStream.h"
#include "SelectorTagRelationshipRequirement.h"

class CSS_Parser
{
public:
	CSS_Parser();
	~CSS_Parser();

	SelectorTagRelationshipRequirement* Parse(TokenStream tok);

private:
	SelectorTagRequirement parseTag(unsigned int & index);
	SelectorAttributeRequirement parseAttribute(unsigned int & index);
	ofstream fout;
	vector<Token> ts;
	string shortenOperator(string o);

};

