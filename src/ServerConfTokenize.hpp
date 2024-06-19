#ifndef INCLUDE_SRC_SERVERCONFTOKENIZE_HPP_
#define INCLUDE_SRC_SERVERCONFTOKENIZE_HPP_

#include <string>
#include "StringTokenizer.hpp"
#include "ServerConfFields.hpp"

std::string tokenizeFile(StringTokenizer strtok);
std::string tokenizeServer(StringTokenizer &);
Field tokenizeLocation(StringTokenizer &);
Field tokenizeField(StringTokenizer &);
ValueList tokenizeValue(const std::string &value);

size_t findClosingBrace(const std::string &tokenString);

#endif // INCLUDE_SRC_SERVERCONFTOKENIZE_HPP_
