#define _LARGE_FILE_API
#include <ctype.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "split_in.c"

#define blockSize 4096
struct Word {
    unsigned long usages;
    char *word;
};

int main(int argc, char *argv[]) {
    char *uselessEndPtr;
    long fileStart = strtoll(argv[1], &uselessEndPtr, 10);
    long fileEnd = strtoll(argv[2], &uselessEndPtr, 10);
    char *outputFile = argv[3];
    int fd = open(argv[4], O_RDONLY);
    long id = strtoll(argv[5], &uselessEndPtr, 10);
    if (fd < 0) return 1;

    char **allowed_words;
    split_in(&allowed_words, "data/dictionary.txt", '\n');

    unsigned long offset = fileStart;
    char *buf = (char *)malloc(blockSize * sizeof(char));
    unsigned long numWords = 0;
    struct Word *words = (struct Word *)malloc(numWords * sizeof(struct Word));

    unsigned int currentBlockSize;
    bool loop = true;
    while (loop) {
        currentBlockSize = pread(fd, buf, blockSize, offset);
        if (currentBlockSize < blockSize) {
            // *(buf + currentBlockSize) = '\0';
            loop = false;
        }
        if (offset + currentBlockSize >= fileEnd) {
            currentBlockSize = fileEnd - offset;
            loop = false;
        }

        char *lastC = strrchr(buf, ' ');
        if (lastC != NULL) {  // the last word in file fails
            // *lastC = '\0';    // removing potential only a part a of word
            currentBlockSize = lastC - buf;
        }

        if (currentBlockSize == 0) break;

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
    }

    FILE *output = fopen(outputFile, "w");
    fclose(output);
    output = fopen(outputFile, "a");

    for (unsigned long i = 0; i < numWords; i++) {
        // printf("%d %s\n", (words + i)->usages, (words + i)->word);
        fprintf(output, "%ld %s\n", (words + i)->usages, (words + i)->word);
    }
    fclose(output);

    printf("numWords: %ld\n", numWords);
    fprintf(output, "numWords: %ld\n", numWords);
}
