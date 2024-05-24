#ifdef INCLUDE_SRC_SERVERCONFVALIDATEFIELD_HPP_
#define INCLUDE_SRC_SERVERCONFVALIDATEFIELD_HPP_

#include <string>

namespace ServerConf
{

	bool isValidHttp(const std::string &);
	bool isValidServer(const std::string &);
	bool isValidListen(const std::string &);
	bool isValidServerName(const std::string &);
	bool isValidRoot(const std::string &);
	bool isValidMethods(const std::string &);
	bool isValidClientMaxBodySize(const std::string &);
	bool isValidAutoindex(const std::string &);
	bool isValidIndex(const std::string &);
	bool isValidCgiPath(const std::string &);
	bool isValidUploadPath(const std::string &);
	bool isValidRewrite(const std::string &);
	bool isValidErrorPage(const std::string &);
	bool isValidFileExt(const std::string &);

}

#endif // __INCLUDE_SRC_SERVERCONFVALIDATEFIELD_HPP
