#include <iostream>
#include "Lexer.h"
#include "Parser.h"
#include "DOMWalker.h"
#include <iomanip>

void download();
string getUrl(int year, string semester, string campus);
string extractTextNodeValue(DOMElement* e);

int main()
{
	//download();
	cout << fixed << showpoint << setprecision(3);
	DocumentObjectModel dom = Parser().Parse(Lexer().Lex("schedule.htm"));
	/*-----------------HERE BE THE PROCESSING SCRIPT--------------*/
	ofstream tsv("output.tsv");
	if(tsv.fail())
	{
		cout << "Failed to open TV file!" << endl;
		cout << "Aborting..." << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
	tsv << "Session\tCourse\tOpen Seats\tPrerequisites\tEnrollment\tSynonym\tLecLab\tSection\tCampus\tBuilding\tRoom\tDays\tTimes\tInstructor" << endl;
	DOMWalker dw;
	vector<DOMElement*> elements;
	dw.setDOM(&dom);
	elements = dw.find("a[title=\"Course Description\"]>TextNode, p.teach_term>TextNode, table.section_line tr");
	string session = "";
	string course = "";
	string section, synonym, prerequisites, enrollment, seats = "";
	bool repeat = false;
	for(auto & e : elements)
	{
		if(e->parent->name == "A")
		{
			course = e->attributes[0].value;
			cout << "Course: " << course << endl;
		}
		else if(e->parent->name == "P")
		{
			session = e->attributes[0].value;
			cout << "Session: " << session << endl;
		}
		else
		{	//e is a tr element
			//so, children are td elements
			//and their children are text nodes
			repeat = false;
			tsv << session << "\t" << course;
			if(extractTextNodeValue(e->children[0]) == "&nbsp; ")
				repeat = true;
			if(!repeat)
			{
				seats = extractTextNodeValue(e->children[0]);
				prerequisites = extractTextNodeValue(e->children[1]);
				if(e->children[3]->children[0]->name == "A")
					enrollment = extractTextNodeValue(e->children[3]->children[0]);	//link
				else
					enrollment = extractTextNodeValue(e->children[3]);	//not a link
				synonym = extractTextNodeValue(e->children[4]);
			}
			tsv << "\t" << seats << "\t" << prerequisites << "\t" << enrollment << "\t" << synonym;

			tsv << "\t" << extractTextNodeValue(e->children[5]);	//Lec/Lab

			if(!repeat)
				section = extractTextNodeValue(e->children[6]);
			tsv << "\t" << section;

			tsv << "\t" << extractTextNodeValue(e->children[7]->children[0]);	//campus is a link to campus' page, so one level deeper
			tsv << "\t" << extractTextNodeValue(e->children[8]);			//building
			tsv << "\t" << extractTextNodeValue(e->children[8]);	//room
			tsv << "\t" << extractTextNodeValue(e->children[9]);	//days
			tsv << "\t" << extractTextNodeValue(e->children[10]);	//times
			if(e->children[15]->children[0]->name == "A")
				tsv << "\t" << extractTextNodeValue(e->children[15]->children[0]);	//link
			else
				tsv << "\t" << extractTextNodeValue(e->children[15]);	//instructor, not a link
			tsv << endl;
		}
	}
	tsv.close();
	/*-----------------YARRRGH!-----------------------------------*/
	system("pause");
	return 0;
}

string extractTextNodeValue(DOMElement* e)
{
	if(e->children.size() > 0)
	{
		if(e->children[0]->name == "TEXTNODE")
			return e->children[0]->attributes[0].value;
	}
	return "";
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