#ifndef INCLUDE_STRINGTOKENIZER_HPP_
#define INCLUDE_STRINGTOKENIZER_HPP_

#include <cstddef>
#include <string>

class StringTokenizer {
    public:
        StringTokenizer(const std::string &str, const char delim);
        ~StringTokenizer();

        std::string remainingString();
        char        delimiter();

        bool        hasMoreTokens();
        std::size_t countTokens();
        std::size_t countTokens(const char separator);

        std::string peakToken();
        std::string nextToken();
        std::string nextToken(const char separator);
        std::string nextToken(std::size_t separatorPosition);

    private:
        std::string _tokenString;
        char        _delimiter;

        std::string extractToken(std::size_t postion);
        void        removeTrailingDelimiter(std::string &token);
};

#endif // INCLUDE_SRC_STRINGTOKENIZER_HPP_
