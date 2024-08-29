#include "EventHandler.hpp"
#include "Logger.hpp"
#include "ServerManager.hpp"
#include "gtest/gtest.h"
#include <asm-generic/socket.h>
#include <cstdlib>
#include <ostream>
#include <set>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

void http_client_test(const std::string &hostname);
void http_server_test(void);

/*TEST(SocketTestSuite, TestProgram) {*/
/**/
/*    if (!fork()) {*/
/*        http_server_test();*/
/*        exit(0);*/
/*    }*/
/*    sleep(2);*/
/*    http_client_test("127.0.0.1");*/
/*}*/

TEST(SocketTestSuite, checkSocketConstruction) {
    ServerManager::getInstance("conf/webserv.conf");
    std::set< EventHandler * > eventHandler = ServerManager::getInstance()->getReactor().getEventHandler();
    for (std::set< EventHandler * >::iterator it = eventHandler.begin(); it != eventHandler.end(); ++it) {
        int fd = (*it)->getSocketFd();
        int port = (*it)->getPort();
        info.log() << "Test fd: " << fd << " open on port " << port << "." << std::endl;
        int       val;
        socklen_t len = sizeof val;
        int       ret = getsockopt((*it)->getSocketFd(), SOL_SOCKET, SO_ACCEPTCONN, &val, &len);
        EXPECT_NE(ret, -1);
        EXPECT_TRUE(val);
    }
    ServerManager::deleteInstance();
}
>>>>>>> 989f27b (Test ServerReactor init)
