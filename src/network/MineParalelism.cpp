//
// Created by dencesun on 21/03/2018.
//

#include <unordered_map>
#include <iostream>
#include <cstdlib>
#include <string>
#include <cassert>
#include <ctime>
#include <boost/asio.hpp>
#include <thread>
#include <future>

#include "../../include/blockchain/Transaction.h"
#include "../../include/database/DataBase.h"
#include "../../include/blockchain/BlockChain.h"
#include "../../include/network/Serveur.h"

#define  SIMULATION_COEFF 0.9

using std::cout;
using std::endl;

Block genesis(0);
bool notified = false;
using paire = std::pair<unsigned long long, unsigned long long>

void mine(bool &stop, std::vector<Transaction> tr, unsigned long long& nonce, unsigned long long& it, std::promise<paire>& pro) {
    Block b(std::make_shared<Block>(genesis), tr);
    try {
        b.solveProofofWork();
        auto res = b.getHeader().getNonce();
        nonce = res.second;
        it = res.first;
        pro.set_value(res);
    } catch (...) {
        pro.set_exception(std::current_exception());
    };
};