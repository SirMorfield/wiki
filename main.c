#define _LARGE_FILE_API
#include <ctype.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct Word {
    unsigned long long usages;
    char *word;
};

void main(int argc, char *argv[]) {
    char *uselessEndPtr;
    long long fileStart = strtoll(argv[1], &uselessEndPtr, 10);
    long long fileEnd = strtoll(argv[2], &uselessEndPtr, 10);
    char *outputFile = argv[3];
    int fd = open(argv[4], O_RDONLY);
    long long id = strtoll(argv[5], &uselessEndPtr, 10);

    // unsigned long long fileStart = 0;
    // unsigned long long fileEnd = 1000;
    // char *outputFile = "output.txt";
    // int fd = open("smallWiki.txt", O_RDONLY);

    if (fd < 0) {
        printf("Can't open file");
        return;
    }

    unsigned long long offset = fileStart;
    unsigned int bufSize = 4096;
    char *buf = (char *)malloc(bufSize * sizeof(char));
    unsigned long long numWords = 0;
    struct Word *words = (struct Word *)malloc(0 * sizeof(struct Word));

    unsigned long long loops = 0;

    unsigned int currentBlockSize;
    bool loop = true;
    while (loop) {
        currentBlockSize = pread(fd, buf, bufSize, offset);
        if (currentBlockSize < bufSize) {
            *(buf + currentBlockSize) = '\0';
            loop = false;
        }
        if (offset + currentBlockSize >= fileEnd) {
            currentBlockSize = fileEnd - offset;
            loop = false;
        }

        // printf("\n(%d) before <%s>\n", currentBlockSize, buf);
        // for (int i = 0; i < currentBlockSize; i++) {
        // if (*(buf + i) == '\n') *(buf + i) = ' ';
        // *(buf + i) = tolower(*(buf + i));
        // }

        char *lastSpace = strrchr(buf, ' ');
        if (lastSpace != NULL) {  // the last word in file fails
            *lastSpace = '\0';    // removing potential only a part a of word
            currentBlockSize = lastSpace - buf;
        }

        // printf("(%d) after <%s>\n", currentBlockSize, buf);
        if (currentBlockSize == 0) break;  // or < 2 ?

        char *begin = *buf == ' ' ? buf + 1 : buf;
        bool reachedEnd = false;
        while (!reachedEnd) {
            char *end = strchr(begin + 1, ' ');
            if (end == NULL) {
                end = buf + currentBlockSize;
                reachedEnd = true;
            }
            unsigned int size = end - begin;
            char *word = malloc(size + 1);
            memcpy(word, begin, size);
            *(word + size) = '\0';

            // printf("<%s> ", word);
            int wordIndex = -1;
            for (int i = 0; i < numWords; i++) {
                if (strcmp((words + i)->word, word) == 0) {
                    wordIndex = i;
                    break;
                }
            }

            if (wordIndex == -1) {
                wordIndex = numWords++;

                words = realloc(words, numWords * sizeof(struct Word));
                (words + wordIndex)->word = malloc(size + 1);
                strcpy((words + wordIndex)->word, word);

                (words + wordIndex)->usages = 1;
            } else {
                (words + wordIndex)->usages++;
            }

            begin = end + 1;
        }
        offset += currentBlockSize;

        if (++loops % 200 == 0) {
            long long startIndex = offset - fileStart;
            long long endIndex = fileEnd - fileStart;
            long double percent = (long double)startIndex / (long double)endIndex;
            FILE *logFile = fopen("log.txt", "a");

            printf("%lld: %Lf%% %lld/%lld, numWords: %lld\n", id, percent, startIndex, endIndex, numWords);
            fprintf(logFile, "%lld: %Lf%% %lld/%lld, numWords: %lld\n", id, percent, startIndex, endIndex, numWords);

            fclose(logFile);
        }
    }

    FILE *output = fopen(outputFile, "w");
    fclose(output);
    output = fopen(outputFile, "a");

    for (unsigned long long i = 0; i < numWords; i++) {
        // printf("%d %s\n", (words + i)->usages, (words + i)->word);
        fprintf(output, "%lld %s\n", (words + i)->usages, (words + i)->word);
    }
    fclose(output);

    printf("numWords: %lld\n", numWords);
    fprintf(output, "numWords: %lld\n", numWords);
}