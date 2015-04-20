#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sys/wait.h>
#include <cstring>
#include <errno.h>
using namespace std;

int main() {
/*    pid_t p1, p2;
    int sta;
    if (p1 = fork()) {
        waitpid(p1, &sta, NULL);
    } else if (!p1) {
        if (p2 = fork()) {
            exit(0);
        } else if (!p2) {
            ofstream of("/tmp/debug16.txt", ofstream::out);
            of << getenv("SSH_CLIENT") << endl;
            of.close();
        }

    }
    */
    char prog[] = "ssh";
    char arg1[] = "ssh";
    char arg2[] = "sluck@192.168.13.9";
    char arg3[] = "tunnel/test";
    char* arg4 = NULL;
    char* arg[] = {arg1, arg2, arg3, arg4};
    char argx[] = "ssh 192.168.13.9 tunnel/test";
    int x = execvp(prog, arg);
    if (x < 0) {
        cout << strerror(errno) << endl;
    }
    sleep(100);
}
