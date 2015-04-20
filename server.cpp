#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
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
#include <fstream>

using namespace std;

int main(int argc, char* argv[]) {
    ofstream ifs("aa.txt", ofstream::out);
    ifs << argv[1] << endl;
    ifs.close();
    
    int iListenfd;
    struct sockaddr_in addr, clientAddr;
    socklen_t clientAddrLen;

    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &(addr.sin_addr));
    //addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(2002);
    iListenfd = socket(AF_INET, SOCK_STREAM, 0);

    int optval = 1;
    setsockopt(iListenfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    int r = bind(iListenfd, (struct sockaddr*)&addr, sizeof(addr));
    if (r < 0) {
        cout << "bind failed" << endl;
    }
    if (listen(iListenfd, 8) < 0) {
        cout << "listen failed" << endl;
    }
    
    fd_set fsRst, fsCurRst;
    FD_ZERO(&fsRst);
    FD_SET(iListenfd, &fsRst);

    r = fork();
    if (r > 0) {
        signal(SIGCHLD, SIG_IGN);
        return 0;
    } else if (r < 0) {
        cerr << "1st fork failed" << endl;
        return 1;
    }

    setsid();
    r = fork();
    if (r != 0) {
        return 0;
    }

    chdir("/");
    umask(0);

    close(STDIN_FILENO);
    open("/dev/null", O_RDONLY);
    //close(STDOUT_FILENO);
    //open("/dev/null", O_WRONLY);

    sleep(10);
    while (1) {
        fsCurRst = fsRst;
        if ( select(iListenfd + 1, &fsRst, NULL, NULL, NULL) > 0 && FD_ISSET(iListenfd, &fsCurRst)) {
            pid_t pid = fork();
            if (pid == 0) {
                clientAddrLen = sizeof(clientAddr);
                int iConn = accept(iListenfd, (struct sockaddr*)&clientAddr, &clientAddrLen);
                cout << "accepted" << endl;
                //close(STDOUT_FILENO);
                //open("/dev/null", O_WRONLY);
                close(iListenfd);
                char ip[20];
                inet_ntop(AF_INET, &clientAddr.sin_addr, ip, 20);

                char buffer[4096];
                while (1) {
                    int x = recv(iConn, buffer, sizeof(buffer), 0);
                    if (!strncmp(buffer, "exit", 5)) {
                        break;
                    }
                    send(iConn, buffer, strlen(buffer), 0);
                }
                close(iConn);
                cout << endl << "Connection with " << ip << " closed!" << endl;
            }
        }
    }
}
