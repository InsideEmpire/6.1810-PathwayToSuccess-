#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    if (argc != 2) {
        write(1, "error\n", 6);
        exit(1);
    }

    int sleep_ticks = atoi(argv[1]);

    sleep(sleep_ticks);

    exit(0);
}