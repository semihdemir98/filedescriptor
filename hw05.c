/* Semih Demir
 * 18120205005
 * 14.05.2020
 * hw05.c
*/
#define _GNU_SOURCE
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#define N 4

int main()
{

    int pipe_fds[N][2];

    for (int i = 0; i < N; i++)
    {
        pipe2(pipe_fds[i], O_CLOEXEC);
    }

    pid_t pid[4];
    pid[0] = fork();

    if (pid[0] == 0)
    {
        dup2(pipe_fds[0][1], 1);
        execlp("ls", "ls", "-1", NULL);//list entire elements
    }
    else if (pid[0] > 0)
    {
        pid[1] = fork();
        if (pid[1] == 0)
        {
            dup2(pipe_fds[0][0], 0);
            dup2(pipe_fds[1][1], 1);
            execlp("cut", "cut", "-f2", "-d.", NULL);//use DELIM instead of TAB for field delimiter
        }
        else if (pid[1] > 0)
        {
            pid[2] = fork();
            if (pid[2] == 0)
            {
                dup2(pipe_fds[1][0], 0);
                dup2(pipe_fds[2][1], 1);
                execlp("sort", "sort", "-d", NULL);// consider only blanks and alphanumeric characters
            }
            else if (pid[2] > 0)
            {
                pid[3] = fork();
                if (pid[3] == 0)
                {
                    dup2(pipe_fds[2][0], 0);
                    dup2(pipe_fds[3][1], 1);
                    execlp("uniq", "uniq", NULL);// Filter adjacent matching lines
                }
                else if (pid[3] > 0)
                {
                    pid[4] = fork();
                    if (pid[4] == 0)
                    {
                        dup2(pipe_fds[3][0], 0);
                        /* dup2(pipe_fds[4][1],1); */
                        execlp("tee", "tee", "file", NULL);//Copy standard input to each FILE, and also to standard output.
                        exit(-1);
                        return 0;
                    }
                    if (pid[4] < 0){perror("fork()- 5 could not work");}
                }
                else{perror("fork()- 4 could not work");}
            }
            else{perror("fork()- 3 could not work");}
        }
        else{perror("fork()- 2 could not work");}
    }
    else{perror("fork()- 1 could not work");}
}
