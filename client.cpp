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
#include <sstream>

using namespace std;

class IP {
public:
    string getIPByNum(const int num) {
        char domain[100];
        sprintf(domain, "srv365-%02d.cewit.stonybrook.edu", num);
        return domain2IP(domain);
    }

    string domain2IP(const char* pcDomain) {
        struct hostent *phEnt;
        char pcAddr[20];
        char **ppc;

        cout << pcDomain << endl;
        if ((phEnt = gethostbyname(pcDomain)) == NULL) {
            cerr << "cannot find IP" << endl;
        } else {
            if (phEnt->h_addrtype == AF_INET) {
                for (ppc = phEnt->h_addr_list; *ppc!= NULL; ++ppc) {
                    inet_ntop(phEnt->h_addrtype, *ppc, pcAddr, sizeof(pcAddr));
                    stringstream ss;
                    ss << pcAddr;
                    return ss.str();
                }
            }
        }
    }

    bool isIP(const string& s) {
        int iSegCnt = 0;
        int iCurSeg = 0;
        bool bEmpty = true;
        for (int i = 0; i < s.length(); ++i) {
            if (s[i] == '.') {
                if (iCurSeg > 255) {
                    return false;
                } else if (iCurSeg == 0) {
                    if (bEmpty) {
                        return false;
                    }
                }
                iCurSeg = 0;
                bEmpty = true;
                if (++iSegCnt > 3) {
                    return false;
                }
            } else if (s[i] >= '0' && s[i] <= '9') {
                iCurSeg += s[i] - '0';
            } else {
                return false;
            }
        }
        return true;
    }
};

int main(int argc, char** argv) {
    if (argc < 3) {
        cout << "usage: " << argv[0] << " domain|IP|abbr port" << endl;
        cout << "abbr:" << endl;
        cout << "    d9:  srv365-09.cewit.stonybrook.edu" << endl;
        cout << "    d11: srv365-11.cewit.stonybrook.edu" << endl;
        cout << "    i9:  192.168.13.9" << endl;
        cout << "    i11: 192.168.13.11" << endl;
        cout << "    i0:  127.0.0.1" << endl;

        return 1;
    }

    struct sockaddr_in addrServ;
    int iConn = socket(AF_INET, SOCK_STREAM, 0);
    class IP IPUtil;
    int iPortNum = atoi(argv[2]);
    string strIP = "";

    if (!strcmp(argv[1], "d9")) {
        strIP = IPUtil.getIPByNum(9);
    } else if (!strcmp(argv[1], "d11")) {
        strIP = IPUtil.getIPByNum(11);
    } else if (!strcmp(argv[1], "i9")) {
        strIP = "192.168.13.9";
    } else if (!strcmp(argv[1], "i11")) {
        strIP = "192.168.131.11";
    } else if (!strcmp(argv[1], "i0")) {
        strIP = "127.0.0.1";
    } else if (IPUtil.isIP(argv[1])) {
        strIP = argv[1];
    } else {
        strIP = IPUtil.domain2IP(argv[1]);
    }

    bzero(&addrServ, sizeof(addrServ));
    addrServ.sin_family = AF_INET;
    addrServ.sin_port = htons(iPortNum);
    inet_pton(AF_INET, strIP.c_str(), &addrServ.sin_addr);
    connect(iConn, (struct sockaddr*) &addrServ, sizeof(addrServ)); 

    return 0;
}

