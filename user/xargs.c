#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

/* This func is used for test
void print_argv(int argc, char *argv[]) {
    for (int i = 0; i < argc - 1; i++) {
        printf("%s ", argv[i]);
    }
    printf("%s\n", argv[argc - 1]);
}
*/

int
append(int base_argc, char *new_argv[])
{
    char append_argv[2048];
    int index = 0;
    int offset = base_argc;

    while (read(0, append_argv + index, 1))
    {
        if (append_argv[index] == ' ') {
            append_argv[index] = '\0';
            new_argv[offset] = malloc(strlen(append_argv) + 1);
            strcpy(new_argv[offset++], append_argv);
            index = 0;
            continue;
        }

        if (append_argv[index] == '\n') {
            append_argv[index] = '\0';
            new_argv[offset] = malloc(strlen(append_argv) + 1);
            strcpy(new_argv[offset++], append_argv);
            new_argv[offset] = 0; // NULL
            break;
        }
        index++;
    }

    return offset;
}

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        write(1, "error\n", 6);
        exit(1);
    }

    char *new_argv[MAXARG];
    char *command = argv[1];
    int base_argc = argc - 1;

    for (int i = 1; i < argc; i++) {
        new_argv[i - 1] = malloc(strlen(argv[i]) + 1);
        strcpy(new_argv[i - 1], argv[i]);
    }

    while (1) {
        int final_argc = append(base_argc, new_argv);
        if (final_argc == base_argc) {
            break;
        }

        if (fork() == 0) {
            exec(command, new_argv);
            fprintf(2, "exec failed\n");
            exit(1);
        } else {
            wait(0);
        }

        for (int i = base_argc; i < final_argc; i++) {
            free(new_argv[i]);
        }
    }

    for (int i = 0; i < base_argc; i++) {
        free(new_argv[i]);
    }

    exit(0);
}
