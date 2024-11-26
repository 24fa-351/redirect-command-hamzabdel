#include <stdio.h>

void parse_cmd(char *cmd, char **argv)
{
    char *token = strtok(cmd, " ");
    int i = 0;
}

void redirect(int fd, int new_fd)
{
    if (fd != new_fd)
    {
        dup2(fd, new_fd);
        close(fd);
    }
}

int main(int argc, int argv[])
{
    char *inp = argv[1];
    char *cmd = argv[2];
    char *out = argv[3];

    parse_cmd(cmd);

    if (pid == 0)
    {
        if (strcmp(inp, "-") != 0)
        {
            int inp_fd = open(inp, O_RDONLY);

            redirect(inp_fd, STDIN_FILENO);
        }
    }

    return 0;
}