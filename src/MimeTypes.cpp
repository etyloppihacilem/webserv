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
    _types() {
    std::fstream file;

    _types["txt"] = "text/plain"; // default, should always be there.
    file.open(path.c_str());
    if (!file.is_open()) {
        error.log() << "Cannot open mime types file " << path << ". No mime types loaded." << std::endl;
        return;
    }

    bool        done = false;
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
            done = true;
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
    if (!done)
        error.log() << path << ": MimeTypes parsing unsuccessful." << std::endl;
    file.close();
}

MimeTypes::~MimeTypes() {}

std::string MimeTypes::get_type(std::string extension) {
    return _types[extension];
}
