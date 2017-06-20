#include <iostream>
#include <list>
using namespace std;

#include "Selector.h"
#include "CSS_Lexer.h"
#include "CSS_Parser.h"

Selector::Selector(string css)
{
	if(css == "")
	{
		cout << "Could not compile blank css selector!" << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
	CSS_Lexer lex;
	TokenStream tokens = lex.Lex(css);
	target = nullptr;
	CSS_Parser parser;
	target = parser.Parse(tokens);
}


Selector::~Selector()
{
	if(target != nullptr)
		delete target;
}

bool Selector::match(DOMElement* de) const
{
	return target->eval(de);
}