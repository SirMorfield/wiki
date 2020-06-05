#include <pthread.h>
#include <stdio.h>

#include "dictionary.c"
#define CORES 8
#define WIKISIZE 13334931761

const long block_size = WIKISIZE / CORES;

void *get_result(void *index) {
    static int j = 0;
    int i = j++;
    size_t end = block_size * (i + 1);
    if (i == (CORES - 1)) end += WIKISIZE % block_size;
    char output[] = "outputs/0.txt";
    output[8] = (char)(i + 48);
    printf("%d %s\n", i, output);
    dictionary(i, block_size * i, end, output, "data/cleanWiki.txt", "data/en.txt");
}

int main() {
    pthread_t *tid = malloc(CORES * sizeof(pthread_t));

    for (int i = 0; i < CORES; i++) {
        pthread_create(&tid[i], NULL, get_result, NULL);
    }
    for (int i = 0; i < CORES; i++) {
        pthread_join(tid[i], NULL);
    }
}