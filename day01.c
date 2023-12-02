#include "common.h"

const char* _ = "0123456789";
const char* _c[10] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

void getFromDigit(char *s, int *idx1, int *idx2, int *n1, int *n2) {
    int idx = -1;
    *idx1 = INT_MAX;
    *idx2 = -1;
    while ((idx += strcspn(&s[++idx], _)) < strlen(s)) {
        *idx1 = (idx < *idx1)? idx : *idx1;
        *idx2 = (idx > *idx2)? idx : *idx2;
    }
    *n1 = ctoi(s[*idx1]);
    *n2 = ctoi(s[*idx2]);
}

void getFromStr(char *s, int *idx1, int *idx2, int *n1, int *n2) {
    *idx1 = INT_MAX;
    *idx2 = -1;
    for (int i = 0; i < 10; ++i) {
        char *p = s-1;

        while(p = strstr(++p, _c[i])) {
            if ((int)(p-s) < *idx1) {
                *idx1 = p - s;
                *n1 = i;
            }
            if ((int)(p-s) > *idx2) {
                *idx2 = p - s;
                *n2 = i;
            }
        }
    }
}

void getFromAny(char *s, int *n1, int *n2) {
    int idxD1, idxD2, idxS1, idxS2;
    int nD1, nD2, nS1, nS2;
    getFromDigit(s, &idxD1, &idxD2, &nD1, &nD2);
    getFromStr(s, &idxS1, &idxS2, &nS1, &nS2);
    PRINTF1("Digit side : idx(%d,%d) val(%d,%d)\n", idxD1, idxD2, nD1, nD2);
    PRINTF1("Str   side : idx(%d,%d) val(%d,%d)\n", idxS1, idxS2, nS1, nS2);
    *n1 = (idxD1 < idxS1)? nD1 : nS1;
    *n2 = (idxD2 > idxS2)? nD2 : nS2;
}


void day(const char *part) {
    int isp1 = !strcmp(part, "p1");
    char s[100];
    int idx1, idx2, n1, n2, total = 0;
    while(fgets(s, 100, stdin)) {
        strtok(s, "\n\r");
        if (isp1)
            getFromDigit(s, &idx1, &idx2, &n1, &n2);
        else
            getFromAny(s, &n1, &n2);
        PRINTF2("%s %d %d\n\n", s, n1, n2);
        total += n1 * 10 + n2;
    }
    printf("%d\n", total);
}