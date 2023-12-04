#include <inttypes.h>
#include <stdlib.h>

typedef struct s_mempool t_mempool;
typedef struct s_recycle t_recycle;

struct s_recycle {
    t_recycle *next;
};

struct s_mempool {
    void   **elemBlocks;
    uint32_t elemSize;
    /* */
    uint32_t nbBlocksPtr;
    uint32_t nbBlocks;
    void    *lastAddr;
    void    *nextAddr;
    uint32_t blockSize;
    /* */
    t_recycle *recycleBin;
};

void initPool(t_mempool *mp, uint32_t sizeFactor, uint32_t elemSize);
void *getElem(t_mempool *mp);
void recycle(t_mempool *mp, void *data);
void throwPool(t_mempool *mp);