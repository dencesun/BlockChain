//
// Created by dencesun on 19/03/2018.
//

#ifndef BLOCKCHAIN_BLOCK_H
#define BLOCKCHAIN_BLOCK_H

#include <memory>
#include <iostream>
#include <vector>
#include <string>
#include <boost/serialization/vector.hpp>
#include "Transaction.h"
#include "BlockHeader.h"
#include "../other/Constante.h"

using std::vector;
using std::string;

class Block {
    using ptr_Block = std::shared_ptr<Block>;
public:

    explicit  Block(int);
    Block(const ptr_Block, const vector<Transaction>&);
    Block(string, int, vector<string>, const BlockHeader);
    Block() = delete;

    void setHashMerkleRoot(string);
    ~Block();

    Block& operator=(Block);
    bool operator==(const Block&) const;

    const vector<string> getTransactionsList() const;
    const BlockHeader& getHeader() const;
    string getPreviousBlockHash() const;
    string getBlockHash() const;

    bool isValid() const;
    bool containsTransactions(const Transaction&) const;
    paire solveProofofWork();
    friend std::ostream&operator<<(std::ostream& os, const Block& p);

    template <class Archive>
    void save(Archive& ar, const unsigned int version) const {
        ar& nombreTransaction;
        ar& header;
        ar& transactions;
        ar& previousBlockHash;
        ar& blockHash;
    }

    template <class Archive>
    void load(Archive& ar, const unsigned int version) {
        ar& nombreTransaction;
        ar& header;
        ar& transactions;
        ar& previousBlockHash;
        ar& blockHash;
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()

private:
    string previousBlockHash;
    string blockHash;

    int nombreTransaction;
    BlockHeader header;

    vector<string> transactions;

    void buildMerkeRoot();

};
#endif //BLOCKCHAIN_BLOCK_H
