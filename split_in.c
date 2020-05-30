#include <ctype.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BLOCKSIZE 10

char* split_in(char* file_name, char seperator, size_t offset) {
    int fd = open(file_name, O_RDONLY);
    if (fd < 0) return NULL;
    size_t length = 0;
    size_t buf_size = 0;
    ssize_t bytes_read;
    size_t words_read = 0;
    bool reached_end = false;
    char** strings = malloc(0 * sizeof(char*));
    char* read_buffer = malloc(BLOCKSIZE * sizeof(char));

    while (1) {
        bytes_read = pread(fd, read_buffer, BLOCKSIZE, offset);
        if (bytes_read < BLOCKSIZE) reached_end = true;

        ssize_t last_seperator_index = -1;
        if (!reached_end) {
            for (ssize_t i = bytes_read - 1; i >= 0; i--) {
                if (read_buffer[i] == seperator) {
                    last_seperator_index = i;
                    break;
                }
            }
            if (last_seperator_index == -1) return NULL;
        } else {
            last_seperator_index = bytes_read - 1;
        }

        size_t last_index = read_buffer[0] == seperator ? 1 : 0;
        for (size_t i = last_index + 1; i <= last_seperator_index; i++) {
            if (read_buffer[i] == seperator) {
                strings = realloc(strings, (words_read + 1) * sizeof(char*));
                size_t string_size = i - last_index;

                strings[words_read] = malloc((string_size + 1) * sizeof(char));
                memcpy(strings[words_read], read_buffer + last_index, string_size);
                strings[words_read][string_size] = '\0';
                words_read++;
                last_index = i + 1;
            }
        }
        offset += last_seperator_index;
        if (reached_end) break;
    }
    for (size_t i = 0; i < words_read; i++) {
        printf("<%s>\n", strings[i]);
    }
}
int main() {
    split_in("data/test", ' ', 0);

    // puts(strings);
}