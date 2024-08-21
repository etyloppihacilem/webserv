#ifndef INCLUDE_SRC_SERVERCONFTOKENIZE_HPP_
#define INCLUDE_SRC_SERVERCONFTOKENIZE_HPP_

#include "ServerConfFields.hpp"
#include "StringTokenizer.hpp"
#include <cstddef>
#include <string>

size_t          findClosingBrace(const std::string &);
StringTokenizer tokenizeFile(const std::string &);
StringTokenizer tokenizeServer(StringTokenizer &);
Field           tokenizeLocation(StringTokenizer &);
Field           tokenizeField(StringTokenizer &);

ConfField getFieldCode(const std::string &);
ValueList getFieldValues(StringTokenizer &); // getTokenizedValues()

#endif // INCLUDE_SRC_SERVERCONFTOKENIZE_HPP_
