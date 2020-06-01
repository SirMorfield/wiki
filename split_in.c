#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ssize_t split_in(char*** items, char* file_name, char seperator) {
    FILE* infile = fopen(file_name, "r");
    if (infile == NULL) return -1;

    fseek(infile, 0L, SEEK_END);
    long numbytes = ftell(infile);
    fseek(infile, 0L, SEEK_SET);  // reset the file position indicator to the beginning of the file
    char* buffer = malloc(numbytes * sizeof(char));
    fread(buffer, sizeof(char), numbytes, infile);
    fclose(infile);

    size_t items_read = 0;
    *items = malloc(0 * sizeof(char*));
    long right_index = numbytes;
    for (ssize_t left_index = right_index - 1; left_index >= 0; left_index--) {
        if (buffer[left_index - 1] == seperator || left_index == 0) {
            *items = realloc(*items, (items_read + 1) * sizeof(char*));
            size_t string_size = right_index - left_index;
            (*items)[items_read] = malloc((string_size + 1) * sizeof(char));
            memcpy((*items)[items_read], buffer + left_index, string_size);
            (*items)[items_read][string_size] = '\0';
            items_read++;
            right_index = left_index - 1;

            buffer = realloc(buffer, (numbytes - string_size) * sizeof(char));
        }
    }

    // TODO: reversing

    free(buffer);
    return items_read;
}
