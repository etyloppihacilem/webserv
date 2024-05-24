#ifdef INCLUDE_SRC_SERVERCONFTOKENIZE_HPP_
#define INCLUDE_SRC_SERVERCONFTOKENIZE_HPP_

#include <string>

namespace ServerConf
{
	typedef std::pair<std::string, std::string> Field;

	std::vector <std::string> split (std::string& text, const std::string& delimiter);
	std::vector<Field> tokenizeServer(const std::string &);
	std::vector<Field> tokenizeLocation(const std::string &);

}

#endif // __INCLUDE_SRC_SERVERCONFTOKENIZE_HPP
