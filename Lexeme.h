#pragma once

#include <string>
using namespace std;

#include "myComplexRegex.h"

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
	Lexeme(string tokenName, string regx, unsigned int moveTo, bool complex);
	Lexeme(const Lexeme& orig);
	Lexeme(Lexeme&& orig);
	Lexeme& operator=(const Lexeme& right);
	~Lexeme();

	string name;
	//regex r;
	myRegex* r;
	unsigned int moveTo;
};

