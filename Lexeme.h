#pragma once

#include <string>
using namespace std;

enum HTML_Lexing_State
{
	start, inTag, attribute, equals, sqValue, dqValue, text,
	closingTag, selfClosingSlash, commentTag, inScriptTag,
	finishScriptOpen, inScript, scriptEquals, scriptDqValue,
	scriptSqValue
};

enum selectorStates
{
	betweenTags, inClassName, inIdName, inTagName,
	afterTagName, inAttributeOperator, inAttributeName, inDqValue,
	inSqValue, inValue
};

class Lexeme
{
public:
	Lexeme(string tokenName, string regx, unsigned int moveTo);
	~Lexeme();

	string name;
	regex r;
	unsigned int moveTo;
};

