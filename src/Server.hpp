#ifndef __INCLUDE_SRC_SERVER__
#define __INCLUDE_SRC_SERVER__

#include <string>
#include <map>

#include "Socket.hpp"
#include "Location.hpp"
class Request;

class Server {

private:

	std::string _ipAddress;
	unsigned int _port;
	unsigned int _maxConnection;
	unsigned int _maxBodySize;
	std::string _rootDir;
	std::map<std::string, Location> _routes;
	std::map<std::string, std::string> _errorPages;
	Socket _socket;

	Request parseRequest(int clientSocket);
	void requestHandler(int clientSocket);
	void addLocation();
	void returnHTML(std::string url, std::string html);
	void returnHTML(std::string url, std::string html, unsigned int statusCode);

public:

	Server();
	Server(std::string ipAddress, unsigned int port, unsigned int maxConnections, std::string rootDir);
	void run();
	~Server();

};

// config file fields 
// SERVER
// - listen (port) - mand -> def=8080
// - root (foler ortigin) - mand/opt? -> def=www
// - index (default page ?) - mand/opt? -> def=index.html
// - serverName (host/ IP) mand -> localhost 127.0.0.1
// - client_max_body_size 
// - methods - mand -> def=GET
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
