#ifndef INCLUDE_SRC_SERVERCONFTOKENIZE_HPP_
#define INCLUDE_SRC_SERVERCONFTOKENIZE_HPP_

#include "ServerConfFields.hpp"
#include "StringTokenizer.hpp"
#include <cstddef>
#include <string>

std::string tokenizeFile(const std::string &);
std::string tokenizeServer(StringTokenizer &);
Field       tokenizeLocation(StringTokenizer &);
Field       tokenizeField(StringTokenizer &);
ValueList   tokenizeValue(const std::string &);

size_t findClosingBrace(const std::string &);
int    findFieldCode(const std::string &);

#endif // INCLUDE_SRC_SERVERCONFTOKENIZE_HPP_
