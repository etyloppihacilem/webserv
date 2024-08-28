/* #############################################################################

               """          GetIndexStrategy.cpp
        -\-    _|__
         |\___/  . \        Created on 10 Jun. 2024 at 14:11
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "GetIndexStrategy.hpp"
#include "BodyWriter.hpp"
#include "HttpError.hpp"
#include "HttpStatusCodes.hpp"
#include "Logger.hpp"
#include "StringUtils.hpp"
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <fcntl.h>
#include <new>
#include <ostream>
#include <sstream>
#include <string>
#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// location ending in /
GetIndexStrategy::GetIndexStrategy(const std::string &location, const std::string &target) :
    ResponseBuildingStrategy(),
    _location(location),
    _target(target),
    _dir_list(0),
    _index(0),
    _len(0),
    _init_done(false),
    _deinit_done(false) {
    if (*_target.rbegin() != '/')
        _target += "/";
}

GetIndexStrategy::~GetIndexStrategy() {
    clean_dir_list();
}

void GetIndexStrategy::clean_dir_list() {
    if (_dir_list) {
        for (; _index < _len; _index++)
            free(_dir_list[_index]);
        free(_dir_list);
        _dir_list = 0;
    }
    _index = 0;
    _len   = 0;
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
    std::string path = _location + (*_location.rbegin() != '/' ? "/" : "") + std::string(name);
    //+ (*_location.rbegin() != '/' ? "/" : "") // not suppsed to be needed 265 103!!!

    if (!stat(path.c_str(), st))
        return name;
    else {
        bzero(st, sizeof(struct stat));
        warn.log() << "GetIndexStrategy: could not stat item at '" << path << "' " << strerror(errno) << std::endl;
        return "Could not access file.";
    }
}

bool GetIndexStrategy::fill_buffer(std::string &buffer, size_t size) {
    if (_done)
        return _done;
    if (!_dir_list) {
        error.log() << "Dir list not initiated or already cleaned, sending " << InternalServerError << std::endl;
        throw HttpError(InternalServerError);
    }
    if (!_init_done)
        buffer += "<head></head><body><h1>" + _target + "</h1><table><tr><td>Type</td><td>Name</td><td>size</td></tr>";

    dir_item   *item = 0;
    std::string name;
    struct stat st;

    while (_index < _len && buffer.size() < size) {
        item = _dir_list[_index];
        debug.log() << "GetIndexStrategy: found and listing " << item->d_name << std::endl;
        name = generateLine(item->d_name, &st);
        std::stringstream stream;
        stream << "<tr><td>" << getType(st.st_mode) << "</td><td><a href=\"" << _target << item->d_name << "\">" << name
               << "</a></td><td>" << (S_ISREG(st.st_mode) ? st.st_size : 0) << "</td></tr>";
        buffer += stream.str();
        free(_dir_list[_index++]);
    }
    if (errno == EBADF) {
        warn.log() << "GetIndexStrategy: Bad directory descriptor for '" << _location << "', sending "
                   << InternalServerError << std::endl;
        throw HttpError(InternalServerError);
    }
    if (buffer.length() < size && !_deinit_done) {
        buffer += "</table></body>";
        _done   = true;
    }
    return _done;
}

// TODO: Order is not garanteed, use the other thing
bool GetIndexStrategy::build_response() {
    if (_built) {
        warn.log() << "GetIndexStrategy : trying to build response, but is already built." << std::endl;
        return _built;
    }
    // TODO: estimate size
    int dir_fd = open(_location.c_str(), O_DIRECTORY | O_RDONLY);
    if (dir_fd < 0) {
        switch (errno) {
            case EACCES:
                info.log() << "GetIndexStrategy: cannot open directory '" << _location
                           << "' permission denied, sending " << Forbidden << std::endl;
                throw HttpError(Forbidden);
            case ENOENT:
                info.log() << "GetIndexStrategy: cannot open directory '" << _location << "' " << strerror(errno)
                           << ", sending " << NotFound << std::endl;
                throw HttpError(NotFound);
            default:
                warn.log() << "GetIndexStrategy: cannot open directory '" << _location << "' " << strerror(errno)
                           << ", sending " << InternalServerError << std::endl
                           << std::endl;
                throw HttpError(InternalServerError);
        }
    }
    if ((_len = scandirat(dir_fd, ".", &_dir_list, NULL, alphasort)) < 0) {
        close(dir_fd);
        switch (errno) {
            case EACCES:
                info.log() << "GetIndexStrategy: cannot scan directory '" << _location
                           << "' permission denied, sending " << Forbidden << std::endl;
                throw HttpError(Forbidden);
            case ENOENT:
                info.log() << "GetIndexStrategy: cannot scan directory '" << _location << "' " << strerror(errno)
                           << ", sending " << NotFound << std::endl;
                throw HttpError(NotFound);
            case ENOMEM:
                throw std::bad_alloc(); // to begin memory recovery procedure
            default:
                warn.log() << "GetIndexStrategy: cannot scan directory '" << _location << "' " << strerror(errno)
                           << ", sending " << InternalServerError << std::endl
                           << std::endl;
                throw HttpError(InternalServerError);
        }
    }
    close(dir_fd);
    _response.add_header("Content-Type", "text/html; charset=utf-8");
    _response.set_body(this);
    if (*_location.rbegin() != '/')
        _location += "/";
    return _built = true;
}

void GetIndexStrategy::save_mem() {
    shrink_to_fit(_location);
    _response.save_mem();
}
