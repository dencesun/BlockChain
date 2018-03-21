//
// Created by dencesun on 20/03/2018.
//

#include "../../include/blockchain/Block.h"


Block::Block(const ptr_Block prevBloc, const vector<Transaction>& _transaction):
            previousBlockHash(prevBloc->blockHash), nombreTransaction(_transaction.size()),
            header(prevBloc->getHeader().getNumer0Bloc()+1),
            transactions(), blockHash() {

    for (auto t : _transaction) {
        transactions.emplace_back(t.getHashTransaction());
    }
    buildMerkeRoot();
}

Block::Block(string previousBlockHash, int nbtransaction, vector<string> tr, const BlockHeader _header) :
            previousBlockHash(previousBlockHash), nombreTransaction(nbtransaction),
            transactions(tr), header(_header), blockHash() {
    buildMerkeRoot();
}

Block::Block(int p) :
    previousBlockHash(), blockHash(Constante::FIRST_BLOCK_HASH), nombreTransaction(0), header(0), transactions(){

}

Block::~Block() {

}

Block& Block::operator=(Block rhs) {
    if (rhs == *this)
        return *this;

    header = rhs.header;
    previousBlockHash = rhs.previousBlockHash;
    blockHash = rhs.blockHash;
    nombreTransaction = rhs.nombreTransaction;
    transactions.clear();
    transactions = rhs.transactions;

    return *this;
}

bool Block::operator==(const Block& rhs) const {
    return (header == rhs.getHeader() && transactions == rhs.transactions
            && previousBlockHash == rhs.getPreviousBlockHash() && nombreTransaction == rhs.nombreTransaction);
}

bool Block::isValid() const {
    if(nombreTransaction != transactions.size())
        return false;
    if (header.getNumer0Bloc() == 0)
        return true;
    if (SHA25::sha256(string(header.getHashMerkleRoot() + std::to_string(header.getNonce().first) + std::to_string(header.getNonce().second)))
        .substr(0, Constante::DIFFICULTY_MINING) != std::string(Constante::DIFFICULTY_MINING, '0'))
        return false;

    std::vector<string> trBuf = transactions;
    std::sort(trBuf.begin(), trBuf.end());

    if (std::unique(trBuf.begin(), trBuf.end()) != trBuf.end())
        return false;

    return true;
}

void Block::buildMerkeRoot() {
    if (transactions.size() & 1) {
        transactions.push_back(transactions.at(transactions.size()-1));
        nombreTransaction++;
    }
    int N = transactions.size();

    vector<string> hashTree;
    hashTree.resize(2*N-1);

    for (int i = 0; i<N; i++) {
        hashTree.at(2*N-2-i) = transactions.at(i);
    }
    for (int i = N-2; i>-1; i--) {
        hashTree.at(i) = SHA25::sha256(SHA25::sha256(hashTree.at(2*i+1)+hashTree.at(2*i+2)));
    }

    header.setHashMerkleRoot(hashTree.at(0));
    header.setTime(boost::posix_time::second_clock::local_time());

    blockHash = SHA25::sha256(SHA25::sha256(header.getHashMerkleRoot()));
}

paire Block::solveProofofWork() {
    unsigned long long nonce = 0, incr = 0;
    unsigned long long const limit = std::numeric_limits<unsigned long long>::max();

    string sol(Constante::DIFFICULTY_MINING, '0');
    while (true) {
        string hash = SHA25::sha256(string(header.getHashMerkleRoot()+std::to_string(incr)+std::to_string(nonce)));
        if (hash.substr(0, Constante::DIFFICULTY_MINING) == sol)
            break;
        else
            ++nonce;

        if (limit-1 == nonce) {
            incr++;
            nonce = 0;
        }
    }

    header.setNonce(paire(incr, nonce));
    return paire(incr, nonce);
}

bool Block::containsTransactions(const Transaction& tr) const {
    return (std::find(transactions.begin(), transactions.end(), tr.getHashTransaction()) != transactions.end());
}

void Block::setHashMerkleRoot(string h) {
    previousBlockHash = h;
}

const BlockHeader& Block::getHeader() const {
    return header;
}

string Block::getPreviousBlockHash() const {
    return previousBlockHash;
}

string Block::getBlockHash() const {
    return blockHash;
}

const vector<string> Block::getTransactionsList() const {
    return transactions;
}

std::ostream& operator<<(std::ostream& os, const Block& p) {
    os << std::endl;
    os << "***************************************************************************" << std::endl;
    os << "Bloc hash         : " << p.blockHash << std::endl;
    os << "Previous hash     : " << p.previousBlockHash << std::endl;
    os << "Numero bloc       : " << p.getHeader().getNumer0Bloc() << std::endl;
    os << "Nonce             : " << p.getHeader().getNonce().second << std::endl;
    os << "Time mined        : " << p.getHeader().getTime() << std::endl;
    os << "Liste des transactions" << std::endl;
    for (const auto& tr : p.getTransactionsList())
    {
        std::cout << "    " << tr << std::endl;
    }
    os << "***************************************************************************" << std::endl;
    os << std::endl;
    return os;
}
