#include "mempool.h"
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>

#define MP_SIZE_SHIFT 12
#define MP_BLOCKS_INC 64

void initPool(t_mempool *mp, uint32_t sizeFactor, uint32_t elemSize) {
    /* elemSize is forced aligned on 64b and >= 1 ptr */
    elemSize = (elemSize > sizeof(void*)) ? elemSize : sizeof(void*);
    *mp = (t_mempool){.elemBlocks = NULL, .elemSize = ((elemSize + 7) / 8) << 3, .nbBlocksPtr = 0, .nbBlocks = 0, .lastAddr = NULL, .nextAddr = NULL, .blockSize = (sizeFactor << MP_SIZE_SHIFT) * elemSize, .recycleBin = NULL};
}

void *getElem(t_mempool *mp) {
    void* ret;
    if (mp->recycleBin) {
        ret = mp->recycleBin;
        mp->recycleBin = mp->recycleBin->next;
        return ret;
    }
    if (mp->nextAddr >= mp->lastAddr) {
        if (mp->nbBlocksPtr == mp->nbBlocks) {
            mp->nbBlocksPtr += MP_BLOCKS_INC;
            mp->elemBlocks = realloc(mp->elemBlocks, mp->nbBlocksPtr * mp->elemSize);
            memset(&mp->elemBlocks[mp->nbBlocksPtr - MP_BLOCKS_INC], 0, MP_BLOCKS_INC * sizeof(void *));
        }
        if (mp->elemBlocks[mp->nbBlocks] == NULL) {
            mp->elemBlocks[mp->nbBlocks] = mmap ( NULL, mp->blockSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0 );
        }
        mp->nextAddr = mp->elemBlocks[mp->nbBlocks++];
        mp->lastAddr = mp->nextAddr + mp->blockSize;
    }
    ret = mp->nextAddr;
    mp->nextAddr += mp->elemSize;
    return ret;
}

void recycle(t_mempool *mp, void *data) {
    ((t_recycle *)data)->next = mp->recycleBin;
    mp->recycleBin = (t_recycle *)data;
}

void recycleAll(t_mempool *mp) {
    mp->nbBlocks = 0;
    mp->nextAddr = mp->elemBlocks[mp->nbBlocks];
    mp->lastAddr = mp->nextAddr + mp->blockSize;
    mp->recycleBin = NULL;
}

void throwPool(t_mempool *mp) {
    for (size_t i = 0; i < mp->nbBlocksPtr; i++) {
        if (mp->elemBlocks[i] == NULL)
            break;
        munmap(mp->elemBlocks[i], mp->blockSize);
    }
    free(mp->elemBlocks);
}

