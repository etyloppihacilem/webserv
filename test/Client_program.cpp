#include "Logger.hpp"
#include <arpa/inet.h>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <netdb.h>
#include <netinet/in.h>
#include <ostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT "4161"
#define MAXDATASIZE 100

void *get_in_addr_client(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET)
        return &(((struct sockaddr_in *) sa)->sin_addr);
    warn.log() << "no IPv4 addr." << std::endl;
    return &(((struct sockaddr_in6 *) sa)->sin6_addr);
}

void http_client_test(const std::string &hostname) {
    struct addrinfo hints;

    std::memset(&hints, 0, sizeof hints);
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int              retvalue;
    struct addrinfo *servinfo;
    if ((retvalue = getaddrinfo(hostname.c_str(), PORT, &hints, &servinfo)) != 0) {
        error.log() << "client: getaddrinfo: " << std::string(gai_strerror(retvalue)) << std::endl;
        return;
    }

    int              sockfd;
    struct addrinfo *p;
    for (p = servinfo; p != 0; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            info.log() << "client: socket: " << std::string(std::strerror(errno)) << std::endl;
            continue;
        }
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            info.log() << "client: connect: " << std::string(std::strerror(errno)) << std::endl;
            continue;
        }
        break;
    }

    if (p == 0) {
        error.log() << "client: failed to connect." << std::endl;
        return;
    }

    char hostaddr[INET_ADDRSTRLEN];
    inet_ntop(p->ai_family, get_in_addr_client((struct sockaddr *) p->ai_addr), hostaddr, sizeof hostaddr);
    info.log() << "client: connecting to " << std::string(hostaddr) << "." << std::endl;
    freeaddrinfo(servinfo);

    int  numbytes;
    char buf[MAXDATASIZE];
    if ((numbytes = read(sockfd, buf, MAXDATASIZE - 1)) == -1) {
        error.log() << "client: read: " << std::string(std::strerror(errno)) << std::endl;
        return;
    }
    buf[numbytes] = '\0';
    info.log() << "client: read: " << std::string(buf) << std::endl;

    close(sockfd);
    return;
}
