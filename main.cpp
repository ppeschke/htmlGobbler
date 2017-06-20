#include <iostream>
#include "Lexer.h"
#include "Parser.h"
#include "DOMWalker.h"
#include <iomanip>

void download();
string getUrl(int year, string semester, string campus);

int main()
{
	//download();
	Lexer l;
	cout << fixed << showpoint << setprecision(3);
	TokenStream toks;
	Parser p;
	DocumentObjectModel dom;
	DOMWalker dw;
	list<DOMElement*> descriptionText;
	toks = l.Lex("test.htm");
	dom = p.Parse(toks);
	dw.setDOM(&dom);
	descriptionText = dw.find("a[title=\"Course Description\"]>TextNode, p.teach_term>TextNode");
	for(auto & dl : descriptionText)
	{
		for(auto & at : dl->attributes)
			cout << at.value << endl;
	}
	system("pause");
	return 0;
}

void download()
{
	int year;
	string semester;
	string campus;
	cout << "Please enter the year: ";
	cin >> year;
	cout << "Please enter the semester: ";
	cin >> semester;
	cout << "Please enter the campus: ";
	cin.ignore();
	getline(cin, campus, '\n');
	string url = getUrl(year, semester, campus);
	cout << "Fetching: " << url << endl;
	string command = "curl -o schedule.htm \"" + url + "\"";
	system(command.c_str());
}

string getUrl(int iYear, string semester, string campus)
{
	string term = "";
	string location = "";
	string year = "";
	if(iYear < 2000)
		iYear += 2000;
	char buffer[16];
	_itoa_s(iYear, buffer, 10);
	year = buffer;
	_itoa_s(iYear - 1800, buffer, 10);
	term = buffer;
	transform(semester.begin(), semester.end(), semester.begin(), ::tolower);
	if(semester == "summer" || semester == "u")
		term += 'U';
	else if(semester == "fall" || semester == "f")
		term += 'F';
	else if(semester == "spring" || semester == "s")
		term += 'S';
	else
	{
		cout << "I'm sorry. I could not understand the semester you gave me." << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
	term += "000";
	transform(campus.begin(), campus.end(), campus.begin(), tolower);
	if(campus == "cyp" || campus == "cypress")
		location = "CYP";
	else if(campus == "evc" || campus == "eastview")
		location = "EVC";
	else if(campus == "egn" || campus == "elgin")
		location = "EGN";
	else if(campus == "hys" || campus == "hays")
		location = "HYS";
	else if(campus == "hlc" || campus == "highland")
		location = "HLC";
	else if(campus == "nrg" || campus == "northridge")
		location = "NRG";
	else if(campus == "pin" || campus == "pinnacle")
		location = "PIN";
	else if(campus == "rgc" || campus == "rio grande")
		location = "RGC";
	else if(campus == "rvs" || campus == "riverside")
		location = "RVS";
	else if(campus == "rrc" || campus == "round rock")
		location = "RRC";
	else if(campus == "sac" || campus == "south austin")
		location = "SAC";
	else
	{
		cout << "I'm sorry. I can't understand the campus you gave me." << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
	string url = "http://www6.austincc.edu/schedule/index.php?op=browse&opclass=ViewSched_location&term=" + term + "&locationid=" + location + "&yr=" + year + "&ct=CC%27%3E";
	return url;
}