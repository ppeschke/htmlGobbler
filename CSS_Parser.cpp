#include <iostream>
#include "CSS_Parser.h"

CSS_Parser::CSS_Parser()
{
}


CSS_Parser::~CSS_Parser()
{
}

SelectorTagRelationshipRequirement* CSS_Parser::Parse(TokenStream t)
{
	fout.open("cssparselog.txt", ios::app);
	if(fout.fail())
	{
		cout << "Log file failed to open!" << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
	SelectorTagRelationshipRequirement* target = new SelectorTagRelationshipRequirement();
	SelectorTagRelationshipRequirement* newTarget;
	fout << "Parsing..." << endl;
	ts = t.tokens;
	bool done = false;
	unsigned int index = 0;
	while(!done && index < ts.size())
	{
		if(ts[index].name == "tagName" || ts[index].name == "attributeStartBracket" || ts[index].name == "classNamePeriod" || ts[index].name == "idNamePoundSign")
		{
			target->targetTag = parseTag(index);
		}
		else if(ts[index].name == "relationalOperator")
		{
			ts[index].lexeme = shortenOperator(ts[index].lexeme);
			if(ts[index].lexeme == ",")
			{
				newTarget = new SelectorTagRelationshipRequirement();
				newTarget->other = target;
				target = newTarget;
				newTarget = nullptr;
			}
			else
			{
				newTarget = new SelectorTagRelationshipRequirement();
				newTarget->relOp = SelectorTagRelationshipRequirement::decideOp(ts[index].lexeme);
				newTarget->ancestry = target;		//push old target back into ancestry of newTarget
				newTarget->other = target->other;	//move target other to newTarget
				target->other = nullptr;			//and delete other from old target
				target = newTarget;					//update target to be new target
				newTarget = nullptr;				//cleanup because why not?
			}
			++index;
		}
		else
		{
			cout << "Unexpected token " << ts[index].name << " (" << ts[index].lexeme << ") in selector." << endl << "Aborting..." << endl;
			fout << "Unexpected token " << ts[index].name << " (" << ts[index].lexeme << ") in selector." << endl << "Aborting..." << endl;
			system("pause");
			exit(EXIT_FAILURE);
		}
	}

	return target;
}

SelectorTagRequirement CSS_Parser::parseTag(unsigned int & index)
{
	SelectorTagRequirement t;
	if(ts[index].name == "tagName")
	{
		t.reqName = ts[index].lexeme;
		++index;
	}
	else
		t.reqName = "";
	bool done = false;
	while(!done && index != ts.size())
	{
		if(ts[index].name == "attributeStartBracket" || ts[index].name == "classNamePeriod" || ts[index].name == "idNamePoundSign")
		{
			t.addAttributeRequirement(parseAttribute(index));
			++index;
		}
		else if(ts[index].name == "relationalOperator")
			return t;
		else
		{
			cout << "Unexpected token " << ts[index].name << " (" << ts[index].lexeme << ") in selector." << endl << "Aborting..." << endl;
			system("pause");
			exit(EXIT_FAILURE);
		}
	}
	return t;
}

SelectorAttributeRequirement CSS_Parser::parseAttribute(unsigned int& index)
{
	string name;
	string value;
	string op;
	int quotes = 0;
	bool done = false;
	while(!done && index < ts.size())
	{
		if(ts[index].name == "attributeStartBracket")
		{
			++index;
		}
		else if(ts[index].name == "classNamePeriod")
		{
			name = "class";
			op = "~=";
			++index;
		}
		else if(ts[index].name == "className")
		{
			value = ts[index].lexeme;
			return SelectorAttributeRequirement(name, Attribute::decideOp(op), value);
		}
		else if(ts[index].name == "idName")
		{
			value = ts[index].lexeme;
			return SelectorAttributeRequirement(name, Attribute::decideOp(op), value);
		}
		else if(ts[index].name == "idNamePoundSign")
		{
			name = "id";
			op = "=";
			++index;
		}
		else if(ts[index].name == "attributeName")
			name = ts[index++].lexeme;
		else if(ts[index].name == "attributeOperator")
			op = ts[index++].lexeme;
		else if(ts[index].name == "singleQuote")
		{
			if(quotes == 1)
			{
				++index;
				return SelectorAttributeRequirement(name, Attribute::decideOp(op), value);
			}
			else if(quotes == 0)
			{
				quotes = 1;
				++index;
			}
			else
			{
				cout << "Unexpected token " << ts[index].name << "(" << ts[index].lexeme << ") in selector." << endl << "Aborting..." << endl;
				fout << "Unexpected token " << ts[index].name << "(" << ts[index].lexeme << ") in selector." << endl << "Aborting..." << endl;
				system("pause");
				exit(EXIT_FAILURE);
			}
		}
		else if(ts[index].name == "doubleQuote")
		{
			if(quotes == 2)
			{
				++index;
				return SelectorAttributeRequirement(name, Attribute::decideOp(op), value);
			}
			else if(quotes == 0)
			{
				quotes = 2;
				++index;
			}
			else
			{
				cout << "Unexpected token " << ts[index].name << "(" << ts[index].lexeme << ") in selector." << endl << "Aborting..." << endl;
				fout << "Unexpected token " << ts[index].name << "(" << ts[index].lexeme << ") in selector." << endl << "Aborting..." << endl;
				system("pause");
				exit(EXIT_FAILURE);
			}
		}
		else if(ts[index].name == "attributeValue")
		{
			value = ts[index++].lexeme;
			if(quotes == 0)
				return SelectorAttributeRequirement(name, Attribute::decideOp(op), value);
		}
		else
		{
			cout << "Unexpected token " << ts[index].name << "(" << ts[index].lexeme << ") in selector." << endl << "Aborting..." << endl;
			fout << "Unexpected token " << ts[index].name << "(" << ts[index].lexeme << ") in selector." << endl << "Aborting..." << endl;
			system("pause");
			exit(EXIT_FAILURE);
		}
	}
	cout << "Could not find end of attribute!" << endl << "Aborting..." << endl;
	fout << "Could not find end of attribute!" << endl << "Aborting..." << endl;
	system("pause");
	exit(EXIT_FAILURE);
}

string CSS_Parser::shortenOperator(string o)
{
	string finalString = "";
	bool foundNonSpaces = false;
	for(auto& c : o)
	{
		if(c != ' ')
			foundNonSpaces = true;
	}
	if(foundNonSpaces)
	{
		for(auto& c : o)
		{
			if(c != ' ')
				finalString.push_back(c);
		}
	}
	else
		finalString = " ";
	return finalString;
}