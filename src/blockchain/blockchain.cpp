//
// Created by dencesun on 20/03/2018.
//

#include "../../include/blockchain/BlockChain.h"

using Cuple = std::tuple<int, string, Block>;

BlockChain::BlockChain() :
        blocks([](const Cuple& x, const Cuple& y)
               {
                   if ((std::get<0>(x) > std::get<0>(y)))
                       return true;
                   if (std::get<0>(x) == std::get<0>(y)) {
                       return (std::get<1>(x) > std::get<1>(y));
                   }
                   return false;
               }),
        orphans([](const Cuple& x, const Cuple& y) {
            return (std::get<0>(x) < std::get<0>(y) && std::get<2>(x).getHeader().getTime() < std::get<2>(y).getHeader().getTime());
        }),
        leadingBlock(blocks.begin()) {

}

BlockChain::~BlockChain() {

}

int BlockChain::push_back(const Block& bloc) {
    if (!bloc.isValid())
        return BlockChain::ERROR_BLOCK_INVALID;
    if (blocks.size() == 0) {
        try {
            blocks.insert(Cuple(bloc.getHeader().getNumer0Bloc(), bloc.getBlockHash(), bloc));
        } catch (const std::exception& e) {
            std::cerr << "An incorrect block has failed to be insert into the BlockChain chain :" << e.what();
        };

        return BlockChain::FIRST_BLOCK_ADDED;
    }

    if (bloc.getBlockHash() == "") {
        try {
            orphans.insert(Cuple(bloc.getHeader().getNumer0Bloc(), bloc.getBlockHash(), bloc));
        } catch (const std::exception& e) {
            std::cerr << "An incorrect block has failed to be insert into the Orphans chain :" << e.what();
        };
        return BlockChain::PREVIOUS_BLOCK_UNKNOWN;
    }

    auto blockIte = blocks.begin();
    for (; blockIte != blocks.end(); ++blockIte) {
        if (std::get<1>(*blockIte) == bloc.getPreviousBlockHash()) {
            try {
                Cuple newBloc = Cuple(bloc.getHeader().getNumer0Bloc(), bloc.getBlockHash(), bloc);
                blocks.insert(newBloc);

                if (bloc.getHeader().getNumer0Bloc() > std::get<2>(*leadingBlock).getHeader().getNumer0Bloc()) {
                    for (std::set<Cuple>::iterator x = blocks.begin(); x != blocks.end(); x++) {
                        if (std::get<2>(*x) == std::get<2>(newBloc)) {
                            leadingBlock = x;
                            break;
                        }
                    }
                }
                return BlockChain::INSERT_NEW_BLOCK;
            } catch (const std::exception& e) {
                std::cerr << "An incorrect block has failed to be insert into the Block chain :" << e.what();
                return BlockChain::UNKNOWN_ERROR_WHILE_ADDIND;
            };
        }
    }
    if (!(blockIte != blocks.end())) {
        try {
            orphans.insert(Cuple(bloc.getHeader().getNumer0Bloc(), bloc.getBlockHash(), bloc));
        } catch (const std::exception& e) {
            std::cerr << "An incorrect block has failed to be insert into the Orphans chain : " << e.what();
        };
        return BlockChain::PREVIOUS_BLOCK_UNKNOWN;
    }
    return BlockChain::UNKNOWN_ERROR_WHILE_ADDIND;
}

void BlockChain::print(std::ostream& os) const {
    for (const auto& exp : blocks) {
        os << std::get<0>(exp) << " " << std::get<1>(exp) << std::endl
                                                         << std::endl;
    }
}

bool BlockChain::find(const Transaction& trans) const {
    if (std::any_of(blocks.rbegin(), blocks.rend(), [&trans](const Cuple& bloc) {

        if (std::get<2>(bloc).getBlockHash() == Constante::FIRST_BLOCK_HASH) // TODO for the first Block
            return false;
        return std::get<2>(bloc).containsTransactions(trans);
    }))
        return true;
    else
        return false;

}

void BlockChain::clear(std::ostream& os) {
    while (orphans.size() > Constante::MAX_SIZE_ORPHANS) {
        auto it = orphans.begin();
        push_back(std::get<2>(*it));
        orphans.erase(it);
    }

    if (std::get<2>(*blocks.begin()).getHeader().getNumer0Bloc() < Constante::DEPTH_DELETION)
        return ;

    auto blockIte = leadingBlock;

    string previousBlockHash = std::get<2>(*leadingBlock).getPreviousBlockHash();
    os << "Last hash : " << previousBlockHash << std::endl;
    while (blockIte != blocks.end()) {
        if (std::get<2>(*blockIte).getHeader().getNumer0Bloc() > std::get<2>(*leadingBlock).getHeader().getNumer0Bloc()-Constante::DEPTH_DELETION) {
            os << "Too early to delete " << std::get<0>(*blockIte) << std::endl;

            if (std::get<2>(*blockIte).getBlockHash() == previousBlockHash) {
                previousBlockHash = std::get<2>(*blockIte).getPreviousBlockHash();
            }
            blockIte++;
        } else {
            if (std::get<2>(*blockIte).getBlockHash() == previousBlockHash)
            {
                os << "No deletion : " << std::get<0>(*blockIte) << std::endl;

                previousBlockHash = std::get<2>(*blockIte).getPreviousBlockHash();
                blockIte++;
            }
                // else delete it
            else
            {
                os << "No too early but deletion : " << std::get<0>(*blockIte) << std::endl;
                blockIte = blocks.erase(blockIte);
            }
        }
    }
}

size_t  BlockChain::size() const {
    return blocks.size() + orphans.size();
}

const Cuple BlockChain::getLeadingBlock() const {
    return *leadingBlock;
}

Cuple BlockChain::getPreviousBlock(const Cuple &cuple) const {
    string previousHash = std::get<2>(cuple).getPreviousBlockHash();
    if (previousHash == "")
        return cuple;
    std::set<Cuple>::iterator iter = blocks.find(cuple);
    while (iter != blocks.end() && std::get<1>(*iter) != previousHash) {
        iter++;
    }
    return *iter;
}