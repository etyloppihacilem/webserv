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
#include "Logger.hpp"
#include "todo.hpp"
#include <cerrno>
#include <cstddef>
#include <cstdlib>
#include <dirent.h>
#include <new>
#include <sstream>
#include <string>

// location ending in /
GetIndexStrategy::GetIndexStrategy(const std::string &location, ResponseBuildState &state):
    ResponseBuildingStrategy(state),
    _location               (location),
    _dir                    (0),
    _init_done              (false),
    _deinit_done            (false) {}

GetIndexStrategy::~GetIndexStrategy() {
    if (_dir)
        closedir(_dir);
}

bool GetIndexStrategy::fill_buffer(std::string &buffer, size_t size) {
    if (_done || !_dir)
        return (_done);
    if (!_init_done)
        buffer += "<head></head><body><h1>" + _location + "</h1><table>";

    dir_item            *item;
    std::stringstream   stream;

    while ((item = readdir(_dir)) && buffer.length() < size) {
        stream  << "<tr><td>" << (item->d_type
                                  == DT_DIR ? "DIR" : "REG") << "</td><td><a href=\"" << _location
                << std::string(item->d_name) << "\">" << item->d_name << "</a></td></tr>";
        stream >> buffer; // TODO check link href value
    }
    if (errno == EBADF)
        throw (HttpError(InternalServerError));
    if (buffer.length() < size && !_deinit_done) {
        buffer += "</table></body>";
        closedir(_dir);
        _dir = 0;
    }
    return (_done);
}

void GetIndexStrategy::buildResponse() {
    if (_dir) {
        warn.log("GetIndexStrategy : trying to build response, but is already built.");
        return;
    }
    {                                                                           // different scope to free stack at the
                                                                                // end
        int size_temp;
        {
            dirent **namelist;

            size_temp = scandir(_location.c_str(), &namelist, 0, versionsort);  // TODO tester avec arg[2] null bc
            free(namelist);                                                     // douteux,
        }

        // attention leaks, verifier si version sort est necessaire ou si peut etre remplacé par null
        // sinon faire une structure et free immediatement.
        // TODO tester si ce ne serait pas utile d'utiliser cette fonction plutot que plusieurs readdir surtout avec un
        // alpha sort ou un version sort
        if (size_temp < 0)
            _estimated_size = MAX_BODY_BUFFER + 1;                      // if an error occur, it is mostly because of
                                                                        // memory, so using least buffer stuff
        // TODO see if reducing buffer size if memory error with errno is possible
        else
            _estimated_size = 47 + 15 + (56 * size_temp);
    }
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
