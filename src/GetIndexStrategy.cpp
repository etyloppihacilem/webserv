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
#include "ResponseBuildState.hpp"
#include "StringUtils.hpp"
#include "todo.hpp"
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <new>
#include <ostream>
#include <sstream>
#include <string>
#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>

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

/**
  Returns the type of the item on 3 character.

  Types are :
    - REG : registry aka normal file
    - DIR : directory aka folder
    - CHR : character peripheral (I have no idea)
    - BLK : block peripheral
    - FIO : FIFO pipe
    - LNK : link
    - SCK : socket
  */
std::string GetIndexStrategy::getType(mode_t mode) {
    if (S_ISREG(mode))
        return "REG";
    if (S_ISDIR(mode))
        return "DIR";
    if (S_ISCHR(mode))
        return "CHR";
    if (S_ISBLK(mode))
        return "BLK";
    if (S_ISFIFO(mode))
        return "FIO";
    if (S_ISLNK(mode))
        return "LNK";
    if (S_ISSOCK(mode))
        return "SCK";
    return "XXX";
}

/**
  Used to fill the stat structure with informations.

  Returns name value or an error message to display.
  */
std::string GetIndexStrategy::generateLine(char *name, struct stat *st) {
    std::string path = _location + std::string(name);

    if (!stat(name, st))
        return name;
    else {
        bzero(st, sizeof(struct stat));
        return "Could not access file.";
    }
}

bool GetIndexStrategy::fill_buffer(std::string &buffer, size_t size) {
    if (_done || !_dir)
        return _done;
    if (!_init_done)
        buffer += "<head></head><body><h1>" + _location
                  + "</h1><table><tr><td>Type</td><td>Name</td><td>size</td></tr>";

    dir_item            *item;
    std::stringstream   stream;
    std::string         name;
    struct stat         st;

    while ((item = readdir(_dir)) && buffer.length() < size) {
        name = generateLine(item->d_name, &st);
        stream  << "<tr><td>" << getType(st.st_mode) << "</td><td><a href=\"" << _location << name << "\">" << name
                << "</a></td><td>" << (S_ISREG(st.st_mode) ? st.st_size : 0) << "</td></tr>";
        stream >> buffer; // TODO:check link href value
    }
    if (errno == EBADF)
        throw HttpError(InternalServerError);
    if (buffer.length() < size && !_deinit_done) {
        buffer += "</table></body>";
        closedir(_dir);
        _dir    = 0;
        _done   = true;
    }
    return _done;
}

int compare(const struct dirent **a, const struct dirent **b) {
    return strcmp((*a)->d_name, (*b)->d_name);
}

bool GetIndexStrategy::build_response() {
    if (_built) {
        warn.log() << "GetIndexStrategy : trying to build response, but is already built." << std::endl;
        return _built;
    }
    {                                                                       // different scope to free stack at the end
        int size_temp;
        {
            dirent **namelist;

            size_temp = scandir(_location.c_str(), &namelist, 0, compare);  // HACK:tester avec arg[2] null bc douteux
            free(namelist);
        }

        // attention leaks, verifier si version sort est necessaire ou si peut etre remplacé par null
        // sinon faire une structure et free immediatement.
        // OPTI:tester si ce ne serait pas utile d'utiliser cette fonction plutot que plusieurs readdir surtout avec un
        // alpha sort ou un version sort
        if (size_temp < 0)
            _estimated_size = MAX_BODY_BUFFER + 1;          // if an error occur, it is mostly because of
                                                            // memory, so using least buffer stuff
        // OPTI:see if reducing buffer size if memory error with errno is possible
        else
            _estimated_size = 47 + 15 + (56 * size_temp);   // TODO:revoir ces estimations
    }
    _dir = opendir(_location.c_str());
    if (!_dir) {
        if (errno == EACCES)
            throw HttpError(Forbidden);
        if (errno == ENOENT)
            throw HttpError(NotFound);
        if (errno == ENOMEM)
            throw std::bad_alloc(); // to begin memory recovery procedure
        throw HttpError(InternalServerError);
    }
    _response.add_header("Content-Type", "text/html; charset=utf-8");
    _response.set_body(*this);
    return _built = true;
}

void GetIndexStrategy::save_mem() {
    shrink_to_fit(  _buffer);
    shrink_to_fit(  _location);
    _response.save_mem();
}
