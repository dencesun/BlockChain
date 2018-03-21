//
// Created by dencesun on 19/03/2018.
//

#ifndef BLOCKCHAIN_MESSAGE_H
#define BLOCKCHAIN_MESSAGE_H

#include <ctype.h>
#include <memory>
#include <boost/serialization/split_member.hpp>

#include "../other/SHA25.h"
#include "KeyPair.h"

using std::string;

class Message {
public:
    Message(): nomDeDomaine(), information(), hashDomainName(), publicKey(), longueurMessage(), signature() {};

    bool operator>(const Message& rhs) const {
        if (information < rhs.information) return true;
        else if (information == rhs.information) return nomDeDomaine < rhs.nomDeDomaine;
        else return false;
    }

    bool operator==(const Message& rhs) const {
        return (rhs.information == this->information & rhs.nomDeDomaine == this->nomDeDomaine
                && signature == rhs.signature);
    }

    Message&operator=(const Message& rhs) {
        if (*this == rhs) return *this;

        information = rhs.information;
        nomDeDomaine = rhs.nomDeDomaine;
        signature = rhs.signature;
        hashDomainName = rhs.hashDomainName;
        publicKey = rhs.publicKey;
        longueurMessage = rhs.longueurMessage;

        return *this;
    }

    Message(string, string, const KeyPair&);
    ~Message();

    string getNomDomaine() const;
    string getInformation() const;
    string getHashDomaineName() const;
    RSA::PublicKey getPublicKey() const;

    bool verifier() const;
    SecByteBlock sign(RSA::PrivateKey&);

    template <class Archive>
    void save(Archive& ar, const unsigned int version) const {
        ar & nomDeDomaine & information & longueurMessage;
        string aa = boost::lexical_cast<std::string>(publicKey.GetPublicExponent());
        string bb = boost::lexical_cast<std::string>(publicKey.GetModulus());

        ar & aa & bb;
    }

    template <class Archive>
    void load(Archive& ar, const unsigned int version) {
        string d, e, f;
        ar & nomDeDomaine & information & longueurMessage;
        ar & d & e;
        publicKey.SetPublicExponent(CryptoPP::Integer(d.c_str()));
        publicKey.SetModulus(CryptoPP::Integer(e.c_str()));
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
private:
    std::string nomDeDomaine;
    std::string information;
    std::string hashDomainName;
    RSA::PublicKey publicKey;
    __int64_t longueurMessage;
    SecByteBlock signature;
};

#endif //BLOCKCHAIN_MESSAGE_H
