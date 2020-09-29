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

#include "../Batman5Files/tvlv.h"

namespace inet {

tvlv::tvlv() {}
tvlv::~tvlv() {}


/**
 * batadv_tvlv_containers_process() - parse the given tvlv buffer to call the
 *  appropriate handlers
 * @bat_priv: the bat priv with all the soft interface information
 * @ogm_source: flag indicating whether the tvlv is an ogm or a unicast packet
 * @orig_node: orig node emitting the ogm packet
 * @src: source mac address of the unicast packet
 * @dst: destination mac address of the unicast packet
 * @tvlv_value: tvlv content
 * @tvlv_value_len: tvlv content length
 *
 * Return: success when processing an OGM or the return value of all called
 * handler callbacks.
 */
int Batman5Routing::batadv_tvlv_containers_process(bool ogm_source, Orig_node_ptr orig_node, MACAddress src, MACAddress dst, void *tvlv_value, uint16_t tvlv_value_len){

    TvlvHandler_ptr tvlv_handler;
    batadv_tvlv_hdr *tvlv_hdr;
    uint16_t tvlv_value_cont_len;
    uint8_t cifnotfound = BATADV_TVLV_HANDLER_OGM_CIFNOTFND;
    int ret = NET_RX_SUCCESS;

    while (tvlv_value_len >= sizeof(*tvlv_hdr)){
        tvlv_hdr = (batadv_tvlv_hdr *) tvlv_value;
        tvlv_value_cont_len = tvlv_hdr->len;
        tvlv_value = tvlv_hdr + 1;
        tvlv_value_len -= sizeof(*tvlv_hdr);

        if (tvlv_value_cont_len > tvlv_value_len)
            break;

        tvlv_handler = batadv_tvlv_handler_get(tvlv_hdr->type, tvlv_hdr->version);

        ret |= batadv_tvlv_call_handler(tvlv_handler, ogm_source, orig_node, src, dst, tvlv_value, tvlv_value_cont_len);

        tvlv_value = (uint8_t *)tvlv_value + tvlv_value_cont_len;
        tvlv_value_len -= tvlv_value_cont_len;
    }//while

    if (!ogm_source)
        return ret;

    for(Handler_list::iterator it = priv_tvlv->handler_list.begin(); it != priv_tvlv->handler_list.end(); it++){
        if ((it->second->flags & BATADV_TVLV_HANDLER_OGM_CIFNOTFND) && !(it->second->flags & BATADV_TVLV_HANDLER_OGM_CALLED)) {

            void (Batman5Routing::*ogm_handler_function)(Orig_node_ptr, uint8_t, void*, uint16_t) = it->second->ogm_handler;
            (this->*ogm_handler_function)(orig_node, cifnotfound, NULL, 0);
            it->second->flags &= ~BATADV_TVLV_HANDLER_OGM_CALLED;
        }//if
    }//for

    return NET_RX_SUCCESS;
}//batadv_tvlv_containers_process



/**
 * batadv_tvlv_unicast_send() - send a unicast packet with tvlv payload to the
 *  specified host
 * @bat_priv: the bat priv with all the soft interface information
 * @src: source mac address of the unicast packet
 * @dst: destination mac address of the unicast packet
 * @type: tvlv type
 * @version: tvlv version
 * @tvlv_value: tvlv content
 * @tvlv_value_len: tvlv content length
 */
void Batman5Routing::batadv_tvlv_unicast_send(MACAddress src, MACAddress dst, uint8_t type, uint8_t version, void *tvlv_value, uint16_t tvlv_value_len){

    batadv_unicast_tvlv_packet *unicast_tvlv_packet = new batadv_unicast_tvlv_packet();
    batadv_tvlv_hdr *tvlv_hdr = NULL;
    Orig_node_ptr orig_node = NULL;
    unsigned int tvlv_len = 0;
    uint16_t packetBytes = 0;
    uint8_t hdr_len = sizeof(*unicast_tvlv_packet);

    orig_node = batadv_orig_find(dst);
    if (!orig_node)
        return;

    tvlv_len = sizeof(*tvlv_hdr) + tvlv_value_len;

    packetBytes = ETH_HLEN + hdr_len + tvlv_len;

    unicast_tvlv_packet->setTtl(BATADV_TTL);
    unicast_tvlv_packet->setReserved(0);
    unicast_tvlv_packet->setTvlv_len(tvlv_len);
    unicast_tvlv_packet->setAlign(0);
    unicast_tvlv_packet->setSrc(src);
    unicast_tvlv_packet->setDst(dst);
    // setByteLegnth to simulate the correct packet Length, packet itself is not used in this simulation model
    unicast_tvlv_packet->setByteLength(packetBytes);

//    tvlv_buff = (unsigned char *)(unicast_tvlv_packet + 1);
//    tvlv_hdr = (struct batadv_tvlv_hdr *)tvlv_buff;
//    tvlv_hdr->version = version;
//    tvlv_hdr->type = type;
//    tvlv_hdr->len = htons(tvlv_value_len);
//    tvlv_buff += sizeof(*tvlv_hdr);

    batadv_send_packet_to_orig(unicast_tvlv_packet, orig_node, NULL);

}//batadv_tvlv_unicast_send


/**
 * batadv_tvlv_container_ogm_append() - append tvlv container content to given
 *  OGM packet buffer
 * @bat_priv: the bat priv with all the soft interface information
 * @packet_buff: ogm packet buffer
 * @packet_buff_len: ogm packet buffer size including ogm header and tvlv
 *  content
 * @packet_min_len: ogm header size to be preserved for the OGM itself
 *
 * The ogm packet might be enlarged or shrunk depending on the current size
 * and the size of the to-be-appended tvlv containers.
 *
 * Return: size of all appended tvlv containers in bytes.
 */
uint16_t Batman5Routing::batadv_tvlv_container_ogm_append(OGMv2_buff &packet_buff, int packet_buff_len, int packet_min_len){

    batadv_tvlv_container *tvlv = NULL;
    batadv_tvlv_hdr *tvlv_hdr = NULL;
    uint16_t tvlv_value_len = 0;
    void *tvlv_value = NULL;
    bool ret;

    tvlv_value_len = batadv_tvlv_container_list_size();
    ret = batadv_tvlv_realloc_packet_buff(packet_buff, packet_buff_len, packet_min_len, tvlv_value_len);

    if (!ret)
        return tvlv_value_len;

    if (!tvlv_value_len)
        return tvlv_value_len;
/*
    tvlv_value = packet_buff + packet_min_len;

    for(Container_list::iterator it = priv_tvlv->container_list.begin(); it != priv_tvlv->container_list.end(); it++){
        tvlv_hdr = tvlv_value;
        tvlv_hdr->type = it->second->tvlv_hdr.type;
        tvlv_hdr->version = it->second->tvlv_hdr.version;
        tvlv_hdr->len = it->second->tvlv_hdr.len;
        tvlv_value = tvlv_hdr + 1;
        memcpy(tvlv_value, it->second + 1, ntohs(it->second->tvlv_hdr.len));
        tvlv_value = (uint8_t *)tvlv_value + ntohs(it->second->tvlv_hdr.len);
    }
*/
    return tvlv_value_len;

}//batadv_tvlv_container_ogm_append


/**
 * batadv_tvlv_container_list_size() - calculate the size of the tvlv container
 *  list entries
 * @bat_priv: the bat priv with all the soft interface information
 *
 * Has to be called with the appropriate locks being acquired
 * (tvlv.container_list_lock).
 *
 * Return: size of all currently registered tvlv containers in bytes.
 */
uint16_t Batman5Routing::batadv_tvlv_container_list_size(){
    uint16_t tvlv_len = 0;

    for(Container_list::iterator it = priv_tvlv->container_list.begin(); it != priv_tvlv->container_list.end(); it++) {
        tvlv_len += sizeof(BATMANV_TVLVPackets);
        tvlv_len += it->second->tvlv_hdr->len;
    }

    return tvlv_len;
}


/**
 * batadv_tvlv_handler_get() - retrieve tvlv handler from the tvlv handler list
 *  based on the provided type and version (both need to match)
 * @bat_priv: the bat priv with all the soft interface information
 * @type: tvlv handler type to look for
 * @version: tvlv handler version to look for
 *
 * Return: tvlv handler if found or NULL otherwise.
 */
TvlvHandler_ptr Batman5Routing::batadv_tvlv_handler_get(uint8_t type, uint8_t version){
    TvlvHandler_ptr tvlv_handler_tmp = NULL;
    TvlvHandler_ptr tvlv_handler = NULL;

    for(Handler_list::iterator it = priv_tvlv->handler_list.begin(); it != priv_tvlv->handler_list.end(); it++){
        tvlv_handler_tmp = it->second;

        if (tvlv_handler_tmp->type != type)
            continue;

        if (tvlv_handler_tmp->version != version)
            continue;

        tvlv_handler = tvlv_handler_tmp;
        break;
    }

    return tvlv_handler;
}//batadv_tvlv_handler_get


/**
 * batadv_tvlv_call_handler() - parse the given tvlv buffer to call the
 *  appropriate handlers
 * @bat_priv: the bat priv with all the soft interface information
 * @tvlv_handler: tvlv callback function handling the tvlv content
 * @ogm_source: flag indicating whether the tvlv is an ogm or a unicast packet
 * @orig_node: orig node emitting the ogm packet
 * @src: source mac address of the unicast packet
 * @dst: destination mac address of the unicast packet
 * @tvlv_value: tvlv content
 * @tvlv_value_len: tvlv content length
 *
 * Return: success if handler was not found or the return value of the handler
 * callback.
 */
int Batman5Routing::batadv_tvlv_call_handler(TvlvHandler_ptr tvlv_handler, bool ogm_source, Orig_node_ptr orig_node, MACAddress src, MACAddress dst, void *tvlv_value, uint16_t tvlv_value_len){
    if (!tvlv_handler)
        return NET_RX_SUCCESS;

    if (ogm_source) {
        if (!tvlv_handler->ogm_handler)
            return NET_RX_SUCCESS;

        if (!orig_node)
            return NET_RX_SUCCESS;

        void (Batman5Routing::*ogm_handler_function)(Orig_node_ptr, uint8_t, void*, uint16_t) = tvlv_handler->ogm_handler;
        (this->*ogm_handler_function)(orig_node, BATADV_NO_FLAGS, tvlv_value, tvlv_value_len);
        tvlv_handler->flags |= BATADV_TVLV_HANDLER_OGM_CALLED;
    }
    else {
        if (src.compareTo(MACAddress("00:00:00:00:00:00")))
            return NET_RX_SUCCESS;

        if (dst.compareTo(MACAddress("00:00:00:00:00:00")))
            return NET_RX_SUCCESS;

        if (!tvlv_handler->unicast_handler)
            return NET_RX_SUCCESS;

        int (Batman5Routing::*tvlv_unicast_handler_function)(MACAddress, MACAddress, void*, uint16_t) = tvlv_handler->unicast_handler;
        return (this->*tvlv_unicast_handler_function)(src, dst, tvlv_value, tvlv_value_len);
    }

    return NET_RX_SUCCESS;
}//batadv_tvlv_call_handler


/**
 * batadv_tvlv_realloc_packet_buff() - reallocate packet buffer to accommodate
 *  requested packet size
 * @packet_buff: packet buffer
 * @packet_buff_len: packet buffer size
 * @min_packet_len: requested packet minimum size
 * @additional_packet_len: requested additional packet size on top of minimum
 *  size
 *
 * Return: true of the packet buffer could be changed to the requested size,
 * false otherwise.
 */
bool Batman5Routing::batadv_tvlv_realloc_packet_buff(OGMv2_buff &packet_buff, int packet_buff_len, int min_packet_len, int additional_packet_len){
 // not used in this simulation model
 /*
    unsigned char *new_buff;

    new_buff = kmalloc(min_packet_len + additional_packet_len, GFP_ATOMIC);

    // keep old buffer if kmalloc should fail
    if (!new_buff)
        return false;

    memcpy(new_buff, *packet_buff, min_packet_len);
    kfree(*packet_buff);
    *packet_buff = new_buff;
    *packet_buff_len = min_packet_len + additional_packet_len;
*/
    return true;
}


/**
 * batadv_tvlv_container_unregister() - unregister tvlv container based on the
 *  provided type and version (both need to match)
 * @bat_priv: the bat priv with all the soft interface information
 * @type: tvlv container type to unregister
 * @version: tvlv container type to unregister
 */
void Batman5Routing::batadv_tvlv_container_unregister(uint8_t type, uint8_t version){
    TvlvContainer_ptr tvlv = NULL;

    tvlv = batadv_tvlv_container_get(type, version);
    batadv_tvlv_container_remove(tvlv);

}//batadv_tvlv_container_unregister


/**
 * batadv_tvlv_container_register() - register tvlv type, version and content
 *  to be propagated with each (primary interface) OGM
 * @bat_priv: the bat priv with all the soft interface information
 * @type: tvlv container type
 * @version: tvlv container version
 * @tvlv_value: tvlv container content
 * @tvlv_value_len: tvlv container content length
 *
 * If a container of the same type and version was already registered the new
 * content is going to replace the old one.
 */
void Batman5Routing::batadv_tvlv_container_register(uint8_t type, uint8_t version, void *tvlv_value, uint16_t tvlv_value_len){

    TvlvContainer_ptr tvlv_new(new batadv_tvlv_container());
    tvlv_new->tvlv_hdr = new batadv_tvlv_hdr();
    TvlvContainer_ptr tvlv_old = NULL;

    if (!tvlv_value)
        tvlv_value_len = 0;

    if(!tvlv_new)
        return;

    tvlv_new->tvlv_hdr->version = version;
    tvlv_new->tvlv_hdr->type = type;
    tvlv_new->tvlv_hdr->len = tvlv_value_len;

    tvlv_old = batadv_tvlv_container_get(type, version);
    batadv_tvlv_container_remove(tvlv_old);
    priv_tvlv->container_list.insert(std::pair<uint8_t, TvlvContainer_ptr>(type, tvlv_new));
}//batadv_tvlv_container_register


/**
 * batadv_tvlv_container_get() - retrieve tvlv container from the tvlv container
 *  list based on the provided type and version (both need to match)
 * @bat_priv: the bat priv with all the soft interface information
 * @type: tvlv container type to look for
 * @version: tvlv container version to look for
 *
 * Has to be called with the appropriate locks being acquired
 * (tvlv.container_list_lock).
 *
 * Return: tvlv container if found or NULL otherwise.
 */
TvlvContainer_ptr Batman5Routing::batadv_tvlv_container_get(uint8_t type, uint8_t version){
    TvlvContainer_ptr tvlv_tmp = NULL;
    TvlvContainer_ptr tvlv = NULL;

    for(Container_list::iterator it = priv_tvlv->container_list.begin(); it != priv_tvlv->container_list.end(); it++){
        tvlv_tmp = it->second;

        if (tvlv_tmp->tvlv_hdr->type != type)
            continue;

        if (tvlv_tmp->tvlv_hdr->version != version)
            continue;

        tvlv = tvlv_tmp;
        break;
    }

    return tvlv;
}//batadv_tvlv_container_get


/**
 * batadv_tvlv_container_remove() - remove tvlv container from the tvlv
 *  container list
 * @bat_priv: the bat priv with all the soft interface information
 * @tvlv: the to be removed tvlv container
 *
 * Has to be called with the appropriate locks being acquired
 * (tvlv.container_list_lock).
 */
void Batman5Routing::batadv_tvlv_container_remove(TvlvContainer_ptr tvlv){

    if (!tvlv)
        return;

    Container_list::iterator it = priv_tvlv->container_list.find(tvlv->tvlv_hdr->type);
    if(it != priv_tvlv->container_list.end()){
        priv_tvlv->container_list.erase(it);
    }

}//batadv_tvlv_container_remove


/**
 * batadv_tvlv_handler_register() - register tvlv handler based on the provided
 *  type and version (both need to match) for ogm tvlv payload and/or unicast
 *  payload
 * @bat_priv: the bat priv with all the soft interface information
 * @optr: ogm tvlv handler callback function. This function receives the orig
 *  node, flags and the tvlv content as argument to process.
 * @uptr: unicast tvlv handler callback function. This function receives the
 *  source & destination of the unicast packet as well as the tvlv content
 *  to process.
 * @type: tvlv handler type to be registered
 * @version: tvlv handler version to be registered
 * @flags: flags to enable or disable TVLV API behavior
 */
void Batman5Routing::batadv_tvlv_handler_register(void(Batman5Routing::*optr)(Orig_node_ptr orig, uint8_t flags, void *tvlv_value, uint16_t tvlv_value_len), int(Batman5Routing::*uptr)(MACAddress src, MACAddress dst, void *tvlv_value, uint16_t tvlv_value_len), uint8_t type, uint8_t version, uint8_t flags){
    TvlvHandler_ptr tvlv_handler = NULL;

    // if handler already exists -> return
    tvlv_handler = batadv_tvlv_handler_get(type, version);
    if (tvlv_handler)
        return;

    TvlvHandler_ptr newHandler(new batadv_tvlv_handler());
    tvlv_handler = newHandler;

    tvlv_handler->ogm_handler = optr;
    tvlv_handler->unicast_handler = uptr;
    tvlv_handler->type = type;
    tvlv_handler->version = version;
    tvlv_handler->flags = flags;

    priv_tvlv->handler_list.insert(std::pair<uint8_t,TvlvHandler_ptr>(tvlv_handler->type, tvlv_handler));

}//batadv_tvlv_handler_register


/**
 * batadv_tvlv_handler_unregister() - unregister tvlv handler based on the
 *  provided type and version (both need to match)
 * @bat_priv: the bat priv with all the soft interface information
 * @type: tvlv handler type to be unregistered
 * @version: tvlv handler version to be unregistered
 */
void Batman5Routing::batadv_tvlv_handler_unregister(uint8_t type, uint8_t version){
    TvlvHandler_ptr tvlv_handler = NULL;

    tvlv_handler = batadv_tvlv_handler_get(type, version);
    if(!tvlv_handler)
        return;

    Handler_list::iterator it = priv_tvlv->handler_list.find(tvlv_handler->type);
    if(it != priv_tvlv->handler_list.end()){
        priv_tvlv->handler_list.erase(it);
    }

}//batadv_tvlv_handler_unregister






} /* namespace inet */
