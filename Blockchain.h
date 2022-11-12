#ifndef TESTCHAIN_BLOCKCHAIN_H
#define TESTCHAIN_BLOCKCHAIN_H

#include <cstdint>
#include <vector>
#include "Block.h"

using namespace std;

class Blockchain {
    public:
        Blockchain();

        void AddBlock(Block newBlock);

    private:
        uint32_t difficulty;
        vector<Block> blockChain;

        const Block GetLastBlock();
};

#endif //TESTCHAIN_BLOCKCHAIN_H
