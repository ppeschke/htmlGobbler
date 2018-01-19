#include "myComplexRegex.h"
#include <iostream>
using namespace std;

myComplexRegex::myComplexRegex(const string s) : myRegex(s)
{
	sections = compile(s);
	sectionCount = (unsigned int)sections.size();
	allowedCharacters = new bool[sections.size() * 128];
	if(allowedCharacters != nullptr)
	{
		//initialize the data
		unsigned int p = -1;
		for(unsigned int i = 0; i < sections.size() * 128; ++i)
		{
			if(i % 128 == 0)
				++p;
			allowedCharacters[i] = sections[p].invert;
		}

		for(unsigned int i = 0; i < sections.size(); ++i)
		{
			for(unsigned int p = 0; p < sections[i].chars.size(); ++p)
			{
				allowedCharacters[i * 128 + sections[i].chars[p]] = !sections[i].invert;
			}
		}
	}
	else
	{
		cout << "Allocation failed for Regex!" << endl;
		cout << "Press any key to abort..." << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
}

myComplexRegex::myComplexRegex(const myComplexRegex & orig) : myRegex(orig)
{
	sectionCount = orig.sectionCount;
	sections = orig.sections;
	unsigned int size = sections.size() * 128;
	allowedCharacters = new bool[size];
	for(unsigned int i = 0; i < size; ++i)
	{
		allowedCharacters[i] = orig.allowedCharacters[i];
	}
}

myComplexRegex::~myComplexRegex()
{
	if(allowedCharacters != nullptr)
		delete[] allowedCharacters;
}

bool myComplexRegex::search(const string& html, unsigned int startPos)
{
	unsigned int i = startPos;
	unsigned char c;
	unsigned int sectionNumber = 0;
	Section* currentSection = &sections[sectionNumber];
	while(1)//loop
	{
		//get a character (c)
		c = html[i];
		//if allowed in this section
		if(allowedCharacters[sectionNumber * 128 + c])
		{
			//if matches == max
			if(currentSection->matches == currentSection->maxCount)
			{
				//next section
				currentSection = &sections[++sectionNumber];
				//re-eval c (fall through to repeat the loop)
			}
			//else
			else
			{
				//++matches
				++currentSection->matches;
				if(currentSection->matches == currentSection->maxCount && sectionNumber != sections.size() - 1)
				{
					//next section
					currentSection = &sections[++sectionNumber];
				}
				//next character
				++i;
			}
		}
		//else
		else
		{
			//if matches >= min && <= max
			if(currentSection->matches >= currentSection->minCount && currentSection->matches <= currentSection->maxCount)
			{
				//if next section exists
				if(sectionNumber < this->sectionCount - 1)
				{
					//next section
					currentSection = &sections[++sectionNumber];
					//re-eval c (fall through to repeat the loop)
				}
				//else
				else
				{
					//record match
					this->match = html.substr(startPos, i - startPos);
					pos = startPos;
					//clean up
					for(int i = sectionNumber; i >= 0; --i)
					{
						sections[i].matches = 0;
					}
					//return true;
					return true;
				}
			}
			//else
			else
			{
				//clean up
				for(int i = sectionNumber; i >= 0; --i)
				{
					sections[i].matches = 0;
				}
				//return false
				return false;
			}
		}
	}
}

vector<Section> myComplexRegex::compile(const string s)
{
	vector<Section> sections;
	for(unsigned int i = 0; i < s.length(); ++i)
	{
		sections.push_back(extractPart(s, i));
	}
	for(vector<Section>::iterator i = sections.begin(); i != sections.end(); ++i)
	{
		if((*i).invert)
			cout << "Not: ";
		cout << '[';
		for(vector<unsigned char>::iterator p = (*i).chars.begin(); p != (*i).chars.end(); ++p)
			cout << (*p);
		cout << ']' << '{' << (*i).minCount << ',';
		if((*i).maxCount == (unsigned int)-1)
			cout << "infinite";
		else
			cout << (*i).maxCount;
		cout << '}' << endl;
	}
	return sections;
}

Section myComplexRegex::extractPart(string s, unsigned int& i)
{
	pair<vector<unsigned char>, bool> chars;
	bool foundChars = false;
	pair<unsigned int, unsigned int> counts = pair<unsigned int, unsigned int>(1, 1);
	Section sec;
	for(; i < s.length(); ++i)
	{
		switch(s[i])
		{
		case '{':
			counts = extractCounts(s, ++i);
			sec.minCount = counts.first;
			sec.maxCount = counts.second;
			return sec;
			break;
		case '?':
			sec.minCount = 0;
			sec.maxCount = 1;
			return sec;
			break;
		case '+':
			sec.minCount = 1;
			sec.maxCount = -1;
			return sec;
			break;
		default:
			if(!foundChars)
			{
				foundChars = true;
				chars = extractCharacters(s, i);
				sec.chars = chars.first;
				sec.invert = chars.second;
			}
			else
			{
				--i;	//unpeek
				sec.minCount = counts.first;
				sec.maxCount = counts.second;
				sec.chars = chars.first;
				sec.invert = chars.second;
				return sec;
			}
			break;
		}
	}
	sec.minCount = counts.first;
	sec.maxCount = counts.second;
	return sec;
}

pair<vector<unsigned char>, bool> myComplexRegex::extractCharacters(string s, unsigned int& i)
{
	pair<vector<unsigned char>, bool> c;
	for(; i < s.length(); ++i)
	{
		switch(s[i])
		{
		case '[':
			c = extractRange(s, ++i);
			return c;
			break;
		case '\\':
			++i;
			c.first.push_back(s[i]);
			return c;
			break;
		default:
			c.first.push_back(s[i]);
			return c;
			break;
		}
	}
	return c;
}

pair<vector<unsigned char>, bool> myComplexRegex::extractRange(string s, unsigned int& i)
{
	pair<vector<unsigned char>, bool> c;
	bool escaped = false;
	c.second = false;
	bool first = true;
	for(; i < s.length(); ++i)
	{
		switch(s[i])
		{
		case '^':
			if(first)
			{
				c.second = true;
				first = false;
			}
			break;
		case ']':
			if(escaped)
			{
				c.first.push_back(s[i]);
				escaped = false;	//reset escaped
			}
			return c;
			break;
		case '\\':
			if(escaped)
				c.first.push_back(s[i]);
			else
				escaped = true;
			break;
		case '-':
			++i;
			{
				unsigned char begin = (*(--c.first.end())) + 1;	//last character we extracted plus one
				unsigned char end = s[i];
				if(end < begin)
				{
					unsigned char temp;
					temp = begin;
					begin = end;
					end = temp;
				}
				//string pattern = "<!?[A-Za-z]{1,}[1-6]?";
				for(; begin <= end; ++begin)
					c.first.push_back(begin);
			}
			break;
		default:
			c.first.push_back(s[i]);
			escaped = false;
			break;
		}
		first = false;
	}
	return c;
}

pair<unsigned int, unsigned int> myComplexRegex::extractCounts(string s, unsigned int& i)
{
	unsigned int first = 0;
	unsigned int second = -1;
	unsigned int number = 0;
	int place = 1;
	for(; i < s.length(); ++i)
	{
		switch(s[i])
		{
		case '}':
			if(s[i - 1] != ',')
				second = number;
			return pair<unsigned int, unsigned int>(first, second);
			break;
		case ',':
			first = number;
			number = 0;
			place = 1;
			break;
		default:
			number += (s[i] - 48) * place;
			place *= 10;
			break;
		}
	}
	return pair<unsigned int, unsigned int>(first, second);
}
