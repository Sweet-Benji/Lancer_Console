// Copyright Benjamin Lantrip 2024
// OC pls do not steal :)

#include "ConsoleIO/Commands.h"
#include "Dialogue Tree/DialogueTree.h"
#include "Lancers.h"

#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

///////////////////////////////////////////////////////////////////////////////
////                          Helper Functions                             ////
///////////////////////////////////////////////////////////////////////////////

// Displays the COMP/CON intro "Loading Screen"; purely for flavor
static void DisplayConsoleIntro()
{
    std::string success(" done\n");
    std::string failure(" error\n");

    std::string titleText("COMPANION/CONCIERGE UNIT INITIALIZING\n"
        "GMS COMP / CON Unit Mk XII Rev 5.9.98b\n"
        "5024.7.29 General Massive Systems // Please Operate Responsibly\n"
        "***************************************************************\n");
    std::string fakeInit1("Initializing semantic Manifold...");
    std::string fakeInit2("Initializing logic gradients...");
    std::string promptText("Loading Company OS...");
    std::string companyText("MRB designation: ");
    std::string company("Steel Rings\n");

    ScrollText(titleText, 5, std::string(LTEXT_BABYBLUE), std::string(LBACKGROUND_BLACK));
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    ScrollText(fakeInit1, 20, std::string(LTEXT_FAINT), std::string(LBACKGROUND_BLACK));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    ScrollText(success, 35, std::string(LTEXT_FAINT), std::string(LBACKGROUND_BLACK));
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    ScrollText(fakeInit2, 20, std::string(LTEXT_FAINT), std::string(LBACKGROUND_BLACK));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    ScrollText(success, 35, std::string(LTEXT_FAINT), std::string(LBACKGROUND_BLACK));
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    ScrollText(promptText, 20, std::string(LTEXT_FAINT), std::string(LBACKGROUND_BLACK));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    ScrollText(success, 35, std::string(LTEXT_FAINT), std::string(LBACKGROUND_BLACK));
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    ScrollText(companyText, 20, std::string(LTEXT_DEFAULT), std::string(LBACKGROUND_BLACK));
    std::this_thread::sleep_for(std::chrono::milliseconds(75));
    ScrollText(company, 75, std::string(LTEXT_BLACK), std::string(LBACKGROUND_WHITE));
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

///////////////////////////////////////////////////////////////////////////////

int main()
{
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hConsole, &mode);

    if (mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING)
    {
        std::cout << "[D_INFO] Virtual Terminal Processing is already on." << std::endl;
    }
    else
    {
        std::cout << "[D_INFO] Virtual Terminal Processing isn't on. Enabling..." << std::endl;
    }
    
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    BOOL result = SetConsoleMode(hConsole, mode);

    if (!result)
    {
        std::cerr << "[D_ERROR] failure enabling ANSI codes on Windows." << std::endl;
        return -1;
    }
#elif __linux__
    // TODO; confirm Unix support
#endif // _WIN32

    bool exit = false;
    const int promptScrollSpeedMs = 50;

    // Diagetic "loading" of the console; comment out if debugging
    DisplayConsoleIntro();

    LConsole console("LancerProfile.Json");

    while (!exit)
    {
        std::string commandPrompt("\033[31mCOMP/CON:\033[0m Input Command.\t");
        std::vector<std::string> commands;
        ScrollText(commandPrompt, promptScrollSpeedMs, std::string(LTEXT_DEFAULT),
                                                       std::string(LBACKGROUND_BLACK));

        std::string rawCommandString;
        std::getline(std::cin, rawCommandString);

        std::istringstream commandIString(rawCommandString);
        std::string value;
        while (commandIString >> value)
        {
            commands.push_back(value);
        }

        CResult result = console.ProcessCommand(commands);

        if (result == CResult::Success_ExitCommand)
        {
            exit = true;
        }
    }

#ifdef _WIN32
    std::cout << std::endl;
    std::system("pause");
#else
    std::system("read");
#endif // _WIN32

}
