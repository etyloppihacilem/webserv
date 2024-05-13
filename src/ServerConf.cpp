#include "ServerConf.hpp"
#include <string>
#include <algorithm>

namespace ServerConf {

	bool isValidHttp(const std::string &value)
	{
		if (*value.begin() == '{' && *value.rbegin() == '}'
			&& std::find(value.begin(), value.end(), "server") != value.end())
		{
			return true;
		}
		return false;
	}

}
