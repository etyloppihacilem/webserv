/* #####################################################################################################################

               """          Path.hpp
        -\-    _|__
         |\___/  . \        Created on 13 Aug. 2024 at 11:03
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#ifndef INCLUDE_SRC_PATH_HPP_
#define INCLUDE_SRC_PATH_HPP_

#include <string>

class Path {
    public:
        Path();
        Path(const std::string &);
        Path(const Path &);
        ~Path();

        Path              &operator=(const Path &);
        Path              &operator=(const std::string &);
        Path               operator+(const Path &) const;
        Path               operator+(const std::string &) const;
        Path              &operator+=(const Path &);
        Path              &operator+=(const std::string &);
        const std::string &str() const;

    private:
        std::string _path;
};

#endif // INCLUDE_SRC_PATH_HPP_
