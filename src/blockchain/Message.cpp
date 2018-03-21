//
// Created by dencesun on 20/03/2018.
//

#include "../../include/blockchain/Message.h"

Message::Message(string _domainName, string _info, const KeyPair &keypair):
    nomDeDomaine(_domainName), information(_info),
    hashDomainName(SHA25::sha256(nomDeDomaine)),
    publicKey(keypair.getClePublique()),
    longueurMessage(nomDeDomaine.size() + information.size() + hashDomainName.size()) {

    sign(keypair.getPrivateKey());
}

Message::~Message() {

}

string Message::getNomDomaine() const {
    return nomDeDomaine;
}

string Message::getInformation() const {
    return information;
}

string Message::getHashDomaineName() const {
    return hashDomainName;
}

RSA::PublicKey Message::getPublicKey() const {
    return publicKey;
}

bool Message::verifier() const {
    CryptoPP::RSASSA_PKCS1v15_SHA_Verifier verifier(publicKey);
    string message = nomDeDomaine + information;

    bool result = verifier.VerifyMessage((const byte*)message.c_str(),
            message.length(), signature, signature.size());
    return result;
}

SecByteBlock Message::sign(RSA::PrivateKey& privateKey) {

    AutoSeededRandomPool rng;
    CryptoPP::RSASSA_PKCS1v15_SHA_Signer signer(privateKey);
    size_t length = signer.MaxSignatureLength();
    signature = SecByteBlock(length);

    // Sign message
    string message = nomDeDomaine + information;
    length = signer.SignMessage(rng, (const byte*)message.c_str(),
                                message.length(), signature);
    signature.resize(length);
    signature = SecByteBlock(signature.data(), signature.size());
    return signature;
}