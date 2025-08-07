#pragma once
#include <functional>
#include "../json/json.h"
#include "../Lancers.h"
#include <string>
#include <vector>

class LConsole;

enum CResult
{
	Success,
	Success_ExitCommand,	// We succeeded, exiting the program
	Failure_UnknownCommand, // Input was bad
	Failure_FunctionError   // Code was bad
};

// vector of user command, and any arguments
typedef std::vector<std::string> CommandArgs;

// Function that corresponds to a user command
typedef std::function<CResult (LConsole&, const CommandArgs&)> CommandFunc;

typedef std::map<std::string, CommandFunc> CommandMap;

class LConsole
{
public:
	LConsole();
	LConsole(const char* filename);

	~LConsole();

	// returns function associated with a given command
	// returns map.end() if given command is unsupported
	CommandMap::const_iterator GetCommand(std::string& command) const;

	CResult ProcessCommand(CommandArgs& input);
private:
	void InitCommandMap();
	void LoadLancerProfile(const char* filename);

	Profile m_playerProfile;		// Contains Lancer profile info
	Json::Value m_root;				// root of the Json tree container player info

	// map of all supported functions, with their string names as keys
	CommandMap m_commandFuncMap;

	// Command Functions

	friend CResult HelpFunc(LConsole& console, const CommandArgs& args);
	friend CResult ExitFunc(LConsole& console, const CommandArgs& args);
	friend CResult LogFunc(LConsole& console, const CommandArgs& args);
};

void ScrollText(std::string& text, int scrollSpeedMilliseconds,
				const std::string& textColor,
				const std::string& backgroundColor);