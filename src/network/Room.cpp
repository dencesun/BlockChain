//
// Created by dencesun on 21/03/2018.
//

#include "../../include/network/Room.h"

Room::Room(Serveur &server) :
        mServer(server),
        blockhain(),
        bdd() {
    std::cout << "New room" << std::endl;
}

void Room::join(boost::shared_ptr<Session> participant) {
    mParticipants.insert(participant);
    Packet e;
    e.mType = Packet::PERSON_CONNECTED;
    deliver(e);
}

void Room::leave(boost::shared_ptr<Session> participant) {
    Packet e;
    e.mType = Packet::PERSON_LEFT;
    deliver(e);

    mParticipants.erase(participant);
}

void Room::deliver(const Packet msg) {
    switch (msg.mType) {
        case Packet::PERSON_CONNECTED:
            break;
        case Packet::PERSON_LEFT:
            break;
        case Packet::EXIST_TRANSACTION:
            std::cerr <<  "This is not implemented : ask for the existence of a transaction";
            break;
        case Packet::ASK_BLOCK_CHAIN:
            {
                int num = msg.numeroBloc;
            }
            break;
        case Packet::NEW_BLOCK:
            {
                Block b = msg.block;
                if (!b.isValid()) {

                }
            }
            break;
        case Packet::NEW_TRANSACTION:
            break;

        default:
            break;
    }
    std::for_each(mParticipants.begin(), mParticipants.end(),
    boost::bind(&Session::deliver, _1, boost::ref(msg)));
}