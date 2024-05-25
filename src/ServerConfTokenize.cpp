#include "ServerConf.hpp"
#include "ServerConfTokenize.hpp"
#include "ServerConfTrim.hpp"
#include "StringTokenizer.hpp"
#include <exception>
#include <string>
#include <vector>
#include <algorithm>

namespace ServerConf
{


typedef std::pair<std::string, std::string> Field;
std::string trimIsspaceLeft(const std::string& input);

// std::vector <std::string> tokenize(const std::string& text, const std::string& delimiter)
// {
// 	std::vector <std::string> tokens;
// 	size_t lastPos = text.find_first_of(delimiter, 0);
// 	size_t pos = text.find_first_of(delimiter, lastPos);
//
// 	if (std::string::npos == pos || std::string::npos == pos)
// 	{
// 		tokens.push_back(text);
// 	}
// 	while (std::string::npos != pos || std::string::npos != lastPos)
// 	{
// 		tokens.push_back (text.substr(lastPos, pos - lastPos));
// 		lastPos = text.find_first_of(delimiter, pos);
// 		pos = text.find_first_of(delimiter, lastPos);
// 	}
// 	return tokens;
// }

bool tokenizeHttp(StringTokenizer strtok)
{
	if ()
	{
		return true;
	}
	return false;
}

void tokenizeFile(const std::string& input)
{
	replace(input.begin(), input.end(), " ", "|");
	replace(input.begin(), input.end(), "\t", "|");
	StringTokenizer strtok = StringTokenizer(input,"|");
	if (false == tokenizeHttp(strtok))
	{
		throw std::exception();
	}
	return ;
}

// std::vector<std::string> tokenizeConfFile(std::ifstream &input)
// {
// 	std::string isspace = " 	";
// 	std::vector<std::string> tokens;
//
// 	return tokens;
// }
//
// std::vector<Field> tokenizeServer(const std::string &)
// {
// 	std::vector<Field> fieldList;
// 	return fieldList;
// }
//
// std::vector<Field> tokenizeLocation(const std::string &)
// {
// 	std::vector<Field> fieldList;
// 	return fieldList;
// }

}
