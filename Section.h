#pragma once
#include <vector>
using namespace std;
class Section
{
public:
	Section();
	~Section();

public:
	unsigned int minCount;
	unsigned int maxCount;
	unsigned int matches;
	vector<unsigned char> chars;
	bool invert;
};

