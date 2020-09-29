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

#include "../Batman5Files/network_coding.h"

namespace inet {

network_coding::network_coding() {}
network_coding::~network_coding() {}


/**
 * batadv_nc_init() - one-time initialization for network coding
 *
 * Return: 0 on success or negative error number in case of failure
 */
int Batman5Routing::batadv_nc_init(){
    int ret = 0;
    /* Register our packet type */
//    ret = batadv_recv_handler_register(BATADV_CODED, batadv_nc_recv_coded_packet);
    return ret;
}//__init batadv_nc_init


/**
 * batadv_nc_start_timer() - initialise the nc periodic worker
 * @bat_priv: the bat priv with all the soft interface information
 */
void Batman5Routing::batadv_nc_start_timer(cMessage *msg){
    if (msg != nullptr){
        periodic_nc_work = msg;
        scheduleAt(simTime()+nc_work_interval, periodic_nc_work);
    }
    else {
        periodic_nc_work = new cMessage("periodic nc worker");
        scheduleAt(0+nc_work_interval, periodic_nc_work);
    }
}//batadv_nc_start_timer


/**
 * batadv_nc_tvlv_container_update() - update the network coding tvlv container
 *  after network coding setting change
 * @bat_priv: the bat priv with all the soft interface information
 */
void Batman5Routing::batadv_nc_tvlv_container_update(){

    char nc_mode;

    if (networkCoding){
        nc_mode = 1;
    }
    else {
        nc_mode = 0;
    }

    switch (nc_mode) {
    case 0:
        batadv_tvlv_container_unregister(BATADV_TVLV_NC, 1);
        break;
    case 1:
        batadv_tvlv_container_register(BATADV_TVLV_NC, 1, nullptr, 0);
        break;
    }

}//batadv_nc_tvlv_container_update

/**
 * batadv_nc_status_update() - update the network coding tvlv container after
 *  network coding setting change
 * @net_dev: the soft interface net device
 */
void Batman5Routing::batadv_nc_status_update(){
    batadv_nc_tvlv_container_update();
}//batadv_nc_status_update


/**
 * batadv_nc_tvlv_ogm_handler_v1() - process incoming nc tvlv container
 * @bat_priv: the bat priv with all the soft interface information
 * @orig: the orig_node of the ogm
 * @flags: flags indicating the tvlv state (see batadv_tvlv_handler_flags)
 * @tvlv_value: tvlv buffer containing the gateway data
 * @tvlv_value_len: tvlv buffer length
 */
void Batman5Routing::batadv_nc_tvlv_ogm_handler_v1(Orig_node_ptr orig, uint8_t flags, void *tvlv_value, uint16_t tvlv_value_len){

    if (!networkCoding && flags & BATADV_TVLV_HANDLER_OGM_CIFNOTFND){
//        std::cout << "Clearing BATADV_ORIG_CAPA_HAS_NC " << std::endl;
        orig->capabilities &= ~(1UL << BATADV_ORIG_CAPA_HAS_NC);
    }
    else{
        orig->capabilities |= 1UL << BATADV_ORIG_CAPA_HAS_NC;
//        std::cout << "Setting BATADV_ORIG_CAPA_HAS_NC" << std::endl;
    }

}//batadv_nc_tvlv_ogm_handler_v1


/**
 * batadv_nc_mesh_init() - initialise coding hash table and start house keeping
 * @bat_priv: the bat priv with all the soft interface information
 *
 * Return: 0 on success or negative error number in case of failure
 */
int Batman5Routing::batadv_nc_mesh_init(){
    bat_priv_nc = new batadv_priv_nc();
    bat_priv_nc->timestamp_fwd_flush = 0;
    bat_priv_nc->timestamp_sniffed_purge = 0;

    if (bat_priv_nc->coding_hash || bat_priv_nc->decoding_hash)
        return 0;

    bat_priv_nc->coding_hash = new NC_Buffer();
    if (!bat_priv_nc->coding_hash)
        return -ENOMEM;

    bat_priv_nc->decoding_hash = new NC_Buffer();
    if (!bat_priv_nc->decoding_hash)
        return -ENOMEM;

    batadv_nc_start_timer(nullptr);

    void (Batman5Routing::*nc_tvlv_ogm_handler)(Orig_node_ptr, uint8_t, void*, uint16_t) = &Batman5Routing::batadv_nc_tvlv_ogm_handler_v1;
    batadv_tvlv_handler_register(nc_tvlv_ogm_handler, nullptr, BATADV_TVLV_NC, 1, BATADV_TVLV_HANDLER_OGM_CIFNOTFND);
    batadv_nc_tvlv_container_update();

    return 0;
}//batadv_nc_mesh_init


/**
 * batadv_nc_init_bat_priv() - initialise the nc specific bat_priv variables
 * @bat_priv: the bat priv with all the soft interface information
 */
void Batman5Routing::batadv_nc_init_bat_priv(){

    if(networkCoding){
        bat_priv_nc->min_tq = 1; //200;
        bat_priv_nc->max_fwd_delay = 10;    // 10ms = 0,01s
        bat_priv_nc->max_buffer_time = 200; // 200ms = 0,2s
    }

}//batadv_nc_init_bat_priv


/**
 * batadv_nc_init_orig() - initialise the nc fields of an orig_node
 * @orig_node: the orig_node which is going to be initialised
 */
void Batman5Routing::batadv_nc_init_orig(Orig_node_ptr orig_node){

    if (orig_node->in_coding_list == nullptr){
        orig_node->in_coding_list = new ncNodeTable();
    }

    if (orig_node->out_coding_list == nullptr){
        orig_node->out_coding_list = new ncNodeTable();
    }

}//batadv_nc_init_orig


/**
 * batadv_nc_packet_free() - frees nc packet
 * @nc_packet: the nc packet to free
 * @dropped: whether the packet is freed because it is dropped
 */
void Batman5Routing::batadv_nc_packet_free(NC_packet_ptr nc_packet, bool dropped){

//    std::cout << hostModule->getFullName() << " getLiveMessageCount: " << cMessage::getLiveMessageCount() << ", getTotalMessageCount: " << cMessage::getTotalMessageCount() << std::endl;

    if(dropped || nc_packet->getDeleteUniPacket()){
        if(nc_packet->getUni_packet() != nullptr){
//            std::cout << hostModule->getFullName() << "Vorher getLiveMessageCount: " << cMessage::getLiveMessageCount() << ", getTotalMessageCount: " << cMessage::getTotalMessageCount() << std::endl;
            batadv_unicast_packet *packet = nc_packet->getUni_packet();
            delete packet;
            nc_packet->setUni_packet(nullptr);
//            std::cout << hostModule->getFullName() << "Nachher: getLiveMessageCount: " << cMessage::getLiveMessageCount() << ", getTotalMessageCount: " << cMessage::getTotalMessageCount() << std::endl;
        }
    }
    else{
        nc_packet->setUni_packet(nullptr);
//        std::cout << "Nur nullptr setzten nicht löschen!" << std::endl;
        // deletes packet with next nc_worker
        nc_packet->setDeleteUniPacket(true);
    }

    nc_packet = nullptr;
}//batadv_nc_packet_free


/**
 * batadv_nc_to_purge_nc_node() - checks whether an nc node has to be purged
 * @bat_priv: the bat priv with all the soft interface information
 * @nc_node: the nc node to check
 *
 * Return: true if the entry has to be purged now, false otherwise
 */
bool Batman5Routing::batadv_nc_to_purge_nc_node(NC_node_ptr nc_node){

    return batadv_has_timed_out(nc_node->last_seen, BATADV_NC_NODE_TIMEOUT);
}//batadv_nc_to_purge_nc_node


/**
 * batadv_nc_to_purge_nc_path_coding() - checks whether an nc path has timed out
 * @bat_priv: the bat priv with all the soft interface information
 * @nc_path: the nc path to check
 *
 * Return: true if the entry has to be purged now, false otherwise
 */
bool Batman5Routing::batadv_nc_to_purge_nc_path_coding(NC_path_ptr nc_path){
    /* purge the path when no packets has been added for 10 times the
     * max_fwd_delay time */
    return batadv_has_timed_out(nc_path->last_valid, bat_priv_nc->max_fwd_delay * 10);
}//batadv_nc_to_purge_nc_path_coding


/**
 * batadv_nc_to_purge_nc_path_decoding() - checks whether an nc path has timed
 *  out
 * @bat_priv: the bat priv with all the soft interface information
 * @nc_path: the nc path to check
 *
 * Return: true if the entry has to be purged now, false otherwise
 */
bool Batman5Routing::batadv_nc_to_purge_nc_path_decoding(NC_path_ptr nc_path){
    /* purge the path when no packets has been added for 10 times the
     * max_buffer time */
    return batadv_has_timed_out(nc_path->last_valid, bat_priv_nc->max_buffer_time * 10);
}//batadv_nc_to_purge_nc_path_decoding


/**
 * batadv_nc_purge_orig_nc_nodes() - go through list of nc nodes and purge stale
 *  entries
 * @bat_priv: the bat priv with all the soft interface information
 * @list: list of nc nodes
 * @lock: nc node list lock
 * @to_purge: function in charge to decide whether an entry has to be purged or
 *        not. This function takes the nc node as argument and has to return
 *        a boolean value: true if the entry has to be deleted, false
 *        otherwise */
void Batman5Routing::batadv_nc_purge_orig_nc_nodes(Orig_node_ptr orig_node, bool (Batman5Routing::*to_purge)(NC_node_ptr)){
    NC_node_ptr nc_node;

    /* Check ingoing nc_node's of this orig_node */
    for (ncNodeTable::iterator it = orig_node->in_coding_list->begin(); it != orig_node->in_coding_list->end(); ){
        /* if an helper function has been passed as parameter,
         * ask it if the entry has to be purged or not */
        nc_node = it->second;
        if ( (to_purge != nullptr) && !(this->*to_purge)(nc_node)){
            it++;
            continue;
        }

//        std::cout << "Removing nc_node " << nc_node->addr.str()  << " -> " << nc_node->orig_node->orig.str() << std::endl;
        nc_node = nullptr;
        it = orig_node->in_coding_list->erase(it);
    }

    /* Check outgoing nc_node's of this orig_node */
    for (ncNodeTable::iterator it = orig_node->out_coding_list->begin(); it != orig_node->out_coding_list->end(); ){
        /* if an helper function has been passed as parameter,
         * ask it if the entry has to be purged or not */
        nc_node = it->second;
        if ( (to_purge != nullptr) && !(this->*to_purge)(nc_node)){
            it++;
            continue;
        }

//        std::cout << "Removing nc_node " << nc_node->addr.str()  << " -> " << nc_node->orig_node->orig.str() << std::endl;
        nc_node = nullptr;
        it = orig_node->out_coding_list->erase(it);
    }
}//batadv_nc_purge_orig_nc_nodes


/**
 * batadv_nc_purge_orig() - purges all nc node data attached of the given
 *  originator
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: orig_node with the nc node entries to be purged
 * @to_purge: function in charge to decide whether an entry has to be purged or
 *        not. This function takes the nc node as argument and has to return
 *        a boolean value: true is the entry has to be deleted, falsebatadv_nc_path
 *        otherwise
 */
void Batman5Routing::batadv_nc_purge_orig(Orig_node_ptr orig_node, bool (Batman5Routing::*to_purge)(NC_node_ptr)){
    batadv_nc_purge_orig_nc_nodes(orig_node, to_purge);
}//batadv_nc_purge_orig


/**
 * batadv_nc_purge_orig_hash() - traverse entire originator hash to check if
 *  they have timed out nc nodes
 * @bat_priv: the bat priv with all the soft interface information
 */
void Batman5Routing::batadv_nc_purge_orig_hash(){
    Orig_node_ptr orig_node = nullptr;

    for(Originator_Table::iterator it = orig_table.begin(); it != orig_table.end(); it++){
        orig_node = it->second;
        bool (Batman5Routing::*nc_to_purge_function)(NC_node_ptr) = &Batman5Routing::batadv_nc_to_purge_nc_node;
        batadv_nc_purge_orig(orig_node, nc_to_purge_function);
    }
}//batadv_nc_purge_orig_hash


/**
 * batadv_nc_purge_paths() - traverse all nc paths part of the hash and remove
 *  unused ones
 * @bat_priv: the bat priv with all the soft interface information
 * @hash: hash table containing the nc paths to check
 * @to_purge: function in charge to decide whether an entry has to be purged or
 *        not. This function takes the nc node as argument and has to return
 *        a boolean value: true is the entry has to be deleted, false
 *        otherwise
 */
void Batman5Routing::batadv_nc_purge_paths(bool (Batman5Routing::*to_purge)(NC_path_ptr)){
    NC_path_ptr nc_path;

    /* For each nc_path in this bin */
    for(ncPathList::iterator it = ncPath.begin(); it != ncPath.end(); ) {
        nc_path = it->second;
        /* if an helper function has been passed as parameter,
         * ask it if the entry has to be purged or not */
        if ( (to_purge != nullptr) && !(this->*to_purge)(nc_path)){
            it++;
            continue;
        }

        /* purging an non-empty nc_path should never happen, but
         * is observed under high CPU load. Delay the purging
         * until next iteration to allow the packet_list to be
         * emptied first. */
        if ( (!nc_path->packet_list_coding.empty() || !nc_path->packet_list_decoding.empty()) && (to_purge != nullptr)) {
//            std::cout << "Skipping free of non-empty nc_path (" << nc_path->prev_hop.str() << " -> " << nc_path->next_hop.str() << ")!" << std::endl;
            it++;
            continue;
        }

        /* nc_path is unused or should be removed, so remove it */
//        std::cout << hostModule->getFullName() << ": Remove nc_path " << nc_path->prev_hop.str() << " -> " << nc_path->next_hop.str()<< ", packet_list-Size: "<< nc_path->packet_list_coding.size() << " & " << nc_path->packet_list_decoding.size() << std::endl;

        // first: clear packet_list_coding and packet_list_decoding
        for (nc_packet_list::iterator it1 = nc_path->packet_list_coding.begin(); it1 != nc_path->packet_list_coding.end(); ){
            batadv_nc_packet_free(it1->second, true);

            it1 = nc_path->packet_list_coding.erase(it1);
        }
        for (nc_packet_list::iterator it2 = nc_path->packet_list_decoding.begin(); it2 != nc_path->packet_list_decoding.end(); ){
            batadv_nc_packet_free(it2->second, true);
            it2 = nc_path->packet_list_decoding.erase(it2);
        }

        // then: remove path
        nc_path = nullptr;
        it = ncPath.erase(it);
    }
}//batadv_nc_purge_paths


/**
 * batadv_nc_hash_compare() - comparing function used in the network coding hash
 *  tables
 * @node: node in the local table
 * @data2: second object to compare the node to
 *
 * Return: true if the two entry are the same, false otherwise
 */
bool Batman5Routing::batadv_nc_hash_compare(NC_path_ptr nc_path1, MACAddress src, MACAddress dest){

    /* Return 1 if the two keys are identical */
    if (!nc_path1->prev_hop.equals(src))
        return false;

    if (!nc_path1->next_hop.equals(dest))
        return false;

    return true;
}


/**
 * batadv_nc_hash_find() - search for an existing nc path and return it
 * @hash: hash table containing the nc path
 * @data: search key
 *
 * Return: the nc_path if found, NULL otherwise.
 */
NC_path_ptr Batman5Routing::batadv_nc_hash_find(MACAddress src, MACAddress dest){
    NC_path_ptr nc_path = nullptr, nc_path_tmp = nullptr;

    for(ncPathList::iterator it = ncPath.begin(); it != ncPath.end(); it++){
        nc_path = it->second;
        if (!batadv_nc_hash_compare(nc_path, src, dest)){
            continue;
        }
        nc_path_tmp = nc_path;
        break;
    }

    return nc_path_tmp;
}//batadv_nc_hash_findbatadv_nc_path


/**
 * batadv_nc_send_packet() - send non-coded packet and free nc_packet struct
 * @nc_packet: the nc packet to send
 */
void Batman5Routing::batadv_nc_send_packet(NC_packet_ptr nc_packet){

    batadv_send_unicast_packet(nc_packet->getUni_packet(), nc_packet->getNeigh_node());
    batadv_nc_packet_free(nc_packet, false);

}//batadv_nc_send_packet


/**
 * batadv_nc_sniffed_purge() - Checks timestamp of given sniffed nc_packet.
 * @bat_priv: the bat priv with all the soft interface information
 * @nc_path: the nc path the packet belongs to
 * @nc_packet: the nc packet to be checked
 *
 * Checks whether the given sniffed (overheard) nc_packet has hit its buffering
 * timeout. If so, the packet is no longer kept and the entry deleted from the
 * queue. Has to be called with the appropriate locks.
 *
 * Return: false as soon as the entry in the fifo queue has not been timed out
 * yet and true otherwise.
 */
bool Batman5Routing::batadv_nc_sniffed_purge(NC_path_ptr nc_path, NC_packet_ptr nc_packet){
    uint32_t timeout = bat_priv_nc->max_buffer_time;
    bool res = false;

    /* Packets are added to tail, so the remaining packets did not time
     * out and we can stop processing the current queue */
    if (!batadv_has_timed_out(nc_packet->getTimestamp(), timeout))
        return res;

    /* purge nc packet */
//    std::cout << hostModule->getFullName() << " Sniffed Purge" << std::endl;
    batadv_nc_packet_free(nc_packet, true);

    res = true;

    return res;
}//batadv_nc_sniffed_purge


/**
 * batadv_nc_fwd_flush() - Checks the timestamp of the given nc packet.
 * @bat_priv: the bat priv with all the soft interface information
 * @nc_path: the nc path the packet belongs to
 * @nc_packet: the nc packet to be checked
 *
 * Checks whether the given nc packet has hit its forward timeout. If so, the
 * packet is no longer delayed, immediately sent and the entry deleted from the
 * queue. Has to be called with the appropriate locks.
 *
 * Return: false as soon as the entry in the fifo queue has not been timed out
 * yet and true otherwise.
 */
bool Batman5Routing::batadv_nc_fwd_flush(NC_path_ptr nc_path, NC_packet_ptr nc_packet){
    uint32_t timeout = bat_priv_nc->max_fwd_delay;

    /* Packets are added to tail, so the remaining packets did not time
     * out and we can stop processing the current queue */
    if (!batadv_has_timed_out(nc_packet->getTimestamp(), timeout))
        return false;

    /* Send packet */
    batadv_nc_send_packet(nc_packet);
    return true;
}//batadv_nc_fwd_flush


/**
 * batadv_nc_process_nc_paths() - traverse given nc packet pool and free timed
 *  out nc packets
 * @bat_priv: the bat priv with all the soft interface information
 * @hash: to be processed hash table
 * @process_fn: Function called to process given nc packet. Should return true
 *          to encourage this function to proceed with the next packet.
 *          Otherwise the rest of the current queue is skipped.
 */
void Batman5Routing::batadv_nc_process_nc_paths(bool (Batman5Routing::*process_fn)(NC_path_ptr, NC_packet_ptr), bool coding){
    NC_packet_ptr nc_packet;
    NC_path_ptr nc_path;
    bool ret;

    /* Loop coding paths */
    for(ncPathList::iterator it1 = ncPath.begin(); it1 != ncPath.end(); it1++){
        nc_path = it1->second;

        /* Loop packets */
        if(coding){ //flush
            for(nc_packet_list::iterator it2 = nc_path->packet_list_coding.begin(); it2 != nc_path->packet_list_coding.end(); ){
                nc_packet = it2->second;
//                std::cout << hostModule->getFullName() << ": Flush" << std::endl;

                if(nc_packet == nullptr || nc_packet->getUni_packet() == nullptr){
                    // if nc_packet was coded and send the packet_list was not decremented and a nc_packet=NULL was created
//                    std::cout << hostModule->getFullName() << ": Fwd Purge: nullptr" << std::endl;
                    ret = true;
                }
                else{
                    ret = (this->*process_fn)(nc_path, nc_packet);


                }

                // if process_fn removed nc_packet, remove it also from the list
                if(ret){
                    std::cout << hostModule->getFullName() << ": Flush Purge " << nc_path->packet_list_coding.size() << std::endl;
                    it2 = nc_path->packet_list_coding.erase(it2);
                    nc_coding_delete_Counter++;
                }
                else{
                    it2++;
                }

            }//for nc_packet_list_coding
        }//if(coding)
        else{ // sniffed_purge
            for(nc_packet_list::iterator it3 = nc_path->packet_list_decoding.begin(); it3 != nc_path->packet_list_decoding.end(); ){
                nc_packet = it3->second;
                std::cout << hostModule->getFullName() << ": Sniffed " << nc_path->packet_list_decoding.size() << std::endl;

                if(nc_packet == nullptr || nc_packet->getUni_packet() == nullptr){
                    // if nc_packet was coded and send the packet_list was not decremented
                    std::cout << hostModule->getFullName() << ": Sniffed Purge: nullptr" << std::endl;
                    ret = true;
                }
                else{
//                   std::cout << hostModule->getFullName() << ": Sniffed Purge: process_fn" << std::endl;
                   ret = (this->*process_fn)(nc_path, nc_packet);
                }

                // if process_fn removed nc_packet, remove it also from the list
                if(ret){
                    it3 = nc_path->packet_list_decoding.erase(it3);
                    ++nc_decoding_delete_Counter;
                    std::cout << hostModule->getFullName() << ": ret=true: " << nc_decoding_Counter << " - " << nc_decoding_delete_Counter << " = " << nc_decoding_Counter-nc_decoding_delete_Counter << std::endl;
                }
                else{
                    it3++;
                    std::cout << hostModule->getFullName() << ": ret=false: " << nc_decoding_Counter << " - " << nc_decoding_delete_Counter << " = " << nc_decoding_Counter-nc_decoding_delete_Counter << std::endl;

                }

            }//for nc_packet_list_decoding
        }//else
    }//for ncPathList
}//batadv_nc_process_nc_paths


/**
 * batadv_nc_worker() - periodic task for house keeping related to network
 *  coding
 * @work: kernel work struct
 */
void Batman5Routing::batadv_nc_worker(){
    unsigned long timeout;

    batadv_nc_purge_orig_hash();

    bool (Batman5Routing::*nc_to_purge_path_coding)(NC_path_ptr) = &Batman5Routing::batadv_nc_to_purge_nc_path_coding;
    batadv_nc_purge_paths(nc_to_purge_path_coding);
    bool (Batman5Routing::*nc_to_purge_path_decoding)(NC_path_ptr) = &Batman5Routing::batadv_nc_to_purge_nc_path_decoding;
    batadv_nc_purge_paths(nc_to_purge_path_decoding);

    timeout = bat_priv_nc->max_fwd_delay;

    if (batadv_has_timed_out(bat_priv_nc->timestamp_fwd_flush, timeout)) {
        bool (Batman5Routing::*nc_fwd_flush)(NC_path_ptr, NC_packet_ptr) = &Batman5Routing::batadv_nc_fwd_flush;
        batadv_nc_process_nc_paths(nc_fwd_flush, true);
        bat_priv_nc->timestamp_fwd_flush = simTime();
    }

    if (batadv_has_timed_out(bat_priv_nc->timestamp_sniffed_purge, bat_priv_nc->max_buffer_time)) {
//        std::cout << hostModule->getFullName() << " nc_worker -> Sniffed Purge" << std::endl;
        bool (Batman5Routing::*nc_sniffed_purge)(NC_path_ptr, NC_packet_ptr) = &Batman5Routing::batadv_nc_sniffed_purge;
        batadv_nc_process_nc_paths(nc_sniffed_purge, false);
        bat_priv_nc->timestamp_sniffed_purge = simTime();
    }

    /* Schedule a new check */
//    batadv_nc_start_timer(); // is done in isSelfMessage
}//batadv_nc_worker


/**
 * batadv_can_nc_with_orig() - checks whether the given orig node is suitable
 *  for coding or not
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: neighboring orig node which may be used as nc candidate
 * @ogm_packet: incoming ogm packet also used for the checks
 *
 * Return: true if:
 *  1) The OGM must have the most recent sequence number.
 *  2) The TTL must be decremented by one and only one.
 *  3) The OGM must be received from the first hop from orig_node.
 *  4) The TQ value of the OGM must be above bat_priv->nc.min_tq.
 */
bool Batman5Routing::batadv_can_nc_with_orig(Orig_node_ptr orig_node, batadv_ogm2_packet *ogm_packet){
    Orig_ifinfo_ptr orig_ifinfo;
    uint32_t last_real_seqno;
    uint8_t last_ttl;

    orig_ifinfo = batadv_orig_ifinfo_get(orig_node, BATADV_IF_DEFAULT);
    if (!orig_ifinfo)
        return false;

    last_ttl = orig_ifinfo->last_ttl;
    last_real_seqno = orig_ifinfo->last_real_seqno;

    if (last_real_seqno != ogm_packet->getSeqno())
        return false;
    if (last_ttl != ogm_packet->getTtl() + 1)
        return false;
    if (!ogm_packet->getOrig().equals(ogm_packet->getPrev_sender()))
        return false;
    if (ogm_packet->getMetric() < bat_priv_nc->min_tq){
        std::cout << hostModule->getFullName() << " Metic < min_tq" << std::endl;
        return false;
    }
    return true;
}//batadv_can_nc_with_orig


/**
 * batadv_nc_find_nc_node() - search for an existing nc node and return it
 * @orig_node: orig node originating the ogm packetbatadv_nc_path
 * @orig_neigh_node: neighboring orig node from which we received the ogm packet
 *  (can be equal to orig_node)
 * @in_coding: traverse incoming or outgoing network coding list
 *
 * Return: the nc_node if found, NULL otherwise.
 */
NC_node_ptr Batman5Routing::batadv_nc_find_nc_node(Orig_node_ptr orig_node, Orig_node_ptr orig_neigh_node, bool in_coding){
    NC_node_ptr nc_node = nullptr, nc_node_out = nullptr;
    ncNodeTable *list = nullptr;

    if (in_coding)
        list = orig_neigh_node->in_coding_list;
    else
        list = orig_neigh_node->out_coding_list;

    /* Traverse list of nc_nodes to orig_node */
    for (ncNodeTable::iterator it = list->begin(); it != list->end(); it++){
        nc_node = it->second;
        if (!nc_node->addr.equals(orig_node->orig))
            continue;

        /* Found a match */
        nc_node_out = nc_node;
        break;
    }

    return nc_node_out;
}//batadv_nc_find_nc_node


/**
 * batadv_nc_get_nc_node() - retrieves an nc node or creates the entry if it was
 *  not found
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: orig node originating the ogm packet
 * @orig_neigh_node: neighboring orig node from which we received the ogm packet
 *  (can be equal to orig_node)
 * @in_coding: traverse incoming or outgoing network coding list
 *
 * Return: the nc_node if found or created, NULL in case of an error.
 */
NC_node_ptr Batman5Routing::batadv_nc_get_nc_node(Orig_node_ptr orig_node, Orig_node_ptr orig_neigh_node, bool in_coding){
    NC_node_ptr nc_node = nullptr;
    ncNodeTable *list;

    /* Check if nc_node is already added */
    nc_node = batadv_nc_find_nc_node(orig_node, orig_neigh_node, in_coding);

    /* Node found */
    if (nc_node)
        return nc_node;

    /* Create new nc_node */
    NC_node_ptr new_nc_node(new batadv_nc_node());

    /* Initialize nc_node */
    new_nc_node->addr =  orig_node->orig;
    new_nc_node->orig_node = orig_neigh_node;
    new_nc_node->refcount = 1;
    new_nc_node->last_seen = simTime();

    /* Select ingoing or outgoing coding node */
    if (in_coding) {
//        std::cout << "Created new in_coding nc_node" << std::endl;
        list = orig_neigh_node->in_coding_list;
    }
    else{
//        std::cout << "Created new out_coding nc_node" << std::endl;
        list = orig_neigh_node->out_coding_list;
    }

    /* Add nc_node to orig_node */
    list->insert(std::pair<MACAddress, NC_node_ptr>(new_nc_node->addr, new_nc_node));

//    std::cout << "Adding nc_node " <<  new_nc_node->addr.str() << " -> " << new_nc_node->orig_node->orig.str() << ", Table-Size: " << list->size() << std::endl;

    return new_nc_node;
}//batadv_nc_get_nc_node


/**
 * batadv_nc_update_nc_node() - updates stored incoming and outgoing nc node
 *  structs (best called on incoming OGMs)
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: orig node originating the ogm packet
 * @orig_neigh_node: neighboring orig node from which we received the ogm packet
 *  (can be equal to orig_node)
 * @ogm_packet: incoming ogm packet
 * @is_single_hop_neigh: orig_node is a single hop neighbor
 */
void Batman5Routing::batadv_nc_update_nc_node(Orig_node_ptr orig_node, Orig_node_ptr orig_neigh_node, batadv_ogm2_packet *ogm_packet, int is_single_hop_neigh){
    NC_node_ptr in_nc_node = nullptr;
    NC_node_ptr out_nc_node = nullptr;

    /* Check if network coding is enabled */
    if (!networkCoding)
        return;

    /* check if orig node is network coding enabled */
    if (!(orig_node->capabilities >> BATADV_ORIG_CAPA_HAS_NC) & 1U){
//        std::cout << "Network Coding in Orig_node not enabled! "<< orig_node->capabilities << std::endl;
        return;
    }

    /* accept ogms from 'good' neighbors and single hop neighbors */
    if (!batadv_can_nc_with_orig(orig_node, ogm_packet) && !is_single_hop_neigh){
//        std::cout << hostModule->getFullName() << " Not a 'good' neighbor or not single_hop_neigh " << ogm_packet->getOrig().str() << std::endl;
        return;
    }

    /* Add orig_node as in_nc_node on hop */
    in_nc_node = batadv_nc_get_nc_node(orig_node, orig_neigh_node, true);
    if (!in_nc_node){
//        std::cout << "No in_nc_node found" << std::endl;
        return;
    }

    in_nc_node->last_seen = simTime();

    /* Add hop as out_nc_node on orig_node */
    out_nc_node = batadv_nc_get_nc_node(orig_neigh_node, orig_node, false);
    if (!out_nc_node){
//        std::cout << "No out_nc_node found" << std::endl;
        return;
    }

    out_nc_node->last_seen = simTime();
}//batadv_nc_update_nc_node


/**
 * batadv_nc_get_path() - get existing nc_path or allocate a new one
 * @bat_priv: the bat priv with all the soft interface information
 * @hash: hash table containing the nc path
 * @src: ethernet source address - first half of the nc path search key
 * @dst: ethernet destination address - second half of the nc path search key
 *
 * Return: pointer to nc_path if the path was found or created, returns NULL
 * on error.
 */
NC_path_ptr Batman5Routing::batadv_nc_get_path(MACAddress src, MACAddress dst){
    NC_path_ptr nc_path = nullptr;

    /* Search for existing nc_path */
    nc_path = batadv_nc_hash_find(src, dst);

    if (nc_path) {
        /* Set timestamp to delay removal of nc_path */
        nc_path->last_valid = simTime();
//        std::cout << "Found existing NC Path" << std::endl;
        return nc_path;
    }

    /* No existing nc_path was found; create a new */
//    std::cout << "No existing NC Path was found -> Create new one, Table-Size: " << ncPath.size() <<  std::endl;
    NC_path_ptr new_nc_path(new batadv_nc_path());

    if (!new_nc_path)
        return nullptr;

    /* Initialize new_nc_path */
//    new_nc_path->packet_list = nc_packet_list();
    new_nc_path->last_valid = simTime();
    new_nc_path->next_hop = dst;
    new_nc_path->prev_hop = src;
//    std::cout << "Adding nc_path " << new_nc_path->prev_hop.str() << " -> " << new_nc_path->next_hop.str() << ", Table-Size: " << ncPath.size() << std::endl;

    /* Add nc_path to hash table */
    ncPath.insert(std::pair<MACAddress, NC_path_ptr>(new_nc_path->next_hop, new_nc_path));

    return new_nc_path;
}//batadv_nc_get_path


/**
 * batadv_nc_random_weight_tq() - scale the receivers TQ-value to avoid unfair
 *  selection of a receiver with slightly lower TQ than the other
 * @tq: to be weighted tq value
 *
 * Return: scaled tq value
 */
uint8_t Batman5Routing::batadv_nc_random_weight_tq(uint8_t tq){
    uint8_t rand_val, rand_tq;

    rand_val = uniform(0, sizeof(rand_val));

    /* randomize the estimated packet loss (max TQ - estimated TQ) */
    rand_tq = rand_val * (BATADV_TQ_MAX_VALUE - tq);

    /* normalize the randomized packet loss */
    rand_tq /= BATADV_TQ_MAX_VALUE;

    /* convert to (randomized) estimated tq again */
    return BATADV_TQ_MAX_VALUE - rand_tq;
}//batadv_nc_random_weight_tq


/**
 * batadv_nc_memxor() - XOR destination with source
 * @dst: byte array to XOR into
 * @src: byte array to XOR from
 * @len: length of destination array
 *
 * Solution for this Simulation:
 *  Ecapsulate both unicast_packets in the coded packet and configure the send packet-length
 *
 */
void Batman5Routing::batadv_nc_memxor(batadv_coded_packet* coded_packet, batadv_unicast_packet *packet1, batadv_unicast_packet *packet2/*char *dst, const char *src, unsigned int len*/){
//    unsigned int i;
//
//    for (i = 0; i < len; ++i)
//        dst[i] ^= src[i];
    int64_t packet_length = packet1->getByteLength() + coded_packet->getByteLength();
    coded_packet->setPacket1(packet1);
    coded_packet->setPacket2(packet2);
    coded_packet->setByteLength(packet_length);
}//batadv_nc_memxor


/**
 * batadv_nc_code_packets() - code a received unicast_packet with an nc packet
 *  into a coded_packet and send it
 * @bat_priv: the bat priv with all the soft interface information
 * @skb: data skb to forwardbatadv_nc_path
 * @ethhdr: pointer to the ethernet header inside the skb
 * @nc_packet: structure containing the packet to the skb can be coded with
 * @neigh_node: next hop to forward packet to
 *
 * Return: true if both packets are consumed, false otherwise.
 */
bool Batman5Routing::batadv_nc_code_packets(batadv_unicast_packet *uni_packet, NC_packet_ptr nc_packet, Neigh_node_ptr neigh_node){
    uint8_t tq_weighted_neigh, tq_weighted_coding, tq_tmp;
    batadv_unicast_packet *packet1 = nullptr;
    batadv_unicast_packet *packet2 = nullptr;
    batadv_coded_packet *coded_packet = nullptr;
    Neigh_node_ptr router_neigh = nullptr, first_dest = nullptr;
    Neigh_node_ptr router_coding = nullptr, second_dest;
    Neigh_ifinfo_ptr router_neigh_ifinfo = nullptr;
    Neigh_ifinfo_ptr router_coding_ifinfo = nullptr;
    MACAddress first_source, second_source;
    uint32_t packet_id1, packet_id2;
    bool res = false;
    int coding_len;
    int unicast_size = sizeof(*packet1);

    /* TODO: do we need to consider the outgoing interface for coded packets? */
    router_neigh = batadv_orig_router_get(neigh_node->orig_node, BATADV_IF_DEFAULT);
    if (!router_neigh)
        return res;

    router_neigh_ifinfo = batadv_neigh_ifinfo_get(router_neigh, BATADV_IF_DEFAULT);
    if (!router_neigh_ifinfo)
        return res;

    router_neigh = nullptr;

    router_coding = batadv_orig_router_get(nc_packet->getNeigh_node()->orig_node, BATADV_IF_DEFAULT);
    if (!router_coding)
        return res;

    router_coding_ifinfo = batadv_neigh_ifinfo_get(router_coding, BATADV_IF_DEFAULT);
    if (!router_coding_ifinfo)
        return res;

    router_coding = nullptr;

    tq_tmp = router_neigh_ifinfo->bat_v->metric_avg;
    tq_weighted_neigh = batadv_nc_random_weight_tq(tq_tmp);
    tq_tmp = router_coding_ifinfo->bat_v->metric_avg;
    tq_weighted_coding = batadv_nc_random_weight_tq(tq_tmp);

    /* Select one destination for the MAC-header dst-field based on
     * weighted TQ-values.*/
    if (tq_weighted_neigh >= tq_weighted_coding) {
        /* Destination from nc_packet is selected for MAC-header */
        first_dest = nc_packet->getNeigh_node();
        first_source = nc_packet->getNc_path()->prev_hop;
        second_dest = neigh_node;
        second_source = uni_packet->getSrc();
        packet1 = nc_packet->getUni_packet();
        packet2 = uni_packet;
        packet_id1 = nc_packet->getPacket_id();
        packet_id2 = uni_packet->getPacket_id();

    }
    else {
        /* Destination for skb is selected for MAC-header */
        first_dest = neigh_node;
        first_source = uni_packet->getSrc();
        second_dest = nc_packet->getNeigh_node();
        second_source = nc_packet->getNc_path()->prev_hop;
        packet1 = uni_packet;
        packet2 = nc_packet->getUni_packet();
        packet_id1 = uni_packet->getPacket_id();
        packet_id2 = nc_packet->getPacket_id();
    }

    /* Instead of zero padding the smallest data buffer, we
     * code into the largest. */
//    if (uni_packet->getByteLength() <= nc_packet->getByteLength()) {
//        skb_dest = nc_packet->uni_packet;
//        skb_src = uni_packet;
//    } else {
//        skb_dest = uni_packet;
//        skb_src = nc_packet->uni_packet;
//    }

    /* coding_len is used when decoding the packet shorter packet */
    coding_len = sizeof(packet2) - unicast_size;

    coded_packet = new batadv_coded_packet();

    coded_packet->setTtl(packet1->getTtl());

    /* Info about first unicast packet */
    coded_packet->setFirst_source(first_source);
    coded_packet->setFirst_orig_dest(packet1->getDest());
    coded_packet->setFirst_crc(packet_id1);
    coded_packet->setFirst_ttvn(packet1->getTtvn());

    /* Info about second unicast packet */
    coded_packet->setSecond_dest(second_dest->addr);
    coded_packet->setSecond_source(second_source);
    coded_packet->setSecond_orig_dest(packet2->getDest());
    coded_packet->setSecond_crc(packet_id2);
    coded_packet->setSecond_ttl(packet2->getTtl());
    coded_packet->setSecond_ttvn(packet2->getTtvn());
    coded_packet->setCoded_len(coding_len);

    /* This is where the magic happens: Code skb_src into skb_dest */
    batadv_nc_memxor(coded_packet, packet1, packet2);

    /* skb_src is now coded into skb_dest, so free it */
//    consume_skb(skb_src);

    /* avoid duplicate free of skb from nc_packet */
    batadv_nc_packet_free(nc_packet, false);

    /* Send the coded packet and return true */
    batadv_send_unicast_packet(coded_packet, first_dest);

    res = true;
    return res;
}//batadv_nc_code_packets


/**
 * batadv_nc_skb_coding_possible() - true if a decoded skb is available at dst.
 * @skb: data skb to forward
 * @dst: destination mac address of the other skb to code with
 * @src: source mac address of skb
 *
 * Whenever we network code a packet we have to check whether we received it in
 * a network coded form. If so, we may not be able to use it for coding because
 * some neighbors may also have received (overheard) the packet in the network
 * coded form without being able to decode it. It is hard to know which of the
 * neighboring nodes was able to decode the packet, therefore we can only
 * re-code the packet if the source of the previous encoded packet is involved.
 * Since the source encoded the packet we can be certain it has all necessary
 * decode information.
 *
 * Return: true if coding of a decoded packet is allowed.
 */
bool Batman5Routing::batadv_nc_packet_coding_possible(MACAddress dst, MACAddress src){
//    BATADV_SKB_CB(skb)->decoded
    if (!dst.equals(src))
        return false;

    return true;
}//batadv_nc_skb_coding_possible


/**
 * batadv_nc_path_search() - Find the coding path matching in_nc_node and
 *  out_nc_node to retrieve a buffered packet that can be used for coding.
 * @bat_priv: the bat priv with all the soft interface information
 * @in_nc_node: pointer to skb next hop's neighbor nc node
 * @out_nc_node: pointer to skb source's neighbor nc node
 * @skb: data skb to forward
 * @eth_dst: next hop mac address of skb
 *
 * Return: true if coding of a decoded skb is allowed.
 */
NC_packet_ptr Batman5Routing::batadv_nc_path_search(NC_node_ptr in_nc_node, NC_node_ptr out_nc_node, MACAddress eth_dst){
    NC_path_ptr nc_path = nullptr;
    NC_packet_ptr nc_packet_out = nullptr;
    NC_packet_ptr nc_packet = nullptr;


    /* Check for coding opportunities in this nc_path */
    for(ncPathList::iterator it = ncPath.begin(); it != ncPath.end(); it++){
        nc_path = it->second;
        if (!nc_path->prev_hop.equals(in_nc_node->addr))
            continue;

        if (!nc_path->next_hop.equals(out_nc_node->addr))
            continue;

        if (nc_path->packet_list_coding.empty()) {
            continue;
        }

        for(nc_packet_list::iterator it = nc_path->packet_list_coding.begin(); it != nc_path->packet_list_coding.end(); it++){
            nc_packet = it->second;
            if (!batadv_nc_packet_coding_possible(eth_dst, in_nc_node->addr))
                continue;

            /* Coding opportunity is found! */
            nc_packet_out = nc_packet;
            break;
        }

        break;
    }

    return nc_packet_out;
}//batadv_nc_path_search


/**
 * batadv_nc_skb_src_search() - Loops through the list of neighoring nodes of
 *  the skb's sender (may be equal to the originator).
 * @bat_priv: the bat priv with all the soft interface information
 * @skb: data skb to forward
 * @eth_dst: next hop mac address of skb
 * @eth_src: source mac address of skb
 * @in_nc_node: pointer to skb next hop's neighbor nc node
 *
 * Return: an nc packet if a suitable coding packet was found, NULL otherwise.
 */
NC_packet_ptr Batman5Routing::batadv_nc_packet_src_search(MACAddress eth_dst, MACAddress eth_src, NC_node_ptr in_nc_node){
    Orig_node_ptr orig_node = nullptr;
    NC_node_ptr out_nc_node = nullptr;
    NC_packet_ptr nc_packet = nullptr;

    orig_node = batadv_orig_find(eth_src);
    if (!orig_node)
        return nullptr;

    for(ncNodeTable::iterator it = orig_node->out_coding_list->begin(); it != orig_node->out_coding_list->end(); it++){
        out_nc_node = it->second;
        /* Check if the skb is decoded and if recoding is possible */
        if (!batadv_nc_packet_coding_possible(out_nc_node->addr, eth_src))
            continue;

        /* Search for an opportunity in this nc_path */
        nc_packet = batadv_nc_path_search(in_nc_node, out_nc_node, eth_dst);
        if (nc_packet){
            break;
        }
    }

    return nc_packet;
}//batadv_nc_skb_src_search


/**
 * batadv_nc_skb_store_before_coding() - set the ethernet src and dst of the
 *  unicast skb before it is stored for use in later decoding
 * @bat_priv: the bat priv with all the soft interface information
 * @skb: data skb to store
 * @eth_dst_new: new destination mac address of skb
 */
void Batman5Routing::batadv_nc_packet_store_before_coding(batadv_unicast_packet *packet, MACAddress eth_dst_new){

    /* Set the mac header as if we actually sent the packet uncoded */
    packet->setH_source(packet->getH_dest());
    packet->setH_dest(eth_dst_new);

//    std::cout << hostModule->getFullName() << " store before coding" << std::endl;
    /* Add the packet to the decoding packet pool */
    batadv_nc_packet_store_for_decoding(packet);

}//batadv_nc_skb_store_before_coding


/**
 * batadv_nc_skb_dst_search() - Loops through list of neighboring nodes to dst.
 * @skb: data skb to forward
 * @neigh_node: next hop to forward packet to
 * @ethhdr: pointer to the ethernet header inside the skb
 *
 * Loops through list of neighboring nodes the next hop has a good connection to
 * (receives OGMs with a sufficient quality). We need to find a neighbor of our
 * next hop that potentially sent a packet which our next hop also received
 * (overheard) and has stored for later decoding.
 *
 * Return: true if the skb was consumed (encoded packet sent) or false otherwise
 */
bool Batman5Routing::batadv_nc_packet_dst_search(batadv_unicast_packet *packet, Neigh_node_ptr neigh_node, MACAddress h_source){
    Orig_node_ptr orig_node = neigh_node->orig_node;
    NC_node_ptr nc_node = nullptr;
    NC_packet_ptr nc_packet = nullptr;
    MACAddress packet_neigh;
    int i = 0;

    for(ncNodeTable::iterator it = orig_node->in_coding_list->begin(); it != orig_node->in_coding_list->end(); it++) {
        nc_node = it->second;
        /* Search for coding opportunity with this in_nc_node */
        nc_packet = batadv_nc_packet_src_search(neigh_node->addr, h_source, nc_node);

        i++;

        /* Opportunity was found, so stop searching */
        if (nc_packet)
            break;
    }

    if (!nc_packet){
//        std::cout << "nc_packet == NULL -> " << i << std::endl;
        return false;
    }

    /* Save packets for later decoding */
    batadv_nc_packet_store_before_coding(packet->dup(), neigh_node->addr);

//    Saving the nc_packet->uni_packet will result in memory leak (undisposed object)
//    batadv_nc_packet_store_before_coding(nc_packet->getUni_packet()->dup(), nc_packet->getNeigh_node()->addr);

    /* Code and send packets */
    if (batadv_nc_code_packets(packet, nc_packet, neigh_node))
        return true;

    /* out of mem ? Coding failed - we have to free the buffered packet
     * to avoid memleaks. The skb passed as argument will be dealt with
     * by the calling function. */
    std::cout << "Coding failed! So send only nc_packet" << std::endl;
    batadv_nc_send_packet(nc_packet);
    return false;
}//batadv_nc_skb_dst_search


/**
 * batadv_nc_skb_add_to_path() - buffer skb for later encoding / decoding
 * @skb: skb to add to path
 * @nc_path: path to add skb to
 * @neigh_node: next hop to forward packet to
 * @packet_id: checksum to identify packet
 *
 * Return: true if the packet was buffered or false in case of an error.
 */
bool Batman5Routing::batadv_nc_packet_add_to_path(batadv_unicast_packet *packet, NC_path_ptr nc_path, Neigh_node_ptr neigh_node, uint32_t packet_id, bool coding){
    NC_packet_ptr nc_packet(new batadv_nc_packet());

    if (!nc_packet)
        return false;

//    std::cout << hostModule->getFullName() << "Add to Path " << std::endl;

    /* Initialize nc_packet */
    nc_packet->setTimestamp(simTime());
    nc_packet->setPacket_id(packet_id);
    nc_packet->setDeleteUniPacket(false);
    nc_packet->setNeigh_node(neigh_node);
    nc_packet->setNc_path(nc_path);
    nc_packet->setUni_packet(packet->dup());

    /* Add coding packet to list */
    if(coding){
        nc_path->packet_list_coding.insert(std::pair<uint64_t, NC_packet_ptr>(nc_packet->getPacket_id(), nc_packet));
        ++nc_coding_Counter;
//        std::cout << "Added new nc_packet to list -> Size: " << nc_coding_Counter << " " << nc_path->packet_list_coding.size() << " " << nc_packet->getPacket_id() << std::endl;
    }
    else{
//        nc_path->packet_list_decoding.insert(std::pair<uint64_t, NC_packet_ptr>(nc_packet->getPacket_id(), nc_packet));
//        nc_decoding_Counter++;
    }

    return true;
}//batadv_nc_packet_add_to_path


/**
 * batadv_nc_skb_forward() - try to code a packet or add it to the coding packet
 *  buffer
 * @skb: data skb to forward
 * @neigh_node: next hop to forward packet to
 *
 * Return: true if the skb was consumed (encoded packet sent) or false otherwise
 */
bool Batman5Routing::batadv_nc_packet_forward(batadv_unicast_packet *packet, Neigh_node_ptr neigh_node){
    NC_path_ptr nc_path;
    MACAddress h_source = packet->getH_source();

    /* Check if network coding is enabled */
    if (!networkCoding){
//        std::cout << "NetworkCoding is not enabled" << std::endl;
        return false;
    }

    /* We only handle unicast packets */
    if (packet->getBatadv_packettype() != BATADV_UNICAST){
//        std::cout << "NC: Packettype check failed!" << std::endl;
        return false;
    }

    /* Try to find a coding opportunity and send the packet if one is found */
    if (batadv_nc_packet_dst_search(packet, neigh_node, h_source)){
//        std::cout << "Coding opportunity found!" << std::endl;
        return true;
    }

    /* Find or create a nc_path for this src-dst pair */
    nc_path = batadv_nc_get_path(h_source, neigh_node->addr);

    if (!nc_path){
        return false;
    }

    /* Add packet to nc_path */
    if (!batadv_nc_packet_add_to_path(packet, nc_path, neigh_node, packet->getPacket_id(), true)){
        return false;
    }

    /* Packet is consumed */
    return true;

}//batadv_nc_skb_forward


/**
 * batadv_nc_skb_store_for_decoding() - save a clone of the skb which can be
 *  used when decoding coded packets
 * @bat_priv: the bat priv with all the soft interface information
 * @skb: data skb to store
 */
void Batman5Routing::batadv_nc_packet_store_for_decoding(batadv_unicast_packet *packet){
    NC_path_ptr nc_path = nullptr;
    MACAddress h_source;
    MACAddress h_dest;

    /* Check if network coding is enabled */
    if (!networkCoding){
        delete packet;
        return;
    }

    /* Check for supported packet type */
    if (packet != nullptr && packet->getBatadv_packettype() != BATADV_UNICAST){
        delete packet;
        return;
    }

    h_source = packet->getH_source();
    h_dest = packet->getH_dest();

    /* Find existing nc_path or create a new */
    nc_path = batadv_nc_get_path(h_source, h_dest);
    if (!nc_path){
        delete packet;
        return;
    }

    /* Add skb to nc_path */
    if ( !batadv_nc_packet_add_to_path(packet, nc_path, nullptr, packet->getPacket_id(),false) ){
        delete packet;
        return;
    }

    if(hostModule->getIndex() == 1){
//        std::cout << hostModule->getFullName() << " unicast packet added to path " << h_dest.str() << " " << nc_path->packet_list_decoding.size() << std::endl;
    }

    return;
}//batadv_nc_skb_store_for_decoding

/**
 * batadv_nc_skb_store_sniffed_unicast() - check if a received unicast packet
 *  should be saved in the decoding buffer and, if so, store it there
 * @bat_priv: the bat priv with all the soft interface information
 * @skb: unicast skb to store
 */
void Batman5Routing::batadv_nc_packet_store_sniffed_unicast(batadv_unicast_packet *packet){
    if (batadv_is_my_mac(packet->getH_dest()))
        return;
//    std::cout << hostModule->getFullName() << " Sniffed a unicast packet" << std::endl;
    batadv_nc_packet_store_for_decoding(packet);
}//batadv_nc_skb_store_sniffed_unicast


/**
 * batadv_nc_skb_decode_packet() - decode given skb using the decode data stored
 *  in nc_packet
 * @bat_priv: the bat priv with all the soft interface information
 * @skb: unicast skb to decode
 * @nc_packet: decode data needed to decode the skb
 *packe
 * Return: pointer to decoded unicast packet if the packet was decoded or NULL
 * in case of an error.
 */
batadv_unicast_packet *Batman5Routing::batadv_nc_packet_decode_packet(batadv_coded_packet *coded_packet, NC_packet_ptr nc_packet){
    batadv_unicast_packet *unicast_packet;
    MACAddress orig_dest;
    MACAddress h_dest;
    uint8_t ttl, ttvn;
    unsigned int coding_len;
    bool second_packet = false;

    /* Select the correct unicast header information based on the location
     * of our mac address in the coded_packet header */
    if (batadv_is_my_mac(coded_packet->getSecond_dest())) {
        /* If we are the second destination the packet was overheard,
         * so the Ethernet address must be copied to h_dest and
         * pkt_type changed from PACKET_OTHERHOST to PACKET_HOST */
        h_dest = coded_packet->getSecond_dest();
        orig_dest = coded_packet->getSecond_orig_dest();
        ttl = coded_packet->getSecond_ttl();
        ttvn = coded_packet->getSecond_ttvn();
        second_packet = true;
    }
    else {
        orig_dest = coded_packet->getFirst_orig_dest();
        ttl = coded_packet->getTtl();
        ttvn = coded_packet->getFirst_ttvn();
        second_packet = false;
    }

    coding_len = coded_packet->getCoded_len();

//    if (coding_len > sizeof(unicast_packet)){
//        std::cout << "coding_len > sizeof(unicast_packet)" << std::endl;
//        return nullptr;
//    }

    /* Here the magic is NULLreversed:
     *   extract the missing packet from the received coded packet
     *   Here not neccessary because in the simulation no real xor is used */
//    batadv_nc_memxor(skb->data + h_size, nc_packet->skb->data + h_size,coding_len);


    /* Create decoded unicast packet */
    if(second_packet){
        unicast_packet = coded_packet->getPacket2()->dup();
    }
    else{
        unicast_packet = coded_packet->getPacket1()->dup();
    }

    unicast_packet->setTtl(ttl);
    unicast_packet->setDest(orig_dest);
    unicast_packet->setTtvn(ttvn);

    batadv_nc_packet_free(nc_packet, true);
    return unicast_packet;
}//batadv_nc_skb_decode_packet


/**
 * batadv_nc_find_decoding_packet() - search through buffered decoding data to
 *  find the data needed to decode the coded packet
 * @bat_priv: the bat priv with all the soft interface information
 * @ethhdr: pointer to the ethernet header inside the coded packet
 * @coded: coded packet we try to find decode data for
 *
 * Return: pointer to nc packet if the needed data was found or NULL otherwise.
 */
NC_packet_ptr Batman5Routing::batadv_nc_find_decoding_packet(batadv_coded_packet *coded, MACAddress h_source, MACAddress h_dest){
    NC_packet_ptr tmp_nc_packet, nc_packet = nullptr;
    NC_path_ptr nc_path;
    MACAddress dest, source;
    uint32_t packet_id;

    /* Select the correct packet id based on the location of our mac-addr */
    dest = h_source;
    if (!batadv_is_my_mac(coded->getSecond_dest())) {
        source = coded->getSecond_source();
        packet_id = coded->getSecond_crc();
    }
    else {
        source = coded->getFirst_source();
        packet_id = coded->getFirst_crc();
    }
//    std::cout << hostModule->getFullName() << " ncPath-Size: " << ncPath.size() << std::endl;
    /* Search for matching coding path */
    for(ncPathList::iterator it = ncPath.begin(); it != ncPath.end(); it++){
        nc_path = it->second;
//        std::cout << hostModule->getFullName() << " nc_packet_list-Size: " << nc_path->packet_list_decoding.size() << std::endl;

        /* Find matching nc_packet */
        for(nc_packet_list::iterator it = nc_path->packet_list_decoding.begin(); it != nc_path->packet_list_decoding.end(); ) {
            tmp_nc_packet = it->second;
//            std::cout << "Packet-ID: " << packet_id << " <-> " << tmp_nc_packet->getPacket_id() << std::endl;
            if (packet_id == tmp_nc_packet->getPacket_id()) {
//                it = nc_path->packet_list_decoding.erase(it);
                nc_packet = tmp_nc_packet;
                it++;
                break;
            }
            else {
                it++;
            }
        }//for nc_packet_list

        if (nc_packet)
            break;
    }//for ncPathList


//    if (!nc_packet)
//        std::cout << "No decoding packet found for " << packet_id << std::endl;

    return nc_packet;
}//batadv_nc_find_decoding_packet


/**
 * batadv_nc_recv_coded_packet() - try to decode coded packet and enqueue the
 *  resulting unicast packet
 * @skb: incoming coded packet
 * @recv_if: pointer to interface this packet was received on
 *
 * Return: NET_RX_SUCCESS if the packet has been consumed or NET_RX_DROP
 * otherwise.
 */
int Batman5Routing::batadv_nc_recv_coded_packet(batadv_coded_packet *coded_packet, batadv_hard_iface *recv_if, MACAddress h_source, MACAddress h_dest){
    batadv_unicast_packet *unicast_packet = nullptr;
    NC_packet_ptr nc_packet = nullptr;
//    std::cout << hostModule->getFullName() << ": batadv_nc_recv_coded_packet" << std::endl;
    /* Check if network coding is enabled */
    if (!networkCoding){
        release_coded_packet(coded_packet);
        return NET_RX_DROP;
    }

    /* Verify frame is destined for us */
    if (!batadv_is_my_mac(coded_packet->getFirst_orig_dest()) && !batadv_is_my_mac(coded_packet->getSecond_dest())){
        release_coded_packet(coded_packet);
        return NET_RX_DROP;
    }

    /* Check for unicast_packets */
    if (coded_packet->getPacket1() == nullptr || coded_packet->getPacket2() == nullptr){
        release_coded_packet(coded_packet);
        return NET_RX_DROP;
    }


    /* Find decoding packet */
    nc_packet = batadv_nc_find_decoding_packet(coded_packet, h_source, h_dest);
    if (!nc_packet) {
        release_coded_packet(coded_packet);
        return NET_RX_DROP;
    }
//    std::cout << hostModule->getFullName() << ": decoding packet found!" << std::endl;

    /* Decode the packet */
    unicast_packet = batadv_nc_packet_decode_packet(coded_packet, nc_packet);
    take(unicast_packet);
    release_coded_packet(coded_packet);

    if (!unicast_packet) {
        return NET_RX_DROP;
    }

    /* Mark packet as decoded to do correct recoding when forwarding */
    unicast_packet->setDecoded(true);
    std::cout << hostModule->getFullName() << " decoding was successful! " << std::endl;
    return batadv_recv_unicast_packet(unicast_packet, recv_if, h_source, unicast_packet->getDest());

}//batadv_nc_recv_coded_packet


/**
 * batadv_nc_mesh_free() - clean up network coding memory
 * @bat_priv: the bat priv with all the soft interface information
 */
void Batman5Routing::batadv_nc_mesh_free(){
    batadv_tvlv_container_unregister(BATADV_TVLV_NC, 1);
    batadv_tvlv_handler_unregister(BATADV_TVLV_NC, 1);

    cancelAndDelete(periodic_nc_work);

    batadv_nc_purge_paths(nullptr);
    bat_priv_nc->coding_hash->clear();
    batadv_nc_purge_paths(nullptr);
    bat_priv_nc->decoding_hash->clear();
    ncPath.clear();
}//batadv_nc_mesh_free

void Batman5Routing::release_coded_packet(batadv_coded_packet *packet){
//    std::cout << hostModule->getFullName() << ": release_coded_packet" << std::endl;

    batadv_unicast_packet *uni_packet1 = packet->getPacket1();
    batadv_unicast_packet *uni_packet2 = packet->getPacket2();

    if(batadv_is_my_mac(packet->getFirst_orig_dest()) && uni_packet1 != nullptr){
//        std::cout << hostModule->getFullName() << " deleting Packet1" << std::endl;
        take(uni_packet1);
        delete uni_packet1;
    }
    if(batadv_is_my_mac(packet->getSecond_orig_dest()) && uni_packet2 != nullptr){
//        std::cout << hostModule->getFullName() << " deleting Packet2" << std::endl;
        take(uni_packet2);
        delete uni_packet2;
    }

//    std::cout << hostModule->getFullName() << " deleting coded_packet" << std::endl;
    delete(packet);

}//release_coded_packet

} /* namespace inet */
