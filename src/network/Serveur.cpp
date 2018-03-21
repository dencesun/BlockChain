//
// Created by dencesun on 21/03/2018.
//

#include "../../include/network/Serveur.h"
#include "../../include/network/Session.h"

Serveur::Serveur(boost::asio::io_service &ioService, const tcp::endpoint &endpoint) :
        mIoService(ioService),
        mAcceptor(ioService, endpoint),
        mRoom(new Room(*this)) {
    std::cout << "Creation d'un serveur" << std::endl;
    waitForConnection();
}

Serveur::~Serveur() {

}
void Serveur::waitForConnection() {
    boost::shared_ptr<Connection> newConnection(new Connection(mIoService));

    mAcceptor.async_accept(newConnection->socket(),
                        boost::bind(&Serveur::handleAccept, this,
                        boost::asio::placeholders::error,
                        newConnection));
}

void Serveur::handleAccept(const boost::system::error_code &error, boost::shared_ptr<Connection>) {
    if (!error) {
        boost::shared_ptr<Session> session = Session::create(newConnection, mRoom);
        mRoom->join(session);
        waitForConnection();
    } else  {
        std::cerr << "Connection refused" << std::endl;
    }
}