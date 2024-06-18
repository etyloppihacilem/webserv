#ifndef INCLUDE_STRINGTOKENIZER_HPP_
#define INCLUDE_STRINGTOKENIZER_HPP_


#include <cstddef>
#include <string>


class StringTokenizer
{
public:
	StringTokenizer(const std::string &str, const std::string &delim);
	~StringTokenizer();

	bool hasMoreTokens();
	std::string remainingString();
	std::size_t countTokens();
	std::size_t countTokens(const std::string &separator);
	std::string peakToken();
	std::string nextToken();
	std::string nextToken(const std::string &separator); // unused so far
	std::string nextToken(std::size_t separatorPosition, const std::string &separator);
	std::string filterNextToken(const std::string& filterString);

private:

	std::string	_tokenString;
	std::string	_delimiter;
	std::size_t	_delimiterLen;
};

#endif // INCLUDE_SRC_STRINGTOKENIZER_HPP_
