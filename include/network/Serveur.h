//
// Created by dencesun on 20/03/2018.
//

#ifndef BLOCKCHAIN_SERVEUR_H
#define BLOCKCHAIN_SERVEUR_H

#include <vector>
#include <functional>
#include <iostream>

#include <boost/asio.hpp>
#include "Room.h"
#include "Connection.h"
class Room;

using boost::asio::ip::tcp;

class Serveur {
public:
    Serveur(boost::asio::io_service& ioService, const tcp::endpoint& endpoint);
    ~Serveur();

    void waitForConnection();
private:
    void handleAccept(const boost::system::error_code& error, boost::shared_ptr<Connection> );

    boost::asio::io_service& mIoService;
    tcp::acceptor mAcceptor;
    boost::shared_ptr<Room> mRoom;
};
#endif //BLOCKCHAIN_SERVEUR_H
