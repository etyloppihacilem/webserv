/* #####################################################################################################################

               """          Path.cpp
        -\-    _|__
         |\___/  . \        Created on 13 Aug. 2024 at 11:03
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#include "Path.hpp"
#include <string>

using std::string;

Path::Path() {}

Path::~Path() {}

Path::Path(const std::string &path) : _path(path) {}

Path::Path(const Path &other) {
    (void) other;
}

Path &Path::operator=(const Path &other) {
    if (&other == this)
        return *this;
    this->_path = other._path;
    return *this;
}

Path &Path::operator=(const string &path) {
    this->_path = path;
    return *this;
}

Path Path::operator+(const Path &b) const {
    bool trailing = *this->_path.rbegin() == '/';
    bool heading  = *b._path.begin() == '/';
    if (trailing && heading) {
        std::string tmp = this->_path;
        tmp.resize(tmp.length() - 1);
        return Path(tmp + b._path);
    }
    if (trailing || heading)
        return Path(this->_path + b._path);
    return Path(this->_path + "/" + b._path);
}

Path &Path::operator+=(const Path &b) {
    bool trailing = *this->_path.rbegin() == '/';
    bool heading  = *b._path.begin() == '/';
    if (trailing && heading) {
        std::string tmp = this->_path;
        this->_path.resize(this->_path.length() - 1);
        this->_path += b._path;
    }
    if (trailing || heading)
        this->_path += b._path;
    this->_path = +"/" + b._path;
    return *this;
}

const std::string &Path::str() const {
    return _path;
}
