#pragma once
#include "myRegex.h"
#include "Section.h"
class myComplexRegex : public myRegex
{
public:
	myComplexRegex(const string s);
	myComplexRegex(const myComplexRegex& orig);
	~myComplexRegex();
	virtual bool search(const string& html, unsigned int startPos);

private:
	vector<Section> compile(const string s);
	Section extractPart(string s, unsigned int& i);
	pair<vector<unsigned char>, bool> extractCharacters(string s, unsigned int& i);
	pair<vector<unsigned char>, bool> extractRange(string s, unsigned int& i);
	pair<unsigned int, unsigned int> extractCounts(string s, unsigned int& i);

	unsigned int sectionCount;		//number of contiguous blocks of 128 unsigned characters
	vector<Section> sections;
	bool* allowedCharacters;		//2D array of pointers to char stored as 1D
};

