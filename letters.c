#define _LARGE_FILE_API
#include <ctype.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define numLetters 128  // as of ASCII
#define bufSize 4096

void main(int argc, char *argv[]) {
    char *uselessEndPtr;
    long long id = strtoll(argv[1], &uselessEndPtr, 10);
    long long fileStart = strtoll(argv[2], &uselessEndPtr, 10);
    long long fileEnd = strtoll(argv[3], &uselessEndPtr, 10);
    char *outputFile = argv[4];
    int fd = open(argv[5], O_RDONLY);

    if (fd < 0) {
        printf("Can't open file");
        return;
    }

    unsigned long long offset = fileStart;
    char *buf = (char *)malloc(buf_size * sizeof(char));

    unsigned long long letters[numLetters] = {};
    for (unsigned int i = 0; i < numLetters; i++) {
        letters[i] = 0;
    }

    unsigned int currentBlockSize;
    bool loop = true;
    while (loop) {
        currentBlockSize = pread(fd, buf, buf_size, offset);
        if (currentBlockSize < buf_size) {
            *(buf + currentBlockSize) = '\0';
            loop = false;
        }
        if (offset + currentBlockSize >= fileEnd) {
            currentBlockSize = fileEnd - offset;
            loop = false;
        }
        if (currentBlockSize == 0) break;

        for (unsigned int i = 0; i < currentBlockSize; i++) {
            letters[(int)*(buf + i)]++;
        }
        offset += currentBlockSize;
    }

    FILE *output = fopen(outputFile, "w");
    fclose(output);
    output = fopen(outputFile, "a");

    for (int i = 32; i <= 127; i++) {
        printf("%c,%lld\n", (char)i, letters[i]);
        fprintf(output, "%c,%lld\n", (char)i, letters[i]);
    }
    fclose(output);

    unsigned long long c = 0;
    for (int i = 0; i < numLetters; i++) {
        c += letters[i];
    }
    printf("%lld", c);
}
