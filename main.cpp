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
			if(e->children[0]->children[0]->attributes[0].value == "&nbsp; ")
				repeat = true;
			if(!repeat)
			{
				seats = e->children[0]->children[0]->attributes[0].value;
				tsv << "\t" << e->children[0]->children[0]->attributes[0].value;	//seats
				prerequisites = e->children[1]->children[0]->attributes[0].value;
				tsv << "\t" << e->children[1]->children[0]->attributes[0].value;	//prerequisites
				enrollment = e->children[3]->children[0]->attributes[0].value;
				tsv << "\t" << e->children[3]->children[0]->attributes[0].value;	//enrollment
				synonym = e->children[4]->children[0]->attributes[0].value;
				tsv << "\t" << e->children[4]->children[0]->attributes[0].value;	//synonym
			}
			else
				tsv << "\t" << seats << "\t" << prerequisites << "\t" << enrollment << "\t" << synonym;
			tsv << "\t" << e->children[6]->children[0]->attributes[0].value;	//Lec/Lab
			if(!repeat)
			{
				section = e->children[7]->children[0]->attributes[0].value;
				tsv << "\t" << e->children[7]->children[0]->attributes[0].value;	//section
			}
			else
				tsv << "\t" << section;
			if(!repeat)
				tsv << "\t" << e->children[8]->children[0]->children[0]->attributes[0].value;	//7 is a link to campus' page, so one level deeper
			else
				tsv << "\t" << e->children[8]->children[1]->children[0]->attributes[0].value;
			tsv << "\t" << e->children[9]->children[0]->attributes[0].value;	//building
			if(!repeat)
			{
				tsv << "\t" << e->children[10]->children[0]->attributes[0].value;	//room
				tsv << "\t" << e->children[11]->children[0]->attributes[0].value;	//days
				tsv << "\t" << e->children[12]->children[0]->attributes[0].value;	//times
				tsv << "\t" << e->children[17]->children[0]->attributes[0].value; //instructor
			}
			else
			{
				tsv << "\t" << e->children[11]->children[0]->attributes[0].value;	//room
				tsv << "\t" << e->children[12]->children[0]->attributes[0].value;	//days
				tsv << "\t" << e->children[13]->children[0]->attributes[0].value;	//times
				tsv << "\t" << e->children[17]->children[0]->attributes[0].value; //instructor
			}
			tsv << endl;
		}
	}
	tsv.close();
	/*-----------------YARRRGH!-----------------------------------*/
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