#include "common.h"

const char *digit = "0123456789";
const size_t _bufsize = 1000;

int ctoi(char c) {
    return c - '0';
}

char * rl(char *s, uint32_t bs) {

    if (!fgets(s, bs, stdin)) {
        free (s);
        s = NULL;
    } else {
        strtok(s, "\n\r");
    }
    return s;
}

