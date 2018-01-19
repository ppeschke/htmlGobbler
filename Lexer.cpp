#include <iostream>
#include <fstream>
#include <iomanip>

#include "Lexer.h"

void gotoXY(unsigned short, unsigned short);

Lexer::Lexer()
{
	states[start] = State(start);
	states[start].AddLexeme("beginScriptTag", "<[sS][cC][rR][iI][pP][tT]", inScriptTag, true);
	states[start].AddLexeme("beginStartTag", "<!?[A-Za-z]{1,}[1-6]?", inTag, true);
	states[start].AddLexeme("beginEndTag", "</[A-Za-z]{1,}[1-6]?", inTag, true);
	states[start].AddLexeme("finishSelfClosingTag", "/>", start, false);
	states[start].AddLexeme("beginCommentTag", "<!--", commentTag, false);
	states[start].AddLexeme("characterData", "[^<\n]{1,}", start, true);
	states[inTag] = State(inTag);
	states[inTag].AddLexeme("finishTag", ">", start, false);
	states[inTag].AddLexeme("finishSelfClosingTag", "/>", start, false);
	states[inTag].AddLexeme("equals", "=", equals, false);
	states[inTag].AddLexeme("attribute", "[A-Za-z]{1,}", inTag, true);
	states[equals] = State(equals);
	states[equals].AddLexeme("doubleQuote", "\"", dqValue, false);
	states[equals].AddLexeme("singleQuote", "'", sqValue, false);
	states[sqValue] = State(sqValue);
	states[sqValue].AddLexeme("singleQuote", "'", inTag, false);
	states[sqValue].AddLexeme("attributeValue", "[^']{1,}", sqValue, true);
	states[dqValue] = State(dqValue);
	states[dqValue].AddLexeme("doubleQuote", "\"", inTag, false);
	states[dqValue].AddLexeme("attributeValue", "[^\"]{1,}", dqValue, true);
	states[commentTag] = State(commentTag);
	states[commentTag].AddLexeme("finishCommentTag", "-->", start, false);
	states[commentTag].AddLexeme("commentData", "[A-Za-z !%:\\.0-9\t_\\*}{]+", commentTag, true);
	states[commentTag].AddLexeme("commentData", "-", commentTag, false);	//last so it doesn't match if this is the beginning of the end
	states[inScriptTag] = State(inScriptTag);
	states[inScriptTag].AddLexeme("finishTag", ">", inScript, false);
	states[inScriptTag].AddLexeme("equals", "=", scriptEquals, false);
	states[inScriptTag].AddLexeme("attribute", "[A-Za-z]{1,}", inScriptTag, true);
	states[scriptEquals] = State(scriptEquals);
	states[scriptEquals].AddLexeme("doubleQuote", "\"", scriptDqValue, false);
	states[scriptEquals].AddLexeme("singleQuote", "'", scriptSqValue, false);
	states[scriptSqValue] = State(scriptSqValue);
	states[scriptSqValue].AddLexeme("singleQuote", "'", inScriptTag, false);
	states[scriptSqValue].AddLexeme("attributeValue", "[^']{1,}", scriptSqValue, true);
	states[scriptDqValue] = State(scriptDqValue);
	states[scriptDqValue].AddLexeme("doubleQuote", "\"", inScriptTag, false);
	states[scriptDqValue].AddLexeme("attributeValue", "[^\"]{1,}", scriptDqValue, true);
	states[inScript] = State(inScript);
	states[inScript].AddLexeme("finishScriptTag", "</script>", start, false);
	states[inScript].AddLexeme("finishScriptTag", "</SCRIPT>", start, false);
	states[inScript].AddLexeme("scriptData", "[^<\n]{1,}", inScript, true);
	states[inScript].AddLexeme("scriptData", "<", inScript, false);
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
	system("cls");
	cout << "Lexing..." << endl;
	fout << "Lexing..." << endl;
	gotoXY(0, 1);
	cout << '[';
	gotoXY(21, 1);
	cout << ']';
	TokenStream ts;
	unsigned int currentState = start;
	int lineNumber = 1;
	unsigned int count = 0;
	short percent = 0;
	short oldPercent = -1;
	short fivesPercent = 0;
	const string lines = open(filename, count);
	bool found;
	fout << "Line Number  Token                      String" << endl;
	fout << "----------------------------------------------" << endl;
	cout << setprecision(2) << fixed << showpoint;
	for(unsigned int subStart = 0; subStart < lines.length(); ++subStart)
	{
		found = false;
		vector<Lexeme>* lexemes = states[(int)currentState].getLexemes();
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
		percent = (short)((lineNumber / (float)count) * 100);
		if(percent != oldPercent)	//if percentage has changed
		{
			fivesPercent = percent / 5;	//update counter by fives (integer division)
			if(fivesPercent > 0)
			{
				gotoXY(fivesPercent, 1);
				cout << (char)219;
			}
			gotoXY(23, 1);
			cout << percent << "%";
			oldPercent = percent;
		}
	}
	cout << endl;
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
