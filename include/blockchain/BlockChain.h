//
// Created by dencesun on 19/03/2018.
//

#ifndef BLOCKCHAIN_BLOCKCHAIN_H
#define BLOCKCHAIN_BLOCKCHAIN_H

#include <set>
#include <iostream>
#include <algorithm>
#include <tuple>
#include <string>
#include "Block.h"
#include "../other/Constante.h"
#include <algorithm>
class BlockChain {
public:
    using Cuple = std::tuple<int, string, Block>;

    BlockChain();
    ~BlockChain();
    int push_back(const Block&);
    bool find(const Transaction& ) const;
    size_t size() const;
    void clear(std::ostream&);

    void print(std::ostream&) const;

    const Cuple getLeadingBlock() const;
    Cuple getPreviousBlock(const Cuple& cuple) const;

    enum {
        FIRST_BLOCK_ADDED = 0,
        ERROR_BLOCK_INVALID = 1,
        PREVIOUS_BLOCK_UNKNOWN = 2,
        INSERT_NEW_BLOCK = 3,
        UNKNOWN_ERROR_WHILE_ADDIND = 4
    };

private:
    std::set<Cuple, std::function<bool(Cuple, Cuple)>> blocks;
    std::set<Cuple, std::function<bool(Cuple, Cuple)>> orphans;
    std::set<Cuple>::iterator leadingBlock;
};
#endif //BLOCKCHAIN_BLOCKCHAIN_H
