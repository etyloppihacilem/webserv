#ifndef INCLUDE_SRC_SERVERCONFVALIDATE_HPP_
#define INCLUDE_SRC_SERVERCONFVALIDATE_HPP_

#include <string>

bool isValidHostname(const std::string &);
bool isValidIPAddress(const std::string &);
bool isValidPath(const std::string &);
bool isValidAbsolutePath(const std::string &);
bool isValidRelativePath(const std::string &);
bool isValidUrl(const std::string &);
bool isValidBool(const std::string &);
bool isValidExtension(const std::string &);

#endif // INCLUDE_SRC_SERVERCONFVALIDATE_HPP
