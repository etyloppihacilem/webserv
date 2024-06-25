#ifndef INCLUDE_SRC_SERVERCONFVALIDATEFIELD_HPP_
#define INCLUDE_SRC_SERVERCONFVALIDATEFIELD_HPP_

#include <string>
#include "ServerConfFields.hpp"

bool isValidConfigFile(const std::string &);
bool isValidFieldName(const std::string &);
bool isValidHttp(const std::string &);
bool isValidServer(const std::string &);
bool isValidLocation(const std::string &);
bool isValidHostNames(const ValueList &);
bool isValidHostName(const std::string &);
bool isValidIPAddress(const std::string &);
bool isValidPort(int value);
bool isValidMethods(const std::string &);
bool isValidRoot(const std::string &);
bool isValidPath(const std::string &, const std::string &root = "");
bool isValidIndex(const std::string &, const std::string &);
bool isValidClientMaxBodySize(const std::string &);
bool isValidAutoindex(const std::string &);
bool isValidFileExt(const std::string &);

#endif // INCLUDE_SRC_SERVERCONFVALIDATEFIELD_HPP
