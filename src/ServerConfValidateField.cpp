#include "ServerConf.hpp"
#include "ServerConfValidateField.hpp"
#include "ServerConfTokenize.hpp"
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

namespace ServerConf
{

	typedef std::pair<std::string, std::string> Field;

	bool isValidHttp(const std::string &value)
	{
		if (*value.begin() == '{' && *value.rbegin() == '}'
			&& value.find("server") != std::string::npos )
		{
			return true;
		}
		return false;
	}

	bool isValidServer(const std::string &value)
	{
		if (*value.begin() == '{' && *value.rbegin() == '}'
			&& value.find("server_name") != std::string::npos
			&& value.find("listen") != std::string::npos
			&& value.find("root") != std::string::npos
			&& value.find("index") != std::string::npos
			&& value.find("methods") != std::string::npos)
		{
			return true;
		}
		return false;
	}
	bool isValidLocation(const std::string &value)
	{
		if (*value.begin() == '{' && *value.rbegin() == '}'
			&& value.find("index") != std::string::npos
			&& value.find("autoindex") != std::string::npos)
		{
			return true;
		}
		return false;
	}
	//
	// bool isValidServerName(const std::string &value)
	// {
	// 	if ()
	// 	{
	// 		return true;
	// 	}
	// 	return false;
	// }
	//
	// bool isValidListen(const std::string &value)
	// {
	// 	if ()
	// 	{
	// 		return true;
	// 	}
	// 	return false;
	// }
	//
	// bool isValidRoot(const std::string &value)
	// {
	// 	if ()
	// 	{
	// 		return true;
	// 	}
	// 	return false;
	// }
	//
	// bool isValidMethods(const std::string &value)
	// {
	// 	if ()
	// 	{
	// 		return true;
	// 	}
	// 	return false;
	// }
	//
	// bool isValidClientMaxBodySize(const std::string &value)
	// {
	// 	if ()
	// 	{
	// 		return true;
	// 	}
	// 	return false;
	// }
	//
	// bool isValidAutoindex(const std::string &value)
	// {
	// 	if ()
	// 	{
	// 		return true;
	// 	}
	// 	return false;
	// }
	//
	// bool isValidIndex(const std::string &value)
	// {
	// 	if ()
	// 	{
	// 		return true;
	// 	}
	// 	return false;
	// }

	bool isValidPath(const std::string &value)
	{
		std::vector<std::string> fieldToken = tokenize(value, " ");
		if ()
		{
			return true;
		}
		return false;
	}

	// bool isValidUploadPath(const std::string &value)
	// {
	// 	if ()
	// 	{
	// 		return true;
	// 	}
	// 	return false;
	// }
	//
	// bool isValidRewrite(const std::string &value)
	// {
	// 	if ()
	// 	{
	// 		return true;
	// 	}
	// 	return false;
	// }
	//
	// bool isValidErrorPage(const std::string &value)
	// {
	// 	if ()
	// 	{
	// 		return true;
	// 	}
	// 	return false;
	// }
	//
	// bool isValidFileExt(const std::string &value)
	// {
	// 	if ()
	// 	{
	// 		return true;
	// 	}
	// 	return false;
	// }

}
