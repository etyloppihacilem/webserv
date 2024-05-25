#ifdef INCLUDE_SRC_SERVERCONFTOKENIZE_HPP_
#define INCLUDE_SRC_SERVERCONFTOKENIZE_HPP_

#include <string>

namespace ServerConf
{

void tokenzieFile(const std::string& );
bool tokenizeHttp(StringTokenizer );
bool tokenizeServer(StringTokenizer );
bool tokenizeLocation(StringTokenizer );
bool tokenizeField(StringTokenizer );

}

#endif // __INCLUDE_SRC_SERVERCONFTOKENIZE_HPP
