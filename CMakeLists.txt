cmake_minimum_required(VERSION 3.9)
project(BlockChain)



include_directories(/usr/local/Cellar/boost/1.66.0/include/)
set(CMAKE_CXX_STANDARD 11)
#set("-lboost_serialization")
#add_definitions(-lboost_serialization)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lboost_serialization")
LINK_DIRECTORIES(/usr/local/Cellar/boost/1.66.0/lib)
add_executable(BlockChain main.cpp include/blockchain/Block.h include/blockchain/BlockChain.h include/blockchain/BlockHeader.h include/blockchain/Identite.h include/blockchain/Key.h include/blockchain/KeyPair.h include/blockchain/Message.h include/blockchain/Transaction.h include/other/Constante.h include/other/SHA25.h include/database/DataBase.h include/ihm/Peer.h include/network/Client.h include/network/Connection.h include/network/MinerNode.h include/network/Node.h include/network/Packet.h include/network/PeerMessage.h include/network/Room.h include/network/Serveur.h include/network/Session.h src/blockchain/Block.cpp src/blockchain/blockchain.cpp src/blockchain/BlockHeader.cpp src/blockchain/Identite.cpp src/blockchain/Key.cpp src/blockchain/KeyPair.cpp src/blockchain/Message.cpp src/blockchain/Transaction.cpp src/database/DataBase.cpp src/ihm/Peer.cpp src/network/Client.cpp src/network/Connection.cpp src/network/MineParalelism.cpp src/network/MinerNode.cpp src/network/Node.cpp src/network/Packet.cpp src/network/Room.cpp src/network/Serveur.cpp src/network/Session.cpp)
#add_executable(BlockChain main.cpp)