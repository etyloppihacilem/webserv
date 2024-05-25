#ifdef INCLUDE_SRC_SERVERCONFTOKENIZE_HPP_
#define INCLUDE_SRC_SERVERCONFTOKENIZE_HPP_

#include <string>

namespace ServerConf
{
	typedef std::pair<std::string, std::string> Field;

	std::vector<std::string> tokenize(std::string& , const std::string& );
	std::vector<std::string> tokenizeField(const std::string &);
	std::vector<Field> tokenizeServer(const std::string &);
	std::vector<Field> tokenizeLocation(const std::string &);

}

#endif // __INCLUDE_SRC_SERVERCONFTOKENIZE_HPP
