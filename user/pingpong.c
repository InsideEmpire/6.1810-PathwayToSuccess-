#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main()
{
    int fds1[2], fds2[2];
    int pid;
    int recv_pid;

    pipe(fds1); // child -> parent
    pipe(fds2); // parent -> child

    pid = fork();

    if (pid == 0) {
        int send_pid = getpid();

        write(fds1[1], &send_pid, sizeof(send_pid));

        if (read(fds2[0], &recv_pid, sizeof(recv_pid)) >= 0) {
            printf("%d: received ping\n", send_pid);
        }

        exit(0);
    } else {
        int send_pid = getpid();

        write(fds2[1], &send_pid, sizeof(send_pid));

        wait((int *)0);

        if (read(fds1[0], &recv_pid, sizeof(recv_pid)) >= 0) {
            printf("%d: received pong\n", send_pid);
        }
    }

    exit(0);
}
