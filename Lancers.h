#pragma once

#include <string>
#include <map>

struct Profile
{
	std::string m_name;
	std::string m_callsign;
	std::string m_textcolor;

	inline bool isValid()
	{
		return !m_name.empty() && !m_callsign.empty() && !m_textcolor.empty();
	}
};