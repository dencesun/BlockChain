//
// Created by dencesun on 19/03/2018.
//

#ifndef BLOCKCHAIN_SHA25_H
#define BLOCKCHAIN_SHA25_H

#include <string>
#include "../../libs/cryptocpp/cryptopp/sha.h"
#include "../../libs/cryptocpp/cryptopp/hex.h"

using std::string;

class SHA25 {
public:
    using SHA256Hash = CryptoPP::SHA256;

    static string sha256(string message) {
        CryptoPP::SHA256 hash;
        byte digest[CryptoPP::SHA256::DIGESTSIZE];
        hash.CalculateDigest(digest, (byte *)message.c_str(), message.length());

        CryptoPP::HexEncoder encoder;
        std::string sha;
        encoder.Attach(new CryptoPP::StringSink(sha));
        encoder.Put(digest, sizeof(digest));
        encoder.MessageEnd();

        return sha;
    }
};
#endif //BLOCKCHAIN_SHA25_H
