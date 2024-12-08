#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/wait.h>

#define MAX_ARGS 256

void parse_cmd(char *cmd, char **argv_cmd)
{
    char *token;
    int i = 0;
    token = strtok(cmd, " ");

    while (token != NULL && i < MAX_ARGS - 1)
    {
        argv_cmd[i++] = token;
        token = strtok(NULL, " ");
    }

    argv_cmd[i] = NULL;
}

void redirect_descriptor(int fd, int new_fd)
{
    if (fd != new_fd)
    {
        dup2(fd, new_fd);
        close(fd);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: redir <inp> <cmd> <out>\n");

        exit(EXIT_FAILURE);
    }

    char *inp = argv[1], *cmd = argv[2], *out = argv[3];
    char *argv_cmd[MAX_ARGS];
    parse_cmd(cmd, argv_cmd);

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");

        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {
        if (strcmp(inp, "-") != 0)
        {
            int inp_fd = open(inp, O_RDONLY);

            if (inp_fd < 0)
            {
                perror("Error opening input file");

                exit(EXIT_FAILURE);
            }

            redirect_descriptor(inp_fd, STDIN_FILENO);
        }

        if (strcmp(out, "-") != 0) // if output is not -
        {
            int out_fd = open(out, O_WRONLY | O_CREAT | O_TRUNC, 0644);

            if (out_fd < 0)
            {
                perror("Error opening file for output");

                exit(EXIT_FAILURE);
            }

            redirect_descriptor(out_fd, STDOUT_FILENO);
        }

        execvp(argv_cmd[0], argv_cmd); // checks path
        perror("execvp");

        exit(EXIT_FAILURE);
    }

    else
    {
        waitpid(pid, NULL, 0);
    }

    return 0;
}
