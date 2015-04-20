#include <netdb.h>
#include <arpa/inet.h>
#include <iostream>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>

using namespace std;


int main() {
    int s = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_port = htons(2002);
    inet_pton(AF_INET, "127.3.3.3", &sa.sin_addr);
    cout << bind(s, (struct sockaddr*) &sa, sizeof(sa)) << endl; 
}

