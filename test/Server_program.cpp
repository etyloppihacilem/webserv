#include "Logger.hpp"
#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <ostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT "4161"
#define BACKLOG 10

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET)
        return &(((struct sockaddr_in *) sa)->sin_addr);
    warn.log() << "no IPv4 addr." << std::endl;
    return &(((struct sockaddr_in6 *) sa)->sin6_addr);
}

void http_server_test(void) {
    struct addrinfo hints;

    memset(&hints, 0, sizeof hints);
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE;

    int              retvalue;
    struct addrinfo *servinfo;
    if ((retvalue = getaddrinfo(0, PORT, &hints, &servinfo))) {
        error.log() << "server: getaddrinfo: " << std::string(gai_strerror(retvalue)) << std::endl;
        return;
    }

    int              sockfd;
    int              yes = 1;
    struct addrinfo *p;
    for (p = servinfo; p != 0; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            warn.log() << "server: socket: " << std::string(std::strerror(errno)) << std::endl;
            continue;
        }
        if (fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1) {
            close(sockfd);
            warn.log() << "server: fcntl: " << std::string(std::strerror(errno)) << std::endl;
            continue;
        }
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            close(sockfd);
            error.log() << "server: setsockopt: " << std::string(std::strerror(errno)) << std::endl;
            return;
        }
        info.log() << "server: bind to addr = " << std::string(p->ai_addr->sa_data) << std::endl;
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            warn.log() << "server: bind: " << std::string(std::strerror(errno)) << std::endl;
            continue;
        }
        break;
    }

    if (p == 0) {
        error.log() << "server: failed to bind" << std::endl;
        return;
    }
    freeaddrinfo(servinfo);

    if (listen(sockfd, BACKLOG) == -1) {
        error.log() << "server: listen: " << std::string(std::strerror(errno)) << std::endl;
        return;
    }
    info.log() << "server: waiting for connections..." << std::endl;

    struct sockaddr_storage client_addr;
    socklen_t               sin_size = sizeof client_addr;
    int                     client_fd;
    char                    s[INET6_ADDRSTRLEN];
    while (1) {
        client_fd = accept(sockfd, (struct sockaddr *) (&client_addr), &sin_size);
        if (client_fd == -1) {
            warn.log() << "server: accept: " << std::string(std::strerror(errno)) << std::endl;
            sleep(1);
            continue;
        } else {
            inet_ntop(client_addr.ss_family, get_in_addr((struct sockaddr *) &client_addr), s, sizeof s);
            info.log() << "server: got connection from " << std::string(s) << "." << std::endl;
            if (write(client_fd, "Hello, world!", 13) == -1)
                error.log() << "server: write: " << std::string(std::strerror(errno)) << std::endl;
            close(client_fd);
            break;
        }
    }
    close(sockfd);
    return;
}
