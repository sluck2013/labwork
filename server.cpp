#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <memory.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <stdarg.h>
#include <math.h>
#include <sys/termios.h>
#include <iostream>

using namespace std;

int main() {
    int iListenfd;
    struct sockaddr_in addr, clientAddr;
    socklen_t clientAddrLen;

    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(2002);
    iListenfd = socket(AF_INET, SOCK_STREAM, 0);

    bind(iListenfd, (struct sockaddr*)&addr, sizeof(addr));
    listen(iListenfd, 10);
    
    fd_set fsRst, fsCurRst;
    FD_ZERO(&fsRst);
    FD_SET(iListenfd, &fsRst);

    cout << "Listening to port 2002..." << endl;

    while (1) {
        fsCurRst = fsRst;
        if ( select(iListenfd + 1, &fsRst, NULL, NULL, NULL) > 0 && FD_ISSET(iListenfd, &fsCurRst)) {
            clientAddrLen = sizeof(clientAddr);
            int iConn = accept(iListenfd, (struct sockaddr*)&clientAddr, &clientAddrLen);
            char ip[20];
            inet_ntop(AF_INET, &clientAddr.sin_addr, ip, 20);
            cout << endl << "Established connection with " << ip << endl;

            char buffer[4096];
            while (1) {
                int x = recv(iConn, buffer, sizeof(buffer), 0);
                cout << "recv: " << buffer << endl;
                if (!strncmp(buffer, "exit", 5)) {
                    break;
                }
                send(iConn, buffer, strlen(buffer), 0);
                cout << "sent: " << buffer << endl;
            }
            close(iConn);
            cout << endl << "Connection with " << ip << " closed!" << endl;
        }
    }
}
