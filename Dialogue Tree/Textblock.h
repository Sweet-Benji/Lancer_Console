#pragma once

#include "../Text Formatting/Color.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

class Textblock
{
	friend class DialogueTree;

public:
	Textblock();

	~Textblock();

	void Display();

	void ScrollText(const std::string& text, int scrollSpeedMilliseconds);
	void ScrollText(const std::string& text, int scrollSpeedMilliseconds,
					const std::string& textColor,
					const std::string& backgroundColor);

private:
	DialogueTree* m_tree = nullptr;
	Textblock* m_parent = nullptr;
	std::string m_textColor;
	std::string m_backgroundColor;

	std::vector<std::string> m_lines;

	bool m_hasPauses = false;
	std::vector<size_t> m_linePausesMs;
};