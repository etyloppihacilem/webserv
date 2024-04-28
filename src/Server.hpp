#ifndef __INCLUDE_SRC_SERVER__
#define __INCLUDE_SRC_SERVER__

#include <string>
#include <map>

#include "Socket.hpp"
class Request;

class Server {

private:

	std::string _ipAddress;
	unsigned int _port;
	unsigned int _maxConnection;
	std::string _rootDir;
	std::map<std::string, std::string> _routes;
	Socket _socket;

	Request parseRequest(int clientSocket);
	void requestHandler(int clientSocket);

public:

	Server();
	Server(std::string ipAddress, unsigned int port, unsigned int maxConnections, std::string rootDir);
	void run();
	~Server();

};

// config file fields 
// SERVER
// - listen (port)
// - root (foler ortigin)
// - index (default page ?)
// - serverName (host/ IP)
// - client_max_body_size 
// - methods 
// SERVER/LOCATION
// - methods
// - index
// - autoindex
// - upload_path
// - rewrite
// - cgi path
// - file_ext
// SERVER/ERROR_PAGE -> file location
//
// CONTEXT:
// Core -> Server -> Location

#endif // !__INCLUDE_SRC_SERVER__
