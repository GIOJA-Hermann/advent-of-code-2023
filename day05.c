#include "common.h"

void day(const char *part) {
    int isp1 = !strcmp(part, "p1");
    char *s = malloc(_bufsize), *p;

    uint64_t val[20];
    uint64_t conv[3];
    uint64_t total;
    uint8_t proc[20] = {0};
    size_t idxNum = 0, idxSpace = 0, loop = 0;

    rl(s, _bufsize);
    idxSpace = strcspn(p = s, " ");
    idxNum = strspn(p + idxSpace + 1, digit);
    do {
        sscanf(p+= idxSpace + 1, "%ld", &val[loop++]);
    } while((idxSpace = strcspn(p, " ")) && (idxNum = strspn(p + idxSpace + 1, digit)));

    while(rl(s,_bufsize)) {
        if (strlen(s) == 0) {
            rl(s,_bufsize);
            for (size_t i = 0; i < 20; i++) {
                proc[i] = 0;
            }
            continue;
        }
        sscanf(s, "%ld %ld %ld\n", &conv[0], &conv[1], &conv[2]);
        for (size_t i = 0; i < loop; i++) {
            if (proc[i] != 0)
                continue;
            if (val[i] >= conv[1] && val[i] < conv[1] + conv[2]) {
                val[i] = conv[0] + val[i] - conv[1];
                proc[i] = 1;
            }
        }
    }
    total = -1;
    for (size_t i = 0; i < loop; i++)
        total = val[i] < total ? val[i] : total;
    printf("%ld\n", total);
}