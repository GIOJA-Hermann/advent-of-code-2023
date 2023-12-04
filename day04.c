#include "common.h"

const size_t nref = 10;
const size_t nval = 25;
const size_t sidxR = 10;
const size_t sidxV = 42;
const size_t lines = 199;

uint32_t weight[199] = {[0 ... 198] = 1};

void day(const char *part) {
    int isp1 = !strcmp(part, "p1");
    char *s = malloc(_bufsize);
    char space[2];
    int shift;
    uint64_t total = 0;
    size_t line = -1;

    while(line++, rl(s,_bufsize)) {
        shift = -1;
        for (size_t ridx = sidxR; ridx < (sidxR + 3 * nref); ridx += 3) {
            for (size_t vidx = sidxV; vidx < (sidxV + 3 * nval); vidx += 3) {
                if (s[ridx] == s[vidx] && s[ridx + 1] == s[vidx + 1]) {
                    shift++;
                }
            }
        }
        if (shift != -1) {
            uint64_t score = 1ULL << shift;
            for (size_t l = line + 1; l < line + 1 + (shift + 1) && l < lines; l++) {
                weight[l] += weight[line];
            }
            total += ((isp1) ? score : weight[line]);
        } else {
            total += ((isp1) ? 0 : weight[line]);
        }
    }
    printf("%ld\n", total);
}