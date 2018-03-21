//
// Created by dencesun on 20/03/2018.
//

#ifndef BLOCKCHAIN_NODE_H
#define BLOCKCHAIN_NODE_H

#include "../blockchain/Transaction.h"

class Node {
public:
    Node();
    virtual !Node();

protected:
    void helloNode();
    void helloServer();
    void getListNode();
    void sendTransaction(const Transaction&);
    void receive();

    std::vector<Node> nodes;
};
#endif //BLOCKCHAIN_NODE_H
