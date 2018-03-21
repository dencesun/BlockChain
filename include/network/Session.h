//
// Created by dencesun on 20/03/2018.
//

#ifndef BLOCKCHAIN_SESSION_H
#define BLOCKCHAIN_SESSION_H

#include <boost/asio.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "Packet.h"
#include "Connection.h"
#include "Room.h"

using boost::asio::ip::tcp;

class Session : public boost::enable_shared_from_this<Session> {
public:
    ~Session();

    static boost::shared_ptr<Session> create(boost::shared_ptr<Connection> Connection, boost::shared_ptr<Room> room) {
        boost::shared_ptr<Session> session(new Session(Connection, root));
        session->wait_for_data();
        return session;
    }

    void deliver(const Packet& msg);

private:
    Session(boost::shared_ptr<Connection> Connection, boost::shared_ptr<Room> room);
    void wait_for_data();

    void handle_write(const boost::system::error_code& error);
    void handle_read(const boost::system::error_code& error);
    boost::shared_ptr<Connection> mConnection;
    boost::weak_ptr<Room> mRoom;
    Packet mMessage;
    bool isLeaving;
};
#endif //BLOCKCHAIN_SESSION_H
