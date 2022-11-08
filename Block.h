#ifndef TESTCHAIN_BLOCK_H
#define TESTCHAIN_BLOCK_H

#include <cstdint>
#include <iostream>
#include <sstream>

using namespace std;

class Block { 
    private:
        uint32_t index;
        uint32_t nonce;
        string data;
        time_t tTime;

        const string CalculateHash();

    public:
        string hash;
        string previousHash;

        Block(uint32_t index, const string &data);
        void MineBlock(uint32_t difficulty);

};

#endif //TESTCHAIN_BLOCK_H
