#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

#include "CSS_Lexer.h"

CSS_Lexer::CSS_Lexer()
{
	css_states[betweenTags] = State(betweenTags);
	css_states[betweenTags].AddLexeme("tagName", "[A-Za-z]{1,}[1-6]?", betweenTags, true);
	css_states[betweenTags].AddLexeme("classNamePeriod", ".", inClassName, false);
	css_states[betweenTags].AddLexeme("idNamePoundSign", "#", inIdName, false);
	css_states[betweenTags].AddLexeme("attributeStartBracket", "[", inAttributeName, false);
	css_states[betweenTags].AddLexeme("relationalOperator", "[ ]?[,>+~]{1,1}[ ]?", betweenTags, true);
	css_states[betweenTags].AddLexeme("relationalOperator", "[ ]{1,}", betweenTags, true);
	css_states[inClassName] = State(inClassName);
	css_states[inClassName].AddLexeme("className", "[A-Za-z\\-_]{1,}[A-Za-z\\-_0-9]?", betweenTags, true);	//this has a problem at the dash. Go through it again.
	css_states[inClassName].AddLexeme("attributeStartBracket", "[", inAttributeName, false);
	css_states[inIdName].AddLexeme("idName", "[A-Za-z\\-_]{1,}[A-Za-z\\-_0-9]?", betweenTags, true);	//same here... problem. Start with the compiled output and see what the memory contains
	css_states[inIdName].AddLexeme("attributeStartBracket", "[", inAttributeName, false);
	css_states[inAttributeName] = State(inAttributeName);
	css_states[inAttributeName].AddLexeme("attributeName", "[A-Za-z\\-_]{1,}[A-Za-z\\-_0-9]?", inAttributeOperator, true);
	css_states[inAttributeOperator] = State(inAttributeOperator);
	css_states[inAttributeOperator].AddLexeme("attributeOperator", "[~|$^*]?=", inAttributeOperator, true);
	css_states[inAttributeOperator].AddLexeme("singleQuote", "'", inSqValue, false);
	css_states[inAttributeOperator].AddLexeme("doubleQuote", "\"", inDqValue, false);
	css_states[inAttributeOperator].AddLexeme("attributeValue", "[A-Za-z\\-\\._0-9]{1,}", inValue, true);
	css_states[inAttributeOperator].AddLexeme("attributeEndBracket", "]", betweenTags, false);
	css_states[inValue] = State(inValue);
	css_states[inValue].AddLexeme("attributeEndBracket", "]", betweenTags, false);
	css_states[inSqValue] = State(inSqValue);
	css_states[inSqValue].AddLexeme("attributeValue", "[A-Za-z \\-._0-9]{1,}", inSqValue, true);
	css_states[inSqValue].AddLexeme("singleQuote", "'", inValue, false);	//this should pick up the close bracket properly
	css_states[inDqValue] = State(inDqValue);
	css_states[inDqValue].AddLexeme("attributeValue", "[A-Za-z \\-._0-9]{1,}", inDqValue, true);
	css_states[inDqValue].AddLexeme("doubleQuote", "\"", inValue, false);	//this should pick up the close bracket properly
}


CSS_Lexer::~CSS_Lexer()
{
}

TokenStream CSS_Lexer::Lex(string line, unsigned int count)
{
	ofstream fout("csslexlog.txt", ios::app);
	if(fout.fail())
	{
		cout << "Failed to open log file! Aborting..." << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
	fout << "Lexing..." << endl;
	TokenStream ts;
	unsigned int currentState = start;
	int lineNumber = 1;
	const string lines = line;
	bool found;
	fout << "Line Number  Token                      String" << endl;
	fout << "----------------------------------------------" << endl;
	cout << setprecision(2) << fixed << showpoint;
	for(unsigned int subStart = 0; subStart < lines.length(); ++subStart)
	{
		found = false;
		vector<Lexeme>* lexemes = css_states[(int)currentState].getLexemes();
		for(unsigned int lindex = 0; !found && lines[subStart] != '\n' && lindex < lexemes->size(); ++lindex)
		{
			if((*lexemes)[lindex].r->search(lines, subStart))
			{
				fout << setw(11) << right << lineNumber << "  " << setw(25) << left << (*lexemes)[lindex].name << "  "
					<< (*lexemes)[lindex].r->getLastMatch() << endl;
				ts.AddToken(Token(lineNumber, (*lexemes)[lindex].name, (*lexemes)[lindex].r->getLastMatch()));
				subStart += (*lexemes)[lindex].r->getLastMatch().length() - 1;
				currentState = (*lexemes)[lindex].moveTo;
				found = true;
			}
		}
		if(!found && lines[subStart] != '\n')
		{
			fout << setw(11) << right << lineNumber << "                             " << left << lines[subStart] << endl;
		}
		if(lines[subStart] == '\n')
			++lineNumber;
	}
	fout.close();
	return ts;
}