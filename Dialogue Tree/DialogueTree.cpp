#include "DialogueTree.h"

DialogueTree::DialogueTree()
{

}

DialogueTree::DialogueTree(const char* filename, std::string name)
{
	m_name = name;

	std::fstream dialogueFile;
	dialogueFile.open(filename, std::fstream::in);

	Textblock* lastTextBlock = nullptr;

	if (dialogueFile.is_open())
	{
		Textblock* textblock = new Textblock();
		
		if (!m_rootBlock)
		{
			m_rootBlock = textblock;
		}

		textblock->m_tree = this;

		// hardcode test
		textblock->m_textColor = LTEXT_BLUE;
		textblock->m_backgroundColor = LBACKGROUND_BRIGHTYELLOW;

		std::string line;
		std::getline(dialogueFile, line);

		textblock->m_lines.push_back(line);

		lastTextBlock = textblock;
	}
	else
	{
		std::cerr << "Failed to Open dialogue file: " << filename << std::endl;
	}

	dialogueFile.close();
}

DialogueTree::~DialogueTree()
{
	delete m_rootBlock;
}

void DialogueTree::SetScrollSpeed(int scrollSpeedMs)
{
	if (scrollSpeedMs > 0)
	{
		m_scrollspeedMs = scrollSpeedMs;
	}
}

int DialogueTree::GetScrollSpeed() const
{
	return m_scrollspeedMs;
}

void DialogueTree::BeginDialogue()
{
	m_rootBlock->Display();
}
