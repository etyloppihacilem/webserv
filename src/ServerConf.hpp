#ifdef INCLUDE_SRC_SERVERCONF_HPP_
#define INCLUDE_SRC_SERVERCONF_HPP_

#include <string>
#include <vector>
#include <limits>

namespace ServerConf
{

	static const int MAX_PORT = 65535;
	static const int MAX_BODY_SIZE = INT_MAX;
	static const int MAX_CLIENT_CONNECTION = 5;
	static const int CONNECTION_TIMEOUT = 180; //second
	static const int REQUEST_TIMEOUT = 60; //second
	static const int INPUT_TIMEOUT = 15; //second
	static const int OUTPUT_TIMEOUT = 180; //second
	static const int CGI_TIMEOUT = 180; //second
	static const std::string ISSPACE = " 	";

	typedef std::pair<std::string, std::string> Field;

	struct Fields {
		static std::vector<std::string> root(1);
		static std::vector<std::string> base(1);
		static std::vector<std::string> server(8);
		static std::vector<std::string> location(8);
	};

	struct Module {
		static std::vector<Field> serverFields;
		static std::map<std::string, std::vector<Field> > locations;
	};

	struct Config {
		static std::string root;
		static std::vector<std::string> serverList;
		static std::vector<Module> serverDetails;
	};
}

ServerConf::Fields::root[0] = "http";
ServerConf::Fields::base[0] = "server";
ServerConf::Fields::server[0] = "listen";
ServerConf::Fields::server[1] = "root";
ServerConf::Fields::server[2] = "index";
ServerConf::Fields::server[3] = "server_name";
ServerConf::Fields::server[4] = "client_max_body_size";
ServerConf::Fields::server[5] = "methods";
ServerConf::Fields::server[6] = "location";
ServerConf::Fields::server[7] = "error_page";
ServerConf::Fields::location[0] = "root";
ServerConf::Fields::location[1] = "index";
ServerConf::Fields::location[2] = "autoindex";
ServerConf::Fields::location[3] = "methods";
ServerConf::Fields::location[4] = "rewrite";
ServerConf::Fields::location[5] = "upload_path";
ServerConf::Fields::location[6] = "cgi_path";
ServerConf::Fields::location[7] = "file_ext";

#endif // __INCLUDE_SRC_SERVERCONF_HPP
