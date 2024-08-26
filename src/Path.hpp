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

        static std::string add_path(const std::string &a, const std::string &b);

        Path &operator=(const Path &);
        Path &operator=(const std::string &);
        Path  operator+(const std::string &) const;
        Path &operator+=(const std::string &);
        // Path               operator+(const Path &) const;
        // Path              &operator+=(const Path &);

        bool operator==(const Path &) const;
        bool operator!=(const Path &) const;
        bool operator>=(const Path &) const;
        bool operator<=(const Path &) const;
        bool operator>(const Path &) const;
        bool operator<(const Path &) const;
        bool in(const Path &) const; ///< Tells if this is inside other Path.

        const std::string &str() const;
        void               normalize();

    private:
        void        make_absolute();
        std::string _path;
};

#endif // INCLUDE_SRC_PATH_HPP_
