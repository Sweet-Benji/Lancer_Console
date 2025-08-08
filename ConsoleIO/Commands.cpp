#include <algorithm>
#include <cctype>
#include <chrono>
#include "Commands.h"
#include "../Color.h"
#include <fstream>
#include <iostream>
#include <map>
#include <thread>
#include <tuple>

///////////////////////////////////////////////////////////////////////////////
////                              Macros                                   ////
///////////////////////////////////////////////////////////////////////////////
#define TITLE_INDEX 0
#define CONTENT_INDEX 1
#define COLOR_INDEX 2

///////////////////////////////////////////////////////////////////////////////
////                          Helper Functions                             ////
///////////////////////////////////////////////////////////////////////////////
void toLowerInplace(char& c)
{
    c = std::tolower(c);
}


///////////////////////////////////////////////////////////////////////////////
////                          Command Functions                            ////
///////////////////////////////////////////////////////////////////////////////

CResult HelpFunc(LConsole& console, const CommandArgs& args)
{
    _CRT_UNUSED(console);
    std::cout << "exit:\t Closes the Console\n"
              << "help:\t Lists all supported commands\n"
              << "logs:\t Lists all available logs, or displays one specfic log\n"
              << "\tArguments: logs [log number]\n";
    return CResult::Success;
}

CResult ExitFunc(LConsole& console, const CommandArgs& args)
{
    _CRT_UNUSED(console);
    std::cout << "Exit signal received. Closing the program..." << std::endl;
    return CResult::Success_ExitCommand;
}

CResult LogFunc(LConsole& console, const CommandArgs& args)
{
    Json::Value logs = console.m_root["Logs"];

    if (!logs.isArray())
    {
        std::cout << "[D_ERROR]: could not parse logs section as array" << std::endl;
    }

    if (args.size() == 1)
    {
        for (unsigned int i = 0; i < logs.size(); i++)
        {
            unsigned int logNumber = i + 1;
            std::string logTitle = logs[i][TITLE_INDEX].asString();

            std::cout << logNumber << ": " << logTitle << std::endl;
        }
    }
    else if (args.size() == 2)
    {
        unsigned int logIndex = std::stoi(args[1]) - 1;

        std::string title = logs[logIndex][TITLE_INDEX].asString();

        title += ": ";

        std::string entry = logs[logIndex][CONTENT_INDEX].asString();;
        title += entry;

        ScrollText(title, 45, LTEXT_DEFAULT, LBACKGROUND_BLACK);

    }
    else
    {
        std::cout << "Too many arguments!" << std::endl;
        return CResult::Failure_FunctionError;
    }
    return CResult::Success;
}

///////////////////////////////////////////////////////////////////////////////

void ScrollText(std::string& text, int scrollSpeedMilliseconds,
    const std::string& textColor,
    const std::string& backgroundColor)
{
    typedef std::string::size_type StringIndex;

    const bool defaultText = textColor.compare(LTEXT_DEFAULT) == 0;
    const bool defaultBackground = backgroundColor.compare(LBACKGROUND_BLACK) == 0;

    auto Is8bitTextColor = [](std::string& code)
        {
            int c = std::stoi(code);
            return (30 <= c <= 36) || (90 <= c <= 97);
        };

    auto ApplyBaseColors = [defaultText, defaultBackground, textColor, backgroundColor]() 
        {
            std::cout << RESET;

            if (!defaultText && defaultBackground)
            {
                std::cout << ESC << textColor << "m";
            }
            else if (defaultText && !defaultBackground)
            {
                std::cout << ESC << backgroundColor << "m";
            }
            else if (!defaultText && !defaultBackground)
            {
                std::cout << ESC << backgroundColor << ";" << textColor << "m";
            }
            else
            {
                // do nothing; no color formatting (or formatting is built into the text)
            }
        };

    ApplyBaseColors();

    //  Map of all inline color codes 
    //  key - index in the scrolling text
    //  value - std::tuple containing:
    //          number of characers that the code applies to
    //          background color to apply 
    //          foreground color to apply 
    std::map<int, std::tuple<size_t, std::string, std::string>> inlineColorMap;

    // parse in-string ANSI codes
    StringIndex index = 0;
    do
    {
        std::string textColor;
        std::string backgroundColor;

        // find beginning escape code
        index = text.find("\\033[");
        if (index != std::string::npos)
        {
            StringIndex bracketIndex = text.find("[", index);

            StringIndex end = text.find("m", index);
            if (end != std::string::npos)
            {
                StringIndex colorIndex = bracketIndex + 1;
                std::string colors(text.begin() + colorIndex, text.begin() + end);

                if (colors.size() != 0)
                {
                    StringIndex separator = colors.find(";");
                    if (separator != std::string::npos)
                    {
                        textColor = colors.substr(separator + 1, colors.size());
                        backgroundColor = colors.substr(0, separator);
                    }
                    else
                    {
                        // single foreground/background element; supply default for the other
                        bool isForeground = Is8bitTextColor(colors);
                        textColor = (isForeground) ? colors : LTEXT_DEFAULT;
                        backgroundColor = (!isForeground) ? colors : LBACKGROUND_BLACK;
                    }
                }

                // remove the escape code
                text.erase(index, end - index + 1);

                // locate & remove the reset code
                StringIndex resetIndex = text.find("\\033[m");
                text.erase(resetIndex, 6); // removes the reset code

                size_t length = (resetIndex - index);
                inlineColorMap[index] = std::tuple<size_t, std::string, std::string>::tuple(length, 
                                                                                            backgroundColor,
                                                                                            textColor);
            }
            else
            {
                std::cout << "[D_ERROR] Error parsing this log; please check the text in LancerProfile.json.\n";
                return;
            }
        }

    } while (index != std::string::npos);

    int inlineStopIndex = 0;
    bool usingInlineColors = false;

    for (int i = 0; i < text.size(); ++i)
    {
        if (usingInlineColors && i == inlineStopIndex)
        {
            ApplyBaseColors();
            usingInlineColors = false;
        }

        if (inlineColorMap.find(i) != inlineColorMap.end())
        {
            usingInlineColors = true;
            std::tuple<size_t, std::string, std::string> value = inlineColorMap[i];
            inlineStopIndex = i + std::get<0>(value);
            
            // Reset color, apply new background/foreground
            std::cout << RESET;
            std::cout << ESC << std::get<1>(value) << ";" << std::get<2>(value) << "m";
        }

        // print a character
        std::cout << text[i];

        // sleep thread for the given time
        std::chrono::milliseconds sleepDuration(scrollSpeedMilliseconds);
        std::this_thread::sleep_for(sleepDuration);
    }
    std::cout << RESET;
}

///////////////////////////////////////////////////////////////////////////////
////                      LConsole Member Functions                        ////
///////////////////////////////////////////////////////////////////////////////

LConsole::LConsole()
{
    InitCommandMap();
}

LConsole::LConsole(const char* filename)
{
    InitCommandMap();
    LoadLancerProfile(filename);
}

LConsole::~LConsole()
{
}

CommandMap::const_iterator LConsole::GetCommand(std::string& command) const
{
    return m_commandFuncMap.find(command);
}

CResult LConsole::ProcessCommand(CommandArgs& input)
{
    CResult result = CResult::Failure_FunctionError;

    // 'clean' the input, turning all arguments lowercase
    std::for_each(input.begin(), input.end(), [](std::string& s) { std::for_each(s.begin(), s.end(), toLowerInplace); });

    std::string command = input[0];

    auto find = GetCommand(command);
    if (find != m_commandFuncMap.end())
    {
        result = find->second(*this, input);
    }
    else
    {
        std::cout << "Unsupported Command. Type 'help' for list of supported commands" << std::endl;
        return CResult::Failure_UnknownCommand;
    }

    return result;
}

void LConsole::InitCommandMap()
{
    // initialize all supported commands
    m_commandFuncMap["help"] = HelpFunc;
    m_commandFuncMap["exit"] = ExitFunc;
    m_commandFuncMap["logs"] = LogFunc;
}

void LConsole::LoadLancerProfile(const char* filename)
{
    std::fstream file;
    file.open(filename, std::fstream::in);

    if (file.is_open())
    {
        Json::Reader reader;

        bool parsingResult = reader.parse(file, m_root);

        if (!parsingResult)
        {
            std::cout << "[D_ERROR] failed to parse JSON file " << filename << std::endl;
            return;
        }

        m_playerProfile.m_name = m_root["Name"].asString();
        m_playerProfile.m_callsign = m_root["Callsign"].asString();

        std::string color = m_root["Color"].asString();
        m_playerProfile.m_textcolor = s_textColorMap[color];

        if (m_playerProfile.isValid())
        {
            const char* callsign = m_playerProfile.m_callsign.c_str();
            std::string welcome("Profile loaded. Welcome, ");
            ScrollText(welcome, 30, std::string(LTEXT_DEFAULT), std::string(LBACKGROUND_BLACK));
            std::string player;
            player += m_playerProfile.m_callsign;
            player += "\n";
            ScrollText(player, 55, m_playerProfile.m_textcolor, std::string(LBACKGROUND_BLACK));
        }
        else
        {
            std::cerr << "[D_ERROR] failed to load player JSON file" << std::endl;
        }
    }
    else
    {
        std::cout << "[D_ERROR] failed to open JSON file " << filename << std::endl;
        return;
    }
}
