#include "DialogueTree.h"
#include "Textblock.h"

Textblock::Textblock()
{
	m_textColor = LTEXT_DEFAULT;
	m_backgroundColor = LBACKGROUND_BLACK;
}

Textblock::~Textblock()
{
	m_parent = nullptr;

	m_lines.clear();
	m_linePausesMs.clear();
}

void Textblock::Display()
{
    for (size_t i = 0; i < m_lines.size(); ++i)
    {
        ScrollText(m_lines[i], m_tree->GetScrollSpeed());
    }
}

void Textblock::ScrollText(const std::string& text, int scrollSpeedMilliseconds)
{
    bool defaultText = m_textColor.compare(LTEXT_DEFAULT) == 0;
    bool defaultBackground = m_backgroundColor.compare(LBACKGROUND_BLACK) == 0;

    if (!defaultText && defaultBackground)
    {
        std::cout << ESC << m_textColor << "m";
    }
    else if (defaultText && !defaultBackground)
    {
        std::cout << ESC << m_backgroundColor << "m";
    }
    else
    {
        std::cout << ESC << m_backgroundColor << ";" << m_textColor << "m";
    }

    for (size_t i = 0; i < text.size(); ++i)
    {
        // print a character
        std::cout << text[i];

        // sleep thread for the given time
        std::chrono::milliseconds sleepDuration(scrollSpeedMilliseconds);
        std::this_thread::sleep_for(sleepDuration);
    }
    std::cout << RESET;
}

void Textblock::ScrollText(const std::string& text, int scrollSpeedMilliseconds, const std::string& textColor, const std::string& backgroundColor)

{
    bool defaultText = textColor.compare(LTEXT_DEFAULT) == 0;
    bool defaultBackground = backgroundColor.compare(LBACKGROUND_BLACK) == 0;

    if (!defaultText && defaultBackground)
    {
        std::cout << ESC << textColor << "m";
    }
    else if (defaultText && !defaultBackground)
    {
        std::cout << ESC << backgroundColor << "m";
    }
    else
    {
        std::cout << ESC << backgroundColor << ";" << textColor << "m";
    }

    for (size_t i = 0; i < text.size(); ++i)
    {
        // print a character
        std::cout << text[i];

        // sleep thread for the given time
        std::chrono::milliseconds sleepDuration(scrollSpeedMilliseconds);
        std::this_thread::sleep_for(sleepDuration);
    }
    std::cout << RESET;
}
