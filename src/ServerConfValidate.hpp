#ifndef INCLUDE_SRC_SERVERCONFVALIDATEFIELD_HPP_
#define INCLUDE_SRC_SERVERCONFVALIDATEFIELD_HPP_

#include <string>

bool isValidConfFile(const std::string &);
bool isValidFieldName(const std::string &); // TO DELETE
bool isValidServer(const std::string &);    // TO DELETE
bool isValidLocation(const std::string &);  // TO DELETE
bool isValidHostname(const std::string &);
bool isValidIPAddress(const std::string &);
bool isValidPort(int value);
bool isValidMethods(const std::string &);
bool isValidPath(const std::string &);
bool isValidAbsolutePath(const std::string &);
bool isValidRelativePath(const std::string &);
bool isValidUrl(const std::string &);
bool isValidIndexFile(const std::string &);
bool isValidClientMaxBodySize(const std::string &);
bool isValidAutoindex(const std::string &);
bool isValidFileExt(const std::string &);

#endif // INCLUDE_SRC_SERVERCONFVALIDATEFIELD_HPP
