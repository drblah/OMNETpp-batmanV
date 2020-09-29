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

#include "../Batman5Files/routing.h"

namespace inet {

routing::routing() {}
routing::~routing() {}


/**
 * _batadv_update_route() - set the router for this originator
 * @orig_node: orig node which is to be configured
 * @recv_if: the receive interface for which this route is set
 * @neigh_node: neighbor which should be the next router
 *
 * This function does not perform any error checks
 */
void Batman5Routing::_batadv_update_route(Orig_node_ptr orig_node, batadv_hard_iface *recv_if, Neigh_node_ptr neigh_node){
    Orig_ifinfo_ptr orig_ifinfo;
    Neigh_node_ptr curr_router;
//    std::cout << "Starting _batadv_update_route" << std::endl;
    orig_ifinfo = batadv_orig_ifinfo_get(orig_node, recv_if);
    if (orig_ifinfo == NULL)
        return;

    /* curr_router used earlier may not be the current orig_ifinfo->router
     * anymore because it was dereferenced outside of the neigh_list_lock
     * protected region. After the new best neighbor has replace the current
     * best neighbor the reference counter needs to decrease. Consequently,
     * the code needs to ensure the curr_router variable contains a pointer
     * to the replaced best neighbor. */
    curr_router = orig_ifinfo->router;

    /* increase refcount of new best neighbor */
    if (neigh_node)
        neigh_node->refcount++;

    orig_ifinfo->router = neigh_node;

    /* route deleted */
    if (curr_router && !neigh_node) {
//        std::cout << hostModule-getFullName() << ": Deleting route towards: " <<  orig_node->orig.str() << std::endl;
        batadv_tt_global_del_orig(orig_node, -1, "Deleted route towards originator");

    /* route added */
    } else if (!curr_router && neigh_node) {
//        std::cout << hostModule->getFullName() << ": Adding route towards: " << orig_node->orig.str() << " (via " << neigh_node->addr.str() << ") "<< std::endl;

    /* route changed */
    } else if (neigh_node && curr_router) {
//        std::cout << hostModule->getFullName() << ": Changing route towards: " << orig_node->orig.str() << " (now via " << neigh_node->addr.str() << " - was via " << curr_router->addr.str() << std::endl;
    }
}//_batadv_update_route

/**
 * batadv_update_route() - set the router for this originator performing checks before
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: orig node which is to be configured
 * @recv_if: the receive interface for which this route is set
 * @neigh_node: neighbor which should be the next router
 */
void Batman5Routing::batadv_update_route(Orig_node_ptr orig_node, batadv_hard_iface *recv_if, Neigh_node_ptr neigh_node){
    Neigh_node_ptr router = NULL;
//    std::cout << hostModule->getFullName() << "Start batadv_update_route" << std::endl;
    if (orig_node == NULL)
        return;

    router = batadv_orig_router_get(orig_node, recv_if);

    if (router != neigh_node){
        _batadv_update_route(orig_node, recv_if, neigh_node);
    }
}//batadv_update_route


/**
 * batadv_check_management_packet: Check preconditions for management packets
 *
 */
bool Batman5Routing::batadv_check_management_packet(cPacket *packet, batadv_hard_iface *hard_iface, int header_len, MACAddress src, MACAddress dest){

    /* drop packet if it has not necessary minimum size */
    if (packet->getByteLength() < header_len){
        return false;
    }

    /* packet with broadcast indication but unicast recipient */
    if(!dest.isBroadcast()){
        return false;
    }

    /* packet with invalid sender address */
    if(src.isBroadcast() || src.isMulticast()){
        return false;
    }

    return true;
}


/**
 * batadv_window_protected() - checks whether the host restarted and is in the protection time.
 * @seq_num_diff: difference between the current/received sequence number and the last sequence number
 * @seq_old_max_diff: maximum age of sequence number not considered as restart
 * @last_reset: jiffies timestamp of the last reset, will be updated when reset is detected
 * @protection_started: is set to true if the protection window was started, doesn't change otherwise.
 *
 * Return: false if the packet is to be accepted.
 *         true  if the packet is to be ignored.
 */
bool Batman5Routing::batadv_window_protected(int32_t seq_num_diff, int32_t seq_old_max_diff, simtime_t *last_reset, bool *protection_started){
    if (seq_num_diff <= -seq_old_max_diff || seq_num_diff >= BATADV_EXPECTED_SEQNO_RANGE) {
        if (!batadv_has_timed_out(*last_reset, BATADV_RESET_PROTECTION_MS))
            return true;

        *last_reset = simTime();
        if (protection_started)
            *protection_started = true;
//        std::cout << hostModule->getFullName() << ": old packet received, start protection" << std::endl;
    }

    return false;
}//batadv_window_protected


/**
 * batadv_find_router() - find a suitable router for this originator
 * @orig_node: the destination node
 * @recv_if: pointer to interface this packet was received on
 *
 * Return: the router which should be used for this orig_node on
 * this interface, or NULL if not available.
 */
Neigh_node_ptr Batman5Routing::batadv_find_router(Orig_node_ptr orig_node, batadv_hard_iface *recv_if){
    Neigh_node_ptr first_candidate_router = NULL;
    Neigh_node_ptr next_candidate_router = NULL;
    Neigh_node_ptr router, cand_router = NULL;
    Neigh_node_ptr last_cand_router = NULL;

    Orig_ifinfo_ptr cand, first_candidate = NULL;
    Orig_ifinfo_ptr next_candidate = NULL;
    Orig_ifinfo_ptr last_candidate = NULL;

    bool last_candidate_found = false;
    bool cand_router_is_NULL = false;

    if (!orig_node){
        return NULL;
    }

    router = batadv_orig_router_get(orig_node, recv_if);

    if (!router){
        return router;
    }

    /* only consider bonding for recv_if == BATADV_IF_DEFAULT (first hop) and if activated. */
    if (!(recv_if == BATADV_IF_DEFAULT && bonding)){
        return router;
    }

    /* bonding: loop through the list of possible routers found
     * for the various outgoing interfaces and find a candidate after
     * the last chosen bonding candidate (next_candidate). If no such
     * router is found, use the first candidate found (the previously
     * chosen bonding candidate might have been the last one in the list).
     * If this can't be found either, return the previously chosen
     * router - obviously there are no other candidates. */

//    last_candidate = batadv_last_bonding_get(orig_node);
    if (last_candidate)
        last_cand_router = last_candidate->router;

    for (std::vector<Orig_ifinfo_ptr>::iterator it = orig_node->ifinfo_list.begin(); it != orig_node->ifinfo_list.end(); it++){
        cand = (*it);
        cand_router = cand->router;
        if (!cand_router){
            cand_router_is_NULL = true;
        }
        if(cand_router_is_NULL){
            /* alternative candidate should be good enough to be considered */
            if (batadv_v_neigh_is_sob(cand_router, cand->if_outgoing, router, recv_if)){

                /* don't use the same router twice */
                if (!(last_cand_router == cand_router)){

                    /* mark the first possible candidate */
                    if (!first_candidate) {
                        first_candidate = cand;
                        first_candidate_router = cand_router;
                    }

                    /* check if the loop has already passed the previously selected
                     * candidate ... this function should select the next candidate
                     * AFTER the previously used bonding candidate. */
                    if (!last_candidate || last_candidate_found) {
                        next_candidate = cand;
                        next_candidate_router = cand_router;
                        break;
                    }

                    if (last_candidate == cand){
                        last_candidate_found = true;
                    }
                }// if (!(last_cand_router == cand_router))
            }//if(neigh.is_similar_or_better)
        }//if(cand_router_is_NULL)

        /* free references */
        if (cand_router != NULL) {
            cand_router = NULL;
        }

    }//for

    /* After finding candidates, handle the three cases:
     * 1) there is a next candidate, use that
     * 2) there is no next candidate, use the first of the list
     * 3) there is no candidate at all, return the default router
     */
    if (next_candidate) {
        router = next_candidate_router;
//        batadv_last_bonding_replace(orig_node, next_candidate);
    }
    else if (first_candidate) {
        router = first_candidate_router;
//        batadv_last_bonding_replace(orig_node, first_candidate);
    }
    else {
//        batadv_last_bonding_replace(orig_node, NULL);
    }

    return router;
}


/**
 * batadv_recv_my_icmp_packet() - receive an icmp packet locally
 * @bat_priv: the bat priv with all the soft interface information
 * @skb: icmp packet to process
 *
 * Return: NET_RX_SUCCESS if the packet has been consumed or NET_RX_DROP
 * otherwise.
 */
int Batman5Routing::batadv_recv_my_icmp_packet(batadv_icmp_packet *icmp_packet){
    batadv_hard_iface *primary_if = NULL;
    Orig_node_ptr orig_node = NULL;
    int res, ret = NET_RX_DROP;

    switch(icmp_packet->getMsg_type()) {
        case BATADV_ECHO_REPLY:
        case BATADV_DESTINATION_UNREACHABLE:
        case BATADV_TTL_EXCEEDED:
            /* receive the packet => send icmp_packet to userspace */
//            batadv_socket_receive_packet(icmph, skb->len);
            break;

        case BATADV_ECHO_REQUEST:
            /* answer echo request (ping) */
            primary_if = batadv_primary_if_get_selected();
            if (primary_if == NULL){
                return ret;
            }

            /* get routing information */
            orig_node = batadv_orig_find(icmp_packet->getOrig());
            if (orig_node == NULL)
                return ret;

            icmp_packet->setDst(icmp_packet->getOrig());
            icmp_packet->setOrig(get_hardif_MAC(primary_if->if_id));
            icmp_packet->setMsg_type(BATADV_ECHO_REPLY);
            icmp_packet->setTtl(BATADV_TTL);

            res = batadv_send_packet_to_orig(icmp_packet, orig_node, NULL);
            if (res == NET_XMIT_SUCCESS){
                ret = NET_RX_SUCCESS;
            }
            break;

        case BATADV_TP:

            batadv_tp_meter_recv(dynamic_cast<batadv_icmp_tp_packet *>(icmp_packet));
            ret = NET_RX_SUCCESS;

        default:
            /* drop unknown type */
            return ret;

        }//switch

    return ret;

}//batadv_recv_my_icmp_packet


int Batman5Routing::batadv_recv_icmp_ttl_exceeded(batadv_icmp_packet *icmp_packet){
    batadv_hard_iface *primary_if = NULL;
    Orig_node_ptr orig_node = NULL;
    int res, ret = NET_RX_DROP;

    /* send TTL exceeded if packet is an echo request (traceroute) */
    if (icmp_packet->getMsg_type() != BATADV_ECHO_REQUEST) {
//        std::cout << hostModule->getFullName() << ": Warning - can't forward icmp packet from " << icmp_packet->getOrig() << " to " << icmp_packet->getDst() << ": ttl exceeded" << std::endl;
        return ret;
    }

    primary_if = batadv_primary_if_get_selected();
    if (primary_if == NULL){
        return ret;
    }

    /* get routing information */
    orig_node = batadv_orig_find(icmp_packet->getOrig());
    if (orig_node == NULL){
        return ret;
    }

    icmp_packet->setDst(icmp_packet->getOrig());
    icmp_packet->setOrig(get_hardif_MAC(primary_if->if_id));
    icmp_packet->setMsg_type(BATADV_TTL_EXCEEDED);
    icmp_packet->setTtl(BATADV_TTL);

    res = batadv_send_packet_to_orig(icmp_packet, orig_node, NULL);
    if (res == NET_RX_SUCCESS){
        ret = NET_XMIT_SUCCESS;
    }

    return ret;
}//batadv_recv_icmp_ttl_exceeded


/**
 * batadv_recv_icmp_packet() - Process incoming icmp packet
 * @skb: incoming packet buffer
 * @recv_if: incoming hard interface
 *
 * Return: NET_RX_SUCCESS on success or NET_RX_DROP in case of failure
 */
int Batman5Routing::batadv_recv_icmp_packet(batadv_icmp_packet *icmp_packet, batadv_hard_iface *recv_if, MACAddress ethhdr_src, MACAddress ethhdr_dest){
    batadv_icmp_packet_rr *icmp_packet_rr;
    Orig_node_ptr orig_node = NULL;
    int res, ret = NET_RX_DROP;

    /* packet with unicast indication but non-unicast recipient */
    if (!is_valid_ether_addr(ethhdr_dest)){
        return ret;
    }

    /* packet with broadcast/multicast sender address */
    if (ethhdr_src.isMulticast() || ethhdr_src.isBroadcast()){
        return ret;
    }

    /* not for me */
    if (!batadv_is_my_mac(ethhdr_dest))
        return ret;

    /* add record route information if not full */
    if ((icmp_packet->getMsg_type() == BATADV_ECHO_REPLY || icmp_packet->getMsg_type() == BATADV_ECHO_REQUEST)){

        icmp_packet_rr = dynamic_cast<batadv_icmp_packet_rr *>(icmp_packet->dup());
        if (icmp_packet_rr->getRr_cur() >= BATADV_RR_LEN){
            return ret;
        }

        icmp_packet_rr->setRr(icmp_packet_rr->getRr_cur(),ethhdr_dest);
        icmp_packet_rr->setRr_cur(icmp_packet_rr->getRr_cur() + 1);
    }

    /* packet for me */
    if (batadv_is_my_mac(icmp_packet->getDst())){
        return batadv_recv_my_icmp_packet(icmp_packet);
    }

    /* TTL exceeded */
    if (icmp_packet->getTtl() < 2){
        return batadv_recv_icmp_ttl_exceeded(icmp_packet);
    }

    /* get routing information */
    orig_node = batadv_orig_find(icmp_packet->getDst());
    if (orig_node == NULL){
        return ret;
    }

    /* decrement ttl */
    icmp_packet->setTtl(icmp_packet->getTtl() - 1);

    /* route it */
    res = batadv_send_packet_to_orig(icmp_packet, orig_node, recv_if);
    if (res == NET_XMIT_SUCCESS){
        ret = NET_RX_SUCCESS;
    }

    return ret;
}//batadv_recv_icmp_packet


/**
 * batadv_check_unicast_packet() - Check for malformed unicast packets
 * @bat_priv: the bat priv with all the soft interface information
 * @skb: packet to check
 * @hdr_size: size of header to pull
 *
 * Check for short header and bad addresses in given packet.
 *
 * Return: negative value when check fails and 0 otherwise. The negative value
 * depends on the reason: -ENODATA for bad header, -EBADR for broadcast
 * destination or source, and -EREMOTE for non-local (other host) destination.
 */
int Batman5Routing::batadv_check_unicast_packet(batadv_unicast_packet *unicast_packet, int hdr_size, MACAddress ethhdr_src, MACAddress ethhdr_dest){

    /* drop packet if it has not necessary minimum size */

    /* packet with unicast indication but non-unicast recipient */
    if (!is_valid_ether_addr(ethhdr_dest)){
        if(ethhdr_dest.isBroadcast()){
            // make an exception for UDP BroadcastApp
            return 0;
        }
        return -EBADR;
    }
    /* packet with broadcast/multicast sender address */
    if (ethhdr_src.isMulticast() || ethhdr_src.isBroadcast())
        return -EBADR;

    /* not for me */
    if (!batadv_is_my_mac(ethhdr_dest))
        return -EREMOTE;

    return 0;
}//batadv_check_unicast_packet


/**
 * batadv_last_bonding_get() - Get last_bonding_candidate of orig_node
 * @orig_node: originator node whose last bonding candidate should be retrieved
 *
 * Return: last bonding candidate of router or NULL if not found
 *
 * The object is returned with refcounter increased by 1.
 */
Orig_ifinfo_ptr Batman5Routing::batadv_last_bonding_get(Orig_node_ptr orig_node){
    Orig_ifinfo_ptr last_bonding_candidate = NULL;

    last_bonding_candidate = orig_node->last_bonding_candidate;

    return last_bonding_candidate;
}//batadv_last_bonding_get



/**
 * batadv_last_bonding_replace() - Replace last_bonding_candidate of orig_node
 * @orig_node: originator node whose bonding candidates should be replaced
 * @new_candidate: new bonding candidate or NULL
 */
void Batman5Routing::batadv_last_bonding_replace(Orig_node_ptr orig_node, Orig_ifinfo_ptr new_candidate){

    orig_node->last_bonding_candidate = new_candidate;

}//batadv_last_bonding_replace



int Batman5Routing::batadv_route_unicast_packet(cPacket *packet, batadv_hard_iface *recv_if, MACAddress ethhdr_src, MACAddress ethhdr_dest){
    Orig_node_ptr orig_node = NULL;
    batadv_unicast_packet *unicast_packet;
    int res, ret = NET_RX_DROP;

    unicast_packet = dynamic_cast<batadv_unicast_packet *>(packet);

    /* TTL exceeded */
    if (unicast_packet->getTtl() < 2) {
//        std::cout << hostModule->getFullName() << ": Warning - can't forward unicast packet from " << ethhdr_src << " to " << unicast_packet->getDest() << ": ttl exceeded" << std::endl;
        delete unicast_packet;
        return ret;
    }

    /* get routing information */
    orig_node = batadv_orig_find(unicast_packet->getDest());
    if (orig_node == NULL){
//        std::cout << hostModule->getFullName() << ": Warning - can't forward unicast packet from " << ethhdr_src << " to " << unicast_packet->getDest() << ": orig_node = NULL" << std::endl;
        delete unicast_packet;
        return ret;
    }

    /* decrement ttl */
    unicast_packet->setTtl(unicast_packet->getTtl() - 1);

    res = batadv_send_packet_to_orig(unicast_packet, orig_node, recv_if);

    /* translate transmit result into receive result */
    if (res == NET_XMIT_SUCCESS) {
        ret = NET_RX_SUCCESS;

//        std::cout << hostModule->getFullName() << ": Send unicast_packet " << simTime() << std::endl;
    }
    else if(res == -EINVAL){
//        std::cout << "EINVAL" << std::endl;
        delete unicast_packet;
    }
    else if(res == -EINPROGRESS){
//        std::cout << "EINPROGRESS" << std::endl;
        delete unicast_packet;
    }

    return ret;
}//batadv_route_unicast_packet

/**
 * batadv_recv_unhandled_unicast_packet() - receive and process packets which
 *  are in the unicast number space but not yet known to the implementation
 * @skb: unicast tvlv packet to process
 * @recv_if: pointer to interface this packet was received on
 *
 * Return: NET_RX_SUCCESS if the packet has been consumed or NET_RX_DROP
 * otherwise.
 */
int Batman5Routing::batadv_recv_unhandled_unicast_packet(batadv_unicast_packet *unicast_packet, batadv_hard_iface *recv_if, MACAddress ethhdr_src, MACAddress ethhdr_dest){
    int check, hdr_size = sizeof(*unicast_packet);

    check = batadv_check_unicast_packet(unicast_packet, hdr_size, ethhdr_src, ethhdr_dest);
    if (check < 0){
        return NET_RX_DROP;
    }

    /* we don't know about this type, drop it. */
    if (batadv_is_my_mac(unicast_packet->getDest())){
        return NET_RX_DROP;
    }

    return batadv_route_unicast_packet(unicast_packet, recv_if, ethhdr_src, ethhdr_dest);
}//batadv_recv_unhandled_unicast_packet


/**
 * batadv_recv_unicast_packet() - Process incoming unicast packet
 * @skb: incoming packet buffer
 * @recv_if: incoming hard interface
 *
 * Return: NET_RX_SUCCESS on success or NET_RX_DROP in case of failure
 */
int Batman5Routing::batadv_recv_unicast_packet(batadv_unicast_packet *unicast_packet, batadv_hard_iface *recv_if, MACAddress ethhdr_src, MACAddress ethhdr_dest){
//    std::cout << hostModule->getFullName() << "recv_unicast_packet: " << std::endl;
    batadv_unicast_4addr_packet *unicast_4addr_packet = NULL;
    batadv_unicast_packet *packet = NULL;
    MACAddress orig_addr, orig_addr_gw;
    Orig_node_ptr orig_node = NULL, orig_node_gw = NULL;
    int check, hdr_size = sizeof(*unicast_packet);
    uint8_t subtype;
    int ret = NET_RX_DROP;
    bool is4addr = false, is_gw = false;

    packet = unicast_packet;

    // Update lastProbedThroughput over the receiver-interface
    Hardif_neigh_node_ptr neigh = batadv_hardif_neigh_get(recv_if, ethhdr_src);
    if(neigh != nullptr){
//        auto it = recv_if->lastProbedThroughput.find(ethhdr_src);
//        if (it != recv_if->lastProbedThroughput.end())
//            it->second = ( packet->getBitLength() / (simTime() - packet->getTimestamp()) );
//        std::cout << "ProbedThroughput from a unicast-packet: " << recv_if->lastProbedThroughput << std::endl;
    }
    else{
//        std::cout << hostModule->getFullName() << "Neighbor not found!" << std::endl;
    }

    if (dynamic_cast<batadv_unicast_4addr_packet *>(unicast_packet) != NULL){
        unicast_4addr_packet = dynamic_cast<batadv_unicast_4addr_packet *>(unicast_packet);
        packet = unicast_4addr_packet;
        is4addr = true;
    }
    /* the caller function should have already pulled 2 bytes */
    if (is4addr){
        hdr_size = BATADV_UNICAST_4ADDR_HLEN;
    }
    /* function returns -EREMOTE for promiscuous packets */
    check = batadv_check_unicast_packet(packet, hdr_size, ethhdr_src, ethhdr_dest);
//    std::cout << hostModule->getFullName() << "Check: " << check << std::endl;

    /* Even though the packet is not for us, we might save a duplicate to use for
     * decoding a later received coded packet */
    if (check == -EREMOTE && networkCoding){
        batadv_nc_packet_store_sniffed_unicast(packet);
        delete packet;
        return ret;
    }

    if (check < 0){
        delete packet;
        return ret;
    }

    /* packet for me */
    if (batadv_is_my_mac(packet->getDest()) || packet->getDest().isBroadcast()) {
        /* If this is a unicast packet from another backgone gw, drop it. */
        orig_addr_gw = ethhdr_src;
        orig_node_gw = batadv_orig_find(orig_addr_gw);
        if (orig_node_gw != NULL) {
//            is_gw = batadv_bla_is_backbone_gw(orig_node_gw, hdr_size);
            if (is_gw) {
//                std::cout << hostModule->getFullName() << ": Dropped unicast pkt received from another backbone gw " << orig_addr_gw.str() << std::endl;
                delete packet;
                return ret;
            }
        }

        if (is4addr) {
            subtype = unicast_4addr_packet->getSubtype();
//            batadv_dat_inc_counter(bat_priv, subtype);

            /* Only payload data should be considered for speedy
             * join. For example, DAT also uses unicast 4addr
             * types, but those packets should not be considered
             * for speedy join, since the clients do not actually
             * reside at the sending originator. */
            if (subtype == BATADV_P_DATA) {
                orig_addr = unicast_4addr_packet->getSrc();
                orig_node = batadv_orig_find(orig_addr);
            }
        }

//        std::cout << hostModule->getFullName() << "Received Unicast-Packet for me from " << unicast_packet->getSrc() << std::endl;
        batadv_interface_rx(packet, NULL); // unicast_packets does not have an originator

        return NET_RX_SUCCESS;
    }

    ret = batadv_route_unicast_packet(packet, recv_if, ethhdr_src, ethhdr_dest);

    return ret;
}//batadv_recv_unicast_packet


/**
 * batadv_recv_unicast_tvlv() - receive and process unicast tvlv packets
 * @skb: unicast tvlv packet to process
 * @recv_if: pointer to interface this packet was received on
 *
 * Return: NET_RX_SUCCESS if the packet has been consumed or NET_RX_DROP
 * otherwise.
 */
int Batman5Routing::batadv_recv_unicast_tvlv(batadv_unicast_tvlv_packet *packet, batadv_hard_iface *recv_if, MACAddress ethhdr_src, MACAddress ethhdr_dest){
    unsigned char *tvlv_buff = NULL;
    int ret = NET_RX_DROP;
    uint16_t tvlv_buff_len = packet->getTvlv_len();

    ret = batadv_tvlv_containers_process(false, NULL, packet->getSrc(), packet->getDst(), tvlv_buff, tvlv_buff_len);

    return ret;
}//batadv_recv_unicast_tvlv


/**
 * batadv_recv_frag_packet() - process received fragment
 * @skb: the received fragment
 * @recv_if: interface that the skb is received on
 *
 * This function does one of the three following things: 1) Forward fragment, if
 * the assembled packet will exceed our MTU; 2) Buffer fragment, if we till
 * lack further fragments; 3) Merge fragments, if we have all needed parts.
 *
 * Return: NET_RX_DROP if the skb is not consumed, NET_RX_SUCCESS otherwise.
 */
int Batman5Routing::batadv_recv_frag_packet(batadv_frag_packet *packet, batadv_hard_iface *recv_if, MACAddress ethhdr_src, MACAddress ethhdr_dest){
    return 0;
}//batadv_recv_frag_packet



/**
 * batadv_recv_bcast_packet() - Process incoming broadcast packet
 * @skb: incoming packet buffer
 * @recv_if: incoming hard interface
 *
 * Return: NET_RX_SUCCESS on success or NET_RX_DROP in case of failure
 */
int Batman5Routing::batadv_recv_bcast_packet(batadv_bcast_packet *bcast_packet, batadv_hard_iface *recv_if, MACAddress ethhdr_src, MACAddress ethhdr_dest){
        Orig_node_ptr orig_node = NULL;
//        int hdr_size = sizeof(*bcast_packet);
        int ret = NET_RX_DROP;
        int32_t seq_diff;
        uint32_t seqno;

        /* drop packet if it has not necessary minimum size */

        /* packet with broadcast indication but unicast recipient */
        if (!ethhdr_dest.isBroadcast()){
            return ret;
        }

        /* packet with broadcast/multicast sender address */
        if (ethhdr_src.isMulticast()){
            return ret;
        }

        /* ignore broadcasts sent by myself */
        if (batadv_is_my_mac(ethhdr_src)){
            return ret;
        }

        /* ignore broadcasts originated by myself */
        if (batadv_is_my_mac(bcast_packet->getOrig())){
            return ret;
        }

        /* TTL expired? */
        if (bcast_packet->getTtl() < 2){
            return ret;
        }

        orig_node = batadv_orig_find(bcast_packet->getOrig());
        if (orig_node == NULL){
            return ret;
        }

        seqno = bcast_packet->getSeqno();
        /* check whether the packet is a duplicate */
        if (batadv_test_bit(/*orig_node->bcast_bits, */orig_node->last_bcast_seqno, seqno)){
            return ret;
        }

        seq_diff = seqno - orig_node->last_bcast_seqno;

        /* check whether the packet is old and the host just restarted. */
        if (batadv_window_protected(seq_diff, BATADV_BCAST_MAX_AGE, &orig_node->bcast_seqno_reset, NULL)){
           return ret;
        }

        /* mark broadcast in flood history, update window position if required. */
        if (batadv_bit_get_packet(/*orig_node->bcast_bits, */seq_diff, 1)){
            orig_node->last_bcast_seqno = seqno;
        }

        /* check whether this has been sent by another originator before */
//        if (batadv_bla_check_bcast_duplist(bcast_packet)){
//            return ret;
//        }

        /* rebroadcast packet */
//        batadv_add_bcast_packet_to_list(bcast_packet, 1, false);

        /* don't hand the broadcast up if it is from an originator from the same backbone. */
//        if (batadv_bla_is_backbone_gw(bcast_packet, orig_node, hdr_size)){
//            return ret;
//        }

//        if (batadv_dat_snoop_incoming_arp_request(bat_priv, skb, hdr_size)){
//            ret = NET_RX_SUCCESS;
//            return ret;
//        }

//        if (batadv_dat_snoop_incoming_arp_reply(bcast_packet, hdr_size)){
//            ret = NET_RX_SUCCESS;
//            return ret;
//        }

        /* broadcast for me */
        batadv_interface_rx(bcast_packet, orig_node);
        ret = NET_RX_SUCCESS;
        return ret;
}//batadv_recv_bcast_packet




} /* namespace inet */
