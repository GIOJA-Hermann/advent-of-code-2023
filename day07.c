#include "common.h"

#define ADDM5 0x3333333333333333ULL
#define MASK5 0x8888888888888888ULL
#define MASK4 0x4444444444444444ULL
#define MASK2 0x2222222222222222ULL

/*                   0,1,2 3 4 5 6 7 8 9 : ; < = > ? @  A B C D E F G H I  J  K L M N O P  Q R S T */
uint64_t conv[40] = {0,0,2,3,4,5,6,7,8,9,0,0,0,0,0,0,0,14,0,0,0,0,0,0,0,0,11,13,0,0,0,0,0,12,0,0,10};

/*                           0  1  2   3  4  5  6  7  8  9*/
/*                           5  4  3  2H 2L H1 H2 H3 H4 H5*/
const uint64_t wShift[10] = {36,32,28,24,20,16,12, 8, 4, 0};

typedef struct s_hand t_hand;
struct s_hand {
    uint64_t raw;
    uint64_t cards;
    uint64_t weight;
    uint64_t ranking;
    uint64_t bid;
    t_hand *next;
};

uint64_t saveWeight(uint64_t *weight, uint64_t *ranking, uint64_t masked, uint64_t shift) {
    if (masked == 0)
        return (~0x0ULL);
    uint64_t card = (63 - __builtin_clzl(masked))/4;
    *ranking |= (1ULL << shift);
    *weight |= (card << shift);
    return (~(0xFULL << (card * 4)));
}

void day(const char *part) {
    int isp1 = !strcmp(part, "p1");
    char *s = malloc(_bufsize), *p;

    t_hand hand[1000] = {0};
    char card[6] = {0};
    size_t handIdx = 0;
    t_hand *list = NULL, *prev, *parse;
    uint64_t lowCard;
    uint64_t leadCard;
    uint64_t numJ = 0, numJsave;

    if (!isp1)
        conv['J'-'0'] = 1;

    while(rl(s,_bufsize)) {
        t_hand *H = &hand[handIdx++];
        sscanf(s, "%5s %lu", card, &H->bid);
        H->raw = 0;
        for (size_t i = 0; i < 5; i++) {
            H->raw = (H->raw << 4) + conv[card[i]-'0'];
            H->cards += (1ULL << (4*conv[card[i]-'0']));
        }
        numJsave = numJ = (H->cards >> 4) & 0xFULL;
        H->cards &= ~0xFFULL;


        H->cards &= saveWeight(&H->weight, &H->ranking, (H->cards + ADDM5) & MASK5, wShift[0]);
        H->cards &= saveWeight(&H->weight, &H->ranking, H->cards & MASK4, wShift[1]);
        if (numJ && H->ranking) {(H->ranking <<= (4 * numJ)); numJ = 0; H->cards &= saveWeight(&H->weight, &H->ranking, H->cards & MASK4, wShift[1]);}
        H->cards &= saveWeight(&H->weight, &H->ranking, H->cards & (H->cards >> 1), wShift[2]);
        if (numJ && H->ranking) {(H->ranking <<= (4 * numJ)); numJ = 0; H->cards &= saveWeight(&H->weight, &H->ranking, H->cards & (H->cards >> 1), wShift[2]);}
        H->cards &= saveWeight(&H->weight, &H->ranking, H->cards & MASK2, wShift[3]);
        if (numJ && H->ranking) {(H->ranking <<= (4 * numJ)); numJ = 0; H->cards &= saveWeight(&H->weight, &H->ranking, H->cards & MASK2, wShift[3]);}
        H->cards &= saveWeight(&H->weight, &H->ranking, H->cards & MASK2, wShift[4]);
        H->cards &= saveWeight(&H->weight, &H->ranking, H->cards, wShift[5]);
        if (numJ && H->ranking) {(H->ranking <<= (4 * (numJ + 1))); numJ = 0; H->cards &= saveWeight(&H->weight, &H->ranking, H->cards, wShift[5]);}
        H->cards &= saveWeight(&H->weight, &H->ranking, H->cards, wShift[6]);
        H->cards &= saveWeight(&H->weight, &H->ranking, H->cards, wShift[7]);
        H->cards &= saveWeight(&H->weight, &H->ranking, H->cards, wShift[8]);
        H->cards &= saveWeight(&H->weight, &H->ranking, H->cards, wShift[9]);
        if (numJ == 5) {H->ranking = 1ULL << wShift[0];}


        if (list == NULL) {
            list = H;
            H->next = NULL;
        } else {
            parse = list;
            prev = list;
            while((parse != NULL) && ((parse->ranking < H->ranking) || ((parse->ranking == H->ranking) && (parse->raw < H->raw)))) {
                prev = parse;
                parse = parse->next;
            }
            H->next = parse;
            if(prev == parse)
                list = H;
            else
                prev->next = H;
        }
    }
    parse = list;
    uint64_t ranking = 1, total = 0;
    while(parse) {
        total += ranking++ * parse->bid;
        parse = parse->next;
    }
    printf("%lu\n", total);
}
