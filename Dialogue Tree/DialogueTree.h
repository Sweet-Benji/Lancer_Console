#pragma once

#include <string>
#include "Textblock.h"

class DialogueTree
{
public:
	DialogueTree();
	DialogueTree(const char* filename, std::string name = std::string("TEST ENTRY"));

	~DialogueTree();

	void SetScrollSpeed(int scrollSpeedMs);
	int GetScrollSpeed() const;

	void BeginDialogue();

private:
	std::string m_name;
	Textblock* m_rootBlock = nullptr; // functions as root node for our tree

	int m_scrollspeedMs = 0;
};