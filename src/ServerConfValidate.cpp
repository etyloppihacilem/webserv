#include "HttpMethods.hpp"
#include "ServerConfFields.hpp"
#include "ServerConfValidate.hpp"
#include <string>
#include <vector>
#include <cstdlib>
#include <climits>
#include <sys/types.h>
#include <sys/stat.h>

bool isValidConfigFile(const std::string &configFile)
{
	if (configFile.size() == 0)
	{
		return false;
	}
	std::string	CONF = ".conf";
	if (configFile.find(CONF, 0) == configFile.size() - CONF.size())
	{
		return true;
	}
	return false;
}

bool isValidFieldName(const std::string &name)
{
	for (int i = 0; i < COUNT_CONF_FIELD; i++)
	{
		if (name == ConfFieldString(i))
		{
			return true;
		}
	}
	return false;
}

bool isValidHttp(const std::string &value)
{
	if (value.size() == 0)
	{
		return false;
	}
	if (*value.begin() == '{' && *value.rbegin() == '}'
		&& value.find("|server|") != std::string::npos )
	{
		return true;
	}
	return false;
}

bool isValidServer(const std::string &value)
{
	if (value.size() == 0)
	{
		return false;
	}
	if (*value.begin() == '{' && *value.rbegin() == '}'
		&& value.find("|server_name|") != std::string::npos
		&& value.find("|listen|") != std::string::npos
		&& value.find("|root|") != std::string::npos
		&& value.find("|index|") != std::string::npos
		&& value.find("|methods|") != std::string::npos)
	{
		return true;
	}
	return false;
}

bool isValidLocation(const std::string &value)
{
	if (value.size() == 0)
	{
		return false;
	}
	if (*value.begin() == '{' && *value.rbegin() == '}'
		&& value.find("|index|") != std::string::npos
		&& value.find("|autoindex|") != std::string::npos)
	{
		return true;
	}
	return false;
}

bool isValidHostname(const std::string &value)
{
	if (value.size() == 9 && value.find("localhost") == 0) // should it be only localhost?
	{
		return true;
	}
	return false;
}

// Split the first element at every '.'
// For an IP address you should have four elements.
// Check that all elements in both vectors are integers between 0 and 255.

bool isValidIPAddress(const std::string &value)
{
	if (value.size() == 0)
	{
		return false;
	}
	std::string IPstring(value);
	std::vector<std::string> IPoctets;
	unsigned long int pos = IPstring.find(".", 0);
	if (pos == std::string::npos)
	{
		return false;
	}
	while (pos != std::string::npos)
	{
		IPoctets.push_back(IPstring.substr(0, pos));
		IPstring = IPstring.substr(pos + 1, IPstring.size() - pos);
		pos = IPstring.find(".", 0);
	}
	if (IPoctets.size() != 4)
	{
		return false;
	}
	for (int i = 0; i < 4; ++i)
	{
		int octet = atoi(IPoctets[i].c_str());
		if (i == 0 && octet != 127)
		{
			return false;
		}
		if (octet < 0 || octet > 255)
		{
			return false;
		}
	}
	return true;
}

// Check that value match a 4 digits string.
// Accept ony port between 8000 and 8999.

bool isValidListen(const std::string &value)
{
	if (value.size() != 4
		&& (!isdigit(value[0]) || !isdigit(value[1]) ||
		!isdigit(value[2]) || !isdigit(value[3])))
	{
		return false;
	}
	int port = atoi(value.c_str());
	if (port < 8000 || port > 8999)
	{
		return false;
	}
	return true;
}

bool isValidMethods(const std::string &value)
{
	size_t len = value.size();
	if (len < MIN_METHOD_SIZE || len > MAX_METHOD_SIZE)
	{
		return false;
	}
	if (value[0] == 'G' && value.find(method_string(GET)) == 0 && len == 3)
	{
		return true;
	}
	if (value[0] == 'P' && value.find(method_string(POST)) == 0 && len == 4)
	{
		return true;
	}
	if (value[0] == 'D' && value.find(method_string(DELETE)) == 0 && len == 6)
	{
		return true;
	}
	return false;
}

bool isValidRoot(const std::string &value)
{
	if (value.size() == 0)
	{
		return false;
	}
	struct stat root;
	if (value[0] !=  '/' && stat(value.c_str(), &root) == 0
		&& root.st_mode && S_ISDIR(root.st_mode))
	{
		return true;
	}
	return false;
}

bool isValidPath(const std::string &value)
{
	if (value.size() == 0)
	{
		return false;
	}
	std::string pathString("/" + value);
	struct stat path;
	if (value[0] == '/' && stat(pathString.c_str(), &path) == 0
		&& path.st_mode && S_ISREG(path.st_mode))
	{
		return true;
	}
	return false;
}

bool isValidIndex(const std::string &value, const std::string &root)
{
	if (value.size() == 0)
	{
		return false;
	}
	std::string	HTML = ".html";
	std::string path("/" + root + "/" + value);
	if (value.find(HTML, 0) == value.size() - HTML.size()
		&& isValidPath(path))
	{
		return true;
	}
	return false;
}

bool isValidClientMaxBodySize(const std::string &value)
{
	long maxBodySize = atol(value.c_str());
	if (maxBodySize > 0 && maxBodySize <= INT_MAX)
	{
		return true;
	}
	return false;
}

bool isValidAutoindex(const std::string &value)
{
	size_t len = value.size();
	if (len < 4 || len > 5)
	{
		return false;
	}
	if (value[0] == 'f' && value.find("false") == 0 && len == 5)
	{
		return true;
	}
	if (value[0] == 't' && value.find("true") == 0 && len == 4)
	{
		return true;
	}
	return false;
}

bool isValidFileExt(const std::string &value)
{
	if (value.size() == 0)
	{
		return false;
	}
	if (value[0] == '.')
	{
		return true;
	}
	return false;
}
