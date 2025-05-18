#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
sieve(int left_fd[]) 
{
    close(left_fd[1]);
    int prime;

    if (read(left_fd[0], &prime, sizeof(prime)) != sizeof(prime)) {
        close(left_fd[0]);
        exit(0);
    }

    printf("prime %d\n", prime);

    int right_fd[2];
    pipe(right_fd);

    if (fork() == 0) {
        close(left_fd[0]);
        sieve(right_fd);
        exit(0);
    } else {
        close(right_fd[0]);
        int num;
        while (read(left_fd[0], &num, sizeof(num)) == sizeof(num)) {
            if (num % prime != 0) {
                write(right_fd[1], &num, sizeof(num));
            }
        }
        close(left_fd[0]);
        close(right_fd[1]);
        wait(0);
    }

    return 0;
}

int
main()
{
    int fd[2];

    pipe(fd);

    for (int i = 2; i <= 35; i++) {
        if (write(fd[1], &i, sizeof(i)) != sizeof(i)) {
            write(1, "error\n", 6);
            exit(1);
        }
    }

    sieve(fd);

    exit(0);
}
