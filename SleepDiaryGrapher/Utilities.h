#pragma once
#include <sstream>
#include <direct.h>

//The following were includes it needed before, but doesn't need now for some reason! ??
//#undef UNICODE // For renameConsoleWindow
//#include <windows.h> //For toClipboard
//#include <iostream>
//#include <limits> //Was for "min", but now I'm using "std::min" instead
//#include <sstream>
//#include <string>

static float LERP(float data1, float data2, float u)
{
	return data1 * (1.0f - u) + data2 * u;
}

static vec2 LERPvec2(vec2 data1, vec2 data2, float u)
{
	return vec2(data1.x * (1.0f - u) + data2.x * u, data1.y * (1.0f - u) + data2.y * u);
}

static float Catmull(float data0, float data1, float data2, float data3, float u)
{
	return 0.5f * (u*u*u*(-data0 + 3.0f*data1 - 3.0f*data2 + data3) + u * u*(2.0f*data0 - 5.0f*data1 + 4.0f*data2 - data3) + u * (-data0 + data2) + (2.0f*data1));
}

static float Bezier(float data0, float data1, float data2, float data3, float u)
{
	return (u*u*u*(-data0 + 3.0f*data1 - 3.0f*data2 + data3) + u * u*(3.0f*data0 - 6.0f*data1 + 3.0f*data2) + u * (-3 * data0 + 3 * data1) + (data0));
}

static bool isOverlappingSquares(vec2 sq1Pos, vec2 sq1Size, vec2 sq2Pos, vec2 sq2Size)
{
	if (sq1Pos.x + sq1Size.x / 2.0f > sq2Pos.x - sq2Size.x / 2.0f && sq1Pos.x - sq1Size.x / 2.0f < sq2Pos.x + sq2Size.x / 2.0f &&
		sq1Pos.y + sq1Size.y / 2.0f > sq2Pos.y - sq2Size.y / 2.0f && sq1Pos.y - sq1Size.y / 2.0f < sq2Pos.y + sq2Size.y / 2.0f)
	{
		return true;
	}
	return false;
}

static std::string ftos(float f, int decimalPlaces)
{
	/*std::ostringstream ostr;
	int tens = std::stoi("1" + std::string(nd, '0'));
	ostr << round(f*tens) / tens;
	return ostr.str();*/
	std::string s(16, '\0');
	std::string format = "%." + std::to_string(decimalPlaces) + "f";
	auto written = std::snprintf(&s[0], s.size(), format.c_str(), f);
	s.resize(written);
	return s;
}

static double strFlOK(const std::string& s)
{
	if (s.find_first_not_of("0123456789+-.") != std::string::npos)
	{
		return false;
	}
	std::istringstream i(s);
	double x;
	if (!(i >> x))
		return false;
	return true;
}

static void convertLocalDirectory(std::string& dirIn)
{
	if (dirIn.substr(0, 2) == ".\\")
	{
		const size_t size = 1024;
		// Allocate a character array to store the directory path
		char buffer[size];

		// Call _getcwd to get the current working directory and store it in buffer
		if (_getcwd(buffer, size) != NULL) {
			// print the current working directory
			std::string cwd = buffer;
			dirIn = cwd + dirIn.substr(1, dirIn.length() - 1);
		}
		else {
			// If _getcwd returns NULL, print an error message
			std::cout << "Error getting current working directory." << std::endl;
		}
	}
}

static bool filePathExists(const std::string& path)
{
	struct stat buffer;
	return (stat((path.substr(0, path.length() - 1)).c_str(), &buffer) == 0);
	return true;
}

static bool fileExists(const std::string& file)
{
	struct stat buffer;
	return (stat(file.c_str(), &buffer) == 0);
}
/*Version of the function with unnessessary checks! Yay! .__.
static bool fileExists(const std::string& path)
{
	bool isPath = filePathExists(path.substr(0, path.length() - path.find_last_of("\\") + 1));
	if (isPath)
	{
		struct stat buffer;
		return (stat(file.c_str(), &buffer) == 0);
	}
	else
	{
		return false;
	}
}*/

static std::string changeCase(std::string in, bool toUpper)
{
	for (auto & c : in) c = (toUpper ? toupper(c) : tolower(c));
	return in;
}

static std::string simplifyString(std::string input, std::string lookingFor = "", bool caseSensative = false, bool acceptOnlyFirstLetter = true, bool acceptIfContainsWord = true)
{
	//std::cout << "input: " << input << " ";
	if (!caseSensative)
	{
		for (auto & c : input) c = tolower(c);
		for (auto & c : lookingFor) c = tolower(c);
	}
	size_t lookForWordFrom = 0;
	bool foundValidWord = false;
	int throughtheloop = 0;
	while (!foundValidWord && lookForWordFrom != std::string::npos && throughtheloop < 5000) //That last term is just an infinite-loop failsafe ;P
	{
		//std::cout << throughtheloop << "\n";
		throughtheloop++;
		//std::cout << "Z: " << input << " ?in " << lookingFor << " = " << (input.find(lookingFor) != std::string::npos) << "\n";
		int lookingforindex = input.find(lookingFor, lookForWordFrom);
		size_t temp = input.find(lookingFor, lookForWordFrom);
		//std::cout << "searching \"" << input << "\"s \"" << input.substr(lookForWordFrom) << "\"\n";
		lookForWordFrom = temp + lookingFor.length();
		if (lookingFor.length() <= 0 || lookingFor.length() > 0 && temp != std::string::npos)
		{
			if (acceptIfContainsWord && lookingFor.length() > 0)
			{
				//A = 65, S = 83, Z = 90, a = 97, s = 115, z = 122
				int afterChar = 0;
				if (lookingforindex + lookingFor.length() < input.length())
				{
					afterChar = int(input[lookingforindex + lookingFor.length()]);
				}
				/*std::cout << char(afterChar) << "\n";
				std::cout << "(" << (lookingforindex - 1 < 0 || input[lookingforindex - 1] == ' ') << " && ("<< (lookingforindex + lookingFor.length() >= input.length()) << " || " << //If char before doesn't exist or = "space", and char after doesn't exist or
				"(" << (afterChar == 115 || !(afterChar >= 97 && afterChar <= 122)) << " && " << //If afterChar = s or is not a lowercase letter or
				(afterChar == 83 || !(afterChar >= 65 && afterChar <= 90)) << ")))"				 //If afterChar = S or is not an uppercase letter
				<< "\n";
				std::cout << ((lookingforindex - 1 < 0 || input[lookingforindex - 1] == ' ') && (lookingforindex + lookingFor.length() >= input.length() || //If char before doesn't exist or = "space", and char after doesn't exist or
				((afterChar == 115 || !(afterChar >= 97 && afterChar <= 122)) && //If afterChar = s or is not a lowercase letter or
				(afterChar == 83 || !(afterChar >= 65 && afterChar <= 90)))))    //If afterChar = S or is not an uppercase letter
				<< "\n";
				*/
				if ((lookingforindex - 1 < 0 || input[lookingforindex - 1] == ' ') && (lookingforindex + lookingFor.length() >= input.length() || //If char before doesn't exist or = "space", and char after doesn't exist or 
					((afterChar == 115 || !(afterChar >= 97 && afterChar <= 122)) && //If afterChar = s or is not a lowercase letter or
					(afterChar == 83 || !(afterChar >= 65 && afterChar <= 90)))))    //If afterChar = S or is not an uppercase letter
				{
					//std::cout << "character after is " << afterChar << "\n";
					//std::cout << (lookingforindex + lookingFor.length() < input.length())<<" && "<<((afterChar == 115 || afterChar == 83)) << "\n";
					if (lookingforindex + lookingFor.length() < input.length() && (afterChar == 115 || afterChar == 83)) //If afterchar is an s
					{
						//std::cout << "character after s detected\n";
						if (lookingforindex + lookingFor.length() + 1 >= input.length() || !( //If the character after the s doesn't exist or isn't
							(input[lookingforindex + lookingFor.length() + 1] >= 65 && input[lookingforindex + lookingFor.length() + 1] <= 90) || //from A-Z or
							(input[lookingforindex + lookingFor.length() + 1] >= 97 && input[lookingforindex + lookingFor.length() + 1] <= 122))) //from a-z
						{
							foundValidWord = true;
							input = lookingFor;
						}
					}
					else //if the word stands alone with void or non-char on both sides
					{
						foundValidWord = true;
						input = lookingFor;
					}
				}
			}
		}
		else //No lookingFor given, or lookingFor not found in string
		{
			return input;
		}
		if (acceptOnlyFirstLetter && (lookingFor.length() <= 0 || input[0] == lookingFor[0]))
		{
			foundValidWord = true;
			input = input[0];
		}
		//std::cout << input << "\n";
		//std::cout << (foundValidWord ? "foundValidWord" : "null") << "\n";
		//system("pause");
	}
	//std::cout << (foundValidWord ? "foundValidWord" : "null") << "\n";
	return input;
}

static std::string simplifyString(std::string input, bool caseSensative, bool acceptOnlyFirstLetter = true)
{
	return simplifyString(input, "", caseSensative, acceptOnlyFirstLetter);
}

static bool stringEquals(std::string primary, std::string filter, bool caseSensative = false, bool acceptOnlyFirstLetter = true, bool acceptIfContainsWord = true)
{
	//std::cout << simplifyString(primary, filter, caseSensative, acceptOnlyFirstLetter, acceptIfContainsWord) << " == " << simplifyString(filter, caseSensative, acceptOnlyFirstLetter) << "?\n";
	return (simplifyString(primary, filter, caseSensative, acceptOnlyFirstLetter, acceptIfContainsWord) == simplifyString(filter, caseSensative, acceptOnlyFirstLetter));
}

static struct findFloatResult
{
	bool success;
	float value;
};
static findFloatResult findFloatInString(std::string userInput, bool acceptNegatives = true, bool acceptDecimals = true, bool acceptIfContainsNumber = true, float rangeMin = 0, float rangeMax = 0, bool onlyAcceptLookFor = false, float lookingFor = 0)
{
	bool noFloatFound = true;
	std::string numAttempt;
	std::size_t continueFrom = 0;
	bool keepSearching = true;
	if (!acceptIfContainsNumber)
	{
		if (strFlOK(userInput))
		{
			numAttempt = userInput;
			noFloatFound = false;
		}
	}
	else
	{
		while (keepSearching && continueFrom != std::string::npos)
		{
			bool noMorePeriods = false;
			std::size_t const n = userInput.find_first_of("0123456789+-.", continueFrom);

			if (n != std::string::npos)
			{
				if (userInput[n] == '.')
				{
					noMorePeriods = true;
				}
				std::size_t m;
				if (n + 1 >= userInput.length())
				{
					m = std::string::npos;
				}
				else
				{
					m = userInput.find_first_not_of("0123456789", n + 1);
					if (m != std::string::npos && !noMorePeriods && userInput[m] == '.' && m + 1 < userInput.length())
					{
						std::size_t mOld = m;
						m = userInput.find_first_not_of("0123456789", m + 1);
						if (m != std::string::npos && m - mOld <= 1) //then it found a letter right after a '.', making the number before the '.' a whole number.
						{
							m = mOld; //Abort finding stuff after the decimal, just take the whole number as the result.
						}
					}
				}
				numAttempt = userInput.substr(n, m != std::string::npos ? (m - n == 0 ? 1 : m - n) : m);
				if (!acceptDecimals && numAttempt.find_first_of(".") != std::string::npos ||
					!acceptNegatives && numAttempt.find_first_of("-") != std::string::npos)
				{
					numAttempt = "null";
				}
				continueFrom = m;
			}
			else
			{
				numAttempt = "null";
				keepSearching = false;
			}
			if (strFlOK(numAttempt))
			{
				keepSearching = false;
				noFloatFound = false;
			}
		}
	}

	if (!noFloatFound)
	{
		if (numAttempt.length() > 35) //Number is too long for stof function. Reject as nonsense.
		{
			return findFloatResult{ false, 0 };
		}
		float resultNumber = stof(numAttempt);
		if (!(rangeMin == 0 && rangeMax == 0))
		{
			if (!(resultNumber >= rangeMin && resultNumber <= rangeMax))
			{
				noFloatFound = true;
			}
		}
		if (!noFloatFound)
		{
			if (!onlyAcceptLookFor || (onlyAcceptLookFor && resultNumber == lookingFor))
			{
				return findFloatResult{ true, resultNumber };
			}
		}
	}
	return findFloatResult{ false, 0 };
}

static std::string askUser(std::string question, bool newlineAnswer, std::vector<std::string> options = std::vector<std::string>({ "Yes","No" }), bool showAnswers = true, bool listOptionsVertically = false, bool caseSensative = false, bool acceptOnlyFirstLetter = true, bool acceptIfContainsWord = true, bool numberOptions = false)
{
	bool addAllTheThesaurusWords = false;
	if (options.size() == 2 && options[0] == "Yes" && options[1] == "No")
	{
		addAllTheThesaurusWords = true;
	}
	bool inputPassthroughOnly = false;
	if (options.size() == 2 && options[0] == "" && options[1] == "")
	{
		showAnswers = false;
		inputPassthroughOnly = true;
	}
	bool keepAsking = true;
	std::string userInput;
	int times = 0;
	while (keepAsking)
	{
		if (times >= 3)
		{
			times = 0;
		}
		if (times == 0)
		{
			std::cout << question;
			if (showAnswers)
			{
				if (listOptionsVertically)
				{
					std::cout << "\n";
					for (int i = 0; i < options.size(); i++)
					{
						if (numberOptions)
						{
							std::cout << "(" << i + 1 << ") ";
						}
						std::cout << options[i] << "\n";
					}
				}
				else
				{
					std::cout << " (";
					for (int i = 0; i < options.size(); i++)
					{
						if (numberOptions)
						{
							std::cout << i + 1 << ": ";
						}
						std::cout << options[i];
						if (i < options.size() - 1)
						{
							if (options.size() > 2)
							{
								std::cout << ", ";
							}
							else
							{
								std::cout << "/";
							}
						}
					}
					std::cout << ")";
				}
			}
			if (!(showAnswers && listOptionsVertically))
			{
				std::cout << (newlineAnswer ? "\n" : ": ");
			}
		}
		std::getline(std::cin, userInput);
		int answersSaid = 0;
		if (inputPassthroughOnly)
		{
			keepAsking = false;
			return userInput;
		}
		else
		{
			std::string answerSave;
			for (int i = 0; i < options.size(); i++)
			{
				//::cout << "testing " << userInput << " against " << options[i] << "\n";
				if (numberOptions)
				{
					findFloatResult floatFound = findFloatInString(userInput, false, false, acceptIfContainsWord, 0, 0, true, i + 1);
					if (floatFound.success)
					{
						return std::to_string(floor(floatFound.value));
					}
				}
				else if (stringEquals(userInput, options[i], caseSensative, acceptOnlyFirstLetter, acceptIfContainsWord))
				{

					answersSaid++;
					keepAsking = false;
					if (answersSaid > 1)
					{
						keepAsking = true;
						break;
					}
					else
					{
						//answerSave = simplifyString(userInput, options[i], caseSensative, acceptOnlyFirstLetter, acceptIfContainsWord);
						answerSave = options[i];
					}
				}
			}
			if (answersSaid <= 0) //If no matches
			{
				if (addAllTheThesaurusWords || options.size() == 2) //The second term here means if there's any two options, run with "former" and "latter"
				{
					bool isYes = false;
					bool isNo = false;
					std::vector<std::string> yesWords;
					std::vector<std::string> noWords;
					if (addAllTheThesaurusWords)
					{
						yesWords = { "Yea", "Yeah", "Absolutely", "Affirmative", "Positive", "Yee", "Yas", "Ye" };
						noWords = { "Noo", "Nah", "Not" ,"Negative", "Nu", "Nuh uh", "Nope" };
					}
					else
					{
						yesWords = { "Former", "First", "Option 1" };
						noWords = { "Latter", "Second", "Option 2" };
					}
					for (int i = 0; i < yesWords.size(); i++)
					{
						//std::cout << "testing " << userInput << " against " << yesWords[i] << "\n";
						if (stringEquals(userInput, yesWords[i], caseSensative, acceptOnlyFirstLetter, acceptIfContainsWord))
						{
							isYes = true;
							break;
						}
					}
					for (int i = 0; i < noWords.size(); i++)
					{
						//std::cout << "testing " << userInput << " against " << noWords[i] << "\n";
						if (stringEquals(userInput, noWords[i], caseSensative, acceptOnlyFirstLetter, acceptIfContainsWord))
						{
							isNo = true;
							break;
						}
					}
					if (isYes && isNo) //multiple answers given
					{
						std::cout << "Well which is it?\n";
						times = 3;
					}
					else if (!isYes && !isNo)
					{
						std::cout << "Response not understood. Please type an understood response.\n";
					}
					else //Is one or the other
					{
						if (addAllTheThesaurusWords)
						{
							//return (isYes ? simplifyString(std::string("Yes"), caseSensative, acceptOnlyFirstLetter) : simplifyString(std::string("No"), caseSensative, acceptOnlyFirstLetter));
							return (isYes ? "Yes" : "No");
						}
						else
						{
							//return (isYes ? simplifyString(std::string(options[0]), caseSensative, acceptOnlyFirstLetter) : simplifyString(std::string(options[1]), caseSensative, acceptOnlyFirstLetter));
							return (isYes ? options[0] : options[1]);
						}
					}
				}
				else
				{
					std::cout << "Response not understood. Please type an understood response.\n";
				}
			}
			else if (answersSaid == 1)
			{
				return answerSave;
			}
			else //multiple answers given
			{
				std::cout << "Well which is it?\n";
				times = 3;
			}
		}
		times++;
	}
}
static std::string askUser(std::string question, std::string positiveAnswer = "Yes", std::string negativeAnswer = "No", bool newlineAnswer = true, bool showAnswers = true, bool listOptionsVertically = false, bool caseSensative = false, bool acceptOnlyFirstLetter = true, bool acceptIfContainsWord = true)
{
	std::vector<std::string> tempvec = std::vector<std::string>({ positiveAnswer, negativeAnswer });
	return askUser(question, newlineAnswer, tempvec, showAnswers, listOptionsVertically, caseSensative, acceptOnlyFirstLetter, acceptIfContainsWord);
}
static int askUserInt(std::string question, std::vector<std::string> options, bool newlineAnswer = true, bool showAnswers = true, bool listOptionsVertically = false, bool caseSensative = false, bool acceptOnlyFirstLetter = true, bool acceptIfContainsWord = true)
{
	std::string userAnswer = askUser(question, newlineAnswer, options, showAnswers, listOptionsVertically, false, acceptOnlyFirstLetter, true, true);
	for (int i = 0; i < options.size(); i++)
	{
		if (atoi(userAnswer.c_str()) == i + 1)
		{
			return i + 1;
		}
	}
	return -1;
}

static float askUserFloat(std::string question, bool newlineAnswer = true, bool acceptNegatives = true, bool acceptDecimals = true, bool acceptIfContainsNumber = true, float rangeMin = 0, float rangeMax = 0, bool tellRange = false)
{
	bool keepAsking = true;
	std::string userInput;
	int times = 0;
	while (keepAsking)
	{
		if (times >= 3)
		{
			times = 0;
		}
		if (times == 0)
		{
			std::cout << question + (tellRange ? (" (" + std::to_string(rangeMin) + " - " + std::to_string(rangeMax) + ")") : "") + (newlineAnswer ? "\n" : ": ");
		}
		std::getline(std::cin, userInput);
		findFloatResult floatFound = findFloatInString(userInput, acceptNegatives, acceptDecimals, acceptIfContainsNumber, rangeMin, rangeMax);
		if (floatFound.success)
		{
			return floatFound.value;
		}
		//If no matches
		std::cout << "That is not a valid input. Please try again.\n";
		times++;
	}
	return -1;
}
static int askUserInt(std::string question, bool newlineAnswer = true, bool acceptNegatives = true, bool acceptIfContainsNumber = true, int rangeMin = 0, int rangeMax = 0, bool tellRange = false)
{
	return round(askUserFloat(question, newlineAnswer, acceptNegatives, false, acceptIfContainsNumber, rangeMin, rangeMax, tellRange));
}

static bool askUserBool(std::string question, bool showAnswers = true, bool newlineAnswer = true, std::string positiveAnswer = "Yes", std::string negativeAnswer = "No", bool caseSensative = false, bool acceptOnlyFirstLetter = true, bool acceptIfContainsWord = true)
{
	std::string userAnswer = askUser(question, positiveAnswer, negativeAnswer, newlineAnswer, showAnswers, false, caseSensative, acceptOnlyFirstLetter, acceptIfContainsWord);
	//std::cout << userAnswer << " =?= " << positiveAnswer << "\n";
	if (userAnswer == positiveAnswer)
	{
		return true;
	}
	else
	{
		return false;
	}
}
static bool askUserBool(std::string question, std::string positiveAnswer, std::string negativeAnswer, bool showAnswers, bool newlineAnswer = true, bool caseSensative = false, bool acceptOnlyFirstLetter = true, bool acceptIfContainsWord = true)
{
	return askUserBool(question, showAnswers, newlineAnswer, positiveAnswer, negativeAnswer, caseSensative, acceptOnlyFirstLetter, acceptIfContainsWord);
}

static bool askUserDirectory(std::string *usersDirectory, bool alsoFilename = true, bool newLine = true, std::string customQuestion = "N/A")
{
	bool wantsToStay = true;
	while (wantsToStay)
	{
		if (customQuestion != "N/A")
		{
			std::cout << customQuestion << (newLine ? "\n" : "");
		}
		else
		{
			std::cout << "Please type the full directory" << (alsoFilename ? " with filename " : "") << (newLine ? "below" : "") << ", or type \"quit\" to quit" << (newLine ? ".\n" : ": ");
		}
		std::string usrInpt;
		std::getline(std::cin, usrInpt);
		if (simplifyString(usrInpt) == "q")
		{
			wantsToStay = false;
			return false;
		}
		else
		{
			convertLocalDirectory(usrInpt);
			bool fileTypeEnd = true;
			if (alsoFilename)
			{
				if (usrInpt.find_last_of('\\') == std::string::npos)
				{
					fileTypeEnd = false;
				}
				std::string tempFileName = usrInpt.substr(usrInpt.find_last_of('\\') + 1, usrInpt.length() - usrInpt.find_last_of('\\') - 1);
				if (tempFileName.length() < 3 || tempFileName.find('.') == std::string::npos)
				{
					fileTypeEnd = false;
				}
			}
			if ((!alsoFilename || fileTypeEnd) && fileExists(usrInpt))
			{
				*usersDirectory = usrInpt;
				return true;
			}
			else
			{
				std::cout << "Invalid directory.\n";
			}
		}
	}
	return false;
}


static void toClipboard(const std::string &s, HWND hwnd = HWND()) //Fill in whnd with GetStdHandle(STD_OUTPUT_HANDLE) in code(?)
{
	OpenClipboard(hwnd);
	EmptyClipboard();
	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size() + 1);
	if (!hg) {
		CloseClipboard();
		return;
	}
	memcpy(GlobalLock(hg), s.c_str(), s.size() + 1);
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();
	GlobalFree(hg);
}

static void maximizeConsoleWindow()
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	SMALL_RECT rc;
	rc.Left = rc.Top = 0;
	rc.Right = (short)(std::min(info.dwMaximumWindowSize.X, info.dwSize.X) - 1);
	rc.Bottom = (short)(std::min(info.dwMaximumWindowSize.Y, info.dwSize.Y) - 1);
	SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), true, &rc);
}

static COORD getConsoleSize()
{
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleInfo);
	return COORD{ consoleInfo.dwSize.X, consoleInfo.dwSize.Y };
}

static vec2 getConsoleSizeVec2()
{
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleInfo);
	return vec2{ float(consoleInfo.dwSize.X), float(consoleInfo.dwSize.Y) };
}

static void renameConsoleWindow(std::string title)
{
	//system(title).c_str()); //The "Bad" way to do this - I dunno, I think they're just puritans. This is easier. Whatever.
	SetConsoleTitle(TEXT(title.c_str())); //Only works with #undef UNICODE at the top (Edit: apparently it's working without it??)
}

static void setConsoleSize(short width, short height)
{
	if (height < 25)
	{
		//I don't know why this is, but that's what happens, lol
		std::cout << "Warning - height below 25 not allowed. Generating random size instead.";
	}
	COORD coord = { width, height };
	SMALL_RECT rect = { 0, 0, width - 1, height - 1 };
	SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &rect);
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	maximizeConsoleWindow();
}

static void writeText(std::string text, short xpos, short ypos)
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ xpos, ypos });
	std::cout << text;
}

static float RandFloat(float min = 0.0f, float max = 1.0f) //Inclusive, I believe
{
	return min + float(rand()) / (RAND_MAX / (max - min));
}

static vec2 RandUnit()
{
	return vec2(RandFloat(), RandFloat()).Normalized();
}

class PingPong
{
private:
	bool forwards;
	bool pingPonging;
	float progress;
	float valueMin;
	float valueMax;

public:
	float* variable;
	float speed;

	PingPong()
	{
		forwards = true;
		pingPonging = true;
		progress = 0.0f;
		speed = 1.0f;
		valueMin = 0.0f;
		valueMax = 1.0f;
	}
	PingPong(float* theVariable, float min = 0.0f, float max = 1.0f, float theSpeed = 1.0f)
	{
		variable = theVariable;
		forwards = true;
		pingPonging = true;
		progress = 0.0f;
		speed = theSpeed;
		valueMin = min;
		valueMax = max;
	}

	void Update(float dTime)
	{
		if (pingPonging)
		{
			if (forwards)
			{
				progress += dTime * (speed / 1000.0f);
			}
			else
			{
				progress -= dTime * (speed / 1000.0f);
			}
			if (progress >= 1.0f)
			{
				progress = 1.0f;
				forwards = false;
			}
			else if (progress <= 0.0f)
			{
				progress = 0.0f;
				forwards = true;
			}
			*variable = LERP(valueMin, valueMax, progress);
		}
	}
	void Reset(bool atTop)
	{
		if (atTop)
		{
			forwards = false;
			progress = 1.0f;
		}
		else
		{
			forwards = true;
			progress = 0.0f;
		}
	}
	void Reset(float newProgress)
	{
		progress = newProgress;
		*variable = LERP(valueMin, valueMax, progress);
	}
	void Reset(float newProgress, bool goingUp)
	{
		forwards = goingUp;
		progress = newProgress;
		*variable = LERP(valueMin, valueMax, progress);
	}
	void SetLimits(float min, float max)
	{
		valueMin = min;
		valueMax = max;
	}
	void Stop()
	{
		pingPonging = false;
	}
	void Start()
	{
		pingPonging = true;
	}
};