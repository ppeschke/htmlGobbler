#include <iostream>
#include <fstream>
#include <iomanip>

#include "Lexer.h"

Lexer::Lexer()
{
	states[start] = State(start);
	states[start].AddLexeme("beginScriptTag", "<[s|S][c|C][r|R][i|I][p|P][t|T]", inScriptTag);
	states[start].AddLexeme("beginStartTag", "<!?[A-Za-z]{1,}[1-6]?", inTag);
	states[start].AddLexeme("beginEndTag", "</[A-Za-z]{1,}[1-6]?", inTag);
	states[start].AddLexeme("finishSelfClosingTag", "/>", start);
	states[start].AddLexeme("beginCommentTag", "<!--", commentTag);
	states[start].AddLexeme("characterData", "[^<\n]{1,}", start);
	states[inTag] = State(inTag);
	states[inTag].AddLexeme("finishTag", ">", start);
	states[inTag].AddLexeme("finishSelfClosingTag", "/>", start);
	states[inTag].AddLexeme("equals", "=", equals);
	states[inTag].AddLexeme("attribute", "[A-Za-z]{1,}", inTag);
	states[equals] = State(equals);
	states[equals].AddLexeme("doubleQuote", "\"", dqValue);
	states[equals].AddLexeme("singleQuote", "'", sqValue);
	states[sqValue] = State(sqValue);
	states[sqValue].AddLexeme("singleQuote", "'", inTag);
	states[sqValue].AddLexeme("attributeValue", "[^']{1,}", sqValue);
	states[dqValue] = State(dqValue);
	states[dqValue].AddLexeme("doubleQuote", "\"", inTag);
	states[dqValue].AddLexeme("attributeValue", "[^\"]{1,}", dqValue);
	states[commentTag] = State(commentTag);
	states[commentTag].AddLexeme("finishCommentTag", "-->", start);
	states[commentTag].AddLexeme("commentData", "[A-Za-z !%:\\.0-9\t_\\*}{]+", commentTag);
	states[commentTag].AddLexeme("commentData", "-", commentTag);	//last so it doesn't match if this is the beginning of the end
	states[inScriptTag] = State(inScriptTag);
	states[inScriptTag].AddLexeme("finishTag", ">", inScript);
	states[inScriptTag].AddLexeme("equals", "=", scriptEquals);
	states[inScriptTag].AddLexeme("attribute", "[A-Za-z]{1,}", inScriptTag);
	states[scriptEquals] = State(scriptEquals);
	states[scriptEquals].AddLexeme("doubleQuote", "\"", scriptDqValue);
	states[scriptEquals].AddLexeme("singleQuote", "'", scriptSqValue);
	states[scriptSqValue] = State(scriptSqValue);
	states[scriptSqValue].AddLexeme("singleQuote", "'", inScriptTag);
	states[scriptSqValue].AddLexeme("attributeValue", "[^']{1,}", scriptSqValue);
	states[scriptDqValue] = State(scriptDqValue);
	states[scriptDqValue].AddLexeme("doubleQuote", "\"", inScriptTag);
	states[scriptDqValue].AddLexeme("attributeValue", "[^\"]{1,}", scriptDqValue);
	states[inScript] = State(inScript);
	states[inScript].AddLexeme("finishScriptTag", "</script>", start);
	states[inScript].AddLexeme("finishScriptTag", "</SCRIPT>", start);
	states[inScript].AddLexeme("scriptData", "[^<\n]{1,}", inScript);
	states[inScript].AddLexeme("scriptData", "<", inScript);
}


Lexer::~Lexer()
{
}

TokenStream Lexer::Lex(string filename)
{
	ofstream fout("lexlog.txt");
	if(fout.fail())
	{
		cout << "Failed to open log file! Aborting..." << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
	cout << "Lexing..." << endl;
	fout << "Lexing..." << endl;
	TokenStream ts;
	unsigned int currentState = start;
	int lineNumber = 1;
	unsigned int count = 0;
	int percent = 0;
	int oldPercent = -1;
	const string lines = open(filename, count);
	smatch m;
	bool found;
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
				//cout << setw(11) << right << lineNumber << "  " << setw(25) << left << (*lexemes)[lindex].name << "  " << m[0] << endl;
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
		percent = (lineNumber / (float)count) * 100;
		if(percent != oldPercent)
		{
			cout << percent << "%" << endl;
			oldPercent = percent;
		}
	}
	fout.close();
	return ts;
}

string Lexer::open(string filename, unsigned int& count)
{
	string line;
	string lines = "";
	ifstream file(filename.c_str());
	if(file.fail())
	{
		cout << "File: " << filename << " failed to open. Aborting." << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
	getline(file, line, '\n');
	while(!file.eof())
	{
		lines += line + '\n';
		++count;
		getline(file, line, '\n');
	}
	lines += line;
	++count;
	file.close();
	return lines;
}
