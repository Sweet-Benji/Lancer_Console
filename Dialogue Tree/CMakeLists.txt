# Add Source files
file(GLOB SOURCES "DialogueTree.h" "Textblock.h" "DialogueTree.cpp" "Textblock.cpp")
target_sources(LancerConsole PRIVATE ${SOURCES})