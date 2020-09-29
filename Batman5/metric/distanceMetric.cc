//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "../../Batman5/metric/distanceMetric.h"

namespace inet {

cModule* Batman5Routing::getHostFromMac(MACAddress &_mac)
{
    cModule *host = 0;
    std::string id = "host";
    int i = (int)_mac.getAddressByte(5) - 1;
    cModule *node = this->getSystemModule()->getSubmodule(id.c_str(), i);

//    std::cout << hostModule->getFullName() << " -> " << node->getFullName() << std::endl;

    if(node)
    {
        Batman5Routing *routing = dynamic_cast<Batman5Routing*>(node->getSubmodule("Batman5Routing"));
        if(routing && routing->get_hardif_MAC("wlan0")==_mac)
        {
//            std::cout << "getHostFromMAC: Everythings is ok! Return node!" << std::endl;
            return node;
        }
    }

    return host;
}

cModule* Batman5Routing::getMobilityFromMAC(MACAddress &_mac){
    cModule *mobility = nullptr;

    cModule *host = getHostFromMac(_mac);
    if(host){
        mobility = host->getSubmodule("mobility");
        if(mobility){
//            std::cout << "getMobilityFromMAC: Everything is ok! Return mobility!" << std::endl;
        }
        else{
//            std::cout << hostModule->getFullName() << " to " << host->getFullName() << " getMobilityFromMAC: Nothing is ok! mobility is nullptr!" << std::endl;
        }
    }

    return mobility;
}

Coord Batman5Routing::getPositionFromMAC(MACAddress &_mac){
    Coord positionOfMAC = Coord(0,0,0);

    MobilityBase *mobility = dynamic_cast<MobilityBase*>(getMobilityFromMAC(_mac));

    if(mobility){
        positionOfMAC = mobility->getCurrentPosition();
    }

    return positionOfMAC;
}

double Batman5Routing::getDistanceToMAC(MACAddress &_mac){

    double distance = 0;
    Coord myPosition = Coord(0,0,0);
    Coord neighPosition = Coord (0,0,0);

    MobilityBase *mobility = dynamic_cast<MobilityBase*>(this->getParentModule()->getSubmodule("mobility"));

    if(mobility)
        myPosition = mobility->getCurrentPosition();

    if(realValues){
        auto it = wlan0->lastProbedPosition.find(_mac);
        if(it != wlan0->lastProbedPosition.end()){
            neighPosition = it->second;
//            std::cout << hostModule->getFullName() << ": " << neighPosition << " - " << getPositionFromMAC(_mac) << " = " << (neighPosition - getPositionFromMAC(_mac)).length() << std::endl;
        }
    }
    else{
        neighPosition = getPositionFromMAC(_mac);
    }

    if(neighPosition == Coord(0,0,0)){
        return distance;
    }

    distance = (myPosition - neighPosition).length();

//    std::cout << hostModule->getFullName() << ": current Positions: " << myPosition << " <-> " << neighPosition << " => " << distance << std::endl;

    return distance;

}

uint32_t Batman5Routing::getDistanceMetric(MACAddress &_mac){

    uint32_t metric = 0;
    double maxDistance = check_and_cast<physicallayer::IMediumLimitCache*>(this->getModuleByPath("radioMedium.mediumLimitCache"))->getMaxCommunicationRange().get();
    double distance = getDistanceToMAC(_mac);


    int alpha = distance_alpha;
    double index = powf((distance/maxDistance),alpha); // 0: very close ,1: far away
//    std::cout << "index: " << index << std::endl;

    if(index > 1){
        // distance is longer than maxDistance
        index = 1.0;
    }
    else if(index < 0){
        index = 0.0;
    }

    uint32_t cakePiece = (BATADV_THROUGHPUT_MAX_VALUE-1) / ogmv2_ttl;
    metric = map(index,0,1,0,cakePiece);

//    std::cout << hostModule->getFullName() << ": distance: " << distance << ", metric: " << metric << " to " << _mac.str() << endl;

    return metric;
}


distanceMetric::distanceMetric() {}
distanceMetric::~distanceMetric() {}

} /* namespace inet */
