//
// Created by dencesun on 20/03/2018.
//

#include "../../include/blockchain/Transaction.h"

Transaction::Transaction(const Identite &ident, const string &_domaineName, const string &_information):
        identiteSender(ident) {
    message = Message(_domaineName, _information, ident.getKeyPair());
    hashTransaction = SHA25::sha256(identiteSender.toString() + message.getHashDomaineName() + message.getInformation());
    timestamp = boost::posix_time::second_clock::local_time();
}

Transaction::~Transaction() {

}

bool Transaction::isCorrect() const {
    if (identiteSender.getNom().size() <= 1 && identiteSender.getPrenom().size() <= 1)
        return false;

    if ((message.getHashDomaineName() != SHA25::sha256(message.getNomDomaine())) ||
            message.getInformation().size() <= 1 ||
            message.getPublicKey().GetPublicExponent() == identiteSender.getPublicKey().GetPublicExponent() ||
            hashTransaction != SHA25::sha256(identiteSender.toString() + message.getHashDomaineName() + message.getInformation())
            ) {
        return false;
    }
    return true;
}

string Transaction::toString() const {
    string os;
    os += identiteSender.toString() + message.getNomDomaine() + message.getInformation();
    return os;
}

Message Transaction::getMessage() const {
    return message;
}

boost::posix_time::ptime Transaction::getTime() const {
    return timestamp;
}

string Transaction::getHashTransaction() const {
    return hashTransaction;
}

Identite Transaction::getIdentite() const {
    return identiteSender;
}