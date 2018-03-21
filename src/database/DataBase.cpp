//
// Created by dencesun on 20/03/2018.
//

#include "../../include/database/DataBase.h"

using std::pair;
using std::map;
using std::multimap;
using paire_int_message = std::pair<int, Message>

DataBase::DataBase() :
        hashTransactionToHashMessage(), hashMessageToMessage() {

}

DataBase::~DataBase() {

}

bool DataBase::push_back(const Transaction &tr) {
    if (getStatusTransaction(tr.getHashTransaction()) != DataBase::NOT_FOUND_TRANSACTION ||
            getStatusTransaction(tr.getHashTransaction()) == DataBase::OTHER_SAME_TRANSACTION_ALREADY_VALID)
        return false;

    try {
        hashTransactionToHashMessage.insert(std::pair<string, string>(tr.getHashTransaction(), tr.getMessage().getHashDomaineName()));
        dataCuple newEntry = std::pair<string, std::pair<int, Message>>(tr.getHashTransaction(), pair_int_message(DataBase::NOT_FOUND_TRANSACTION, tr.getMessage()));
        hashMessageToMessage.insert(std::pair<string, dataCuple>(tr.getMessage().getHashDomaineName(), newEntry));
    } catch (std::exception e) {
        return false;
    };

    return true;
}

int DataBase::getStatusTransaction(const string &transactionHash) const {
    if (hashMessageToMessage.find(transactionHash) != hashMessageToMessage.end())
        return getStatusMessage(hashTransactionToHashMessage.at(transactionHash), transactionHash);
    else
        return DataBase::NOT_FOUND_TRANSACTION;
}

int DataBase::getStatusMessage(const string &messageHash, const string &transactionHash) const {
    int resCode = DataBase::NOT_FOUND_TRANSACTION;
    auto ppp = hashTransactionToHashMessage.equal_range(messageHash);
    if (ppp.first == ppp.second) {
        std::cerr << "Big Mistake in the Database" << std::endl;
    }

    for (auto it2 = ppp.first; it2 != ppp.second; ++it2) {
        if (it2->second.first == transactionHash) {
            resCode = it2->second.first;
        } else if (it2->second->second.first == VALIDATED_TRANSACTION) {
            return OTHER_SAME_TRANSACTION_ALREADY_VALID;
        }
    }
    return resCode;
}

std::pair<string, std::pair<int, Message>> DataBase::get(string transactionHash) const {
    if (getStatusMessage(transactionHash) == DataBase::NOT_FOUND_TRANSACTION) {
        Message message("", "", KeyPair());
        return dataCuple(transactionHash, pair_int_Message(DataBase::NOT_FOUND_TRANSACTION, message));
    } else {
        auto ppp = hashMessageToMessage.equal_range(hashTransactionToHashMessage.at(transactionHash));
        for (auto it2 = ppp.first; it2 != ppp.second; ++it2) {
            if (it2->second.first == transactionHash)
                return it2->second;
        }
    }
};

void DataBase::update(const Block &block, int code) {
    for (const auto tr : block.getTransactionsList()) {
        if (getStatusTransaction(tr) != DataBase::NOT_FOUND_TRANSACTION) {
            update(tr, code);
        } else  {
            std::cerr << "This is really strange";
        }
    }
}

void DataBase::update(const string& tr, int code) {
    try {
        auto ppp = hashMessageToMessage.equal_range(hashTransactionToHashMessage.at(tr));
        for (auto it2 = ppp.first; it2 != ppp.second; ++it2) {
            if (it2->second.first == tr) {
                it2->second.first = code;
                break;
            }
        } catch (const std::exception&) {
                std::cerr << "Error while updating the database" << std::endl;
        }

    }
}

void  DataBase::request(const string& domainName, std::ostream& os) const
{
    os << std::endl;
    os << "***************************************************************************" << std::endl;
    auto ppp = hashMessageToMessage.equal_range(SHA25::sha256(domainName));
    os << "Request for the domain name : " << domainName << std::endl;
    os << "Hash of the Domain name     : " << SHA25::sha256(domainName) << std::endl;
    os << "Number of message for the DN: " << hashMessageToMessage.count(SHA25::sha256(domainName)) << std::endl;
    for (auto it2 = ppp.first;
         it2 != ppp.second;
         ++it2)
    {
        os << "Message  " << ((*it2).second.second.first == DataBase::VALIDATED_TRANSACTION ? "VALIDATED  " : "NOT VALIDATED ") << std::endl;
        os << "  Information in the Message : " << (*it2).second.second.second.getInformation() << std::endl;
        os << "  Public Exponent Key of the Sender   : " << (*it2).second.second.second.getPublicKey().GetPublicExponent() << std::endl;
        os << "  Modulus Key of the Sender   : " << (*it2).second.second.second.getPublicKey().GetModulus() << std::endl;

        os << "  Validity of the Message  : " << ((*it2).second.second.second.verifier() == true ? "correct " : "non correct") << std::endl;
        os << std::endl;
        os << std::endl;

    }
    os << "***************************************************************************" << std::endl;
}

void  DataBase::request(const string& domainName) const
{
    std::cout << std::endl;
    std::cout << "***************************************************************************" << std::endl;
    auto ppp = hashMessageToMessage.equal_range(SHA25::sha256(domainName));
    std::cout << "Request for the domain name : " << domainName << std::endl;
    std::cout << "Hash of the Domain name     : " << SHA25::sha256(domainName) << std::endl;
    std::cout << "Number of message for the DN: " << hashMessageToMessage.count(SHA25::sha256(domainName)) << std::endl;
    for (auto it2 = ppp.first;
         it2 != ppp.second;
         ++it2)
    {
        std::cout << "Message  " << ((*it2).second.second.first == DataBase::VALIDATED_TRANSACTION ? "VALIDATED  " : "NOT VALIDATED ") << std::endl;
        std::cout << "  Information in the Message : " << (*it2).second.second.second.getInformation() << std::endl;
        std::cout << "  Public Exponent Key of the Sender   : " << (*it2).second.second.second.getPublicKey().GetPublicExponent() << std::endl;
        std::cout << "  Modulus Key of the Sender   : " << (*it2).second.second.second.getPublicKey().GetModulus() << std::endl;

        std::cout << "  Validity of the Message  : " << ((*it2).second.second.second.verifier() == true ? "correct " : "non correct") << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;

    }
    std::cout<< "***************************************************************************" << std::endl;
}

void DataBase::save(const string &fichier) const {
    std::ofstream is(fichier);
    if (!is.is_open())
        return ;
    for (auto it = hashMessageToMessage.begin(); it != hashMessageToMessage().end(); ++it) {
        request(it->first, is);
    }
    is.close();
}

size_t  DataBase::size() const {
    return hashMessageToMessage.size();
}

