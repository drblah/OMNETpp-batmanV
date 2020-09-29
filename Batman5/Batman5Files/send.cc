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

#include "../Batman5Files/send.h"
namespace inet {

send::send() {}
send::~send() {}

/**
 * sendUDPUnicast: sending UDP unicast packet
 */
void Batman5Routing::sendUDPUnicast(UDPPacket *udp_packet, MACAddress dest, MACAddress src){
    int ret;

    batadv_unicast_packet *unicast_packet = batadvUnicastEncapsulation(udp_packet, dest, src);

    if(dest.isBroadcast()){
//        std::cout << hostModule->getFullName() << " send UDP Broadcast" << std::endl;
        ret = batadv_send_broadcast_packet(unicast_packet,primary_iface);
    }
    else{
        ret = batadv_send_packet_to_orig(unicast_packet, batadv_orig_find(dest), primary_iface);
    }
    if(ret == NET_XMIT_SUCCESS){
//        std::cout << hostModule->getFullName() << "Packet was send successful" << std::endl;
        emit(sent_UDP, unicast_packet);
    }
    else if(ret == -EINVAL){
//        std::cout << "Could not find router!" << std::endl;
        emit(noRouter_found_UDP,unicast_packet);
        delete unicast_packet;
    }
    else if(ret == -EINPROGRESS){
//        std::cout << "UDP-Packet will be send over Network Coding mechanism!" << std::endl;
    }

}//sendUDPUnicast

/**
 * sendTCP: sending the TCP packets as unicast
 */
void Batman5Routing::sendTCP(cPacket *packet, MACAddress dest, MACAddress src){
    int ret;
    std::string packetName = packet->getFullName();

    batadv_unicast_packet *unicast_packet = batadvUnicastEncapsulation(packet, dest, src);
    ret = batadv_send_packet_to_orig(unicast_packet, batadv_orig_find(dest), primary_iface);

    if(ret == NET_XMIT_SUCCESS){
//        std::cout << hostModule->getFullName() << ": " << packetName << " was send successful" << std::endl;
    }
    else if(ret == -EINVAL){
//        std::cout << hostModule->getFullName() << ": Sending" << packetName << " failed -> Could not find router!" << std::endl;
        delete unicast_packet;
    }
    else if(ret == -EINPROGRESS){
//        std::cout << "TCP-Packet will be send over Network Coding mechanism!" << std::endl;
    }

}//sendTCP


/**
 * sendELPBroadcast: Will be called with elp_reminder->batadv_v_elp_periodic_work
 *                   Creating and sending own ELP-packets and scheduling next reminder
 */
void Batman5Routing::sendELPBroadcast(batadv_hard_iface *hard_iface){
/* Interface will be shut down */
    if (hard_iface->if_status == BATADV_IF_NOT_IN_USE || hard_iface->if_status == BATADV_IF_TO_BE_REMOVED)
        return;

/* Interface was enabled but may not be ready yet */
    if (hard_iface->if_status != BATADV_IF_ACTIVE)
        return;

/* Create ELP-Packet, encapsulate it and send it per broadcast out */
    batadv_elp_packet *elp_packet = createELPPacket(hard_iface);
    EthernetIIFrame* framed_elp = EthernetPacketEncapsulation(elp_packet, elp_packet->getOrig(), MACAddress("ff:ff:ff:ff:ff:ff"));
    Ieee802CtrlEncapsulation(framed_elp, MACAddress("ff:ff:ff:ff:ff:ff"), hard_iface);
    emit(sent_ELP, framed_elp);
    emit(sent_packet_to_lower,framed_elp);
    send(framed_elp,"lowerlayerOut", hard_iface->if_num);
}//sendELPBroadcast

/**
 * sendELPUnicast:
 */
void Batman5Routing::sendELPUnicast(Hardif_neigh_node_ptr neigh){
    batadv_hard_iface *if_outgoing = neigh->if_incoming;

    /* Interface will be shut down */
    if (if_outgoing->if_status == BATADV_IF_NOT_IN_USE || if_outgoing->if_status == BATADV_IF_TO_BE_REMOVED)
        return;

    /* Interface was enabled but may not be ready yet */
    if (if_outgoing->if_status != BATADV_IF_ACTIVE)
        return;

    /* Create ELP-Packet, encapsulate it and send it per broadcast out */
    batadv_elpProbe_packet *elpProbe_packet = createELPProbePacket(if_outgoing);
    EthernetIIFrame* framed_elp = EthernetPacketEncapsulation(elpProbe_packet, elpProbe_packet->getOrig(), neigh->addr);
    Ieee802CtrlEncapsulation(framed_elp, neigh->addr, if_outgoing);
    emit(sent_ELP_probes, framed_elp);
    emit(sent_packet_to_lower,framed_elp);
    send(framed_elp,"lowerlayerOut",if_outgoing->if_num);
//    std::cout << "Sent ELP-Probe!" << std::endl;

}//sendELPUnicast


/**
 * batadvUnicastEncapsulation: Encapsulate a packet into a batadv_unicast_packet
 */
batadv_unicast_packet* Batman5Routing::batadvUnicastEncapsulation(cPacket* packet, MACAddress dest, MACAddress src){
    batadv_unicast_packet *uni_packet = new batadv_unicast_packet();
    uni_packet->setTtl(ogmv2_ttl);
    uni_packet->setTtvn(0);
    uni_packet->setDest(dest);
    uni_packet->setSrc(src);
    uni_packet->setH_source(src);
    uni_packet->setH_dest(dest);
    uni_packet->setPacket_id(getRandomId());
    uni_packet->encapsulate(packet);
    return uni_packet;
}//batadvUnicastEncapsulation


/**
 * EthernetPacketEncapsulation: Encapsulate the packet into an ethernetframe
 */
EthernetIIFrame* Batman5Routing::EthernetPacketEncapsulation(cPacket* packet, MACAddress Src, MACAddress Dest, int ethertype){
    /*   Ethernet Packet Encapsulation   */
    EthernetIIFrame* Ethframe = new EthernetIIFrame();
    Ethframe->setSrc(Src);
    Ethframe->setDest(Dest);
    Ethframe->setEtherType(ethertype);
    Ethframe->encapsulate(packet);
    return Ethframe;
}//EthernetPacketEncapsulation



/**
 * IPv4CtrlEncapsulation: Add IPv4 control info to packet
 */
void Batman5Routing::IPv4CtrlEncapsulation(cPacket *packet, L3Address dest, L3Address src, uint8_t transportProtocolID){
    IPv4ControlInfo *controlInfo = new IPv4ControlInfo();
    controlInfo->setTransportProtocol(transportProtocolID); // this is value has to be set but for UDPSink the value itself is not important
    controlInfo->setDestinationAddress(dest);
    controlInfo->setSourceAddress(src);
    IPv4Datagram *datagram = new IPv4Datagram();
    controlInfo->setOrigDatagram(datagram);
    packet->setControlInfo(controlInfo);
}//IPv4CtrlEncapsulation

/**
 * UDPDataIndication: Add UDP ControlInfo to packet
 * Neccessary for UDPVideoStream!
 */
void Batman5Routing::UDPDataIndicationEncapsulation(cPacket *packet, L3Address srcAddr, int srcPort, L3Address destAddr, int destPort){
    UDPDataIndication *controlInfo = new UDPDataIndication();
    controlInfo->setSrcAddr(srcAddr);
    controlInfo->setSrcPort(srcPort);
    controlInfo->setDestPort(destPort);
    controlInfo->setDestAddr(destAddr);
    packet->setControlInfo(controlInfo);
}

/**
 * Ieee802CtrlEncapsulation: Add Ieee802 control info to ethernet frame
 */
void Batman5Routing::Ieee802CtrlEncapsulation(EthernetIIFrame *packet, MACAddress dest, batadv_hard_iface *hard_if){
    /*   Add ControlInfo for WLAN   */
    Ieee802Ctrl *controlInfo = new Ieee802Ctrl();
    controlInfo->setDest(dest);
    controlInfo->setEtherType(0x4305); // B.A.T.M.A.N.-Advanced packet [ NOT AN OFFICIALLY REGISTERED ID ]
    controlInfo->setInterfaceId(hard_if->if_num);
    packet->setControlInfo(controlInfo);
}//Ieee802CtrlEncapsulation


/**
 * batadv_send_broadcast_packet: Send broadcast packet via hard interface
 * Return: A negative errno code is returned on a failure. A success does not
 * guarantee the frame will be transmitted as it may be dropped due
 * to congestion or traffic shaping.
 */
int Batman5Routing::batadv_send_broadcast_packet(cPacket* packet, batadv_hard_iface *hard_iface){

    return batadv_send_packet(packet, hard_iface, MACAddress("ff:ff:ff:ff:ff:ff"));

}//batadv_send_broadcast_packet


/**
 * batadv_send_unicast_skb() - Send unicast packet to neighbor
 * @skb: packet to be transmitted (with batadv header and no outer eth header)
 * @neigh: neighbor which is used as next hop to destination
 *
 * Return: A negative errno code is returned on a failure. A success does not
 * guarantee the frame will be transmitted as it may be dropped due
 * to congestion or traffic shaping.
 */
int Batman5Routing::batadv_send_unicast_packet(cPacket *packet, Neigh_node_ptr neigh){
    Hardif_neigh_node_ptr hardif_neigh = NULL;
    int ret;

//    std::cout << hostModule->getFullName() << ": Sending unicast packet to " << neigh->addr.str() << std::endl;
    ret = batadv_send_packet(packet, neigh->if_incoming, neigh->addr);

    hardif_neigh = batadv_hardif_neigh_get(neigh->if_incoming, neigh->addr);
    if (hardif_neigh != NULL && ret != NET_XMIT_DROP){
        hardif_neigh->bat_v->last_unicast_tx = simTime();
        emit(forward_unicast, packet);
    }

    return ret;
}//batadv_send_unicast_skb

/**
 * batadv_send_packet: send an already prepared packet
 * Return: A negative errno code is returned on a failure. A success does not
 * guarantee the frame will be transmitted as it may be dropped due
 * to congestion or traffic shaping.
 */
int Batman5Routing::batadv_send_packet(cPacket *packet, batadv_hard_iface *hard_iface, MACAddress dst_addr){

    batadv_unicast_packet *uni_packet = nullptr;
    EthernetIIFrame* framed_packet = nullptr;

    if (hard_iface->if_status != BATADV_IF_ACTIVE){
        delete packet;
        return NET_XMIT_DROP;
    }

    packet->setTimestamp();

    if (networkCoding && dynamic_cast<batadv_unicast_packet *>(packet) != NULL){
        uni_packet = dynamic_cast<batadv_unicast_packet *>(packet)->dup();
    }
//    std::cout << hostModule->getFullName() << ": Encapsulate and Send packet!" << std::endl;
    if (networkCoding && dynamic_cast<batadv_coded_packet *>(packet) != NULL){
        std::cout << "Sending coded_packet" << std::endl;
        framed_packet = EthernetPacketEncapsulation(packet, hostMAC, MACAddress("ff:ff:ff:ff:ff:ff"));
        Ieee802CtrlEncapsulation(framed_packet, MACAddress("ff:ff:ff:ff:ff:ff"), hard_iface);
    }
    else{
        framed_packet = EthernetPacketEncapsulation(packet, hostMAC, dst_addr);
        Ieee802CtrlEncapsulation(framed_packet, dst_addr, hard_iface);
    }

    emit(sent_packet_to_lower,framed_packet);
    send(framed_packet,"lowerlayerOut", hard_iface->if_num);

    if (networkCoding && dynamic_cast<batadv_coded_packet *>(packet) != NULL){
//        std::cout << "coded_packet send" << std::endl;
    }

    if(!dst_addr.isBroadcast()){
//        std::cout << hostModule->getFullName() << ": Send packet to " << dst_addr.str() << "!" << std::endl;
    }

    /* Save a clone of the skb to use when decoding coded packets */
    if(networkCoding && uni_packet != nullptr){
//        std::cout << hostModule->getFullName() << " batadv_send_packet: Save unicast_packet for Network Coding" << std::endl;
        batadv_nc_packet_store_for_decoding(uni_packet);
    }

    return NET_XMIT_SUCCESS;
}// batadv_send_packet


/**
 * batadv_send_skb_to_orig() - Lookup next-hop and transmit skb.
 * @skb: Packet to be transmitted.
 * @orig_node: Final destination of the packet.
 * @recv_if: Interface used when receiving the packet (can be NULL).
 *
 * Looks up the best next-hop towards the passed originator and passes the
 * skb on for preparation of MAC header. If the packet originated from this
 * host, NULL can be passed as recv_if and no interface alternating is
 * attempted.
 *
 * Return: negative errno code on a failure, -EINPROGRESS if the skb is
 * buffered for later transmit or the NET_XMIT status returned by the
 * lower routine if the packet has been passed down.
 */
int Batman5Routing::batadv_send_packet_to_orig(cPacket *packet, Orig_node_ptr orig_node, batadv_hard_iface *recv_if){
    Neigh_node_ptr neigh_node;
    int ret;
//    std::cout << hostModule->getFullName() << ": Send packet to orig " << orig_node->orig.str() << std::endl;

    /* batadv_find_router() increases neigh_nodes refcount if found. */
    neigh_node = batadv_find_router(orig_node, recv_if);

    if (neigh_node == NULL) {
        ret = -EINVAL;
        return ret;
    }

    /* Check if the skb is too large to send in one piece and fragment it if needed. */
    if (fragmentation && packet->getByteLength() > neigh_node->if_incoming->mtu) {
        int ret= 0;
        /* Fragment and send packet. */
//        ret = batadv_frag_send_packet(packet, orig_node, neigh_node);
        /* skb was consumed */

        return ret;
    }

    /* try to network code the packet, if it is received on an interface
     * (i.e. being forwarded). If the packet originates from this node or if
     * network coding fails, then send the packet as usual. */
    if (recv_if != NULL && dynamic_cast<batadv_unicast_packet *>(packet) != NULL && batadv_nc_packet_forward(dynamic_cast<batadv_unicast_packet *>(packet), neigh_node)){
//        std::cout << hostModule->getFullName() << " Network Coding: batadv_send_packet_to_orig" << std::endl;
//        delete packet;
        ret = -EINPROGRESS;
    }
    else{
        ret = batadv_send_unicast_packet(packet, neigh_node);
    }

    return ret;

}//batadv_send_packet_to_orig


/**
 * batadv_send_skb_push_fill_unicast() - extend the buffer and initialize the
 *  common fields for unicast packets
 * @skb: the skb carrying the unicast header to initialize
 * @hdr_size: amount of bytes to push at the beginning of the skb
 * @orig_node: the destination node
 *
 * Return: false if the buffer extension was not possible or true otherwise.
 */
bool Batman5Routing::batadv_send_push_fill_unicast(cPacket *packet, int hdr_size, Orig_node_ptr orig_node){
    batadv_unicast_packet *unicast_packet = NULL;
    uint32_t ttvn = orig_node->last_ttvn;

    unicast_packet = new batadv_unicast_packet();
    /* set unicast ttl */
    unicast_packet->setTtl(ogmv2_ttl);
    /* copy the destination for faster routing */
    unicast_packet->setDest(orig_node->orig);
    /* set the destination tt version number */
    unicast_packet->setTtvn(ttvn);

    unicast_packet->encapsulate(packet);
    packet = unicast_packet;
    return true;
}//batadv_send_push_fill_unicast


/**
 * batadv_send_skb_prepare_unicast() - encapsulate an skb with a unicast header
 * @skb: the skb containing the payload to encapsulate
 * @orig_node: the destination node
 *
 * Return: false if the payload could not be encapsulated or true otherwise.
 */
bool Batman5Routing::batadv_send_prepare_unicast(cPacket *packet, Orig_node_ptr orig_node){
    std::size_t uni_size = sizeof(batadv_unicast_packet);

    return batadv_send_push_fill_unicast(packet, uni_size, orig_node);

}//batadv_send_skb_prepare_unicast



/**
 * batadv_send_skb_prepare_unicast_4addr() - encapsulate an skb with a
 *  unicast 4addr header
 * @bat_priv: the bat priv with all the soft interface information
 * @skb: the skb containing the payload to encapsulate
 * @orig: the destination node
 * @packet_subtype: the unicast 4addr packet subtype to use
 *
 * Return: false if the payload could not be encapsulated or true otherwise.
 */
bool Batman5Routing::batadv_send_prepare_unicast_4addr(cPacket *packet, Orig_node_ptr orig, int packet_subtype){
    batadv_hard_iface *primary_if = NULL;
    batadv_unicast_4addr_packet *uc_4addr_packet = NULL;
    batadv_unicast_packet * unicast_packet = NULL;
    bool ret = false;

    if (dynamic_cast<batadv_unicast_packet *>(packet) != NULL){
        unicast_packet = dynamic_cast<batadv_unicast_packet *>(packet);
    }

    primary_if = batadv_primary_if_get_selected();
    if (primary_if == NULL){
        return ret;
    }

    /* Pull the header space and fill the unicast_packet substructure.
     * We can do that because the first member of the uc_4addr_packet
     * is of type struct unicast_packet */
    if (!batadv_send_push_fill_unicast(packet, sizeof(*uc_4addr_packet), orig)){
        return ret;
    }

    uc_4addr_packet = new batadv_unicast_4addr_packet();
    uc_4addr_packet->setSrc(get_hardif_MAC(primary_if->if_id));
    uc_4addr_packet->setSubtype(packet_subtype);
    uc_4addr_packet->setReserved(0);
    uc_4addr_packet->setTtl(unicast_packet->getTtl());
    uc_4addr_packet->setTtvn(unicast_packet->getTtvn());
    uc_4addr_packet->setDest(unicast_packet->getDest());

    packet = uc_4addr_packet;
    ret = true;

    return ret;
}//batadv_send_skb_prepare_unicast_4addr


/**
 * batadv_send_skb_unicast() - encapsulate and send an skb via unicast
 * @bat_priv: the bat priv with all the soft interface information
 * @skb: payload to send
 * @packet_type: the batman unicast packet type to use
 * @packet_subtype: the unicast 4addr packet subtype (only relevant for unicast
 *  4addr packets)
 * @orig_node: the originator to send the packet to
 * @vid: the vid to be used to search the translation table
 *
 * Wrap the given skb into a batman-adv unicast or unicast-4addr header
 * depending on whether BATADV_UNICAST or BATADV_UNICAST_4ADDR was supplied
 * as packet_type. Then send this frame to the given orig_node.
 *
 * Return: NET_XMIT_DROP in case of error or NET_XMIT_SUCCESS otherwise.
 */
int Batman5Routing::batadv_send_packet_unicast(cPacket *packet, int packet_type, int packet_subtype, Orig_node_ptr orig_node, unsigned short vid){
    batadv_unicast_packet *unicast_packet;
    MACAddress ethhdr;
    int ret = NET_XMIT_DROP;

    if(orig_node == NULL){
        return ret;
    }

    switch (packet_type) {
    case BATADV_UNICAST:
        if (!batadv_send_prepare_unicast(packet, orig_node)){
            return ret;
        }
        unicast_packet = dynamic_cast<batadv_unicast_packet *>(packet);
        break;
    case BATADV_UNICAST_4ADDR:
        if (!batadv_send_prepare_unicast_4addr(packet, orig_node, packet_subtype)){
            return ret;
        }
        unicast_packet = dynamic_cast<batadv_unicast_packet *>(packet);
        break;
    default:
        /* this function supports UNICAST and UNICAST_4ADDR only. It
         * should never be invoked with any other packet type
         */
        return ret;
    }

    /* inform the destination node that we are still missing a correct route
     * for this client. The destination will receive this packet and will
     * try to reroute it because the ttvn contained in the header is less
     * than the current one */
    if (false){ //batadv_tt_global_client_is_roaming(ethhdr, vid)){
        unicast_packet->setTtvn(unicast_packet->getTtvn() - 1);
    }

    ret = batadv_send_packet_to_orig(unicast_packet, orig_node, NULL);

    return ret;
}//batadv_send_skb_unicast



/**
 * batadv_send_skb_via_gw() - send an skb via gateway lookup
 * @bat_priv: the bat priv with all the soft interface information
 * @skb: payload to send
 * @vid: the vid to be used to search the translation table
 *
 * Look up the currently selected gateway. Wrap the given skb into a batman-adv
 * unicast header and send this frame to this gateway node.
 *
 * Return: NET_XMIT_DROP in case of error or NET_XMIT_SUCCESS otherwise.
 */
int Batman5Routing::batadv_send_packet_via_gw(cPacket *packet, unsigned short vid){
    Orig_node_ptr orig_node = NULL;
    int ret;

//    orig_node = batadv_gw_get_selected_orig();
    ret = batadv_send_packet_unicast(packet, BATADV_UNICAST_4ADDR, BATADV_P_DATA, orig_node, vid);

    return ret;
}//batadv_send_skb_via_gw




} /* namespace inet */
