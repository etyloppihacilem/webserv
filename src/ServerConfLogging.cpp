#include "ServerConfLogging.hpp"
#include "Logger.hpp"
#include "ServerConfFields.hpp"
#include <algorithm>
#include <cstddef>
#include <ostream>
#include <string>

std::string buildLoggingInfo(const std::string &content) {
    std::string troncatedReadableContent = content.substr(0, 30);

    std::replace(troncatedReadableContent.begin(), troncatedReadableContent.end(), '|', ' ');

    return troncatedReadableContent;
}

void logParsingStarted(ConfField fieldCode, const std::string &content, const std::string &locationName) {
    info.log() << ConfFieldString(fieldCode) << " " << locationName << " { " << buildLoggingInfo(content)
               << " ... : parsing started ..." << std::endl;
}

void logParsingEnded(ConfField fieldCode) {
    debug.log() << ConfFieldString(fieldCode) << " ... parsing ended." << std::endl;
}

void logMissingFirstToken(ConfField fieldCode, const std::string &loggingInfo) {
    error.log() << loggingInfo << " ... : the first token is not '" << ConfFieldString(fieldCode)
                << "', parsing canceled." << std::endl;
}

void logMissingOpenBrace(ConfField fieldCode, const std::string &loggingInfo) {
    error.log() << loggingInfo << " ... : token '{' is missing after token '" << ConfFieldString(fieldCode)
                << "' , parsing canceled." << std::endl;
}

void logMissingCloseBrace(ConfField fieldCode, const std::string &loggingInfo) {
    error.log() << loggingInfo << " ... : token '}' is missing unable to locate the end of '"
                << ConfFieldString(fieldCode) << "' module , parsing canceled." << std::endl;
}

void logFieldRedefinition(ConfField fieldCode, const ValueList &values) {
    std::ostream &out = warn.log();
    out << ConfFieldString(fieldCode) << ": ";
    for (size_t i = 0; i < values.size(); ++i) {
        out << values[i];
        if (i < values.size() - 1)
            out << ", ";
    }
    out << ": is a redifinition of the field value, input ignored." << std::endl;
}

void logInvalidValuesCount(ConfField fieldCode, const ValueList &values) {
    std::ostream &out = warn.log();
    out << ConfFieldString(fieldCode) << ": ";
    for (size_t i = 0; i < values.size(); ++i) {
        out << values[i];
        if (i < values.size() - 1)
            out << ", ";
    }
    if (fieldCode == rewrite)
        out << ", fail to parse field, it accept only a redir code and a path value." << std::endl;
    else if (fieldCode == error_page)
        out << ", fail to parse field, it accept only a error code and a path value." << std::endl;
    else
        out << ": fail to parse field, it accept only one value." << std::endl;
}

void logInvalidIntConvertion(ConfField fieldCode, const std::string &value) {
    warn.log() << ConfFieldString(fieldCode) << ": " << value << ": is not a valid int." << std::endl;
}
