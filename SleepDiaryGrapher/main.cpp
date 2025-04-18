#include <SFML\Graphics.hpp>
#include <SFML\OpenGL.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include "drawShapes.h"
#include "entity.h"
#include "Vector.h"
#include "Timer.h"
#include "Utilities.h"
//#include <algorithm>
//#include "Matrix.h"
//#include <conio.h>

//If anything goes wrong, start with line 520's debugging code.

bool drawMainDots = true;
bool drawOtherDots = true;
bool drawEarlyLine = false;
bool drawEarlyLineAverage = false;
bool drawMainLine = true;
bool drawMainLineAverage = true;
bool animateApproximates = false;
bool drawApproximates = false;
const float averageRange = 0.5f;
float horScalar = 10.0f;
float vertScalar = 50.0f;
int numHours = 24;
int minAverageDegree = 10;
int maxAverageDegree = 40;

bool running = true;
bool inFocus = true;
bool showFps = false;
bool holdingLShift = false;
bool holdingRShift = false;
bool holdingLCtrl = false;
bool holdingLAlt = false;
float windowWidth(800), windowHeight(600);
float fpsTimeCounter = 1;
float averageOffset = 0;
int fpsCounter = 0;
int fpsManualCount = 999;
int restingOnRow = 0;
int restingOnColumn = 0;
int averageDegree;
sf::RenderWindow window;
sf::Event event;
vec2 mouseLocation(0, 0);
vec2 mouseLocationOld(0, 0);
vec2 mouseScreenLocation(0, 0);
vec2 mouseScreenLocationOld(0, 0);
camera cam = camera(windowWidth / 2, windowHeight / 2, 1.001f, 1.0f);
Timer *worldTimer;
std::ifstream inputStream;
std::ofstream outputStream;
std::string parentDirectory = "C:\\Users\\NAME\\FOLDER\\"; //To hardcode here, for convinience when testing
std::string fileName = "FILENAME.rtf";
std::string directoryLocFile = "GrapherDirectory.txt";
std::vector<std::string> lineGot;
sf::Font arial;
sf::Text text;
PingPong pingPong;

struct Date
{
	int year;
	int month;
	int day;
};

std::string getPrintMonth(int monthNum);

class Times;
Times *heightAdjustDummyTime;
class Entry
{
public:
	Date date;
	std::vector<Times> times;

	Entry()
	{
		times = std::vector<Times>();
	}

	std::string getPrintDate()
	{
		return getPrintMonth(date.month) + " " + std::to_string(date.day) + ", " + std::to_string(date.year);
	}
};
std::vector<Entry> entry;

class Times
{
public:
	int hour;
	int minute;
	bool am;
	std::string comment;
	bool approximate;
	bool post;
	bool noData;

	Times()
	{
		comment = "";
		approximate = false;
		post = false;
		noData = false;
	}

	std::string getPrintTime()
	{
		std::string theMinute = std::to_string(minute);
		if (theMinute.length() < 2)
		{
			theMinute = "0" + theMinute;
		}
		if (noData)
		{
			return "No Data";
		}
		else
		{
			std::string retVal = (approximate ? "~" : "") + std::to_string(hour) + ":" + theMinute + (am ? "am" : "pm");
			return retVal;
		}
	}

	float getHeight(int ind, bool useAverage = true)
	{
		vec2 lastGoodPoint;
		vec2 nextGoodPoint;
		if (noData)
		{
			int i = ind;
			while (i > 0)
			{
				i--;
				if (!entry[i].times.back().noData)
				{
					lastGoodPoint.x = i;
					lastGoodPoint.y = entry[i].times.back().getHeight(i);
					break;
				}
			}
			nextGoodPoint = lastGoodPoint;
			i = ind;
			while (i < entry.size() - 1)
			{
				i++;
				if (!entry[i].times.back().noData)
				{
					nextGoodPoint.x = i;
					nextGoodPoint.y = entry[i].times.back().getHeight(i);
					break;
				}
			}
			return LERP(lastGoodPoint.y, nextGoodPoint.y, (ind - lastGoodPoint.x) / (nextGoodPoint.x - lastGoodPoint.x));
		}
		else
		{
			if (approximate && useAverage)
			{
				return float(hour + (am ? 12 : 0)) + ((hour == 12) ? -12 : 0) + (minute / 60.0f) + averageOffset;
			}
			else
			{
				return float(hour + (am ? 12 : 0)) + ((hour == 12) ? -12 : 0) + (minute / 60.0f);
			}
		}
	}
};
Times *restingOn = nullptr;
int restingOnEntry = -1;

void getLines(std::string fileDirectory, std::vector<std::string>& lineGot)
{
	//Get all lines from file
	inputStream.open(fileDirectory, std::ios::out);
	std::string temp;
	while (!inputStream.eof()) {
		std::getline(inputStream, temp);
		lineGot.push_back(temp);
	}
	inputStream.close();
}

size_t findNextWithoutCharBefore(std::string line, char find, char exclude, size_t beg)
{
	while (true)
	{
		beg = line.find(find, beg);
		if (beg == std::string::npos)
		{
			return beg;
		}
		if (beg == 0 || line[beg - 1] != exclude)
		{
			return beg;
		}
		if (beg == line.length() - 1)
		{
			return std::string::npos;
		}
		beg += 1;
	}
}

size_t findNextWithoutCharAfter(std::string line, char find, char exclude, size_t beg)
{
	while (true)
	{
		beg = line.find(find, beg);
		if (beg == std::string::npos)
		{
			return beg;
		}
		if (beg == line.length() - 1 || line[beg + 1] != exclude)
		{
			return beg;
		}
		if (beg == line.length() - 1)
		{
			return std::string::npos;
		}
		beg += 1;
	}
}

size_t findNextWithoutCharAfter(std::string line, char find, std::string exclude, size_t beg)
{
	while (true)
	{
		beg = line.find(find, beg);
		if (beg == std::string::npos)
		{
			return beg;
		}
		bool noExcludeCharNext = true;
		if (beg < line.length() - 1)
		{
			for (int i = 0; i < exclude.length(); i++)
			{
				if (line[beg + 1] == exclude[i])
				{
					noExcludeCharNext = false;
				}
			}
		}
		if (beg == line.length() - 1 || noExcludeCharNext)
		{
			return beg;
		}
		if (beg == line.length() - 1)
		{
			return std::string::npos;
		}
		beg += 1;
	}
}

//Didn't have the heart to use this one-line function, but keeping it here as a reminder of how to do this.
size_t findWithinNext(std::string line, std::string find, size_t pos, int numCharsToSearch)
{
	return line.substr(pos, numCharsToSearch).find(find, 0) + pos;
}

void rtfToPlaintext(std::vector<std::string>& lineGot)
{
	//Go through all lines
	for (int i = 0; i < lineGot.size(); i++) {
		//Reformat hyperlinks
		if (lineGot[i].find("HYPERLINK") != std::string::npos) {
			if (lineGot[i].find("\"") != std::string::npos) {
				if (lineGot[i].find("\"") != lineGot[i].find_last_of("\"")) {
					lineGot[i].erase(0, lineGot[i].find("\"") + 1);
					lineGot[i].erase(lineGot[i].find_last_of("\""), lineGot[i].length() - lineGot[i].find_last_of("\""));
				}
			}
		}
		//Translate tabs, dashes, underlines, and newlines
		while (lineGot[i].find("\\endash") != std::string::npos) {
			lineGot[i].replace(lineGot[i].find("\\endash"), 8, "-");
		}
		while (lineGot[i].find("\\tab") != std::string::npos) {
			lineGot[i].replace(lineGot[i].find("\\tab"), 4, "\t");
		}
		while (lineGot[i].find("\\b") != std::string::npos) {
			lineGot[i].replace(lineGot[i].find("\\b"), lineGot[i].find(" ", lineGot[i].find("\\b")) - lineGot[i].find("\\b") + 1, "*");
		}
		while (lineGot[i].find("\\i") != std::string::npos) {
			lineGot[i].replace(lineGot[i].find("\\i"), lineGot[i].find(" ", lineGot[i].find("\\i")) - lineGot[i].find("\\i") + 1, "*");
		}
		while (lineGot[i].find("\\ul") != std::string::npos) {
			lineGot[i].replace(lineGot[i].find("\\ul"), lineGot[i].find(" ", lineGot[i].find("\\ul")) - lineGot[i].find("\\ul") + 1, "_");
		}
		while (lineGot[i].find("\\pard") != std::string::npos) {
			lineGot[i].replace(lineGot[i].find("\\pard"), 5, ""); //Used to be replaced with "/n", but I think \pard is just something I want to delete?
		}
		while (lineGot[i].find("\\par") != std::string::npos) {
			lineGot[i].replace(lineGot[i].find("\\par"), 4, "\n");
		}
		//Delete all curly brackets that are not typed by user (user ones have "\" before them)
		while (true)
		{
			size_t beg = findNextWithoutCharBefore(lineGot[i], '{', '\\', 0);
			size_t posInLine = beg;
			if (beg != std::string::npos) //If an open bracket was found
			{
				int ignoreAClose = 0;

				while (true)
				{
					size_t nexOp = findNextWithoutCharBefore(lineGot[i], '{', '\\', posInLine + 1);
					size_t nexCl = findNextWithoutCharBefore(lineGot[i], '}', '\\', posInLine + 1);
					if (nexCl == std::string::npos) //If no close bracket, delete rest of line
					{
						if (i != 0)
						{
							std::cout << "Warning, this is very bizarre. No matching close bracket on line \"" << lineGot[i] << "\".\n";
						}
						lineGot[i].erase(beg, lineGot[i].length() - beg);
						break;
					}
					else if (nexOp == std::string::npos || nexCl < nexOp)
					{
						if (ignoreAClose == 0)
						{
							lineGot[i].erase(beg, nexCl + 1 - beg);
							break;
						}
						else
						{
							ignoreAClose--;
							posInLine = nexCl;
						}
					}
					else
					{
						ignoreAClose++;
						posInLine = nexOp;
					}
				}
			}
			else if (findNextWithoutCharBefore(lineGot[i], '}', '\\', 0) != std::string::npos) //If a close bracket with no open was found
			{
				//Who knows what caused this, just delete the close bracket. (This only happens at the end of a file(second-last line)?)
				if (i < lineGot.size() - 2)
				{
					std::cout << "Warning, this is very bizarre. No matching open bracket on line \"" << lineGot[i] << "\".\n";
				}
				lineGot[i].erase(findNextWithoutCharBefore(lineGot[i], '}', '\\', 0));
			}
			else
			{
				break;
			}
		}
		//Remove all other backslashed things except "\{", "\}", and "\\".
		while (findNextWithoutCharAfter(lineGot[i], '\\', "{}\\", 0) != std::string::npos)
		{
			lineGot[i].erase(findNextWithoutCharAfter(lineGot[i], '\\', "{}\\", 0), lineGot[i].find(" ", findNextWithoutCharAfter(lineGot[i], '\\', "{}\\", 0)) - findNextWithoutCharAfter(lineGot[i], '\\', "{}\\", 0));
		}
		//Fix  "\{", "\}", and "\\" to "{", "}", and "\".
		size_t pos = 0;
		pos = lineGot[i].find("\\{", pos);
		while (pos != std::string::npos)
		{
			lineGot[i].erase(pos, 1);
			pos = lineGot[i].find("\\{", pos);
		}
		pos = 0;
		pos = lineGot[i].find("\\}", pos);
		while (pos != std::string::npos)
		{
			lineGot[i].erase(pos, 1);
			pos = lineGot[i].find("\\}", pos);
		}
		pos = 0;
		pos = lineGot[i].find("\\\\", pos);
		while (pos != std::string::npos)
		{
			lineGot[i].erase(pos, 1);
			pos = lineGot[i].find("\\\\", pos);
		}
		//If any line begins with a space, delete the space
		if (lineGot[i][0] == ' ')
		{
			lineGot[i].erase(lineGot[i].begin());
		}
	}
	//Delete line entries that were completely wiped
	for (int i = lineGot.size() - 1; i >= 0; i--)
	{
		if (lineGot[i].length() == 0)
		{
			lineGot.erase(lineGot.begin() + i);
		}
	}
}

void SetupGL()
{
	glClearColor(1, 1, 1, 0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, GLsizei(windowWidth), GLsizei(windowHeight));
	glOrtho(-windowWidth / 2, windowWidth / 2, -windowHeight / 2, windowHeight / 2, 0.1, 10);
	//glOrtho(-windowWidth / 20, windowWidth / 20, -windowHeight / 20, windowHeight / 20, 0.1, 10);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D); //turn on texturing
	SetForegroundWindow(window.getSystemHandle()); //Set window focus - not really necessary, just prevents possible focus flag initial incorrectness.

	//setConsoleSize(100,30);
}

std::string getPrintMonth(int monthNum)
{
	switch (monthNum) {
	case 1:
		return "January";
		break;
	case 2:
		return "Febuary";
		break;
	case 3:
		return "March";
		break;
	case 4:
		return "April";
		break;
	case 5:
		return "May";
		break;
	case 6:
		return "June";
		break;
	case 7:
		return "July";
		break;
	case 8:
		return "August";
		break;
	case 9:
		return "September";
		break;
	case 10:
		return "October";
		break;
	case 11:
		return "November";
		break;
	case 12:
		return "December";
		break;
	}
	return "Unknownuary";
}

void showInstructions()
{
	std::cout << "Keybinds:\n";
	std::cout << "0: Toggle show fps\n";
	std::cout << "1: Cycle (1) draw blue \"latest entry\" dots, (2) draw teal other entry dots also, (3) draw neither\n";
	std::cout << "2: Cycle (1) draw blue \"latest entry\" line, (2) draw red average line also, (3) draw red only, (4) draw neither\n";
	std::cout << "3: Cycle (1) draw green \"earliest entry\" line, (2) draw orange average line also, (3) draw orange only, (4) draw neither\n";
	std::cout << "4: Toggle average line smoothness\n";
	std::cout << "5: Toggle draw approximates\n";
	std::cout << "6: Toggle animate approximates\n";
	return;
}

void Init()
{
	//Init timer
	worldTimer = new Timer("World Timer");
	arial = loadFont("Fonts\\arial.ttf");
	text.setFont(arial);

	averageDegree = minAverageDegree;
	pingPong = PingPong(&averageOffset, -averageRange, averageRange, 0.7f);
	heightAdjustDummyTime = new Times();

	
	showInstructions();

	//Get all lines from file
	getLines(parentDirectory + fileName, lineGot);

	//The EndOfDocument marker, "null", is ascii value 0, and the last line is just this character. Delete it.
	if (lineGot.back().length() == 1 && int(lineGot.back()[0]) == 0)
	{
		lineGot.erase(lineGot.begin() + (lineGot.size() - 1));
	}

	//for (int i = 0; i < lineGot.size(); i++)
	//{
	//	std::cout << lineGot[i] << "\n";
	//}
	//system("pause");

	//Translate rtf to plain text
	rtfToPlaintext(lineGot);

	//Unpack data into data structures
	for (int i = 2; i < lineGot.size(); i++) //Skip lines 1 and 2
	{
		//Seek '/' within first three characters.
		if (lineGot[i].length() > 3 && lineGot[i].substr(0, 3).find("/", 0))
		{
			//Save the numbers between the slashes into a "date" data structure
			Entry tempEnt = Entry();
			size_t ind = lineGot[i].substr(0, 3).find("/", 0);
			tempEnt.date.month = stoi(lineGot[i].substr(0, ind));
			size_t ind2 = lineGot[i].substr(ind + 1, 3).find("/", 0) + ind + 1;
			tempEnt.date.day = stoi(lineGot[i].substr(ind + 1, ind2 - 1 - ind));
			tempEnt.date.year = stoi(lineGot[i].substr(ind2 + 1, 4));
			//For debugging!
			//std::cout << tempEnt.date.month << "/" << tempEnt.date.day << "/" << tempEnt.date.year << "\n";
			entry.push_back(tempEnt);
		}
		else
		{
			if (!(i > int(lineGot.size()) - 30 && lineGot[i] == "\n")) //If wihtin last 30 lines and found newline instead of date, assume end of file. Exit.
			{
				std::cout << "Uncorrectable format break ";
				if (entry.size() > 0)
				{
					std::cout << "after entry " << entry.back().date.month << "/" << entry.back().date.day << "/" << entry.back().date.year << " ";
				}
				std::cout << "on line:\n\"" << lineGot[i] << "\"\nStopping read. Partial list only.";
			}
			break;
		}

		while (i < lineGot.size() - 1) //Get time entries
		{
			i++;
			if (lineGot[i] == "\n") //Break for next entry
			{
				break;
			}
			size_t pos = 0; //pos is character index in line
			entry.back().times.push_back(Times());
			//Seek "Post: " as first 6 characters.
			if (lineGot[i].substr(0, 6) == "Post: ")
			{
				//update pos to this point
				pos = 6;
				entry.back().times.back().post = true;
			}

			//Seek ':' after pos within first three characters.
			if (lineGot[i].substr(pos, 3).find(":", 0) != std::string::npos)
			{
				size_t ind = lineGot[i].substr(pos, 3).find(":", 0) + pos;
				entry.back().times.back().hour = stoi(lineGot[i].substr(pos, ind - pos));
				entry.back().times.back().minute = stoi(lineGot[i].substr(ind + 1, 2));
				entry.back().times.back().am = (lineGot[i].substr(ind + 3, 2) == "am");
				//Seek 'ish' after time
				if (lineGot[i].substr(ind + 5, 3) == "ish")
				{
					entry.back().times.back().approximate = true;
				}
				else
				{
					entry.back().times.back().approximate = false;
				}
				pos = lineGot[i].find(":", ind + 1) + 2;
			}
			else if (lineGot[i].substr(pos, 4).find("m", 0) != std::string::npos)//seek 'm' within first 4 characters
			{
				if (entry.back().date.month == 10 && entry.back().date.day == 25 && entry.back().date.year == 2021)
				{
					std::cout << "6 - " << i << "\n";
					std::cout << lineGot[i] << "\n";
				}
				size_t ind = lineGot[i].substr(pos, 4).find("m", 0) + pos;
				//Check if the "m" you found is part of a valid time
				if (!((lineGot[i][ind - 1] == 'a' && isdigit(lineGot[i][ind - 2])) || lineGot[i][ind - 1] == 'p'))
				{
					//std::cout << "Warning: Format break on line: " << lineGot[i] << ". Skipping.\n";
					entry.back().times.back().noData = true;
				}
				else
				{
					entry.back().times.back().hour = stoi(lineGot[i].substr(pos, ind - 1 - pos));
					entry.back().times.back().minute = 00;
					entry.back().times.back().am = (lineGot[i].substr(ind - 1, 2)) == "am";
					entry.back().times.back().approximate = true;
					pos = lineGot[i].find(":", ind);
					if (pos != std::string::npos)
					{
						pos += 2;
					}
				}
			}
			else //No time recorded
			{
				//std::cout << "Warning: Format break on line: " << lineGot[i] << ". Skipping.\n";
				entry.back().times.back().noData = true;
			}
			//if 'pos' marker is not past end of line
			if (pos != std::string::npos && pos < lineGot[i].length() - 1)
			{
				entry.back().times.back().comment = lineGot[i].substr(pos, lineGot[i].length() - pos - 1);//all of gotLine after and including 'pos' marker;
			}
		}
	}
	//Make sure stating animating state results correctly
	if (!animateApproximates)
	{
		pingPong.Reset(0.5f);
		pingPong.Stop();
	}
	else
	{
		pingPong.Start();
	}
	cam.MoveTo((float(entry.size()) - 5.0f) * horScalar / 2.0f, float(numHours) * vertScalar / 2.0f, ((float(entry.size()) + 20.0f) * horScalar) / windowWidth);

}

void Update(double deltaTime)
{
	cam.Update(deltaTime);

	pingPong.Update(deltaTime);

	if (restingOn == heightAdjustDummyTime)
	{
		vertScalar = (mouseLocation.y - 35.0f * (vertScalar * 0.02f)) / (numHours - 1);
		if (vertScalar < 1)
		{
			vertScalar = 1;
		}
	}
}

void Display()
{
	//Do camera stuff
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glOrtho(cam.GetOrtho(0), cam.GetOrtho(1), cam.GetOrtho(2), cam.GetOrtho(3), 0.1f, 10.0f);

	bool restingOnGraph = false;
	if (mouseLocation.x > 0 && mouseLocation.x < float(entry.size()) * horScalar && mouseLocation.y > 0 && mouseLocation.y < 23.0f * vertScalar)
	{
		restingOnGraph = true;
	}
	//Draw raster graphics

	//Draw height resize ball (vertical scale knob)
	drawLine(0, (numHours - 1) * vertScalar + 1.0f * (vertScalar * 0.02f), 0, (numHours - 1) * vertScalar + 15.0f * (vertScalar * 0.02f), torgbColor({ 0.6f, 0.6f, 0.6f }));
	drawCircle(1, 20, 20 * (vertScalar * 0.02f), 0, (numHours - 1) * vertScalar + 35.0f * (vertScalar * 0.02f), torgbColor({ 0.9f, 0.9f, 0.9f }));
	drawCircle(0, 20, 20 * (vertScalar * 0.02f), 0, (numHours - 1) * vertScalar + 35.0f * (vertScalar * 0.02f), torgbColor({ 0.6f, 0.6f, 0.6f }));

	//Draw date axis
	int month = entry[0].date.month;
	vec2 monthStart = vec2(month, 0);
	int oldMonth = month;
	for (int i = 0; i < entry.size(); i++)
	{
		//Draw grey day lines
		drawLine(i * horScalar, 0, i * horScalar, (numHours - 1) * vertScalar, torgbColor({ 0.9f, 0.9f, 0.9f }));
		//Draw month delimiter lines
		month = entry[i].date.month;
		if (i == 0 || month != oldMonth || i == entry.size() - 1)
		{
			drawLine(i * horScalar, -60, i * horScalar, (numHours - 1) * vertScalar, torgbColor({ 0.55f, 0.55f, 0.55f }));
		}
		//Draw day-you-are-on line
		if (i == restingOnColumn && restingOnGraph)
		{
			drawLine(i * horScalar, 0, i * horScalar, (numHours - 1) * vertScalar, torgbColor({ 0.5f, 0, 0.5f }));
		}
		//Draw month names
		if (month != oldMonth || i == entry.size() - 1)
		{
			vec2 loc = cam.WorldToScreenCoords({ ((monthStart.y + float(i)) / 2.0f) * horScalar, -50.0f }, window);
			if (loc.y < 16)
			{
				loc.y = 16;
			}
			drawText(getPrintMonth(oldMonth), text, loc.x, loc.y, 25 / cam.scale, -1, window, 1);
			monthStart = vec2(month, i);
		}
		oldMonth = month;
	}

	//Draw day label
	if (restingOnGraph)
	{
		vec2 loc = cam.WorldToScreenCoords({ restingOnColumn * horScalar, -25.0f }, window);
		if (loc.y < 40 / cam.scale)
		{
			loc.y = 40 / cam.scale;
		}
		drawText(std::to_string(entry[restingOnColumn].date.day), text, loc.x, loc.y, 25 / cam.scale, -1, window, 1);
		loc = cam.WorldToScreenCoords({ restingOnColumn * horScalar, float(numHours - 1) * vertScalar + 7.0f }, window);
		if (loc.y > windowHeight - 25.0f / cam.scale)
		{
			loc.y = windowHeight - 25.0f / cam.scale;
		}
		drawText(std::to_string(entry[restingOnColumn].date.year), text, loc.x, loc.y, 25 / cam.scale, -1, window, 1);
	}

	//Draw time axis
	for (int i = 0; i < numHours; i++)
	{
		std::string dispTime = std::to_string(i);
		bool amEd = false;
		if (i >= 12)
		{
			if (i < 24)
			{
				dispTime = std::to_string((i == 12 ? i : i - 12)) + "am";
				amEd = true;
			}
		}
		if (i == 0)
		{
			dispTime = "12";
		}
		if (!amEd)
		{
			dispTime += "pm";
		}
		vec2 loc = cam.WorldToScreenCoords({ -5.0f , i * vertScalar }, window);
		if (loc.x < 90 / cam.scale)
		{
			loc.x = 90 / cam.scale;
		}
		drawText(dispTime, text, loc.x, loc.y, 25 / cam.scale * (vertScalar * 0.03f), -1, window, 2);
		drawLine(0, i * vertScalar, (entry.size() - 1) * horScalar, i * vertScalar, torgbColor({ 0, 0, 0 }));
	}

	vec2 lastLatePoint;
	vec2 lastEarlyPoint;
	float lastLateAveragePoint;
	float lastEarlyAveragePoint;
	vec2 smallestRowDist = vec2(std::abs(entry[0].times.back().getHeight(0) * vertScalar - mouseLocation.y), 0);
	vec2 smallestColumnDist = vec2(mouseLocation.x, 0);
	float smallestDist = (vec2(0, entry[0].times.back().getHeight(0) * vertScalar) - mouseLocation).LengthSquared();
	Times* smallestDistT = &entry[0].times.back();
	int smallestEntryInd = 0;
	int foundEarly = 0;
	vec2 thisPointHolderForLastEarlyPoint;
	//Graph data
	for (int i = 0; i < entry.size(); i++)
	{
		foundEarly = 0;
		//Draw raw data
		for (int i2 = 0; i2 < entry[i].times.size(); i2++)
		{
			bool lastTime = (i2 == entry[i].times.size() - 1);
			if (foundEarly == 0 && (!entry[i].times[i2].post || lastTime))
			{
				foundEarly = 1;
			}
			vec2 thisPoint = vec2(i * horScalar, entry[i].times[i2].getHeight(i) * vertScalar);
			vec2 thisPointMiddle = vec2(i * horScalar, entry[i].times[i2].getHeight(i, false) * vertScalar);
			vec2 currentDist = vec2((thisPoint - mouseLocation).LengthSquared(), i);
			vec2 currentRowDist = vec2(std::abs(thisPoint.y - mouseLocation.y), i);
			vec2 currentColumnDist = vec2(std::abs(thisPoint.x - mouseLocation.x), i);
			if (currentDist.x < smallestDist)
			{
				smallestDist = currentDist.x;
				smallestDistT = &entry[currentDist.y].times[i2];
				smallestEntryInd = i;
			}
			if (currentRowDist.x < smallestRowDist.x)
			{
				smallestRowDist = currentRowDist;
			}
			if (currentColumnDist.x < smallestColumnDist.x)
			{
				smallestColumnDist = currentColumnDist;
			}

			//Draw average ranges
			rgbColour avgClr = torgbColor({ 1, 0.6f, 0.6f });
			if (drawApproximates && entry[i].times[i2].approximate)
			{
				drawLine(thisPointMiddle.x, thisPointMiddle.y - (averageRange * vertScalar), thisPointMiddle.x, thisPointMiddle.y + (averageRange * vertScalar), avgClr);
				drawLine(thisPointMiddle.x - (0.05f * vertScalar), thisPointMiddle.y + (averageRange * vertScalar), thisPointMiddle.x + (0.05f * vertScalar), thisPointMiddle.y + (averageRange * vertScalar), avgClr);
				drawLine(thisPointMiddle.x - (0.05f * vertScalar), thisPointMiddle.y - (averageRange * vertScalar), thisPointMiddle.x + (0.05f * vertScalar), thisPointMiddle.y - (averageRange * vertScalar), avgClr);
			}

			//Draw latest and earliest lines
			if (drawEarlyLine && i > 0 && foundEarly == 1)
			{
				drawLine(lastEarlyPoint.x, lastEarlyPoint.y, thisPoint.x, thisPoint.y, torgbColor({ 0, 1, 0 }));
			}
			if (drawMainLine && i > 0 && lastTime)
			{
				drawLine(lastLatePoint.x, lastLatePoint.y, thisPoint.x, thisPoint.y, torgbColor({ 0, 0.3f, 1 }));
			}
			if (foundEarly == 1)
			{
				thisPointHolderForLastEarlyPoint = thisPoint;
				//lastEarlyPoint = thisPoint;
				foundEarly = 2;
			}
		}
		vec2 thisPoint = vec2(i * horScalar, entry[i].times.back().getHeight(i) * vertScalar);
		//Draw average line
		if (drawMainLineAverage || drawEarlyLineAverage)
		{
			float amountBeforeAfter = ((averageDegree % 2) == 0 ? (averageDegree / 2) : ((averageDegree - 1) / 2));
			int i2Min = i - averageDegree / 2;
			int i2Max = i + averageDegree / 2;
			float numTaken = ((averageDegree % 2) == 0 ? averageDegree + 1 : averageDegree);
			if (i - averageDegree / 2 < 0)
			{
				i2Min -= i - averageDegree / 2;
				numTaken += i - averageDegree / 2;
			}
			if (i + averageDegree / 2 >= entry.size())
			{
				i2Max -= (i + averageDegree / 2) - (int(entry.size()) - 1);
				numTaken -= (i + averageDegree / 2) - (int(entry.size()) - 1);
			}
			float thisAveragePoint = 0;
			if (drawMainLineAverage)
			{
				for (int i2 = i2Min; i2 <= i2Max; i2++)
				{
					thisAveragePoint += entry[i2].times.back().getHeight(i2) * vertScalar; //This is the line that slows things down. It's accessing entry.times.back - for some reason, this takes hugely longer than saying, "+= 5"
				}
				thisAveragePoint /= numTaken;

				if (i > 0)
				{
					drawLine(lastLatePoint.x, lastLateAveragePoint, thisPoint.x, thisAveragePoint, torgbColor({ 1, 0, 0 }));
				}
				lastLateAveragePoint = thisAveragePoint;
			}
			thisAveragePoint = 0;
			if (drawEarlyLineAverage)
			{
				for (int i2 = i2Min; i2 <= i2Max; i2++)
				{
					for (int i3 = 0; i3 <= entry[i2].times.size(); i3++)
					{
						bool lastTime = (i3 == entry[i2].times.size() - 1);
						if (!entry[i2].times[i3].post || lastTime)
						{
							thisAveragePoint += entry[i2].times[i3].getHeight(i2) * vertScalar; //This is the line that slows things down. It's accessing entry.times.back - for some reason, this takes hugely longer than saying, "+= 5"
							break;
						}
					}
				}
				thisAveragePoint /= numTaken;

				if (i > 0)
				{
					drawLine(lastEarlyPoint.x, lastEarlyAveragePoint, thisPoint.x, thisAveragePoint, torgbColor({ 1, 0.5f, 0 }));
				}
				lastEarlyAveragePoint = thisAveragePoint;
			}
		}
		lastLatePoint = thisPoint;
		lastEarlyPoint = thisPointHolderForLastEarlyPoint;
	}
	//Draw circles
	if (drawMainDots || drawOtherDots)
	{
		for (int i = 0; i < entry.size(); i++)
		{
			for (int i2 = 0; i2 < entry[i].times.size(); i2++)
			{
				bool lastTime = (i2 == entry[i].times.size() - 1);
				vec2 thisPoint = vec2(i * horScalar, entry[i].times[i2].getHeight(i) * vertScalar);

				rgbColour circleClr = (lastTime ? torgbColor({ 0, 0.3f, 1 }) : torgbColor({ 0, 1, 1 }));
				float sizeScale = (vertScalar * 0.019f);
				if (vertScalar > 60)
				{
					sizeScale = (60.0f * 0.019f);
				}
				if (entry[i].times.back().noData)
				{
					circleClr = torgbColor({ 1, 0, 0 });
				}
				if (lastTime && drawMainDots || !lastTime && drawOtherDots)
				{
					drawCircle(1, 10, (lastTime ? 5.0f : 3.0f)* sizeScale, thisPoint.x, thisPoint.y, circleClr);
				}
				if (!lastTime && drawOtherDots)
				{
					drawCircle(0, 10, 3.0f* sizeScale, thisPoint.x, thisPoint.y, torgbColor({ 0, 0.6f, 0.6f }));
				}
			}
		}
	}
	if (restingOn != heightAdjustDummyTime)
	{
		if (smallestDist < 500.0f)
		{
			restingOn = smallestDistT;
			restingOnEntry = smallestEntryInd;
		}
		else
		{
			restingOn = nullptr;
			restingOnEntry = -1;
		}
	}
	restingOnRow = smallestRowDist.y;
	restingOnColumn = smallestColumnDist.y;

	//Draw hover time text
	if (restingOn != nullptr && restingOn != heightAdjustDummyTime)
	{
		vec2 mpoint = cam.WorldToScreenCoords(mouseLocation, window);
		drawText(restingOn->getPrintTime(), text, mpoint.x, mpoint.y, 20, -1, window, 2);
	}
}

//This is the automatic one
void ReShapeWindow(float width, float height)
{
	windowWidth = width;
	windowHeight = height;
	SetupGL();

	//Reset SFML's "view" - for fixing text
	sf::View mGUIView = sf::View(sf::FloatRect(0.f, 0.f, windowWidth, windowHeight));
	window.setView(mGUIView);
}

//This is the manual (in code) one
void ReSizeWindow(float x1, float y1, float x2, float y2, char windowTitle[])
{
	LPCSTR windTitle = windowTitle;
	HWND handle = FindWindow(NULL, windTitle);
	SetWindowPos(handle, 0, x1, GetSystemMetrics(SM_CYSCREEN) - (y1 + y2), x2, y2, SWP_NOOWNERZORDER | SWP_NOZORDER);
	ReShapeWindow(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) * 0.8f);
}

void HandleKeyboardFree(float deltaTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		cam.Move(0, 1, deltaTime * cam.scale);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		cam.Move(0, -1, deltaTime * cam.scale);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		cam.Move(-1, 0, deltaTime * cam.scale);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		cam.Move(1, 0, deltaTime * cam.scale);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageUp) || sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		cam.ZoomIn(deltaTime * cam.scale);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageDown) || sf::Keyboard::isKeyPressed(sf::Keyboard::E))
	{
		cam.ZoomOut(deltaTime * cam.scale);
	}

	/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::PageUp) || sf::Keyboard::isKeyPressed(sf::Keyboard::Q) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::PageDown) || sf::Keyboard::isKeyPressed(sf::Keyboard::E))
	{
		std::cout << cam.pos.x << ", " << cam.pos.y << ", " << cam.scale << "\n";
	}*/

	holdingLShift = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
	holdingRShift = sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);
	holdingLCtrl = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl);
	holdingLAlt = sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt);
}

void HandleKeyboard()
{

	/*
	std::cout << "Keybinds:\n";
	std::cout << "0: Toggle show fps\n";
	std::cout << "1: Toggle show fps\n";
	std::cout << "2: Cycle (1) draw blue \"latest entry\" line, (2) draw red average line also, (3) draw both, (4) draw neither\n";
	std::cout << "3: Cycle (1) draw green \"earliest entry\" line, (2) draw orange average line also, (3) draw both, (4) draw neither\n";
	std::cout << "4: Toggle average line smoothness\n";
	std::cout << "5: Toggle draw approximates\n";
	std::cout << "6: Toggle animate approximates\n";
	*/
	if (event.type == sf::Event::KeyPressed)
	{
		switch (event.key.code) {
		case sf::Keyboard::Escape:
			running = false;
			break;
		case sf::Keyboard::Num0:
			showFps = !showFps;
			break;
		case sf::Keyboard::Num1:
			if (drawMainDots && drawOtherDots)
			{
				drawMainDots = false;
				drawOtherDots = false;
			}
			else if (!drawMainDots && !drawOtherDots)
			{
				drawMainDots = true;
			}
			else if (drawMainDots && !drawOtherDots)
			{
				drawOtherDots = true;
			}
			break;
		case sf::Keyboard::Num2:
			if (drawMainLine && !drawMainLineAverage)
			{
				drawMainLineAverage = true;
			}
			else if (drawMainLine && drawMainLineAverage)
			{
				drawMainLine = false;
			}
			else if (!drawMainLine && drawMainLineAverage)
			{
				drawMainLineAverage = false;
			}
			else if (!drawMainLine && !drawMainLineAverage)
			{
				drawMainLine = true;
			}
			break;
		case sf::Keyboard::Num3:
			if (drawEarlyLine && !drawEarlyLineAverage)
			{
				drawEarlyLineAverage = true;
			}
			else if (drawEarlyLine && drawEarlyLineAverage)
			{
				drawEarlyLine = false;
			}
			else if (!drawEarlyLine && drawEarlyLineAverage)
			{
				drawEarlyLineAverage = false;
			}
			else if (!drawEarlyLine && !drawEarlyLineAverage)
			{
				drawEarlyLine = true;
			}
			break;
		case sf::Keyboard::Num4:
			if (averageDegree > minAverageDegree)
			{
				averageDegree = minAverageDegree;
			}
			else
			{
				averageDegree = maxAverageDegree;
			}
			break;
		case sf::Keyboard::Num5:
			drawApproximates = !drawApproximates;
			break;
		case sf::Keyboard::Num6:
			animateApproximates = !animateApproximates;
			if (!animateApproximates)
			{
				pingPong.Reset(0.5f);
				pingPong.Stop();
			}
			else
			{
				pingPong.Start();
			}
			break;
		}
	}
}

void HandleMouse(bool isClick) //Get mouse position and movement
{							   //This code only runs when there is a mouse or keyboard update, AKA when the mouse is moved or clicked (or a key is pressed?).
	mouseScreenLocationOld = mouseScreenLocation; //Save old mouse location
	if (event.type == sf::Event::MouseMoved)
	{
		//Update mouse position
		mouseScreenLocation.x = event.mouseMove.x;
		mouseScreenLocation.y = windowHeight - event.mouseMove.y;
	}
	mouseLocationOld = mouseLocation; //Save old mouse location
	mouseLocation = cam.ScreenToWorldCoords(mouseScreenLocation, window);

	if (isClick)
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		{
			if ((mouseLocation - vec2(0, (numHours - 1) * vertScalar + 35.0f * (vertScalar * 0.02f))).Length() <= 20 * (vertScalar * 0.02f))
			{
				restingOn = heightAdjustDummyTime;
			}
			else if (restingOn != heightAdjustDummyTime)
			{
				system("cls");
				if (restingOn != nullptr && restingOnEntry != -1)
				{
					std::cout << entry[restingOnEntry].getPrintDate() << ", " << restingOn->getPrintTime() << ": " << restingOn->comment;
				}
				else
				{
					showInstructions();
				}
			}
		}
		else //Click released
		{
			if (restingOn == heightAdjustDummyTime)
			{
				restingOn = nullptr;
			}
		}
	}
}

int main()
{
	char windowTitle[] = "Sleep Diary Grapher";
	window.create(sf::VideoMode(unsigned int(windowWidth), unsigned int(windowHeight)), windowTitle);

	SetupGL();

	//Resize all windows:
	float screenX = GetSystemMetrics(SM_CXSCREEN);
	float screenY = GetSystemMetrics(SM_CYSCREEN);
	ReSizeWindow(0, screenY * 0.2f, screenX, screenY * 0.8f, windowTitle);
	RECT rect;
	int winWidth = 0;
	int winHeight = 0;
	if (GetWindowRect(GetConsoleWindow(), &rect))
	{
		winWidth = rect.right - rect.left;
		winHeight = rect.bottom - rect.top;
	}
	vec2 consoleSize = getConsoleSizeVec2();
	/*
	std::cout << "screenX: " << screenX << " screenY: " << screenY << "\n";
	std::cout << "winWidth: " << winWidth << " winHeight: " << winHeight << "\n";
	std::cout << "consoleSize.x: " << consoleSize.x << " consoleSize.y: " << consoleSize.y << "\n";
	std::cout << "Console character -> pixel multipliers are " << float(winWidth) / (consoleSize.x+4) << " " << float(winHeight) / consoleSize.y << "\n";
	*/
	float charxToPixelRatio = float(winWidth) / (consoleSize.x + 3); //Magic number 3! This simulates getting the console size in chars, then adding 3 for some reason!
	setConsoleSize(screenX / charxToPixelRatio, 25);
	SetWindowPos(GetConsoleWindow(), 0, 0, screenY * 0.8f, screenX * charxToPixelRatio, screenY * 0.16f, SWP_NOZORDER);
	//MoveWindow(GetConsoleWindow(), 0, screenY * 0.8f, screenX, screenY, true);

	//Search for file
	if (!fileExists(parentDirectory + fileName)) //if not found, search for text file with url in it
	{
		if (fileExists(directoryLocFile))
		{
			//Get contents
			std::string dirFileContents;
			inputStream.open(directoryLocFile, std::ios::out);
			std::string temp;
			std::getline(inputStream, dirFileContents);
			inputStream.close();
			std::cout << dirFileContents << "\n";
			convertLocalDirectory(dirFileContents);
			if (!fileExists(dirFileContents))
			{
				//Tell user it was an invalid directory and to check if it is correct (hint, it's not)
				std::cout << "Invalid directory - Directory \"" << dirFileContents << "\" is Invalid. Is the directory path "
					<< "typed in the \"GrapherDirectory.txt\" file correct? (Should look something like \"C:\\Users\\...\")\n";
				if (!askUserBool("Do you want to enter a directory manually now?"))
				{
					running = false;
					return 0;
				}
				std::string usersDirectory;
				if (!askUserDirectory(&usersDirectory))
				{
					//Tell user it was an invalid directory and close.
					std::cout << "Invalid directory. Program closing.\n";
				}
				else
				{
					parentDirectory = usersDirectory.substr(0, usersDirectory.find_last_of('\\') + 1);
					fileName = usersDirectory.substr(usersDirectory.find_last_of('\\') + 1, usersDirectory.length() - usersDirectory.find_last_of('\\') - 1);
				}
			}
			else
			{
				parentDirectory = dirFileContents.substr(0, dirFileContents.find_last_of('\\') + 1);
				fileName = dirFileContents.substr(dirFileContents.find_last_of('\\') + 1, dirFileContents.length() - dirFileContents.find_last_of('\\') - 1);
			}
		}
		else //if not found
		{
			//ask user to type a directory, or to create the text file for the future.
			std::cout << "You must enter a directory and filename to run this program on. Or, create a text document called, \"GrapherDirectory.txt\" and put that "
				<< "in the same directory as this exe, and then type a single line in that text file that is the directory of your file (with filename included).\n";
			if (!askUserBool("Do you want to enter a directory manually now?"))
			{
				running = false;
				return 0;
			}
			std::string usersDirectory;
			if (!askUserDirectory(&usersDirectory))
			{
				//Tell user it was an invalid directory and close.
				std::cout << "Invalid directory. Program closing.\n";
				running = false;
				return 0;
			}
			else
			{
				parentDirectory = usersDirectory.substr(0, usersDirectory.find_last_of('\\') + 1);
				fileName = usersDirectory.substr(usersDirectory.find_last_of('\\') + 1, usersDirectory.length() - usersDirectory.find_last_of('\\') - 1);
			}
		}
	}

	

	Init();
	
	double deltaTime;
	while (running)
	{
		worldTimer->tick();
		deltaTime = worldTimer->getElapsedTimeMS(); //deltaTime retrieved from SFML's Clock class

		if (inFocus)
		{
			HandleKeyboardFree(deltaTime);
		}
		while (window.pollEvent(event))
		{

			if (event.type == sf::Event::GainedFocus) inFocus = true;
			if (event.type == sf::Event::LostFocus) inFocus = false;

			if (inFocus)
			{
				HandleKeyboard();

				if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased)
					HandleMouse(true);

				HandleMouse(false);
			}

			if (event.type == sf::Event::Closed)
				running = false;
			else if (event.type == sf::Event::Resized)
				ReShapeWindow(float(event.size.width), float(event.size.height));
		}

		if (inFocus)
		{
			Update(deltaTime);
			Display();

			//fps output
			if (showFps)
			{
				drawText(std::to_string(fpsManualCount) + " fps", text, 10, windowHeight - 20, 20, -1, window);
				fpsTimeCounter += deltaTime;
				fpsCounter++;
				if (fpsTimeCounter >= 1000)
				{
					fpsTimeCounter = 0;
					fpsManualCount = fpsCounter;
					//SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0,0 });
					//system("cls"); //Clear the console window
					//std::cout << fpsCounter << " fps\n";
					//std::cout << "Actual: " << fpsCounter << "\n";			//Fps manually counted each frame - unyeildingly accurate
					//std::cout << "Rough: " << 1000.00 / deltaTime << "\n";	//Fps calculated according to deltaTime (which is got from SFML's Clock class)
					//std::cout << "Clock: " << 1000.00 / tmp << "\n";		//Fps calculated using inbuilt clock
					//std::cout << "Difference: " << (1000.00 / tmp) - fpsCounter << "\n";
					fpsCounter = 0;
				}
			}
			window.display();
		}
	}
	std::cout << "Program closing.\n";
}
