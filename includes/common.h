#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <limits.h>
#include <ctype.h>
#include "debug.h"

extern const char *digit;
const size_t _bufsize;

int ctoi(char c);
char * rl(char *s, uint32_t bs);