#ifdef INCLUDE_SRC_SERVERCONF_HPP_
#define INCLUDE_SRC_SERVERCONF_HPP_

namespace ServerConf {

static const int MAX_PORT = 65535;

struct Fields {

	static std::vector<std::string> root(1);
	static std::vector<std::string> base(1);
	static std::vector<std::string> server(8);
	static std::vector<std::string> location(8);

};

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

};

ServerConf::root[0] = "http";
ServerConf::base[0] = "server";
ServerConf::server[0] = "listen";
ServerConf::server[1] = "root";
ServerConf::server[2] = "index";
ServerConf::server[3] = "server_name";
ServerConf::server[4] = "client_max_body_size";
ServerConf::server[5] = "methods";
ServerConf::server[6] = "location";
ServerConf::server[7] = "error_page";
ServerConf::location[0] = "root";
ServerConf::location[1] = "index";
ServerConf::location[2] = "autoindex";
ServerConf::location[3] = "methods";
ServerConf::location[4] = "rewrite";
ServerConf::location[5] = "upload_path";
ServerConf::location[6] = "cgi_path";
ServerConf::location[7] = "file_ext";

#endif // __INCLUDE_SRC_SERVERCONF_
