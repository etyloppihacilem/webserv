#ifndef INCLUDE_STRINGTOKENIZER_H
#define INCLUDE_STRINGTOKENIZER_H


#include <cstddef>
#include <string>


class StringTokenizer
{
public:
	StringTokenizer(const std::string& str, const std::string& delim);
	~StringTokenizer();

	bool hasMoreTokens();
	std::string remainingString();
	std::size_t countTokens();
	std::size_t countTokens(const std::string &separator);
	std::string nextToken();
	std::string nextToken(const std::string& separator);
	// std::string filterNextToken(const std::string& filterStr);

private:

	std::string	_tokenString;
	std::string	_delimiter;
	std::size_t	_delimiterLen;
};

#endif

