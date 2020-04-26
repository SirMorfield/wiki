#define _LARGE_FILE_API
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct Word {
    int usages;
    char *word;
};

void main() {
    // int fd = open("temp.txt", O_RDONLY);
    int fd = open("cleanWiki.txt", O_RDONLY);
    if (fd < 0) {
        printf("Can't open file");
        return;
    }
    unsigned long long offset = 0;
    unsigned int bytesRead;
    unsigned int bufSize = 4096;
    char *buf = (char *)malloc(bufSize * sizeof(char));
    unsigned long long numWords = 0;
    struct Word *words = (struct Word *)malloc(0 * sizeof(struct Word));

    unsigned long long loops = 0;
    while ((bytesRead = pread(fd, buf, bufSize, offset)) > 0) {
        for (int i = 0; i < bytesRead; i++) {
            if (*(buf + i) == '\n') *(buf + i) = ' ';
        }
        // printf("\nbefore <%s>\n", buf);
        char *lastSpace = strrchr(buf, ' ');  // err?
        *lastSpace = '\0';                    // removing potential only a part a of word
        // printf("after <%s>\n", buf);
        if (strlen(buf) == 0) break;

        char *begin = *buf == ' ' ? buf + 1 : buf;
        bool reachedEnd = false;
        while (!reachedEnd) {
            char *end = strchr(begin + 1, ' ');
            if (end == NULL) {
                end = buf + (bytesRead - 1);
                reachedEnd = true;
            }
            unsigned int size = end - begin;
            char *word = malloc(size + 1);
            memcpy(word, begin, size);

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
                (words + wordIndex)->word = malloc(strlen(word) + 1);
                strcpy((words + wordIndex)->word, word);

                (words + wordIndex)->usages = 1;
            } else {
                (words + wordIndex)->usages++;
            }
            //
            begin = end + 1;
        }
        offset += strlen(buf);

        if (++loops % 200 == 0) {
            printf("offset: %lld, numWords: %lld\n", offset, numWords);
        }
    }

    FILE *output = fopen("output.txt", "a");
    for (unsigned long long i = 0; i < numWords; i++) {
        // printf("{\"usages\":%d, \"word\":\"%s\"}\n", (words + i)->usages, (words + i)->word);
        fprintf(output, "%d %s\n", (words + i)->usages, (words + i)->word);
    }
    fclose(output);

    printf("numWords: %lld\n", numWords);
    fprintf(output, "numWords: %lld\n", numWords);
}