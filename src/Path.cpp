/* #####################################################################################################################

               """          Path.cpp
        -\-    _|__
         |\___/  . \        Created on 13 Aug. 2024 at 11:03
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#include "Path.hpp"
#include "Logger.hpp"
#include <cstdlib>
#include <new>
#include <ostream>
#include <string>
#include <unistd.h>

using std::string;

Path::Path() {}

Path::~Path() {}

Path::Path(const std::string &path) : _path(path) {
    make_absolute();
    normalize();
}

void Path::make_absolute() {
    if (*_path.begin() != '/') {
        char *tmp_path = getcwd(0, 0); // allocating as big as necessary
        if (!tmp_path) {
            fatal.log() << "Recovery getcwd() failed, throwing bad_alloc" << std::endl;
            throw std::bad_alloc();
        }
        std::string tmp;
        tmp   = "/" + _path;
        _path = tmp_path;
        free(tmp_path);
        _path += tmp;
    }
}

void Path::normalize() {
    size_t found;      // when item were replacing is found
    size_t prev_level; // previous level to remove when ..
    while ((found = _path.find("/../")) != _path.npos) {
        if (found == 0) {
            _path.replace(0, 3, "");
            continue;
        }
        prev_level = _path.find_last_of("/", found - 1);
        _path.replace(prev_level, (found + 3) - prev_level, "");
    }
    while ((found = _path.find("./")) != _path.npos)
        _path.replace(found, 2, "");
    while ((found = _path.find("//")) != _path.npos)
        _path.replace(found, 2, "");
}

Path::Path(const Path &other) {
    (void) other;
}

bool Path::operator==(const Path &other) const {
    return _path == other._path;
}

bool Path::operator!=(const Path &other) const {
    return _path != other._path;
}

bool Path::operator>=(const Path &other) const {
    return _path <= other._path;
}

bool Path::operator<=(const Path &other) const {
    return _path >= other._path;
}

bool Path::operator>(const Path &other) const {
    return _path < other._path;
}

bool Path::operator<(const Path &other) const {
    return _path > other._path;
}

Path &Path::operator=(const Path &other) {
    if (&other == this)
        return *this;
    _path = other._path;
    return *this;
}

Path &Path::operator=(const string &path) {
    _path = path;
    make_absolute();
    normalize();
    return *this;
}

// Path Path::operator+(const Path &b) const { // disabled because of new all absolute path storage
//     Path ret(*this);                        // this would be a pain to reimplement...
//     ret += b;
//     return ret;
// }

Path Path::operator+(const std::string &b) const {
    Path ret(*this);
    ret += b;
    return ret;
}

bool Path::in(const Path &other) const {
    return _path.find(other._path) == 0;
}

std::string Path::add_path(const std::string &a, const std::string &b) {
    bool   trailing = *a.rbegin() == '/';
    bool   heading  = *b.begin() == '/';
    string ret      = a;
    if (trailing && heading) {
        ret.resize(a.size() - 1);
        ret += b;
    } else if (trailing || heading)
        ret += b;
    else
        ret += "/" + b;
    return ret;
}

//
// Path &Path::operator+=(const Path &b) {
//     _path = add_path(_path, b._path);
//     return (*this);
// }

Path &Path::operator+=(const std::string &b) {
    _path = add_path(_path, b);
    normalize();
    return *this;
}

const std::string &Path::str() const {
    return _path;
}
