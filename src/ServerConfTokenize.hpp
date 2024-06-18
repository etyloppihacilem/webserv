#ifndef INCLUDE_SRC_SERVERCONFTOKENIZE_HPP_
#define INCLUDE_SRC_SERVERCONFTOKENIZE_HPP_

#include <string>
#include <vector>
#include "StringTokenizer.hpp"

typedef std::pair<std::string, std::string> Field;
typedef std::vector<std::string> ValueList;

// void tokenizeFile(const std::string& input);
std::string tokenizeHttp(StringTokenizer strtok);
std::string tokenizeServer(StringTokenizer );
std::pair<std::string, std::string> tokenizeLocation(StringTokenizer );
std::vector<std::string> tokenizeField(const std::string &value);

size_t findClosingBrace(const std::string &tokenString);

#endif // INCLUDE_SRC_SERVERCONFTOKENIZE_HPP_
