#pragma once

#include <vector>
#include <regex>
#include <string>
using namespace std;

#include "Lexeme.h"

class State
{
public:
	State();
	State(unsigned int s);
	~State();
	void AddLexeme(string name, string regx, unsigned int ls, bool complex);
	vector<Lexeme>* getLexemes();

private:
	unsigned int name;
	vector<Lexeme> lexemes;
};

