#include <sys/stat.h>
#include <sys/types.h>
#include <stdarg.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_ENAME 132

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

/* DECIDED NOT TO USE THIS FUNCTION */

/*static void
outputError(char useErr, int err, char flushStdout,
        const char *format, va_list ap)
{
#define BUF_SIZE 500
    char buf[BUF_SIZE], usrMsg[BUF_SIZE], errText[BUF_SIZE];

    vsnprintf(usrMsg, BUF_SIZE, format, ap);

    snprintf(errText, BUF_SIZE, ":");

    snprintf(buf, BUF_SIZE, "ERROR%s %s\n", errText, usrMsg);

    if(flushStdout)
        fflush(stdout);

    fputs(buf, stderr);
    fflush(stderr);
}*/

void
usageErr(const char *format, ...) {
    va_list argList;

    fflush(stdout);

    fprintf(stderr, "Usage: ");
    va_start(argList, format);
    vfprintf(stderr, format, argList);
    va_end(argList);

    fflush(stderr);
    exit(EXIT_FAILURE);
}

void
errExit(const char *format, ...)
{
    va_list argList;

    fflush(stdout);

    fprintf(stderr, "Error: ");
    va_start(argList, format);
    vfprintf(stderr, format, argList);
    va_end(argList);

    fflush(stderr);
    exit(EXIT_FAILURE);
}

void
fatal(const char *fmt)
{
    fflush(stdout);

    fprintf(stderr, "%s\n", fmt);
    
    fflush(stderr);
    exit(EXIT_FAILURE);
}

int
main(const int argc, const char *argv[])
{
    int inputFd, outputFd, openFlags;
    mode_t filePerms;
    ssize_t numRead;
    char buf[BUF_SIZE];

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s old-file new-file\n", argv[0]);

    /* Open input and output files */

    inputFd = open(argv[1], O_RDONLY);
    if (inputFd == -1)
        errExit("opening file %s", argv[1]);

    openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                S_IROTH | S_IWOTH;
    outputFd = open(argv[2], openFlags, filePerms);
    if(outputFd == -1)
        errExit("opening file %s\n", argv[2]);

    /* Transfer data until we encounter end of input or error */

    while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0)
        if(write(outputFd, buf, numRead) != numRead)
            fatal("couldn't write whole buffer");
    if(numRead == -1)
        errExit("read\n");

    if(close(inputFd) == -1)
        errExit("close input\n");
    if(close(outputFd) == -1)
        errExit("close output\n");

        exit(EXIT_SUCCESS);

    return EXIT_SUCCESS;
}
