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

#include "../Batman5Files/bat_v_elp.h"

namespace inet {

bat_v_elp::bat_v_elp() {}
bat_v_elp::~bat_v_elp() {}

/**
 * createELPPacket: Creating an random ELP-packet
 */
batadv_elp_packet* Batman5Routing::createELPPacket(batadv_hard_iface *hard_iface){

    batadv_elp_packet *packet = new batadv_elp_packet();
    packet->setOrig(hostMAC);
    packet->setSeqno(hard_iface->bat_v.elp_seqno);
    packet->setElp_interval(hard_iface->bat_v.elp_interval);
    hard_iface->bat_v.elp_seqno += 1;

//    std::cout << hostModule->getFullName() << ": Creating ELP-Packet with Bytelength: " << packet->getByteLength() << std::endl;
    if( (metricType.compare("predictiveMobilityMetric") == 0 || metricType.compare("distanceMetric") == 0) && realValues ){
        Coord currentPosition = mobility->getCurrentPosition();
        packet->setX(currentPosition.x);
        packet->setY(currentPosition.y);
        packet->setZ(currentPosition.z);
        packet->setByteLength(packet->getByteLength() + sizeof(currentPosition.x) + sizeof(currentPosition.y) + sizeof(currentPosition.z));
//        std::cout << hostModule->getFullName() << ": Creating ELP-Packet with Bytelength: " << packet->getByteLength() << std::endl;
    }

//    std::cout << hostModule->getFullName() << " created ELP-Packet with seqno " << packet->getSeqno() << std::endl;
    return packet;

}//createELPPacket

batadv_elpProbe_packet* Batman5Routing::createELPProbePacket(batadv_hard_iface *hard_iface){
    batadv_elpProbe_packet *packet = new batadv_elpProbe_packet();
    packet->setOrig(hostMAC);
    packet->setSeqno(hard_iface->bat_v.elp_seqno);
    packet->setElp_interval(hard_iface->bat_v.elp_interval);
    hard_iface->bat_v.elp_seqno += 1;
    packet->setByteLength(par("sizeELPProbes"));
    packet->setTimestamp();
    return packet;
}//createELPProbePacket

/**
 * batadv_v_elp_iface_enable() - setup the ELP interface private resources
 * @hard_iface: interface for which the data has to be prepared
 * Return: 0 on success
 */
int Batman5Routing::batadv_v_elp_iface_enable(batadv_hard_iface *hard_iface){
    if(hard_iface->if_num == 0){
        ELP_reminder_wlan0 = new elp_reminder();
        ELP_reminder_wlan0->hard_iface = hard_iface;
        ELP_reminder_wlan0->iface_id = hard_iface->if_id;
        scheduleAt(0+jitter,ELP_reminder_wlan0);

        // Timeout works for all Interfaces
        // initialized with first possible interface
        hardiface_neighborTableTimeout = new cMessage("neightable_timeout");
        scheduleAt(0+neigh_timeout_interval,hardiface_neighborTableTimeout);
    }
    else if(hard_iface->if_num == 1){
        ELP_reminder_wlan1 = new elp_reminder();
        ELP_reminder_wlan1->hard_iface = hard_iface;
        ELP_reminder_wlan1->iface_id = hard_iface->if_id;
        scheduleAt(0+jitter,ELP_reminder_wlan1);
    }
    else{
        return 1;
    }
    return 0;
}//batadv_v_elp_iface_enable


void Batman5Routing::batadv_v_elp_iface_disable(batadv_hard_iface *hard_iface){
    if(hard_iface->if_num == 0){
        ELP_reminder_wlan0->hard_iface = NULL;
        ELP_reminder_wlan0->iface_id = 0;
        cancelAndDelete(ELP_reminder_wlan0);
        cancelAndDelete(hardiface_neighborTableTimeout);
    }
    if(hard_iface->if_num == 1){
        ELP_reminder_wlan1->hard_iface = NULL;
        ELP_reminder_wlan1->iface_id = 0;
        cancelAndDelete(ELP_reminder_wlan1);
    }

}//batadv_v_elp_iface_disable

/**
 * batadv_v_elp_wifi_neigh_probe: send link probing packets to a neighbour
 *  returns true on success and false in case of error
 */
bool Batman5Routing::batadv_v_elp_wifi_neigh_probe(Hardif_neigh_node_ptr neigh){
    if(elpProbing){
        batadv_hard_iface *hard_iface = neigh->if_incoming;
        double last_tx_diff = 0;

        int numELPProbes = par("numELPProbes");

        /* this probing routine is for Wifi neighbours only */
        if (!batadv_is_wifi_hardif(hard_iface)){
//            std::cout << "Interface is no wifi interface! No elpProbing!" << std::endl;
            return true;
        }

        /* probe the neighbor only if no unicast packets have been sent
         * to it in the last 100 milliseconds: this is the rate control
         * algorithm sampling interval (minstrel). In this way, if not
         * enough traffic has been sent to the neighbor, batman-adv can
         * generate 2 probe packets and push the RC algorithm to perform
         * the sampling
         */

        last_tx_diff = (simTime()- neigh->bat_v->last_unicast_tx).dbl() * 1000 ;
//        std::cout << "Calculate last_tx_diff: " << last_tx_diff << std::endl;
        if (last_tx_diff <= BATADV_ELP_PROBE_MAX_TX_DIFF){
            return true;
        }

//        std::cout << hostModule->getFullName() << " sending ELP-probes" << std::endl;
        for (int i = 0; i < numELPProbes; i++) {
//            std::cout << "Probing Neighbor " << neigh->addr.str() << ": " << i << std::endl;
            sendELPUnicast(neigh);
        }
        return true;
    }
    else{
//        std::cout << "elpProbing is " << elpProbing << std::endl;
        return true;
    }
}//batadv_v_elp_wifi_neigh_probe


/**
 * batadv_v_elp_get_throughput: get the throughput towards a neighbour
 * Return: The throughput towards the given neighbour in multiples of 100kpbs
 *         (a value of '1' equals to 0.1Mbps, '10' equals 1Mbps, etc).
 */
 uint32_t Batman5Routing::batadv_v_elp_get_throughput(Hardif_neigh_node_ptr neigh){
     batadv_hard_iface *hard_iface = neigh->if_incoming;
     uint32_t throughput = 0;
     int ret = 0;

     /* if the user specified a customised value for this interface, then
       * return it directly */
      throughput = hard_iface->bat_v.throughput_override;
      if (throughput != 0)
          return throughput;

      /* if this is a wireless device, then ask its throughput through
       * cfg80211 API*/
      if (batadv_is_wifi_hardif(hard_iface)) {
          if (!batadv_is_cfg80211_hardif(hard_iface)){
//              std::cout << "unsupported WiFi driver version" << std::endl;
              return BATADV_THROUGHPUT_DEFAULT_VALUE;
          }
          auto it = hard_iface->lastProbedThroughput.find(neigh->addr);
          if(it != hard_iface->lastProbedThroughput.end() && it->second != 0){
//              std::cout << hostModule->getFullName() << ": getting lastProbedThroughput = " << hard_iface->lastProbedThroughput << std::endl;
              return it->second;
          }

          if (ret == -2 /*-ENOENT*/) {
              /* Node is not associated anymore! It would be
               * possible to delete this neighbor. For now set
               * the throughput metric to 0. */
              return 0;
          }
      }
      else{
      /* if not a wifi interface, check if this device provides data via
         * ethtool (e.g. an Ethernet adapter) */
//          std::cout << "Ethtool-Throughput is set to Default" << std::endl;
          // This simulation is written for VANETS and does not use ethernet
          // So here the throughput is set to default !

          if (!(hard_iface->bat_v.flags & BATADV_WARNING_DEFAULT)){
              std::cout << "WiFi driver or ethtool info does not provide information about link speeds on interface " << hard_iface->name << " , therefore defaulting to hardcoded throughput values of " << BATADV_THROUGHPUT_DEFAULT_VALUE/10 <<"."<< BATADV_THROUGHPUT_DEFAULT_VALUE%10<< "Mbps." << std::endl;
              hard_iface->bat_v.flags |= BATADV_WARNING_DEFAULT;
          }

          return BATADV_THROUGHPUT_DEFAULT_VALUE;
      }
 }//batadv_v_elp_get_throughput


/**
 * batadv_v_elp_throughput_metric_update:
 * updating the throughput metric of a single hop neighbour
 */
void Batman5Routing::batadv_v_elp_throughput_metric_update(Hardif_neigh_node_ptr neigh_node){

    ewma_metric_add(&neigh_node->bat_v->metric, getLinkMetric(neigh_node));

    auto neigh_search = neigh_node->if_incoming->neigh_table->find(neigh_node->addr);
    if(neigh_search != neigh_node->if_incoming->neigh_table->end()){
        neigh_search->second->hardif_neigh->bat_v->metric = neigh_node->bat_v->metric;
    }
}//batadv_v_elp_throughput_metric_update

/**
 * batadv_v_elp_periodic_work: periodic work of the Echo Location Protocol
 *              1. send broadcast ELP-packet
 *              2. probe known neighbors with unicast elp-packets
 *              3. update throughput
 */
void Batman5Routing::batadv_v_elp_periodic_work(batadv_hard_iface *hard_iface){
    // 1) Create and send ELP-packet
    sendELPBroadcast(hard_iface);

    /* Update Throuput metric on each sent packet:
     * 2) send a number of unicast ELPs for probing/sampling to each neighbor
     * 3) update the throughput metric value of each neighbor */
    for (Neighbor_Table::iterator it = hard_iface->neigh_table->begin(); it != hard_iface->neigh_table->end(); it++){
        //std::cout << hostModule->getFullName() << ": Updating Throughput to known neighbors!" << std::endl;
        if(!batadv_v_elp_wifi_neigh_probe(it->second->hardif_neigh)){
            /* if something goes wrong while probing, better to stop
             * sending packets immediately and reschedule the task */
            break;
        }
        batadv_v_elp_throughput_metric_update(it->second->hardif_neigh);
    }

}//batadv_v_elp_periodic_work

/**
 * handleELPPacket: main ELP packet handler
 * Handle an icoming ELP-packet and update the Neighbortable if necessary
 * returns NET_RX_DROP=1 if failed or NET_RX_SUCCESS=0 if succeded
 */
int Batman5Routing::handleELPPacket(batadv_elp_packet* packet, batadv_hard_iface *if_incoming, MACAddress src, MACAddress dest){
    MACAddress orig_address = packet->getOrig();
    bool res;
    int ret = NET_RX_DROP;

    res = batadv_check_management_packet(packet, if_incoming, BATADV_ELP_HLEN, src, dest);
    if (!res){
        std::cout << "ELP-Packet failed management packet Check! Deleting packet!" << std::endl;
        delete packet;
        return ret;
    }
    // am i the originator of the packet
    if (batadv_is_my_mac(orig_address)){
//        std::cout << "Received my own ELP-Packet! Deleting packet!" << std::endl;
        delete packet;
        return ret;
    }
    /* did we receive a B.A.T.M.A.N. V ELP packet on an interface
     * that does not have B.A.T.M.A.N. V ELP enabled ?
     * in this case not neccessary because only batman v nodes exist in this simulation */

//    std::cout << "Received ELP packet from " << orig_address << " seqno " << packet->getSeqno() << std::endl;

    batadv_v_elp_neigh_update(orig_address, if_incoming, packet);

    /* Getting Throughput from the received ELP Messages */
    auto it = if_incoming->lastProbedThroughput.find(orig_address);
    if(it != if_incoming->lastProbedThroughput.end()){
        it->second = ( (packet->getBitLength()) / (simTime() - packet->getCreationTime()) ) / 1000;
//        std::cout << hostModule->getFullName() << ": Probed Throughput: " << it->second << "kbit/s, " << packet->getBitLength() << " <-> " <<  (simTime() - packet->getCreationTime()) << std::endl;
    }

    // Getting Position from received ELP Message
    if((metricType.compare("predictiveMobilityMetric") == 0 || metricType.compare("distanceMetric") == 0) && realValues){
        auto it = if_incoming->lastProbedPosition.find(orig_address);
        if(it != if_incoming->lastProbedPosition.end()){
            Coord position = Coord(0,0,0);
            position.x = packet->getX();
            position.y = packet->getY();
            position.z = packet->getZ();
            it->second = position;
    //        std::cout << hostModule->getFullName() << ": Probed Position to " << orig_address.str() << ": " << it->second << std::endl;
        }
    }

    ret = NET_RX_SUCCESS;
    emit(received_ELP, packet);
    delete packet;
    return ret;
}// HandleELPPacket


/*
 * ELP-Probes updates the variable lastProbedThroughput of an batadv_hard_iface
 * This can be read out when next time the "Driver" is asked for estimated throughput
 */
void Batman5Routing::handleELPProbePacket(batadv_elpProbe_packet* packet, batadv_hard_iface *if_incoming, MACAddress src, MACAddress dest){
//    std::cout << hostModule->getFullName() << ": Received ELP-Probe: " << packet->getByteLength() << " " << packet->getTimestamp() <<" " << simTime() << std::endl;

    auto it = if_incoming->lastProbedThroughput.find(packet->getOrig());
    if(it != if_incoming->lastProbedThroughput.end()){
        it->second = ( (packet->getBitLength()) / (simTime() - packet->getTimestamp()) ) / 1000;
//        std::cout << hostModule->getFullName() << ": Probed Throughput: " << it->second << "kbit/s" << std::endl;
    }
    delete packet;
}

/**
 * batadv_v_elp_neigh_update:
 * Updates the ELP neighbour node state with the data received within the new ELP packet.
 */
void Batman5Routing::batadv_v_elp_neigh_update(MACAddress neigh_addr, batadv_hard_iface *if_incoming, batadv_elp_packet* elp_packet){
    Neigh_node_ptr neigh = NULL;
    Orig_node_ptr orig_neigh = NULL;
    Hardif_neigh_node_ptr hardif_neigh = NULL;
    int32_t seqno_diff;
    int32_t elp_latest_seqno;

    orig_neigh = batadv_v_ogm_orig_get(neigh_addr);
    if (orig_neigh == NULL){
        // NULL only in case of failure
        return;
    }
    neigh = batadv_neigh_node_get_or_create(orig_neigh, if_incoming, neigh_addr);
    if (neigh == NULL){
            return;
    }

    hardif_neigh = batadv_hardif_neigh_get(if_incoming, neigh_addr);
    if (hardif_neigh == NULL){
            return;
    }

    elp_latest_seqno = hardif_neigh->bat_v->elp_latest_seqno;
    seqno_diff = elp_packet->getSeqno()- elp_latest_seqno;

    /* known or older sequence numbers are ignored. However always adopt
     * if the router seems to have been restarted. */
    if (seqno_diff < 1 && seqno_diff > -BATADV_ELP_MAX_AGE){
        return;
    }

    /* Passed the tests -> update elp neigh */
//    std::cout << hostModule->getFullName() << ": Update ELP Neighbor -> " << neigh_addr << ", " << simTime() << std::endl;
    auto search_neigh = orig_neigh->neigh_list->find(neigh_addr);
    search_neigh->second->last_seen = simTime();
    search_neigh->second->hardif_neigh->last_seen = simTime();
    search_neigh->second->hardif_neigh->bat_v->elp_latest_seqno = elp_packet->getSeqno();

    search_neigh->second->hardif_neigh->bat_v->elp_interval = (double)elp_packet->getElp_interval() / 1000; // uint32 in ms to simtime

//    std::cout << "uint32_t in ms to simtime: " << elp_packet->getElp_interval() << " -> " << (double)elp_packet->getElp_interval()/1000 << ", current: " << search_neigh->second->hardif_neigh->bat_v->elp_interval << endl;

    // if neighbor node is not known yet
    auto probeSearch = if_incoming->lastProbedThroughput.find(neigh_addr);
    if (probeSearch == if_incoming->lastProbedThroughput.end()){
        // Add new lastProbedThroughput-entry for neighbor
        if_incoming->lastProbedThroughput.insert(std::pair<MACAddress,uint32_t>(neigh_addr, 0));
    }

    // if neighbor node is not known yet
    if((metricType.compare("predictiveMobilityMetric") == 0 || metricType.compare("distanceMetric") == 0) && realValues){
        auto probeSearch = if_incoming->lastProbedPosition.find(neigh_addr);
        if (probeSearch == if_incoming->lastProbedPosition.end()){
           // Add new lastProbedPosition-entry for neighbor
           Coord position = Coord(0,0,0);
           position.x = elp_packet->getX();
           position.y = elp_packet->getY();
           position.z = elp_packet->getZ();
           if_incoming->lastProbedPosition.insert(std::pair<MACAddress,Coord>(neigh_addr, position));
        }
    }


}//batadv_v_elp_neigh_update

} /* namespace inet */
