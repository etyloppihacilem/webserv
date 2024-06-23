/* #############################################################################

               """          MimeTypes.hpp
        -\-    _|__
         |\___/  . \        Created on 23 Jun. 2024 at 12:06
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_MIMETYPES_HPP_
#define INCLUDE_SRC_MIMETYPES_HPP_

#include <map>
#include <string>

/**
  Used to parse and access mime types file.
  format needs to be the following :
  ```
  types {
        mime/type   ext1 ext2 extN...;
        ...
  }
  ```
  With mime/type being a mime type and extN being a file extension.
  */
class MimeTypes {
    public:
        MimeTypes(std::string path = "./conf/mime.types");
        ~MimeTypes();

        std::string                         get_type(std::string extension);

    private:
        std::map<std::string, std::string>  _types; ///< Stores mime types <extension>:<mimetype>
};

#endif  // INCLUDE_SRC_MIMETYPES_HPP_
