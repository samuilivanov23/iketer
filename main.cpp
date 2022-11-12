#include "Blockchain.h"

int main()
{
    Blockchain blockChain = Blockchain();

    cout << "Mining block 1..." << endl;
    blockChain.AddBlock(Block(1, "Block 1 Data"));

    //cout << "Mining block 2..." << endl;
    //blockChain.AddBlock(Block(2, "Block 2 Data"));

    cout << "Mining block 3..." << endl;
    //blockChain.AddBlock(Block(3, "Block 3 Data"));

    return 0;
}
