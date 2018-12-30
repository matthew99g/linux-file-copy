// Linux FD Copy
// main.c

#include "errlib.h"
#include "getnum.h"

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
        outputError(TRUE, TRUE, "opening file %s", argv[1]);

    openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                S_IROTH | S_IWOTH;
    outputFd = open(argv[2], openFlags, filePerms);
    if(outputFd == -1)
        errnoExit("opening file %s\n", argv[2]);

    /* Transfer data until we encounter end of input or error */

    while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0)
        if(write(outputFd, buf, numRead) != numRead)
            fatal("couldn't write whole buffer");
    if(numRead == -1)
        outputError(TRUE, TRUE, "read");

    if(close(inputFd) == -1)
        outputError(TRUE, TRUE, "close input");
    if(close(outputFd) == -1)
        outputError(TRUE, TRUE, "close output");

    return EXIT_SUCCESS;
}