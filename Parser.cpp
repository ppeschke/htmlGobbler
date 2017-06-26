#include <iostream>
#include <algorithm>
#include "Parser.h"

Parser::Parser()
{
}


Parser::~Parser()
{
}

DocumentObjectModel Parser::Parse(TokenStream tknstrm)
{
	fout.open("parselog.txt");
	if(fout.fail())
	{
		cout << "Log file failed to open!" << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
	cout << "Parsing..." << endl;
	fout << "Parsing..." << endl;
	ts = tknstrm.tokens;
	DocumentObjectModel dom;
	unsigned int size = ts.size();
	unsigned int index = 0;
	while(index != size)
	{

		if(ts[index].name == "beginScriptTag" || ts[index].name == "beginStartTag" || ts[index].name == "beginCommentTag")
		{
			vector<DOMElement*> a = parseElements(nullptr, index);
			dom.roots.insert(dom.roots.end(), a.begin(), a.end());
		}
		else
		{
			cout << "Unexpected token \"" << ts[index].name << "\" (" << ts[index].lexeme << ") at line " << ts[index].lineNumber << endl;
			fout << "Unexpected token \"" << ts[index].name << "\" (" << ts[index].lexeme << ") at line " << ts[index].lineNumber << endl;
			++index;
		}
	}
	fout.close();
	return dom;
}



vector<DOMElement*> Parser::parseElements(DOMElement* parent, unsigned int & index)
{
	vector < DOMElement*> elements;
	bool done = false;
	DOMElement* elem = nullptr;
	string scriptData;
	string commentData;
	while(!done && index < ts.size())
	{
		if(ts[index].name == "beginStartTag" || ts[index].name == "beginScriptTag" || ts[index].name == "beginCommentTag")
		{
			elem = new DOMElement(ts[index].lexeme.substr(1), parent);
			elem->decideSelfClose();
			++index;
		}
		else if(ts[index].name == "scriptData")
		{
			unsigned int lastLine = ts[index].lineNumber;
			scriptData = "";
			while(index < ts.size() && ts[index].name != "finishScriptTag")
			{
				if(ts[index].name == "scriptData")
				{
					scriptData += ts[index++].lexeme;
				}
				else
				{
					cout << "Unexpected token \"" << ts[index].name << "\" (" << ts[index].lexeme << ") at line " << ts[index].lineNumber << endl;
					fout << "Unexpected token \"" << ts[index].name << "\" (" << ts[index].lexeme << ") at line " << ts[index].lineNumber << endl;
				}
				if(ts[index].lineNumber != lastLine)
				{
					scriptData += '\n';
					lastLine = ts[index].lineNumber;
				}
			}
			DOMElement* TextNode = new DOMElement("TEXTNODE", elem);
			TextNode->attributes.push_back(Attribute("Text", scriptData));
			elements.push_back(TextNode);
			++index;
			done = true;
		}
		else if(ts[index].name == "attribute")
		{
			if(elem)
				elem->attributes.push_back(parseAttribute(index));
		}
		else if(ts[index].name == "finishTag" || ts[index].name == "finishSelfClosingTag")
		{
			if(elem != nullptr)
			{
				++index;
				if(!elem->selfClose)
				{
					elem->AddChildren(parseElements(elem, index));
				}
				elements.push_back(elem);	//put element into the dom
				elem = nullptr;
			}

		}
		else if(ts[index].name == "characterData")
		{
			if(!whitespaceOnly(ts[index].lexeme))
			{
				DOMElement* TextNode = new DOMElement("TEXTNODE", parent);
				TextNode->attributes.push_back(Attribute("text", ts[index].lexeme));
				elements.push_back(TextNode);
			}
			++index;
		}
		else if(ts[index].name == "beginEndTag")
		{
			transform(ts[index].lexeme.begin(), ts[index].lexeme.end(), ts[index].lexeme.begin(), ::toupper);
			if(ts[index].lexeme.substr(2) == parent->name)
			{
				//close parent
				index += 2;	//should be endTag and finishTag
				done = true;
			}
			else
			{
				bool found = searchAncestryFor(parent, ts[index].lexeme.substr(2));
				if(!found)
				{
					cout << "Stray closing tag: " << ts[index].lexeme << '>' << endl;
					fout << "Stray closing tag: " << ts[index].lexeme << '>' << endl;
					index += 2;	//assume stray closing tag has a finish tag token after it.
				}
				else
				{
					//close all up to that point by returning
					cout << "End tag " << ts[index].lexeme << " found, but there were open elements on line " << ts[index].lineNumber << "." << endl;
					fout << "End tag " << ts[index].lexeme << " found, but there were open elements on line " << ts[index].lineNumber << "." << endl;
					return elements;
				}
			}
		}
		else if(ts[index].name == "commentData")
		{
			unsigned int lastLine = ts[index].lineNumber;
			commentData = "";
			while(index < ts.size() && ts[index].name != "finishCommentTag")
			{
				if(ts[index].name == "commentData")
				{
					commentData += ts[index++].lexeme;
				}
				else
				{
					cout << "Unexpected token \"" << ts[index].name << "\" (" << ts[index].lexeme << ") at line " << ts[index].lineNumber << endl;
					fout << "Unexpected token \"" << ts[index].name << "\" (" << ts[index].lexeme << ") at line " << ts[index].lineNumber << endl;
				}
				if(ts[index].lineNumber != lastLine)
				{
					commentData += '\n';
					lastLine = ts[index].lineNumber;
				}
			}
			elem->attributes.push_back(Attribute("CommentData", commentData));
			elements.push_back(elem);
			elem = nullptr;
			++index;
		}
		else if(ts[index].name == "finishScriptTag")
		{
			//if we got here, the script was empty (probably a src'd script)
			++index;
			return elements;
		}
	}
	return elements;
}

Attribute Parser::parseAttribute(unsigned int& index)
{
	unsigned int size = ts.size();
	Attribute a;
	bool done = false;
	int quotes = 0;	//keeps track of single or double quotes
	while(!done && index != size)
	{
		if(ts[index].name == "attribute")
		{
			a.name = ts[index].lexeme;
			++index;
		}
		else if(ts[index].name == "equals")
		{
			++index;
		}
		else if(ts[index].name == "singleQuote")
		{
			if(quotes == 2)
			{
				cout << "Unexpected token \"" << ts[index].name << "\" (" << ts[index].lexeme << ") at line " << ts[index].lineNumber << endl;
				fout << "Unexpected token \"" << ts[index].name << "\" (" << ts[index].lexeme << ") at line " << ts[index].lineNumber << endl;
			}
			else
			{
				if(quotes == 1)
					done = true;
				else
					quotes = 1;
			}
			++index;
		}
		else if(ts[index].name == "doubleQuote")
		{
			if(quotes == 1)
			{
				cout << "Unexpected token \"" << ts[index].name << "\" (" << ts[index].lexeme << ") at line " << ts[index].lineNumber << endl;
				fout << "Unexpected token \"" << ts[index].name << "\" (" << ts[index].lexeme << ") at line " << ts[index].lineNumber << endl;
			}
			else
			{
				if(quotes == 2)
					done = true;
				else
					quotes = 2;
			}
			++index;
		}
		else if(ts[index].name == "attributeValue")
		{
			a.value = ts[index].lexeme;
			++index;
		}
		else
			done = true;
	}
	return a;
}

bool Parser::searchAncestryFor(DOMElement * parent, string tagname)
{
	while(parent != nullptr)
	{
		if(parent->name == tagname)
			return true;
		parent = parent->parent;
	}
	return false;
}

bool Parser::whitespaceOnly(string s)
{
	for(auto & c : s)
		if(c != ' ' && c != '\t')
			return false;
	return true;
}