#include <unistd.h>
#include <iostream>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

using namespace std;

int main() {
    pid_t pid = fork();
    if (pid == 0) {
        char *cmd[] = {"ssh", "192.168.13.11", "-f", "tunnel/tunClient",  (char*)0};
        if ( execvp("ssh", cmd) < 0) {
            cout << endl << strerror(errno) << endl;
        }
    } else if (pid > 0){
        int status;
        cout << "parent" << endl;
        waitpid(pid, &status, 0);
    } else {
        cerr << "failed" << endl;
    }
}
