#pragma once
#include <iostream>
#include <map>
#include <string>

typedef std::map<std::string, std::string> ColorMap;

// Macros to make ANSI escape codes more readable

// Begins ANSI sequence
#define ESC "\033["
// Ends/Reset ANSI sequence
#define RESET "\033[m"

// Text Macros
// commented out unsuported ANSI Formatting Codes
#define LTEXT_DEFAULT "0"
//#define LTEXT_BOLD "1" // Identical to LTEXT_WHITE from what I can tell
#define LTEXT_FAINT "38;5;242"
//#define LTEXT_ITALIC 3
#define LTEXT_UNDERLINE "4"
//#define LTEXT_BLINK 5
#define LTEXT_INVERSE "7"
//#define LTEXT_HIDDEN 8
//#define LTEXT_STRIKETHROUGH "9"

#define LTEXT_BLACK "30"
#define LTEXT_RED "31"
#define LTEXT_GREEN "32"
#define LTEXT_YELLOW "33"
#define LTEXT_BLUE "34"
#define LTEXT_PURPLE "35"
#define LTEXT_BABYBLUE "36"
#define LTEXT_DARKGREY "90"
#define LTEXT_BRIGHTRED "91"
#define LTEXT_BRIGHTGREEN "92"
#define LTEXT_BRIGHTYELLOW "93"
#define LTEXT_BRIGHTBLUE "94"
#define LTEXT_BRIGHTPURPLE "95"
#define LTEXT_BRIGHTCYAN "96"
#define LTEXT_WHITE "97"

#define LTEXT_ORANGE "38;5;202"
#define LTEXT_DARKBLUE "38;5;21"
#define LTEXT_SHREKGREEN "38;5;112"

#define LBACKGROUND_BLACK "40"
#define LBACKGROUND_RED "41"
#define LBACKGROUND_GREEN "42"
#define LBACKGROUND_YELOW "43"
#define LBACKGROUND_BLUE "44"
#define LBACKGROUND_PURPLE "45"
#define LBACKGROUND_BABYBLUE "46"
#define LBACKGROUND_GREY "47"
#define LBACKGROUND_DARKGREY "100"
#define LBACKGROUND_BRIGHTRED "101"
#define LBACKGROUND_BRIGHTGREEN "102"
#define LBACKGROUND_BRIGHTYELLOW "103"
#define LBACKGROUND_BRIGHTBLUE "104"
#define LBACKGROUND_BRIGHTPURPLE "105"
#define LBACKGROUND_BRIGHTCYAN "106"
#define LBACKGROUND_WHITE "107"

#define LBACKGROUND_DARKBLUE "48;5;17"
#define LBACKGROUND_DARKRED "48;5;124"

static ColorMap s_textColorMap =
{
    {"LTEXT_DEFAULT", LTEXT_DEFAULT},
    {"LTEXT_FAINT", LTEXT_FAINT},
    {"LTEXT_UNDERLINE", LTEXT_UNDERLINE},
    {"LTEXT_INVERSE", LTEXT_INVERSE},
    {"LTEXT_RED", LTEXT_RED}
};

static ColorMap s_backgroundColorMap;


// Helper functions to display all possible color texts/backgrounds
// credit to Vitor Matias via Medium article.
// https://medium.com/@vitorcosta.matias/print-coloured-texts-in-console-a0db6f589138

inline void print_8_colours()
{
    for (int i = 0; i < 108; i++)
    {
        if (i % 9 == 0 && i != 0)
            std::cout << std::endl;
        printf("\033[%dm %3d\033[m", i, i);
    }
}

inline void print_256_colours_txt()
{
    for (int i = 0; i < 256; i++)
    {
        if (i % 16 == 0 && i != 0)
            std::cout << std::endl;
        // '38' here indicates we're working with text color
        printf("\033[38;5;%dm %3d\033[m", i, i);
    }
}

inline void print_256_colours_background()
{
    for (int i = 0; i < 256; i++)
    {
        if (i % 16 == 0 && i != 0)
            std::cout << std::endl;
        // '48' here indicates we're working with background color
        printf("\033[48;5;%dm %3d\033[m", i, i);
    }
}