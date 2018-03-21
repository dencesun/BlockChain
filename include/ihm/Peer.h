//
// Created by dencesun on 19/03/2018.
//

#ifndef BLOCKCHAIN_PEER_H
#define BLOCKCHAIN_PEER_H

#include <memory>

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "../blockchain/Identite.h"
#include "../network/PeerMessage.h"
#include "../blockchain/Transaction.h"
#include "../network/Connection.h"
#include "../database/DataBase.h"
#include "../blockchain/BlockChain.h"
#include "../network/Packet.h"

using ptr_Identite = std::shared_ptr<Identite>;
using boost::asio::ip::tcp

class Client;

class Peer {
    using Cuple = std::tuple<int, string, Block>;

public:
    peer(boost::asio::io_service&, tcp::endpoint&);

    Peer() = delete;
    Peer(const Peer&) = delete;
    Peer& operator=(const Peer&) = delete;

    ~Peer();

    std::shared_ptr<Transaction> createTransaction();
    void connexion();
    void sauvegarderCle(const RSA::PrivateKey& , const RSA::PublicKey&);
    void print(const string&);
    void displayMenu();
    void receivePacket(const Packet&);
    void addClient(std::shared_ptr<Client>);
    void startMining();
    int receiveTransaction(const Packet&);
    int receiveBlock(const Packet&);
    void updateTransactionList(Cuple, const Block&);


    enum {
        WRONG_PACKET_RECEIVE = 0,
        WRONG_BLOCK_WITH_TRANSACTIONS_UNKNOWN = 2,
        WRONG_PACKET_WITH_TRANSACTION_ALREADY_VALIDATED = 3,
        UNKNOWN_ERROR = 1,
        CORRECT_BLOCK_RECEIVED = 4,
        CORRECT_TRANSACTION_ADDED = 5,
        WRONG_BLOCK_RECEIVED = 6
    };

private:
    ptr_Identite identite;
    std::shared_ptr<Client> client;
    DataBase baseDeDonnee;
    BlockChain blockchain;
    void cleanScreen();
    void showBanner();

};


class Client: public std::enable_shared_from_this<Client> {
public:
    static std::shared_ptr<Client> create(boost::asio::io_service& ios, tcp::endpoint& endpoint, std::shared_ptr<Peer> perr) {
        return std::shared_ptr<Client>(new Client(ios, endpoint, perr));
    }
    void write(const boost::system::error_code& error, const Packet& packet) {
        if (!error) {
            newConnection->async_write(packet,
                boost::bind(&Client::handleWrite, shared_from_this(),
                boost::asio::placeholders::error)
            );
        }
    }

private:
    Client(boost::asio::io_service& ioService, tcp::endpoint& endpoint, std::shared_ptr<Peer> peer):
        mIoService(ioService), packet(), peerConnecte(perr) {
        connect(endpoint);
    }

    void connect(tcp::endpoint& endpoint) {
        newConnection = boost::make_shared<Connection>(mIoService);
        tcp::socket& socket = newConnection->socket();
        socket.async_connect(endpoint, boost::bind(&Client::handle_connect, this, boost::asio::placeholders::error));

    }
    void handle_connect(const boost::system::error_code& error) {
        if (!error) {
//             read(error);
        }
    }
    void read(const boost::system::error_code& error) {
        if (!error) {
            packet = Packet();
            newConnection->async_read(packet, boost::bind(&Client::handle_read, shared_from_this(), boost::asio::placeholders::error));

        }
    }
    void handle_read(const boost::system::error_code& e) {
        std::shared_ptr<Peer> mPeer = peerConnecte.lock();

        if (!e) {
            mPeer->receivePacket(packet);
        } else {
            std::err << "Error in handle_read " << e.message() << std::endl;
        }
    }
    void handle_write(const boost::system::error_code & e) {
        if (!e) {
            read(e);
        } else {
            std::cerr << "Error in handle_write" << e.message() << std::endl;
        }
    }
    boost::shared_ptr<Connection> newConnection;
    Packet packet;
    boost::asio::io_service& mIoService;
    std::weak_ptr<Peer> peerConnecte;
};
#endif //BLOCKCHAIN_PEER_H
