//
// Created by dencesun on 21/03/2018.
//

#include "../../include/ihm/Peer.h"

Peer::Peer(boost::asio::io_service& ioService, boost::asio::ip::tcp::endpoint& endpoint) :
        baseDeDonnee(),
        blockchain(),{

}

Peer::~Peer() {

}

void Peer::receivePacket(const Packet &packet) {
    std::cerr << packet;
    switch (packet.mType) {
        case 3:
            receiveTransaction(packet);
            break;
        case 4:
            receiveBlock(packet);
            break;
        default:
            break;
    }
}

int Peer::receiveBlock(const Packet &packet) {
    if (blockchain.size() == 0) {
        blockchain.push_back(packet.block);
        return Peer::CORRECT_BLOCK_RECEIVED;
    }
    if (!packet.block.isValid()) {
        return false;
    }

    try {
        vector<Message> trBuf;
        for (const auto& tr : packet.block.getTransactionsList()) {
            int status = baseDeDonnee.getStatusTransaction(tr);
            if (status == DataBase::NOT_FOUND_TRANSACTION) {
                return Peer::WRONG_BLOCK_WITH_TRANSACTIONS_UNKNOWN;
            }
            if (status == DataBase::VALIDATED_TRANSACTION) {
                return Peer::WRONG_PACKET_WITH_TRANSACTION_ALREADY_VALIDATED;
            }
            if (status == DataBase::OTHER_SAME_TRANSACTION_ALREADY_VALID) {
                return 0;
            }
            trBuf.push_back(baseDeDonnee.get(tr).second.second);
        }
        for (int i = 0; i<trBuf.size(); ++i) {
            for (int j = i+1; j<trBuf.size(); ++j) {
                if (trBuf.at(i).getHashDomaineName() == trBuf.at(j).getHashDomaineName())
                    return 0;
            }
        }
        if (packet.block.getHeader().getNumer0Bloc() > std::get<0>(blockchain.getLeadingBlock())) {
            auto previousLeading = blockchain.getLeadingBlock();
            int pushCode = blockchain.push_back(packet.block);
            if (pushCode == BlockChain::INSERT_NEW_BLOCK) {
                updateTransactionList(previousLeading, packet.block);
            } else if (pushCode == BlockChain::PREVIOUS_BLOCK_UNKNOWN) {
                return Peer::CORRECT_BLOCK_RECEIVED;
            } else {
                return Peer::WRONG_BLOCK_RECEIVED;
            }
        } else {
            int pushCode = blockchain.push_back(packet.block);
            if (pushCode == BlockChain::INSERT_NEW_BLOCK || pushCode == BlockChain::PREVIOUS_BLOCK_UNKNOWN) {
                return Peer::CORRECT_BLOCK_RECEIVED;
            } else {
                return Peer::WRONG_PACKET_RECEIVE;
            }
        }
    } catch (std::exception e) {
        return Peer::UNKNOWN_ERROR;
    };

    return Peer::CORRECT_BLOCK_RECEIVED;
}

void Peer::updateTransactionList(Cuple leading, const Block &block) {
    using Cuple  = std::tuple<int, string, Block>;

    Cuple newBloc = Cuple(block.getHeader().getNumer0Bloc(), block.getBlockHash(), block);
    int num = std::get<0>(leading);
    while (num < std::get<0>(newBloc)) {
        baseDeDonnee.update(std::get<2>(newBloc), DataBase::VALIDATED_TRANSACTION);
        newBloc = blockchain.getPreviousBlock(newBloc);
    }

    do {
        baseDeDonnee.update(std::get<2>(leading), DataBase::NOT_FOUND_TRANSACTION);
        baseDeDonnee.update(std::get<2>(newBloc), DataBase::VALIDATED_TRANSACTION);

        newBloc = blockchain.getPreviousBlock(newBloc);
        leading = blockchain.getPreviousBlock(leading);
    } while (leading != newBloc);
}

int Peer::receiveBlock(const Packet &packet) {
    if (!packet.transaction.isCorrect()) {
        return Peer::WRONG_PACKET_RECEIVE;
    }
    try {
        baseDeDonnee.push_back(packet.transaction);
    } catch (...) {
        return Peer::UNKNOWN_ERROR;
    };

    return Peer::CORRECT_TRANSACTION_ADDED;
}

void Peer::addClient(std::shared_ptr<Client> nvuClient) {
    client = nvuClient;
}

void Peer::showBanner() {
    cleanScreen();
    print(MessageIHM::introMessage);
    if (!(identite == nullptr)) {
        print ("Your ar connected as :" + identite->getPrenom() + " " + identite->getNom());
    }
}

void Peer::connexion() {
    showBanner();
    string n, pn, key;
    print (MessageIHM::formulation_demande_identite);
    print(MessageIHM::formulation_demande_nom);
    std::cin >> n;
    print (MessageIHM::formulation_demande_prenom);
    std::cin >> pn;
    n = "franc";
    pn = "denis";
    RSA::PrivateKey pvkey;
    RSA::PublicKey pbkey;
    KeyPair cle;
    bool archive;

    DEMANDE_CLE:
    print (MessageIHM::formulation_demande_possession_cle);
    std::cin >> key;
    key = "n";
    if (key.size() != 1)
        goto DEMANDE_CLE;
    switch (key.at(0)) {
        case 'y':
            print (MessageIHM::formulation_demande_location_cle_publique);
            std::cin >> key;
            archive = KeyPair::loadPublicKey(key, pbkey);
            if (!archive)
                goto DEMANDE_CLE;
            print (MessageIHM::formulation_demande_location_cle_privee);
            std::cin >> key;
            archive = KeyPair::loadPrivateKey(key, pvkey);
            if (!archive)
                goto DEMANDE_CLE;
            cle = KeyPair(pbkey, pvkey);
            break;
        case 'n':
            print (MessageIHM::information_generation_cle);
            sauvegarderCle(pvkey, pbkey);
            break;
        default:
            goto DEMANDE_CLE;
            break;
    }
    identite = std::make_shared<Identite>(n, pn, cle);
    showBanner();
    displayMenu();
}

void Peer::sauvegarderCle(const RSA::PrivateKey &pvkey, const RSA::PublicKey &pbkey) {
    showBanner();
    SAUVEGARDE_CLE:
    print (MessageIHM::formulation_sauvegarde_cle);
    string ok;
    std::cin >> ok;
    ok = "n";
    if (ok.size() != 1)
        goto SAUVEGARDE_CLE;
    switch (ok.at(0)) {
        case 'y':
            KeyPair::savePrivateKey("cleprivee.key", pvkey);
            KeyPair::savePublicKey("clepublique.key", pbkey);
            print (MessageIHM::infomation_cle_sauvegarde);
            break;
        case 'n':
            break;
        default:
            goto SAUVEGARDE_CLE;
            break;
    }
}

inline  void Peer::print(const string& m) {
    std::cout << m << std::endl;
}

void Peer::cleanScreen() {

}

void Peer::startMining() {

}

void Peer::displayMenu() {
    DISPLAY_MENU:
    print(MessageIHM::affichage_menu_principal);
    string choix;
    std::cin >> choix;
    choix = "1";
    if (choix.size() != 1)
        goto DISPLAY_MENU;
    switch (choix.at(0))
    {
        case '1':
        {
            std::shared_ptr<Transaction> ptrT = createTransaction();
            if (ptrT == nullptr) {
                print(MessageIHM::error_while_creating_transaction);
                break;
            }
            Packet p; p.mType = Packet::NEW_TRANSACTION; p.transaction = *ptrT;
            std::cerr << p;
            client->write(boost::system::error_code(), p);

            break;
        }
        case '2':
        {
            // start mining
            break;
        }
        case '3':
            connexion(); // return to the first menu
            break;
        default:
            goto DISPLAY_MENU;
            break;
    }
}

std::shared_ptr<Transaction> Peer::createTransaction()
{
    clean_screen();
    string choix;
    string domaineName, informationNameDomain;
    CREATE_TRANSACTION:
    string msg = MessageIHM::formulation_demande_creation_transaction_sous_idenite + identite->getPrenom() + " " + identite->getNom() + MessageIHM::y_or_n;
    print(msg);
    std::cin >> choix;
    choix = "y";
    if (choix.size() != 1)
        goto CREATE_TRANSACTION;
    switch (choix.at(0))
    {
        case 'y':
        {
            print(MessageIHM::formulation_demande_nom_de_domaine);
            std::cin >> domaineName;
            print(MessageIHM::formulation_demande_information);
            std::cin >> informationNameDomain;
            domaineName = "facebook.com";
            informationNameDomain = "hello world";

            std::shared_ptr<Transaction> ptrT = std::make_shared<Transaction>(*identite, domaineName, informationNameDomain);
            if (!ptrT->isCorrect())
                return false;
            return ptrT;
            break;
        }
        case 'n':
            displayMenu();
            return nullptr;
            break;
    }
    return nullptr;
}
