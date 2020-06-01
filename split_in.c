#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BLOCKSIZE 6

long split_in(char*** strings, char* file_name, char seperator) {
    int fd = open(file_name, O_RDONLY);
    if (fd < 0) return -1;
    size_t offset = 0;
    size_t length = 0;
    ssize_t bytes_read;
    size_t words_read = 0;
    bool reached_end = false;
    *strings = malloc(0 * sizeof(char*));
    char* read_buffer = malloc(0 * sizeof(char));
    while (1) {
        ssize_t length = -1;
        size_t buf_size = 0;
        size_t total_bytes_read = 0;
        read_buffer = malloc(buf_size * sizeof(char));
        if (read_buffer == NULL) return -1;

        while (1) {
            buf_size += BLOCKSIZE;
            read_buffer = realloc(read_buffer, buf_size * sizeof(char));
            if (read_buffer == NULL) return -1;

            bytes_read = pread(fd, read_buffer + total_bytes_read, BLOCKSIZE, offset);
            total_bytes_read += bytes_read;

            if (bytes_read < BLOCKSIZE) {
                reached_end = true;
                length = total_bytes_read;
                break;
            }

            for (ssize_t i = total_bytes_read - 1; i >= (total_bytes_read - bytes_read); i--) {
                if (read_buffer[i] == seperator) {
                    length = i;
                    break;
                }
            }

            if (length != -1) {
                offset += (total_bytes_read - length) + 1;
                break;
            } else {
                offset += bytes_read;
            }
        }

        printf("<%s>", read_buffer);
        printf("%ld\n", length);
        ssize_t prev_seperator_index = 0;
        for (size_t i = 1; i < length; i++) {
            if (read_buffer[i] == seperator || i == (length - 1)) {
                *strings = realloc(*strings, (words_read + 1) * sizeof(char*));
                size_t string_size = (i - prev_seperator_index) + 1;

                (*strings)[words_read] = malloc((string_size + 1) * sizeof(char));
                memcpy((*strings)[words_read], read_buffer + prev_seperator_index, string_size);
                (*strings)[words_read][string_size] = '\0';
                words_read++;
                prev_seperator_index = i + 1;
            }
        }
        if (reached_end) break;
    }
    return words_read;
}
int main() {
    char** strings;
    long words_read = split_in(&strings, "data/test", ' ');
    if (words_read > 0) {
        printf("words_read %ld\n", words_read);
        for (size_t i = 0; i < words_read; i++) {
            printf("<%s>\n", strings[i]);
        }
    } else {
        printf("err");
    }
}