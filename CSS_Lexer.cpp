#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

#include "CSS_Lexer.h"

CSS_Lexer::CSS_Lexer()
{
	states[betweenTags] = State(betweenTags);
	states[betweenTags].AddLexeme("tagName", "[A-Za-z]{1,}[1-6]?", betweenTags);
	states[betweenTags].AddLexeme("classNamePeriod", "\\.", inClassName);
	states[betweenTags].AddLexeme("idNamePoundSign", "#", inIdName);
	states[betweenTags].AddLexeme("attributeStartBracket", "\\[", inAttributeName);
	states[betweenTags].AddLexeme("relationalOperator", "[ ]?[,>+~]{1,1}[ ]?", betweenTags);
	states[betweenTags].AddLexeme("relationalOperator", "[ ]{1,}", betweenTags);
	states[inClassName] = State(inClassName);
	states[inClassName].AddLexeme("className", "[a-zA-z\\-_]{1,}[a-zA-z\\-_0-9]?", betweenTags);
	states[inClassName].AddLexeme("attributeStartBracket", "\\[", inAttributeName);
	states[inIdName].AddLexeme("idName", "[a-zA-z\\-_]{1,}[a-zA-z\\-_0-9]?", betweenTags);
	states[inIdName].AddLexeme("attributeStartBracket", "\\[", inAttributeName);
	states[inAttributeName] = State(inAttributeName);
	states[inAttributeName].AddLexeme("attributeName", "[a-zA-z\\-_]{1,}[a-zA-z\\-_0-9]?", inAttributeOperator);
	states[inAttributeOperator] = State(inAttributeOperator);
	states[inAttributeOperator].AddLexeme("attributeOperator", "[~\\|\\$\\^\\*]?=", inAttributeOperator);
	states[inAttributeOperator].AddLexeme("singleQuote", "'", inSqValue);
	states[inAttributeOperator].AddLexeme("doubleQuote", "\\\"", inDqValue);
	states[inAttributeOperator].AddLexeme("attributeValue", "[a-zA-z\\-\\._0-9]{1,}", inValue);
	states[inAttributeOperator].AddLexeme("attributeEndBracket", "\\]", betweenTags);
	states[inValue] = State(inValue);
	states[inValue].AddLexeme("attributeEndBracket", "\\]", betweenTags);
	states[inSqValue] = State(inSqValue);
	states[inSqValue].AddLexeme("attributeValue", "[a-zA-z \\-\\._0-9]{1,}", inSqValue);
	states[inSqValue].AddLexeme("singleQuote", "'", inValue);	//this should pick up the close bracket properly
	states[inDqValue] = State(inDqValue);
	states[inDqValue].AddLexeme("attributeValue", "[a-zA-z \\-\\._0-9]{1,}", inDqValue);
	states[inDqValue].AddLexeme("doubleQuote", "\\\"", inValue);	//this should pick up the close bracket properly

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
	smatch m;
	bool found;
	float lastProgress = 0;
	fout << "Line Number  Token                      String" << endl;
	fout << "----------------------------------------------" << endl;
	cout << setprecision(2) << fixed << showpoint;
	string::const_iterator subStart = lines.begin();
	for(unsigned int i = 0; i < lines.length(); ++i)
	{
		found = false;
		vector<Lexeme>* lexemes = states[(int)currentState].getLexemes();
		for(unsigned int lindex = 0; !found && *subStart != '\n' && lindex < lexemes->size(); ++lindex)
		{
			if(regex_search(subStart, lines.end(), m, (*lexemes)[lindex].r, regex_constants::match_continuous))
			{
				fout << setw(11) << right << lineNumber << "  " << setw(25) << left << (*lexemes)[lindex].name << "  " << m[0] << endl;
				ts.AddToken(Token(lineNumber, (*lexemes)[lindex].name, m[0]));
				i += m[0].length() - 1;
				subStart += m[0].length() - 1;
				currentState = (*lexemes)[lindex].moveTo;
				found = true;
			}
		}
		if(!found && *subStart != '\n')
		{
			fout << setw(11) << right << lineNumber << "                             " << left << lines[i] << endl;
		}
		if(*subStart == '\n')
			++lineNumber;
		++subStart;
	}
	fout.close();
	return ts;
}