#include <inttypes.h>
#include <stdlib.h>

struct s_mempool {
    void   **elemBlocks;
    uint32_t elemPerBlock;
    uint32_t elemSize;
    /* */
    uint32_t nbBlocksPtr;
    uint32_t nbBlocks;
    uint32_t nextIdx;
};

typedef struct s_mempool t_mempool;

void initPool(t_mempool *mp, uint32_t elemPerBlock, uint32_t elemSize);
void *getElem(t_mempool *mp);