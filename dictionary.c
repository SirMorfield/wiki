#define _LARGE_FILE_API
#include <ctype.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "split_in.c"

#define BLOCKSIZE 4096
#define LONGEST_EN_WORD 21

int dictionary(int id, size_t file_start, size_t file_end, char *output_filename, char *filename, char *dictionary_filename) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) return 1;
    FILE *output_file = fopen(output_filename, "w");
    fclose(output_file);

    size_t file_size = file_end - file_start;

    char **words;
    ssize_t num_words = split_in(&words, dictionary_filename, '\n');
    unsigned long word_usages[num_words];
    unsigned int word_lengths[num_words];
    unsigned int last_i_with_length[LONGEST_EN_WORD + 1] = {0};
    for (size_t i = 0; i < num_words; i++) {
        word_usages[i] = 0;
        word_lengths[i] = strlen(words[i]);
        last_i_with_length[word_lengths[i]] = i;
    }
    // for (size_t i = 1; i < LONGEST_EN_WORD + 1; i++) printf("%ld %d\n", i, last_i_with_length[i]);

    size_t offset = file_start;
    char *buf = malloc(BLOCKSIZE * sizeof(char));
    bool reached_end = false;
    unsigned long total_words_read = 0;
    while (1) {
        ssize_t buf_length = -1;
        size_t bytes_read = pread(fd, buf, BLOCKSIZE, offset);
        if ((offset + bytes_read) > file_end) {
            buf_length = file_end - offset;
            reached_end = true;
        }
        if (bytes_read < BLOCKSIZE) {
            reached_end = true;
            buf_length = bytes_read;
        }
        if (!reached_end) {
            for (ssize_t i = BLOCKSIZE - 1; i >= 0; i--) {
                if (buf[i] == ' ') {
                    buf_length = i;
                    break;
                }
            }
            if (buf_length == -1) {
                printf("very bad\n");
                exit(1);  // should not happen
            }
        }
        offset += buf_length;

        size_t left = 0;
        for (size_t bufI = 1; bufI < buf_length; bufI++) {
            if (buf[bufI] == ' ' || bufI == (buf_length - 1)) {
                size_t word_length = bufI - left;
                if (word_length > LONGEST_EN_WORD || word_length == 0) continue;
                for (ssize_t wordI = last_i_with_length[word_length - 1] + 1; wordI <= last_i_with_length[word_length]; wordI++) {
                    //    printf("<");
                    //     for (size_t i = 0; i < word_length; i++) printf("%c", buf[left + i]);
                    //     printf(">");
                    bool shouldBreak = false;
                    bool same = true;
                    for (size_t charI = 0; charI < word_length; charI++) {
                        if (words[wordI][charI] > buf[left + charI]) {
                            same = false;
                            break;
                        }

                        if (words[wordI][charI] < buf[left + charI]) {
                            same = false;  // not actually needed;
                            shouldBreak = true;
                            break;
                        }
                    }
                    if (shouldBreak) break;
                    if (same) {
                        word_usages[wordI]++;
                        total_words_read++;
                        if (total_words_read % 1000000 == 0) {
                            output_file = fopen(output_filename, "a");
                            double p = ((double)offset - (double)file_start) / (double)file_size;
                            // printf("%d,%ld,%lf\n", id, total_words_read, p);
                            fprintf(output_file, "%d,%ld,%lf\n", id, total_words_read, p);
                            fclose(output_file);
                        }
                        break;
                    }
                }
                left = bufI + 1;  // +1 fo excluding seperator
            }
        }
        if (reached_end) break;
    }

    output_file = fopen(output_filename, "w");
    fclose(output_file);
    output_file = fopen(output_filename, "a");

    printf("num_words %ld", num_words);
    for (unsigned long i = 0; i < num_words; i++) {
        // printf("%ld %s\n", word_usages[i], words[i]);
        fprintf(output_file, "%ld,%s\n", word_usages[i], words[i]);
    }
    fclose(output_file);
    return 0;
}

// int main(int argc, char *argv[]) {
//     if (argc < 7) return -1;

//     char *end;
//     int id = strtoll(argv[1], &end, 1);
//     long file_start = strtoll(argv[2], &end, 10);
//     long file_end = strtoll(argv[3], &end, 10);
//     char *output_file = argv[4];
//     char *filename = argv[5];
//     char *dictionary_filename = argv[6];

//     int val = dictionary(id, file_start, file_end, output_file, filename, dictionary_filename);

//     // int val = dictionary(0, 0, 9999999999, "/tmp/a.txt", "data/test", "data/testd");

//     printf("exit %d\n", val);
//     return val;
// }
