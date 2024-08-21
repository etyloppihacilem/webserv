#ifndef INCLUDE_SRC_SERVERCONFLOGGING_HPP_
#define INCLUDE_SRC_SERVERCONFLOGGING_HPP_

#include "ServerConfFields.hpp"
#include <string>

std::string buildLoggingInfo(const std::string &content);
void logParsingStarted(ConfField fieldCode, const std::string &content, const std::string &locationName = "");
void logParsingEnded(ConfField fieldCode);
void logMissingFirstToken(ConfField fieldCode, const std::string &loggingInfo);
void logMissingOpenBrace(ConfField fieldCode, const std::string &loggingInfo);
void logMissingCloseBrace(ConfField fieldCode, const std::string &loggingInfo);
void logFieldRedefinition(ConfField fieldCode, const ValueList &values);
void logInvalidValuesCount(ConfField fieldCode, const ValueList &values);
void logInvalidIntConvertion(ConfField fieldCode, const std::string &value);

#endif // !__INCLUDE_SRC_SERVERCONFLOGGING_HPP
