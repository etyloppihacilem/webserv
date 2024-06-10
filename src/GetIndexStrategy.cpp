/* #############################################################################

               """          GetIndexStrategy.cpp
        -\-    _|__
         |\___/  . \        Created on 10 Jun. 2024 at 14:11
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "BodyWriter.hpp"
#include "GetIndexStrategy.hpp"
#include "HttpError.hpp"
#include "HttpStatusCodes.hpp"
#include <cerrno>
#include <cstddef>
#include <dirent.h>
#include <new>
#include <sstream>
#include <string>

// location ending in /
GetIndexStrategy::GetIndexStrategy(const std::string &location, ResponseBuildState &state):
    ResponseBuildingStrategy(state),
    _location               (),
    _dir                    (0),
    _init_done              (false),
    _deinit_done            (false) {
    _dir = opendir(_location.c_str());
    if (!_dir) {
        if (errno == EACCES)
            throw (HttpError(Forbidden));
        if (errno == ENOENT)
            throw (HttpError(NotFound));
        if (errno == ENOMEM)
            throw (std::bad_alloc()); // to begin memory recovery procedure
        throw (HttpError(InternalServerError));
    }
}

GetIndexStrategy::~GetIndexStrategy() {
    if (_dir)
        closedir(_dir);
}

bool GetIndexStrategy::fill_buffer(std::string &buffer, size_t size) {
    if (_done)
        return (_done);
    if (!_init_done)
        buffer += "<head></head><body><h1>" + _location + "</h1><table>";

    dir_item            *item;
    std::stringstream   stream;

    while ((item = readdir(_dir)) && buffer.length() < size) {
        stream  << "<tr><td>" << (item->d_type == DT_DIR ? "DIR" : "REG")
                << "</td><td><a href=\"" << _location << std::string(item->d_name) << "\">"
                << item->d_name << "</a></td></tr>";
        stream >> buffer;
    }
    if (errno == EBADF)
        throw (HttpError(InternalServerError));
    if (buffer.length() < size && !_deinit_done) {
        buffer += "</table></body>";
    }
    return (_done);
}

void GetIndexStrategy::buildResponse() {
    ;
}
