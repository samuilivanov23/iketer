#include "Blockchain.h"

Blockchain::Blockchain()
{
    this->blockChain.emplace_back(Block(0, "Genesis Block"));
    this->difficulty = 6;
}

void Blockchain::AddBlock(Block newBlock)
{
    newBlock.previousHash = this->GetLastBlock().hash;
    newBlock.MineBlock(this->difficulty);
    this->blockChain.push_back(newBlock);
}

const Block Blockchain::GetLastBlock()
{
    return this->blockChain.back();
}
