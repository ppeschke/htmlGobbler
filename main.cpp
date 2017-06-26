#include <iostream>
#include "Lexer.h"
#include "Parser.h"
#include "DOMWalker.h"
#include <iomanip>

void download();
string getUrl(int year, string semester, string campus);
string extractTextNodeValue(DOMElement* e);
string wrapInQuotes(string s);
string quotesWithComma(string s);

int main()
{
	cout << "Download fresh schedule? (y/n)" << endl;
	char response;
	cin >> response;
	response = tolower(response);
	if(response == 'y')
		download();
	cout << fixed << showpoint << setprecision(3);
	DocumentObjectModel dom = Parser().Parse(Lexer().Lex("schedule.htm"));
	/*-----------------HERE BE THE PROCESSING SCRIPT--------------*/
	ofstream tsv("output.tsv");
	ofstream js("courses.js");
	if(tsv.fail() || js.fail())
	{
		cout << "Failed to open .tsv or .js file!" << endl;
		cout << "Aborting..." << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
	tsv << "Session\tCourse\tOpen Seats\tPrerequisites\tEnrollment\tSynonym\tLecLab\tSection\tCampus\tBuilding\tRoom\tDays\tTimes\tInstructor" << endl;
	DOMWalker dw;
	vector<DOMElement*> elements;
	dw.setDOM(&dom);
	elements = dw.find("a[title=\"Course Description\"]>TextNode, p.teach_term>TextNode, table.section_line tr");
	string session, course, section, synonym, prerequisites, enrollment, seats, lecLab, campus, building, room, days, times, instructor;
	string rubric;
	string number;
	string courseName;
	vector<string> meetings;
	string sessionStart;
	string sessionEnd;
	unsigned int delim;
	bool repeat = false;
	for(auto & e : elements)
	{
		if(e->parent->name == "A")
		{
			course = e->attributes[0].value;
			cout << "Course: " << course << endl;
			rubric = course.substr(0, 4);
			number = course.substr(5, 4);
			courseName = course.substr(10);
		}
		else if(e->parent->name == "P")
		{
			session = e->attributes[0].value;
			cout << "Session: " << session << endl;
			delim = session.find(":");
			sessionStart = session.substr(delim + 2, session.find(" -") - delim - 2);
			delim = session.find("-");
			sessionEnd = session.substr(delim + 2);
		}
		else
		{	//e is a tr element
			//so, children are td elements
			//and their children are text nodes

			//DATA EXTRACTION
			repeat = false;
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
			lecLab = extractTextNodeValue(e->children[5]);
			if(!repeat)
				section = extractTextNodeValue(e->children[6]);
			campus = extractTextNodeValue(e->children[7]->children[0]);
			building = extractTextNodeValue(e->children[8]);
			room = extractTextNodeValue(e->children[9]);
			days = extractTextNodeValue(e->children[10]);
			times = extractTextNodeValue(e->children[11]);
			if(e->children[15]->children[0]->name == "A")
				instructor = extractTextNodeValue(e->children[15]->children[0]);
			else
				instructor = extractTextNodeValue(e->children[15]);



			//DATA PROCESSING
			string startTime = times.substr(1, times.find("-") - 1);
			string endTime = times.substr(times.find("-") + 1);
			if(!repeat)
			{
				synonym = synonym.substr(1, 5);
				section = section.substr(1, 3);
				enrollment = enrollment.substr(1, enrollment.size() - 2);
			}
			if(room != "")
				room = room.substr(1);	//remove space before room
			days = days.substr(1);		//and days
			lecLab = lecLab.substr(1);		//and lecture/lab/practicum
			meetings.clear();
			if(days != "TBA")
			{
				string temp = "";
				for(auto & c : days)
				{
					if(isupper(c))
					{
						if(temp != "")
							meetings.push_back(temp);
						temp = c;
					}
					else
						temp += c;
				}
				meetings.push_back(temp);
			}
			else
				meetings.push_back("TBA");



			//OUTPUT
			if(seats != "X")
			{
				tsv << session << "\t" << course << "\t" << seats << "\t" << prerequisites << "\t" << enrollment << "\t" << synonym
					<< "\t" << lecLab << "\t" << section << "\t" << campus << "\t" << building << "\t" << room << "\t" << days
					<< "\t" << times << "\t" << instructor << endl;

				for(auto & m : meetings)
				{
					js << "courses.push(new Course(" << quotesWithComma(sessionStart) << quotesWithComma(sessionEnd) << quotesWithComma(rubric)
						<< quotesWithComma(courseName) << quotesWithComma(seats) << quotesWithComma(enrollment) << quotesWithComma(synonym)
						<< quotesWithComma(lecLab) << quotesWithComma(section) << quotesWithComma(campus) << quotesWithComma(building)
						<< quotesWithComma(room) << quotesWithComma(m) << quotesWithComma(startTime) << quotesWithComma(endTime) << wrapInQuotes(instructor)
						<< "));" << endl;
				}
			}
		}
	}
	tsv.close();
	js.close();
	/*-----------------YARRRGH!-----------------------------------*/
	system("pause");
	return 0;
}

inline string wrapInQuotes(string s)
{
	return "\"" + s + "\"";
}

inline string quotesWithComma(string s)
{
	return wrapInQuotes(s) + ", ";
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