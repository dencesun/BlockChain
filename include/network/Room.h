//
// Created by dencesun on 20/03/2018.
//

#ifndef BLOCKCHAIN_ROOM_H
#define BLOCKCHAIN_ROOM_H

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <set>
#include "Session.h"
#include "Serveur.h"
#include "Packet.h"
#include "../blockchain/Identite.h"
#include "../blockchain/Transaction.h"
#include "../blockchain/BlockChain.h"
#include "../database/DataBase.h"

class Session;
class Serveur;

class Room {
public:
    Room(Serveur& server);

    void join(boost::shared_ptr<Session> participant);
    void leave(boost::shared_ptr<Session> participant);
    void deliver(const Packet msg);

    bool addBlock(const Block&);
    bool addTransaction(const Transaction&);
    bool addIdentite(const Identite&);

private:
    std::set<boost::shared_ptr<Session>> mParticipants;
    Serveur& mServer;
    std::vector<Identite> identites;
    std::vector<Transaction> transactions;
    BlockChain blockhain;
    DataBase bdd;
};

typedef std::shared_ptr<Room> roomPtr;
typedef std::weak_ptr<Room> roomWptr;
#endif //BLOCKCHAIN_ROOM_H
