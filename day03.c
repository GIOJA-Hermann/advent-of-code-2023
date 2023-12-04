#include "common.h"
#include "mempool.h"

const int nrow = 140;
const int ncol = 140;
#define _maxSymb 8

typedef struct s_number t_number;
struct s_number {
    size_t idxBef;
    size_t idxAft;
    uint32_t val;
    uint32_t nsymb;
    char symb[_maxSymb]; // Adjacent symbols
    uint32_t ngear;
    size_t gearIdx;
    t_number *next;
};

void verifySymbol(char *wholedata, t_number *cur, size_t checkIdx) {
    if (wholedata[checkIdx] != '.' && !isdigit(wholedata[checkIdx])) {
        if (cur->nsymb == _maxSymb) {
            fprintf(stderr, "Symb space is too low\n");
            exit;
        }
        cur->symb[cur->nsymb++] = wholedata[checkIdx];
    }
}

void day(const char *part) {
    int isp1 = !strcmp(part, "p1");
    char *s = malloc(1000);
    t_mempool mp;
    int line = 1;
    initPool(&mp, 100, sizeof(t_number));
    // add a dot border to manage corner cases
    char *wholedata = calloc((nrow + 2) * (ncol + 2) + 1, 1);
    for (int i = 0; i < ncol + 2; i++)
        wholedata[i] = wholedata[(ncol + 2) * (nrow + 1) + i] = '.';
    for (int i = 0; i < nrow + 2; i++)
        wholedata[(ncol + 2) * i] = wholedata[(ncol + 2) * i + (nrow + 1)] = '.';
    // data as a single string

    while(rl(s,1000)) {
        memcpy(&wholedata[(ncol + 2) * line++ + 1], s, ncol);
    }

    size_t startIdx, endIdx = 0, checkIdx;
    char *p = wholedata;
    uint32_t numNumbers = 0;
    uint64_t total = 0, gearCount, gearRatio;
    t_number *cur = NULL, *first;
    // Read numbers
    while (startIdx = endIdx + strcspn(&wholedata[endIdx], digit), endIdx = startIdx + strspn(&wholedata[startIdx], digit), startIdx != endIdx) {
        t_number *new = (t_number *)getElem(&mp);
        cur = cur ? (cur->next = new) : (first = new);
        *cur = (t_number) {.idxBef = startIdx, .idxAft = endIdx, .val = 0, .nsymb = 0, .symb = {'\0'}, .next = NULL};
        sscanf(&wholedata[cur->idxBef], "%d", &cur->val);
        PRINTF1("%ld %ld %d\n", cur->idxBef, cur->idxAft, cur->val);
    }

    if (isp1) {
        // Check neighborhood
        cur = first;
        while (cur) {
            verifySymbol(wholedata, cur, cur->idxBef - 1);
            verifySymbol(wholedata, cur, cur->idxAft);
            for (size_t cIdx = cur->idxBef - 1; cIdx < cur->idxAft + 1; ++cIdx) {
                verifySymbol(wholedata, cur, cIdx - (ncol +2));
                verifySymbol(wholedata, cur, cIdx + (ncol +2));
                checkIdx = cIdx - (ncol +2);
            }
            if (cur->nsymb) {
                total += cur->val;
            }
            cur = cur->next;
        }
    } else {
        // Not optimal to check all numbers for each potential gear, but efficient enough here.
        // To make it more efficient : 
        //   Convert number "list" to "ordered-table" to allow dichotomic access (log(n))
        //   For each gear, find a number which is in prev, current or next line
        //   If no number -> drop potential gear
        //   Otherwise, check prev&next numbers in the prev,current,next line if they are neighbors
        // For fun, could also build symb objects, and map symb <-> objects with same method, which
        // would then provide the full mapping info.
        for (size_t i = (ncol + 3); i < (nrow + 1) * (ncol + 2); ++i) {
            gearRatio = 1;
            gearCount = 0;
            if (wholedata[i] == '*') {
                cur = first;
                while (cur) {
                    if (cur->idxBef <= (i - (ncol + 2) + 1) && cur->idxAft >= (i - (ncol + 2)) || // above
                        cur->idxBef <= (i + (ncol + 2) + 1) && cur->idxAft >= (i + (ncol + 2)) || // below
                        cur->idxBef == (i + 1) || cur->idxAft == i) // left or right
                    {
                        gearCount++;
                        gearRatio *= cur->val;

                    }
                    cur = cur->next;
                }
                if (gearCount == 2)
                    total += gearRatio;
            }
        }
    }
    throwPool(&mp);
    printf("%ld\n", total);
}