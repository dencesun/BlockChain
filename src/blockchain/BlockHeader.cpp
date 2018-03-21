//
// Created by dencesun on 20/03/2018.
//

#include "../../include/blockchain/BlockHeader.h"

BlockHeader::BlockHeader(int _numeroBloc) :
    numeroBloc(_numeroBloc), merkleRootHash(""), timestamp(), nonce() {

}

BlockHeader::~BlockHeader() {

}

bool BlockHeader::operator==(const BlockHeader &rhs) const {
    return (merkleRootHash == rhs.merkleRootHash && numeroBloc == rhs.getNumer0Bloc() && timestamp == rhs.getTime());
}

void BlockHeader::setHashMerkleRoot(string hash) {
    merkleRootHash = hash;
}

void BlockHeader::setTime(boost::posix_time::ptime time) {
    timestamp = time;
}

paire BlockHeader::getNonce() const {
    return nonce;
}

int BlockHeader::getNumer0Bloc() const {
    return numeroBloc;
}

void BlockHeader::setNonce(paire nce) {
    nonce = nce;
}

void BlockHeader::setNumero(int nmb) {
    numeroBloc = nmb;
}

boost::posix_time::ptime BlockHeader::getTime() const {
    return timestamp;
}

string BlockHeader::getHashMerkleRoot() const {
    return merkleRootHash;
}