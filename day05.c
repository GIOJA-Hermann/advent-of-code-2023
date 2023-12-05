#include "common.h"

typedef struct s_range t_range;
struct s_range {
    uint64_t from;
    uint64_t size;
    t_range *next;
};

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

    t_range *rangesFrom = NULL, *rangesTo = NULL, *cur, *elem, *newEl, *prev, *next;
    for (size_t i = 0; i < loop; i+=2) {
        cur = malloc(sizeof(t_range));
        cur->from = val[i];
        cur->size = val[i+1];
        cur->next = rangesFrom;
        rangesFrom = cur;
    }

    while(rl(s,_bufsize)) {
        if (strlen(s) == 0) {
            rl(s,_bufsize);
            for (size_t i = 0; i < 20; i++) {
                proc[i] = 0;
            }
            while(rangesFrom != NULL) {
                elem = rangesFrom;
                rangesFrom = elem->next;
                elem->next = rangesTo;
                rangesTo = elem;
                PRINTF1("  no-match : %ld->%ld\n", elem->from, elem->from + elem->size -1);
            }
            rangesFrom = rangesTo;
            elem = rangesFrom;
            uint32_t count = 0;
            while (elem != NULL) {
                elem = elem->next;
                count++;
            }

            rangesTo = NULL;
            PRINTF1("--------(%d)\n", count);
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
            elem = rangesFrom;
            prev = NULL;
            while (elem != NULL) {
                next = elem->next;
                if (elem->from < conv[1] + conv[2] && elem->from + elem->size > conv[1]) {
                    PRINTF1("overlap : %ld->%ld || %ld->%ld\n", elem->from, elem->from + elem->size - 1, conv[1], conv[1] + conv[2] - 1);
                    // manage below
                    if (elem->from < conv[1]) {
                        PRINTF1("   below : %ld->%ld\n", elem->from, conv[1] - 1);
                        newEl = malloc(sizeof(t_range));
                        newEl->from = elem->from;
                        newEl->size = conv[1] - elem->from;
                        newEl->next = elem;
                        if (prev)
                            prev->next = newEl;
                        else
                            rangesFrom = newEl;
                        prev = newEl;
                        elem->from = conv[1];
                    }
                    // manage above
                    if (elem->from + elem->size > conv[1] + conv[2]) {
                        PRINTF1("   above : %ld->%ld\n", conv[1] + conv[2], elem->from + elem->size - 1);
                        newEl = malloc(sizeof(t_range));
                        newEl->from = conv[1] + conv[2];
                        newEl->size = (elem->from + elem->size) - (conv[1] + conv[2]);
                        newEl->next = elem;
                        if (prev)
                            prev->next = newEl;
                        else
                            rangesFrom = newEl;
                        prev = newEl;
                        elem->size = conv[2] - (elem->from - conv[1]);
                    }
                    // dequeue elem and keep prev
                    if (prev)
                        prev->next = elem->next;
                    else
                        rangesFrom = elem->next;
                    // convert overlap
                    PRINTF1("   match : %ld->%ld ===> %ld->%ld\n", elem->from, elem->from + elem->size - 1, conv[0] + elem->from - conv[1], conv[0] + elem->from - conv[1] + elem->size -1);
                    elem->from = conv[0] + (elem->from - conv[1]);
                    elem->next = rangesTo;
                    rangesTo = elem;
                } else {
                    prev = elem;
                }
                elem = next;
            }
        }
    }
    total = -1;
    if (isp1) {
        for (size_t i = 0; i < loop; i++)
            total = val[i] < total ? val[i] : total;
    } else {
        while(rangesTo != NULL) {
            elem = rangesTo;
            rangesTo = elem->next;
            elem->next = rangesFrom;
            rangesFrom = elem;
        }
        while(rangesFrom != NULL) {
            elem = rangesFrom;
            rangesFrom = elem->next;
            total = elem->from < total ? elem->from : total;
            free(elem);
        }
    }
    printf("%ld\n", total);
}