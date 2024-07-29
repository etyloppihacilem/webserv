/* #############################################################################

               """          MimeTypes.cpp
        -\-    _|__
         |\___/  . \        Created on 23 Jun. 2024 at 12:06
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "MimeTypes.hpp"
#include "Logger.hpp"
#include <cmath>
#include <cstddef>
#include <fstream>
#include <map>
#include <ostream>
#include <string>

MimeTypes::MimeTypes(std::string path): _done(false), _types() {
    std::fstream file;

    file.open(path.c_str());
    if (!file.is_open()) {
        error.log() << "Cannot open mime types file " << path << ". No mime types loaded." << std::endl;
        return;
    }

    size_t      coma; // it looks for semicolon
    size_t      count = 0;
    std::string type;
    std::string word;

    while (file >> word) {
        if ((count == 0 && word != "types") || (count == 1 && word != "{")) {
            error.log() << path << ": Syntax error, no types array found." << std::endl;
            break;
        }
        if (count++ < 2)
            continue;
        if (word == "}") {
            _done = true;
            break;
        }
        if (type == "")
            type = word;
        else {
            coma = word.find(';');
            if (coma != word.npos) {
                if (coma != word.length() - 1)
                    break;
                word         = word.substr(0, coma);
                _types[word] = type;
                type         = "";
            } else
                _types[word] = type;
        }
    }
    if (!_done)
        error.log() << path << ": MimeTypes parsing unsuccessful." << std::endl;
    file.close();
}

MimeTypes::MimeTypes(MimeTypes &other): _done(), _types() {
    (void) other;
    error.log() << "FATAL you should not duplicate any MimeTypes object." << std::endl;
}

MimeTypes::~MimeTypes() {}

/**
  Returns mime type for given extension.

  If the extension is unknown, text/plain is returned.
  */
std::string MimeTypes::get_type(const std::string &extension) const {
    std::map<std::string, std::string>::const_iterator it = _types.find(extension);

    if (it == _types.end()) {
        warn.log() << "MimeTypes.get_type(" << extension << ") while extension is not in mime table." << std::endl;
        return "application/octet-stream";
    }
    return it->second;
}

bool MimeTypes::has_type(const std::string &extension) const {
    return _types.find(extension) != _types.end();
}

/**
  Returns true if object is built with no mistakes.

  False means there were mistakes while building it and that every mime type may not be present. There will always be
  a "txt text/plain" as mime type.
  */
bool MimeTypes::is_done() const {
    return _done;
}

MimeTypes mime_types;
