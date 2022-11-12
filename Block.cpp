#include "Block.h"
#include "sha256.h"

Block::Block(uint32_t indexIn, const string &dataIn)
{
    this->index = index;
    this->data = data;
    this->nonce = 0;
    this->tTime = time(nullptr);
    this->hash = CalculateHash();
}

void Block::MineBlock(uint32_t difficulty)
{
    char difficultyCharString[difficulty + 1];
    for (uint32_t i = 0; i < difficulty; ++i)
    {
        difficultyCharString[i] = '0';
    }
    difficultyCharString[difficulty] = '\0';

    string difficultyString(difficultyCharString);

    do
    {
        this->nonce++;
        this->hash = CalculateHash();
    }
    while (this->hash.substr(0, difficulty) != difficultyString);

    cout << "Block mined: " << this->hash << endl;
}

inline const string Block::CalculateHash()
{
    stringstream hashStringStream;
    hashStringStream << this->index << this->previousHash << this->tTime << this->data << nonce;

    return sha256(hashStringStream.str());
}
