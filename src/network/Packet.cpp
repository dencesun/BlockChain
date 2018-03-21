//
// Created by dencesun on 21/03/2018.
//

#include "../../include/network/Packet.h"

std::ostream& operator<<(std::ostream& os, const Packet& p)
{
    os << std::endl;
    os << "***************************************************************************" << std::endl;
    os << "**_______________________________________________________________________**" << std::endl;
    os << "**|                                                                     |**" << std::endl;
    switch (p.mType)
    {
        case 1:
            os << "**|                                                                     |**" << std::endl;
            os << "**|                                                                     |**" << std::endl;
            os << "**|" << "     " << "Packet type : " << std::setw(45) << p.mType << "     |**" << std::endl;
            os << "**|" << "     " << "Information : " << std::setw(45) << "a person has disconnected" << "     |**" << std::endl;
            os << "**|                                                                     |**" << std::endl;
            os << "**|                                                                     |**" << std::endl;
            os << "**|                                                                     |**" << std::endl;

            break;
        case 2:
            os << "**|                                                                     |**" << std::endl;
            os << "**|                                                                     |**" << std::endl;
            os << "**|" << "     " << "Packet type : " << std::setw(45) << p.mType << "     |**" << std::endl;
            os << "**|" << "     " << "Information : " << std::setw(45) << "a person has just join the network" << "     |**" << std::endl;
            os << "**|                                                                     |**" << std::endl;
            os << "**|                                                                     |**" << std::endl;
            os << "**|                                                                     |**" << std::endl;

            break;
        case 3:
            os << "**|                                                                     |**" << std::endl;
            os << "**|      Hash Transaction : " << std::setw(38) << p.transaction.getHashTransaction().substr(0, 38) << "...   |**" << std::endl;
            os << "**|      Message          : " << std::setw(40) << p.transaction.getMessage().getNomDomaine().substr(0, 40) << "    |**" << std::endl;
            os << "**|      Information      : " << std::setw(43) << p.transaction.getMessage().getInformation().substr(0, 40) << " |**" << std::endl;
            os << "**|      Module clePub    : " << std::setw(43) << boost::lexical_cast<std::string>(p.transaction.getMessage().getPublicKey().GetModulus()).substr(0, 43) << " |**" << std::endl;
            os << "**|      Exponent clePub  : " << std::setw(43) << boost::lexical_cast<std::string>(p.transaction.getMessage().getPublicKey().GetPublicExponent()).substr(0, 43) << " |**" << std::endl;
            os << "**|                                                                     |**" << std::endl;
            break;
        case 4:
            break;
    }
    os << "**|_____________________________________________________________________|**" << std::endl;
    os << "***************************************************************************" << std::endl;
    os << "***************************************************************************" << std::endl;
    os << std::endl;
    return os;
}
