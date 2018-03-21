//
// Created by dencesun on 19/03/2018.
//

#ifndef BLOCKCHAIN_DATABASE_H
#define BLOCKCHAIN_DATABASE_H

#include <map>
#include <fstream>

#include "../blockchain/Transaction.h"
#include "../blockchain/Block.h"

class DataBase {
    using dataCuple = std::pair<string, std::pair<int, Message>>;

public:
    DataBase();
    ~DataBase();

    void request(const string&, std::ostream&) const;
    void request(const string& domainName) const;

    dataCuple get(string) const;
    int getStatusTransaction(const string&) const;
    int getStatusMessage(const string&, const string&) const;
    void save(const string&) const;

    void update(const Block&, int);
    void update(const string&, int);
    void print() const;
    bool push_back(const Transaction&);
    size_t size() const;

    enum {
        NOT_FOUND_TRANSACTION = 0,
        NOT_VALIDATED_TRANSACTION = 1,
        VALIDATED_TRANSACTION = 2,
        OTHER_SAME_TRANSACTION_ALREADY_VALID = 3
    };

private:
    std::map<string, string> hashTransactionToHashMessage;
    std::multimap<string, dataCuple> hashMessageToMessage;
};
#endif //BLOCKCHAIN_DATABASE_H
