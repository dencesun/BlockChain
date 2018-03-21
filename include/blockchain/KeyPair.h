//
// Created by dencesun on 19/03/2018.
//

#ifndef BLOCKCHAIN_KEYPAIR_H
#define BLOCKCHAIN_KEYPAIR_H

#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/split_member.hpp>
#include "../../libs/cryptocpp/cryptopp/rsa.h"
#include "../../libs/cryptocpp/cryptopp/osrng.h"

using CryptoPP::RSA;
using CryptoPP::InvertibleRSAFunction;
using CryptoPP::RSAES_OAEP_SHA_Decryptor;
using CryptoPP::RSAES_OAEP_SHA_Encryptor;
using CryptoPP::AutoSeededRandomPool;

#include "../../libs/cryptocpp/cryptopp/sha.h"
using CryptoPP::SHA1;

#include "../../libs/cryptocpp/cryptopp/filters.h"
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::PK_EncryptorFilter;
using CryptoPP::PK_DecryptorFilter;

#include "../../libs/cryptocpp/cryptopp/files.h"
using CryptoPP::FileSink;
using CryptoPP::FileSource;

#include "../../libs/cryptocpp/cryptopp/osrng.h"
using CryptoPP::SecByteBlock;

#include "../../libs/cryptocpp/cryptopp/cryptlib.h"
using CryptoPP::Exception;
using CryptoPP::DecodingResult;

#include <string>
using std::string;

#include <exception>
using std::exception;

#ifndef SIZE_KEY
#define SIZE_KEY 1024
#endif

class KeyPair {
public:
    KeyPair();
    KeyPair(const RSA::PublicKey&, const RSA::PrivateKey&);
    KeyPair(const KeyPair&);

    ~KeyPair() {};

    RSA::PublicKey getClePublique() const;
    RSA::PrivateKey getPrivateKey() const;
    void setPrivateKey(const RSA::PrivateKey&);
    void setPublicKey(const RSA::PublicKey&);
    string encrypt(string);
    string decrypt(string);

    static void savePrivateKey(const string& filename, const RSA::PublicKey& key);
    static void savePublicKey(const string& filename, const RSA::PublicKey& key);
    static bool loadPrivateKey(const string& filename, RSA::PrivateKey& key);
    static bool loadPublicKey(const string& filename, RSA::PublicKey& key);

    template <class Archive>
    void save(Archive & ar, const unsigned int version) const {
        string aa, bb, cc, dd;
        aa = boost::lexical_cast<std::string>(privateKey.GetPrivateExponent());
        bb = boost::lexical_cast<std::string>(privateKey.GetModulus());
        cc = boost::lexical_cast<std::string>(publicKey.GetPublicExponent());
        dd = boost::lexical_cast<std::string>(publicKey.GetModulus());

        ar & aa & bb & cc & dd;
    }

    template <class Archive>
    void load(Archive& ar, const unsigned int version) {
        string aa, bb, cc, dd;
        ar & aa & bb & cc & dd;
        privateKey.SetPrivateExponent(CryptoPP::Integer(aa.c_str()));
        privateKey.SetModulus(CryptoPP::Integer(bb.c_str()));
        publicKey.SetPublicExponent(CryptoPP::Integer(cc.c_str()));
        publicKey.SetModulus(CryptoPP::Integer(dd.c_str()));
    }

    template <class Archive>
    void serialize(Archive& ar, const unsigned int fileVersion) {
        boost::serialization::split_member(ar, *this, fileVersion);
    }

private:
    static void Save(const string& filename, const CryptoPP::BufferedTransformation& bt);
    static void Load(const string& filename, CryptoPP::BufferedTransformation& bt);

    RSA::PrivateKey privateKey;
    RSA::PublicKey publicKey;
};
#endif //BLOCKCHAIN_KEYPAIR_H
