//
// Created by dencesun on 21/03/2018.
//

#include "../../include/network/Session.h"

Session::Session(boost::shared_ptr<Connection> Connection, boost::shared_ptr<Room> room):
        mConnection(Connection),
        mRoom(room),
        mMessage(), {
    isLeaving = false;
    std::cout << "New Session !" << std::endl;
}

Session::~Session() {
    std::cout << "Deleted session" << std::endl;
}

void Session::wait_for_data() {
    std::cout << 'Waiting for data' << std::endl;
    mConnection->async_read(mMessage,
            boost::bind(&Session::handle_read, shared_from_this(),
            boost::asio::placeholders::error));
}

void Session::handle_read(const boost::system::error_code &error) {
    boost::shared_ptr<Room> room = mRoom.lock();
    if (room) {
        if (!error) {
            room->deliver(mMessage);
            std::cout << "---- Reception ----" << mMessage.mType << " " << std::endl;
            std::cout << mMessage << std::endl;
            std::cout << "-------------------" << std::endl;
            wait_for_data();
        } else {
            if (!isLeaving) {
                isLeaving = true;
                room->leave(shared_from_this());
            }
        }
    }
}

void Session::deliver(const Packet &msg) {
    mConnection->async_write(msg,
        boost::bind(&Session::handle_write, shared_from_this(),
        boost::asio::placeholders::error));

    std::cout << "message envoye :" << msg.mType << " " << std::endl;

}

void Session::handle_write(const boost::system::error_code &error) {
    std::cerr << "Message bien envoye !" << std::endl;
    boost::shared_ptr<Room> room = mRoom.lock();
    if (room && error && (!isLeaving)) {
        isLeaving = true;
        room->leave(shared_from_this());
    }
}