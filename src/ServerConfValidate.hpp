#ifndef INCLUDE_SRC_SERVERCONFVALIDATEFIELD_HPP_
#define INCLUDE_SRC_SERVERCONFVALIDATEFIELD_HPP_

#include <string>

bool isValidHttp(const std::string &);
bool isValidServer(const std::string &);
bool isValidLocation(const std::string &);
bool isValidHostName(const std::string &);
bool isValidIPAddress(const std::string &);
bool isValidListen(const std::string &);
bool isValidMethods(const std::string &);
bool isValidRoot(const std::string &);
bool isValidPath(const std::string &);
bool isValidIndex(const std::string &);
bool isValidClientMaxBodySize(const std::string &);
bool isValidAutoindex(const std::string &);
bool isValidFileExt(const std::string &);

#endif // __INCLUDE_SRC_SERVERCONFVALIDATEFIELD_HPP
