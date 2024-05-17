#include "ServerConf.hpp"
#include <string>
#include <algorithm>

namespace ServerConf {

std::string trimIsspace(const std::string &input)
{
	std::string s(input);
	s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());
	return s;
}

bool isValidHttp(const std::string &value)
{
	if (*value.begin() == '{' && *value.rbegin() == '}'
		&& std::find(value.begin(), value.end(), "server") != value.end())
	{
		return true;
	}
	return false;
}

bool isValidServer(const std::string &value)
{
	if (*value.begin() == '{' && *value.rbegin() == '}'
		&& std::find(value.begin(), value.end(), "server_name") != value.end()
		&& std::find(value.begin(), value.end(), "listen") != value.end()
		&& std::find(value.begin(), value.end(), "root") != value.end()
		&& std::find(value.begin(), value.end(), "index") != value.end()
		)
	{
		return true;
	}
	return false;
}

}
