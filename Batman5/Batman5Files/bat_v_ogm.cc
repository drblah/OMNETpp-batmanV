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

#include "../Batman5Files/bat_v_ogm.h"

namespace inet {

bat_v_ogm::bat_v_ogm() {}
bat_v_ogm::~bat_v_ogm() {}


///**
// * createOGMv2Packet: Create an default OGMv2-Packet
// * first try to simulate the changing ogmv2 size because of tvlv
// */
//batadv_ogm2_packet* Batman5Routing::createOGMv2Packet(){
//    bool tt_vlan = true; // 50B OGMv2
//    bool tt_change = false;
//    bool tt_vlan_change = false; // 62B OGMv2
//    bool usingNC = networkCoding;
//    int TVLV_Bytes = 0;
//
//    batadv_ogm2_packet *packet = new batadv_ogm2_packet();
//    packet->setTtl(ogmv2_ttl);
//    packet->setFlags(BATADV_NO_FLAGS);
//    packet->setSeqno(ogmv2_seqno);
//    packet->setOrig(hostMAC);
//    packet->setMetric(BATADV_THROUGHPUT_MAX_VALUE);
//    packet->setPrev_sender(MACAddress("00:00:00:00:00:00"));
//
//    // Configure ByteLength for the tvlv containers
//    if(usingNC){
//        batadv_tvlv_nc *nc_packet = new batadv_tvlv_nc();
//        TVLV_Bytes = 4;
//        packet->encapsulate(nc_packet);
//    }
//
//    if(tt_change){
//        batadv_tvlv_tt_change *tvlv_packet = new batadv_tvlv_tt_change();
//        packet->setTvlv_len(20-4 + TVLV_Bytes);
//        packet->encapsulate(tvlv_packet);
//    }
//    else if(tt_vlan_change){
//        batadv_tvlv_tt_vlan_change *tvlv_packet = new batadv_tvlv_tt_vlan_change();
//        packet->setTvlv_len(24 + TVLV_Bytes);
//        packet->encapsulate(tvlv_packet);
//    }
//    else if(tt_vlan){
//        batadv_tvlv_tt_vlan *tvlv_packet = new batadv_tvlv_tt_vlan();
//        packet->setTvlv_len(16-4 + TVLV_Bytes);
//        packet->encapsulate(tvlv_packet);
//    }
//    else{
////        tvlv_packet = NULL;
//        packet->setTvlv_len(0);
//        packet->setByteLength(20 + TVLV_Bytes);
//    }
//
//    ogmv2_seqno += 1;
//    //std::cout << hostModule->getFullName() << " created OGMv2 with seqno " << packet->getSeqno() << " and ttl " << (int)packet->getTtl() << std::endl;
//    return packet;
//
//}//createOGMv2Packet

/**
 * batadv_v_ogm_orig_get: retrieve and possibly create an originator node
 * Return: the orig_node corresponding to the specified address. If such object
 * does not exist it is allocated here. In case of allocation failure returns NULL.
 */
Orig_node_ptr Batman5Routing::batadv_v_ogm_orig_get(MACAddress addr){
    Orig_node_ptr orig_node = NULL;
//    std::cout << "Starting batadv_v_ogm_orig_get, ";

    orig_node = batadv_orig_find(addr);
    if (orig_node != NULL)
        return orig_node;

    orig_node = batadv_orig_node_new(addr);
    if (orig_node == NULL)
        return NULL;

    // new originator-table-entry
    orig_table.insert(std::pair<MACAddress, Orig_node_ptr>(orig_node->orig,orig_node));

    return orig_node;
}

/**
 * batadv_v_ogm_send_to_if: send a batman ogm using a given interface
 */
void Batman5Routing::batadv_v_ogm_send_to_if(batadv_ogm2_packet *ogmv2_packet, batadv_hard_iface *hard_iface){

    if (hard_iface->if_status != BATADV_IF_ACTIVE)
        return;

    int ret = batadv_send_broadcast_packet(ogmv2_packet, hard_iface);

    if (ret ==0){
        // Success on batadv_send_packet
//        std::cout << hostModule->getFullName() << ": Success on sending OGMv2-Packet over " << hard_iface->name << endl;
        emit(sent_OGMv2, ogmv2_packet);
    }
    else{
//        std::cout << hostModule->getFullName() << ": Sending OGMv2-Packet failed over " << hard_iface->name << endl;
    }
}

/**
 * batadv_v_ogm_send: periodic worker broadcasting the own OGM
 */
void Batman5Routing::batadv_v_ogm_send(){
    batadv_hard_iface *hard_iface = NULL;
    batadv_ogm2_packet *ogmv2_packet;
    OGMv2_buff ogm_buff;
    int ogm_buff_len;
    uint16_t tvlv_len;
    int ret;

    ogm_buff = priv_bat_v->ogm_buff;
    ogm_buff_len = priv_bat_v->ogm_buff_len;

/* ATTENTION:
 * tt functionality is not implemented in this version of the model
 * because tt is not neccessary for the use of Batman5 in VANETs !
 * The generel functions have been transfered to this model but they are
 * not filled with functionality.
 * With this a later implementation of tt functions is possible!
 */

    /* tt changes have to be committed before the tvlv data is
     * appended as it may alter the tt tvlv container */
    batadv_tt_local_commit_changes();
    tvlv_len = batadv_tvlv_container_ogm_append(ogm_buff, ogm_buff_len, BATADV_OGM2_HLEN);

    priv_bat_v->ogm_buff = ogm_buff;
    priv_bat_v->ogm_buff_len = ogm_buff_len;

    ogmv2_packet = priv_bat_v->ogm_buff.front().dup();
    ogmv2_packet->setSeqno(priv_bat_v->ogm_seqno);
    priv_bat_v->ogm_seqno += 1;


    if(bat_priv_tt.vlan_expanded){
        ogmv2_packet->setTvlv_len(28);
        ogmv2_packet->setByteLength(62);
        bat_priv_tt.vlan_expanded = false;
    }
    else{
        ogmv2_packet->setTvlv_len(16);
        ogmv2_packet->setByteLength(50);
    }

    /* broadcast on every interface */
    for (int i=0; i < p_interfaceTable->getNumInterfaces(); i++){
        hard_iface = getHardif_by_ifID(p_interfaceTable->getInterface(i)->getInterfaceId());

        ret = batadv_hardif_no_broadcast(hard_iface, MACAddress("00:00:00:00:00:00"), MACAddress("00:00:00:00:00:00"));
        if (ret) {
            std::string type;
            switch (ret) {
            case BATADV_HARDIF_BCAST_NORECIPIENT:
                type = "no neighbor";
                break;
            case BATADV_HARDIF_BCAST_DUPFWD:
                type = "single neighbor is source";
                break;
            case BATADV_HARDIF_BCAST_DUPORIG:
                type = "single neighbor is originator";
                break;
            case 99:
                type = "all checks failed";
                break;
            default:
                type = "unknown";
            }
//            std::cout << hostModule->getFullName() << ": OGM2 from ourselves on " << hard_iface->name << " suppressed: " << type << " " << ret << std::endl;
            continue;
        }

//        std::cout << hostModule->getFullName() << ": Sending own OGMv2 packet (originator " << ogmv2_packet->getOrig().str() << ", seqno " << ogmv2_packet->getSeqno() << ", throughput " << ogmv2_packet->getMetric() << ", TTL " << (int)ogmv2_packet->getTtl() << ") on interface "<< hard_iface->name << " TVLV-Length: " <<  ogmv2_packet->getTvlv_len() << ", Size: " << ogmv2_packet->getByteLength() << std::endl;

        /* this ogmv2-packet gets consumed by batadv_v_ogm_send_to_if() */
        // sending duplicate of ogmv2_packet so all interfaces can broadcast the same ogmv2_packet
        batadv_v_ogm_send_to_if(ogmv2_packet->dup(), hard_iface);
    }
    delete ogmv2_packet;

}//batadv_v_ogm_send


/**
 * batadv_v_ogm_iface_enable() - prepare an interface for B.A.T.M.A.N. V
 * @hard_iface: the interface to prepare
 * Takes care of scheduling own OGM sending routine for this interface.
 * Return: 0 on success or a negative error code otherwise
 */
int Batman5Routing::batadv_v_ogm_enable(){

    priv_bat_v->ogm_buff_len = BATADV_OGM2_HLEN;
    batadv_ogm2_packet *ogm_packet = new batadv_ogm2_packet();
    ogm_packet->setTtl(ogmv2_ttl);
    ogm_packet->setFlags(BATADV_NO_FLAGS);
    ogm_packet->setMetric(BATADV_THROUGHPUT_MAX_VALUE);
    ogm_packet->setOrig(hostMAC);

    priv_bat_v->ogm_buff.push_back(*ogm_packet);
    delete ogm_packet;
    /* randomize initial seqno to avoid collision */
    priv_bat_v->ogm_seqno = getRandomSeqno();

    /* Initialize OMNeT++ selfMessages */
    if(this->OGMv2_reminder == NULL){
        OGMv2_reminder = new cMessage("ogmv2_reminder");
        scheduleAt(0+ogmv2_interval+jitter,OGMv2_reminder);
    }

    if(this->originatorTableTimeout == NULL){
        originatorTableTimeout = new cMessage("origTableTimeout");
        scheduleAt(0+orig_timeout_interval,originatorTableTimeout);
    }

    return 0;
}// batadv_v_ogm_iface_enable

void Batman5Routing::batadv_v_ogm_disable(){
    for(OGMv2_buff::iterator it = priv_bat_v->ogm_buff.begin(); it != priv_bat_v->ogm_buff.end(); it++){
        batadv_ogm2_packet *delete_packet = &(*it);
        if(delete_packet != NULL){
            delete delete_packet;
        }
    }

    priv_bat_v->ogm_buff_len = 0;
    priv_bat_v->ogm_seqno = 0;

    if (OGMv2_reminder != NULL){
        if(this->OGMv2_reminder->isScheduled()){
//            std::cout << "Check OGMv2_reminder" << std::endl;
            cancelAndDelete(OGMv2_reminder);
        }
    }

    if (originatorTableTimeout != NULL){
        if(this->originatorTableTimeout->isScheduled()){
//            std::cout << "Check OrigTable_TimeOut" << std::endl;
            cancelAndDelete(originatorTableTimeout);
        }
    }

}// batadv_v_ogm_iface_disable

/**
 * batadv_v_forward_penalty() - apply a penalty to the throughput metric forwarded with B.A.T.M.A.N. V OGMs
 * @if_incoming: the interface where the OGM has been received
 * @if_outgoing: the interface where the OGM has to be forwarded to
 * @throughput: the current throughput
 *
 * Apply a penalty on the current throughput metric value based on the
 * characteristic of the interface where the OGM has been received. The return
 * value is computed as follows:
 * - throughput * 50%          if the incoming and outgoing interface are the
 *                             same WiFi interface and the throughput is above
 *                             1MBit/s
 * - throughput                if the outgoing interface is the default
 *                             interface (i.e. this OGM is processed for the
 *                             internal table and not forwarded)
 * - throughput * hop penalty  otherwise
 *
 * Return: the penalised throughput metric.
 */
uint32_t Batman5Routing::batadv_v_forward_penalty(batadv_hard_iface *if_incoming, batadv_hard_iface *if_outgoing, uint32_t throughput){
    int hop_penalty = this->hop_penalty;
    int hop_penalty_max = BATADV_TQ_MAX_VALUE;

    /* Don't apply hop penalty in default originator table. */
    if (if_outgoing == BATADV_IF_DEFAULT)
        return throughput;

    /* Forwarding on the same WiFi interface cuts the throughput in half
     * due to the store & forward characteristics of WIFI.
     * Very low throughput values (under 1.0Mbits) are the exception. */
    if (throughput > 1000 && if_incoming == if_outgoing && !(if_incoming->bat_v.flags & BATADV_FULL_DUPLEX)){
//        std::cout << "Halbierung: " << throughput << " -> " << throughput / 2 << std::endl;
        return throughput / 2;
    }

    /* hop penalty of 255 equals 100% */
//    std::cout << "HopPenalty: " << throughput << " -> " << throughput * (hop_penalty_max - hop_penalty) / hop_penalty_max << std::endl;
    return throughput * (hop_penalty_max - hop_penalty) / hop_penalty_max;
}

/**
 * batadv_v_ogm_forward() - check conditions and forward an OGM to the given outgoing interface
 * @ogm_received: previously received OGM to be forwarded
 * @orig_node: the originator which has been updated
 * @neigh_node: the neigh_node through with the OGM has been received
 * @if_incoming: the interface on which this OGM was received on
 * @if_outgoing: the interface to which the OGM has to be forwarded to
 *
 * Forward an OGM to an interface after having altered the throughput metric and
 * the TTL value contained in it. The original OGM isn't modified.
 */
void Batman5Routing::batadv_v_ogm_forward(batadv_ogm2_packet *ogm_received, Orig_node_ptr orig_node, Neigh_node_ptr neigh_node,
                                          batadv_hard_iface *if_incoming, batadv_hard_iface *if_outgoing)
{
    Neigh_ifinfo_ptr neigh_ifinfo = NULL;
    Orig_ifinfo_ptr orig_ifinfo = NULL;
    Neigh_node_ptr router = NULL;
    batadv_ogm2_packet *ogm_forward;

    /* only forward for specific interfaces, not for the default one. */
    if (if_outgoing == BATADV_IF_DEFAULT){
        return;
    }

    orig_ifinfo = batadv_orig_ifinfo_new(orig_node, if_outgoing);
    if (!orig_ifinfo){
        return;
    }

    /* acquire possibly updated router */
    router = batadv_orig_router_get(orig_node, if_outgoing);
    /* strict rule: forward packets coming from the best next hop only */
    if (neigh_node != router){
        return;
    }

    /* don't forward the same seqno twice on one interface */
    if (orig_ifinfo->last_seqno_forwarded == ogm_received->getSeqno()){
        return;
    }

    orig_ifinfo->last_seqno_forwarded = ogm_received->getSeqno();

    // Check TTL of ogm_packet
    if (ogm_received->getTtl() <= 1) {
//        std::cout << "TTL exceeded from OGMv2 with seqno " << ogm_received->getSeqno() << std::endl;
        return;
    }

    neigh_ifinfo = batadv_neigh_ifinfo_get(neigh_node, if_outgoing);
    if (!neigh_ifinfo){
        return;
    }

    /* apply forward penalty */
    ogm_forward = ogm_received->dup();
    ogm_forward->setMetric(neigh_ifinfo->bat_v->metricValue);
    ogm_forward->setTtl(ogm_forward->getTtl()-1);
//    std::cout << "Forwarding OGM2 packet on " << if_outgoing->name << ": throughput " << ogm_forward->getThroughput() << ", ttl " << ogm_forward->getTtl() << ", received via " << if_incoming->name << std::endl;
    batadv_v_ogm_send_to_if(ogm_forward, if_outgoing);
}// batadv_v_ogm_forward

/**
 * batadv_v_ogm_metric_update() - update route metric based on OGM
 * @ogm2: OGM2 structure
 * @orig_node: Originator structure for which the OGM has been received
 * @neigh_node: the neigh_node through with the OGM has been received
 * @if_incoming: the interface where this packet was received
 * @if_outgoing: the interface for which the packet should be considered
 *
 * Return:
 *  1  if the OGM is new,
 *  0  if it is not new but valid,
 *  <0 on error (e.g. old OGM)
 */
int Batman5Routing::batadv_v_ogm_metric_update(batadv_ogm2_packet *ogm2, Orig_node_ptr orig_node, Neigh_node_ptr neigh_node,
                                               batadv_hard_iface *if_incoming, batadv_hard_iface *if_outgoing){
    Orig_ifinfo_ptr orig_ifinfo;
    Neigh_ifinfo_ptr neigh_ifinfo = NULL;
    bool protection_started = false;
    int ret = -EINVAL; //= -22      /* Invalid argument */
    uint32_t path_metric;
    int32_t seq_diff;

//    std::cout << "Starting batadv_v_ogm_metric_update" << std::endl;

    orig_ifinfo = batadv_orig_ifinfo_new(orig_node, if_outgoing);
    if (!orig_ifinfo){
        return ret;
    }

    seq_diff = ogm2->getSeqno() - orig_ifinfo->last_real_seqno;

    if (!orig_node->neigh_list->empty() && batadv_window_protected(seq_diff, BATADV_OGM_MAX_AGE, &orig_ifinfo->batman_seqno_reset, &protection_started)) {

        std::cout << "Drop packet: packet within window protection time from " << ogm2->getOrig().str() << std::endl;
//        std::cout << "Last reset: " << orig_ifinfo->batman_seqno_reset << ", " << simTime() << std::endl;
        return ret;
    }

    /* drop packets with old seqnos, however accept the first packet after
     * a host has been rebooted. */
    if (seq_diff < 0 && !protection_started){
        return ret;
    }

    neigh_node->last_seen = simTime();
    orig_node->last_seen = simTime();

    orig_ifinfo->last_real_seqno = ogm2->getSeqno();
    orig_ifinfo->last_ttl = ogm2->getTtl();

    neigh_ifinfo = batadv_neigh_ifinfo_new(neigh_node, if_outgoing);
    if (neigh_ifinfo == NULL)
        return ret;

    path_metric = calculatePathMetric(if_incoming, if_outgoing, ogm2->getMetric());

    // save recent path_metric in lastMetricValue-vector and write the mean in metricValue
//    addReversePathMetric(neigh_ifinfo, path_metric);
//    path_metric = getReversePathMetric(neigh_ifinfo);

    neigh_ifinfo->bat_v->metricValue = path_metric;
    neigh_ifinfo->bat_v->last_seqno = ogm2->getSeqno();
    neigh_ifinfo->last_ttl = ogm2->getTtl();

    if (seq_diff > 0 || protection_started){
        ret = 1;
    }
    else{
        ret = 0;
    }
//    std::cout << "Ending batadv_v_ogm_metric_update" << std::endl;
    return ret;
} //batadv_v_ogm_metric_update


/**
 * batadv_v_ogm_route_update() - update routes based on OGM
 * @ethhdr: the Ethernet header of the OGM2
 * @ogm2: OGM2 structure
 * @orig_node: Originator structure for which the OGM has been received
 * @neigh_node: the neigh_node through with the OGM has been received
 * @if_incoming: the interface where this packet was received
 * @if_outgoing: the interface for which the packet should be considered
 *
 * Return: true if the packet should be forwarded, false otherwise
 */
bool Batman5Routing::batadv_v_ogm_route_update(MACAddress ethhdr, batadv_ogm2_packet *ogm2, Orig_node_ptr orig_node,
                      Neigh_node_ptr neigh_node, batadv_hard_iface *if_incoming, batadv_hard_iface *if_outgoing)
{
//    std::cout << "Starting batadv_v_ogm_route_update, ";
    Neigh_node_ptr router = NULL; // Direct Single-Hop Neighbor
    Orig_node_ptr orig_neigh_node = NULL; //
    Neigh_node_ptr orig_neigh_router = NULL;
    Neigh_ifinfo_ptr router_ifinfo = NULL, neigh_ifinfo = NULL;
    uint32_t router_throughput, neigh_throughput;
    uint32_t router_last_seqno;
    uint32_t neigh_last_seqno;
    int32_t neigh_seq_diff;
    bool forward = false;

    orig_neigh_node = batadv_v_ogm_orig_get(ethhdr); // If Orig_node is known get it, if not create a new one, return value is NULL on error
    if (orig_neigh_node == NULL){
        return forward;
    }

    orig_neigh_router = batadv_orig_router_get(orig_neigh_node, if_outgoing);

    /* drop packet if sender is not a direct neighbor and if we don't route towards it */
    router = batadv_orig_router_get(orig_node, if_outgoing);

    if (router && router->orig_node != orig_node && !orig_neigh_router) {
//        std::cout << hostModule->getFullName() << ": Drop packet: OGM via unknown neighbor: " << ogm2->getOrig().str() << std::endl;
        return forward;
    }

    /* Mark the OGM to be considered for forwarding, and update routes if needed. */
    forward = true;
//    std::cout << "Searching and updating originator entry of received packet" << std::endl;

    /* if this neighbor already is our next hop there is nothing to change */
    if (router == neigh_node){
//        std::cout << hostModule->getFullName() << ": neighbor already is our next hop there is nothing to change here" << std::endl;
        return forward;
    }

    /* don't consider neighbours with worse throughput.
     * also switch route if this seqno is BATADV_V_MAX_ORIGDIFF newer than the last received seqno from our best next hop. */
    if (router) {
        router_ifinfo = batadv_neigh_ifinfo_get(router, if_outgoing);
        neigh_ifinfo = batadv_neigh_ifinfo_get(neigh_node, if_outgoing);

        /* if these are not allocated, something is wrong. */
        if (!router_ifinfo || !neigh_ifinfo)
            return forward;

        neigh_last_seqno = neigh_ifinfo->bat_v->last_seqno;
        router_last_seqno = router_ifinfo->bat_v->last_seqno;
        neigh_seq_diff = neigh_last_seqno - router_last_seqno;
        router_throughput = router_ifinfo->bat_v->metricValue;
        neigh_throughput = neigh_ifinfo->bat_v->metricValue;


        if (neigh_seq_diff < BATADV_OGM_MAX_ORIGDIFF && router_throughput >= neigh_throughput)
            return forward;
    }

    /* if everything is correct with the new router and the throughput is higher or the seqno is more than 5 numbers newer
     * consider the new router to be the next best hop */
    batadv_update_route(orig_node, if_outgoing, neigh_node);

    return forward;
}//batadv_v_ogm_route_update


/**
 * batadv_v_ogm_process_per_outif() - process a batman v OGM for an outgoing if
 * @bat_priv: the bat priv with all the soft interface information
 * @ethhdr: the Ethernet header of the OGM2
 * @ogm2: OGM2 structure
 * @orig_node: Originator structure for which the OGM has been received
 * @neigh_node: the neigh_node through with the OGM has been received
 * @if_incoming: the interface where this packet was received
 * @if_outgoing: the interface for which the packet should be considered
 */
void Batman5Routing::batadv_v_ogm_process_per_outif(MACAddress ethhdr ,batadv_ogm2_packet *ogm2, Orig_node_ptr orig_node,
                    Neigh_node_ptr neigh_node, batadv_hard_iface *if_incoming, batadv_hard_iface *if_outgoing)
{
    int seqno_age;
    bool forward;
    bool is_single_hop_neigh = false;
    Orig_node_ptr orig_neigh_node = NULL;
//    std::cout << "Starting batadv_v_ogm_process_per_outif" << std::endl;

    /* first, update the metric with according sanity checks */
    seqno_age = batadv_v_ogm_metric_update(ogm2, orig_node, neigh_node, if_incoming, if_outgoing);

    /* outdated sequence numbers are to be discarded */
    if (seqno_age < 0)
        return;

    /* only unknown & newer OGMs contain TVLVs we are interested in */
    if (seqno_age > 0 && if_outgoing == BATADV_IF_DEFAULT){
        batadv_tvlv_containers_process(true, orig_node, MACAddress("00:00:00:00:00:00"), MACAddress("00:00:00:00:00:00"), (unsigned char *)(ogm2 + 1), ogm2->getTvlv_len());
    }

/* Begin: From bat_iv_ogm.c for using network coding in BATMAN V */
    // Set prev_sender for network coding
    ogm2->setPrev_sender(ethhdr);

    if(ethhdr.equals(ogm2->getOrig()))
        is_single_hop_neigh= true;

    if(is_single_hop_neigh){
        orig_neigh_node = orig_node;
    }
    else{
        orig_neigh_node = batadv_v_ogm_orig_get(ethhdr);
    }

    if (!orig_neigh_node)
        return;

    /* Update nc_nodes of the originator */
    batadv_nc_update_nc_node(orig_node, orig_neigh_node, ogm2, is_single_hop_neigh);
/* End: From bat_iv_ogm.c for using network coding in BATMAN V */

    /* if the metric update went through, update routes if needed */
    forward = batadv_v_ogm_route_update(ethhdr, ogm2, orig_node, neigh_node, if_incoming, if_outgoing);
    /* if the routes have been processed correctly, check and forward */
    if (forward){
        batadv_v_ogm_forward(ogm2, orig_node, neigh_node, if_incoming, if_outgoing);
    }
    else{
        return;
    }
}//batadv_v_ogm_process_per_outif


/**
 * batadv_v_ogm_aggr_packet() - checks if there is another OGM aggregated
 * @buff_pos: current position in the skb
 * @packet_len: total length of the skb
 * @tvlv_len: tvlv length of the previously considered OGM
 *
 * Return: true if there is enough space for another OGM, false otherwise.
 */
bool Batman5Routing::batadv_v_ogm_aggr_packet(int buff_pos, int packet_len, uint16_t tvlv_len){
    int next_buff_pos = 0;

    next_buff_pos += buff_pos + BATADV_OGM2_HLEN;
    next_buff_pos += tvlv_len;

    return (next_buff_pos <= packet_len) && (next_buff_pos <= BATADV_MAX_AGGREGATION_BYTES);
}//batadv_v_ogm_aggr_packet


/**
 * batadv_v_ogm_process() - process an incoming BATMAN V OGM
 * @ogm_offset: offset to the OGM which should be processed (for aggregates)
 * @if_incoming: the interface where this packet was receved
 * @neigh: MACAddress of the neighbor we received the ogm from
 */
void Batman5Routing::batadv_v_ogm_process(batadv_ogm2_packet *ogm_packet, int ogm_offset, batadv_hard_iface *if_incoming, MACAddress neigh){
    Orig_node_ptr orig_node = NULL;
    Hardif_neigh_node_ptr hardif_neigh = NULL;
    Neigh_node_ptr neigh_node = NULL;
    batadv_hard_iface *hard_iface;
    uint32_t ogm_metric;
    ogm_metric = ogm_packet->getMetric();
    int ret;

//    std::cout << "Received OGM2 packet via NB: " << neigh.str() << ", IF: " << if_incoming->name << " (from OG: " << ogm_packet->getOrig().str() << ", seqno " << ogm_packet->getSeqno() << ", throughput " << ogm_metric << ", TTL " << (int)ogm_packet->getTtl() << ", V " << (int)ogm_packet->getBatadv_version() << ", tvlv_len "<< ogm_packet->getTvlv_len() << ")" << std::endl;

    /* If the throughput metric is 0, immediately drop the packet. No need
     * to create orig_node / neigh_node for an unusable route. */
    if (ogm_metric == 0) {
//        std::cout << hostModule->getFullName() << ": Drop packet: originator packet with throughput metric of 0" << std::endl;
        delete ogm_packet;
        return;
    }

    /* require ELP packets to be received from this neighbor first */
    hardif_neigh = batadv_hardif_neigh_get(if_incoming, neigh);
    if (hardif_neigh == NULL) {
//        std::cout << hostModule->getFullName() << ": Drop packet: OGM via unknown neighbor: " << ogm_packet->getOrig().str() << std::endl;
        delete ogm_packet;
        return;
    }

    orig_node = batadv_v_ogm_orig_get(ogm_packet->getOrig());
    if (orig_node == NULL){
        delete ogm_packet;
        return;
    }

    neigh_node = batadv_neigh_node_get_or_create(orig_node, if_incoming, neigh);
    if (neigh_node == NULL){
        delete ogm_packet;
        return;
    }

    // metric
    metricMatchLinkCharacteristicts(hardif_neigh, ogm_packet);

    batadv_v_ogm_process_per_outif(neigh, ogm_packet, orig_node, neigh_node, if_incoming, BATADV_IF_DEFAULT);

    for (int i=0; i < p_interfaceTable->getNumInterfaces(); i++){
        hard_iface = getHardif_by_ifID(p_interfaceTable->getInterface(i)->getInterfaceId());

        if (hard_iface->if_status != BATADV_IF_ACTIVE)
            continue;

        // Check whether the packet needs to be rebroadcasted on the given interface
        ret = batadv_hardif_no_broadcast(hard_iface, ogm_packet->getOrig(), hardif_neigh->orig);

        if (ret) {
            std::string type;

            switch (ret) {
                case BATADV_HARDIF_BCAST_NORECIPIENT:
                    type = "no neighbor";
                    break;
                case BATADV_HARDIF_BCAST_DUPFWD:
                    type = "single neighbor is source";
                    break;
                case BATADV_HARDIF_BCAST_DUPORIG:
                    type = "single neighbor is originator";
                    break;
                default:
                    type = "unknown";
            }//switch (ret)

//            std::cout << "OGM2 packet from " << ogm_packet->getOrig() << " on " << hard_iface->name << ": No rebroadcast necessary -> " << type << std::endl;
            continue;
        }//if (ret)

        batadv_v_ogm_process_per_outif(neigh, ogm_packet, orig_node, neigh_node, if_incoming, hard_iface);
    }
    delete ogm_packet;
}

/**
 * batadv_v_ogm_packet_recv() - OGM2 receiving handler
 * @if_incoming: the interface where this OGM has been received
 *
 * Return: NET_RX_DROP=1 if failed or NET_RX_SUCCESS=0 on success
 */
int Batman5Routing::batadv_v_ogm_packet_recv(batadv_ogm2_packet *ogm_packet, batadv_hard_iface *if_incoming, MACAddress src, MACAddress dest){
//    int ogm_offset = 0;
//    uint8_t packet_pos;
    int ret = NET_RX_DROP;

    if (!batadv_check_management_packet(ogm_packet, if_incoming, BATADV_OGM2_HLEN, src, dest))
        return ret;

    // Did I sent this OGMv2?
    if (batadv_is_my_mac(src))
        return ret;

    // Am I the Originator of this OGMv2?
    if (batadv_is_my_mac(ogm_packet->getOrig()))
        return ret;

/*    while (batadv_v_ogm_aggr_packet(ogm_offset, BATADV_OGM2_HLEN, ogm_packet->getTvlv_len())) {
        // Handle aggregated OGMs
        batadv_v_ogm_process(ogm_packet, ogm_offset, if_incoming, neigh);

        ogm_offset += BATADV_OGM2_HLEN;
        ogm_offset += ogm_packet->getTvlv_len();

//        packet_pos = skb->data + ogm_offset;
//        ogm_packet = (struct batadv_ogm2_packet *)packet_pos;
    }
*/
    ret = NET_RX_SUCCESS;

    return ret;
}//batadv_v_ogm_packet_recv


} /* namespace inet */
