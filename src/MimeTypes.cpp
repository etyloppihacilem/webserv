/* #############################################################################

               """          MimeTypes.cpp
        -\-    _|__
         |\___/  . \        Created on 23 Jun. 2024 at 12:06
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "MimeTypes.hpp"
#include "Logger.hpp"
#include <cstddef>
#include <fstream>
#include <string>

MimeTypes::MimeTypes(std::string path):
    _done   (false),
    _types  () {
    std::fstream file;

    _types["txt"] = "text/plain"; // default, should always be there.
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
                word            = word.substr(0, coma);
                _types[word]    = type;
                type            = "";
            } else
                _types[word] = type;
        }
    }
    if (!_done) {
        _types["txt"] = "text/plain"; // default, should always be there.
        error.log() << path << ": MimeTypes parsing unsuccessful." << std::endl;
    }
    file.close();
}

MimeTypes::~MimeTypes() {}

/**
  Returns mime type for given extension.

  If the extension is unknown, text/plain is returned.
  */
std::string MimeTypes::get_type(std::string extension) {
    if (_types.find(extension) == _types.end()) {
        warn.log() << "MimeTypes.get_type(" << extension << ")" << std::endl;
        return "text/plain";
    }
    return _types[extension];
}

/**
  Returns true if object is built with no mistakes.

  False means there were mistakes while building it and that every mime type may not be present. There will always be
  a "txt text/plain" as mime type.
  */
bool MimeTypes::is_done() const {
    return _done;
}
