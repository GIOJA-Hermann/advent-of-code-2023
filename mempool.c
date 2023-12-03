#include "mempool.h"

void initPool(t_mempool *mp, uint32_t elemPerBlock, uint32_t elemSize) {
    /* elemSize is forced aligned on 64b*/
    *mp = (t_mempool){.elemBlocks = NULL, .elemPerBlock = elemPerBlock, .elemSize = ((elemSize + 7) / 8) << 3, .nbBlocksPtr = 0, .nbBlocks = 0, .nextIdx = 0};
}

void *getElem(t_mempool *mp) {
    if (mp->nextIdx == (mp->nbBlocks * mp->elemPerBlock)) {
        if (mp->nbBlocksPtr == mp->nbBlocks) {
            mp->nbBlocksPtr += 100;
            mp->elemBlocks = realloc(mp->elemBlocks, mp->nbBlocksPtr * mp->elemSize);
        }
        mp->elemBlocks[mp->nbBlocks++] = malloc(mp->elemPerBlock * mp->elemSize);
    }
    void* ret = (void*)(((char*)mp->elemBlocks[mp->nextIdx / mp->elemPerBlock]) + ((mp->nextIdx % mp->elemPerBlock) * mp->elemSize));
    mp->nextIdx++;
    return ret;
}

