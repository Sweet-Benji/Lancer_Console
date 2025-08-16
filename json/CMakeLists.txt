# Add Source files
file(GLOB SOURCES "json-forwards.h" "json.h" "jsoncpp.cpp")
target_sources(LancerConsole PRIVATE ${SOURCES})