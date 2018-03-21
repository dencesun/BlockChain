//
// Created by dencesun on 20/03/2018.
//

#ifndef BLOCKCHAIN_PACKET_H
#define BLOCKCHAIN_PACKET_H

#include <vector>
#include <boost/serialization/vector.hpp>
#include <string>
#include "../blockchain/Block.h"
#include "../blockchain/Transaction.h"

class Packet {
public:
    int mType;
    Transaction transaction;
    Block block;
    int numeroBloc；

    Packet():
            mType(0),
            transaction(Identite("", ""), "", ""),
            block(0), numeroBloc() {

    }

    friend std::ostream& operator<<(std::ostream& os, const Packet& p);

    template <class Archive>
    void save(Archive& ar, const unsigned int version) const {
        ar & mType & transaction & block & numeroBloc;
    }

    template <class Archive>
    void load(Archive & ar, const unsigned int version) {
        ar & mType & transaction & block & numeroBloc;
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()

    enum {
        NEW_MSG = 0;
        PERSON_LEFT = 1,
        PERSON_CONNECTED = 2,
        NEW_TRANSACTION = 3,
        NEW_BLOCK = 4,
        ASK_BLOCK_CHAIN,
        EXIST_TRANSACTION
    };

};
#endif //BLOCKCHAIN_PACKET_H
