#include "ServerConf.hpp"
#include "ServerConfTrim.hpp"
#include <string>
#include <algorithm>

namespace ServerConf
{

	static const std::string ISSPACE = " 	";

	std::string trimIsspace(const std::string &input) // LOL useless
	{
		std::string s(input);
		s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());
		return s;
	}

	std::string trimIsspaceLeft(const std::string& input);
	// {
	// 	std::string s(input);
	// 	s.erase(s.begin(), std::find_first_of(s.begin(), s.end(), ::isspace));
	// 	return s;
	// }

}
