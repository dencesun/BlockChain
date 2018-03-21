//
// Created by dencesun on 20/03/2018.
//

#ifndef BLOCKCHAIN_MINERNODE_H
#define BLOCKCHAIN_MINERNODE_H

#include <vector>
#include <set>
#include "Node.h"
#include "../blockchain/BlockChain.h"
#include "Block.h"
//#include "../blockchain/Transaction.h"

using std::shared_ptr;
using std::vector;

class MinerNode: Node {
public:
    MinerNode();
    ~MinerNode();

    bool isTransactionsExist(const Transaction& ) const;
    bool startNewBlock() const;
    vector<Transaction> getListTransaction(int);

    bool sendBlock();
    bool receiveBlock();
    bool addTransaction(const Transaction&);
    void removeVectorTransaction(vector<shared_ptr<Transaction>>);
    void addVectorTransaction(vector<shared_ptr<Transaction>>);

private:
    std::shared_ptr<BlockChain> blockchain;
    std::set<Transaction> unconfirmedTransaction;
};
#endif //BLOCKCHAIN_MINERNODE_H
