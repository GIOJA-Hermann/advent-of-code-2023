#include "common.h"


void day(const char *part) {
    char conv[128];
    conv['r'] = 0, conv['g'] = 1, conv['b'] = 2;
    int isp1 = !strcmp(part, "p1");
    char s[1000], c, *p;
    int r[3] = {12, 13, 14}, v[3], vs, bad = 0, run = 0, total = 0;
    while(run++, bad=0, memset(v, 0, 3*sizeof(int)), fgets(s, 1000, stdin)) {
        strtok(p = s, "\n\r");
        while((!bad || !isp1) && ( p = strpbrk(p+1, ":,;"))) {
            sscanf(p+1, " %d %c", &vs, &c);
            bad = vs > r[conv[c]];
            v[conv[c]] = (vs > v[conv[c]]) ? vs : v[conv[c]];
        }
        total += (isp1)? (bad?0:run) : (v[conv['r']] * v[conv['g']] * v[conv['b']]);
    }
    printf("%d\n", total);
}