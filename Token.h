#pragma once
#include <string>
using namespace std;

class Token
{
public:
	Token(int, string, string);
	~Token();

	string name;
	int lineNumber;
	string lexeme;
};

