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

#include "../Batman5Files/hard_interface.h"

namespace inet {

hard_interface::hard_interface() {}
hard_interface::~hard_interface() {}


/**
 * batadv_is_cfg80211_hardif() - check if the given hardif is a cfg80211 wifi
 *  interface
 * @hard_iface: the device to check
 *
 * Return: true if the net device is a cfg80211 wireless device, false
 *  otherwise.
 */
bool Batman5Routing::batadv_is_cfg80211_hardif(batadv_hard_iface *hard_iface)
{
    uint32_t allowed_flags = 0;

    allowed_flags |= BATADV_HARDIF_WIFI_CFG80211_DIRECT;
    allowed_flags |= BATADV_HARDIF_WIFI_CFG80211_INDIRECT;

    return !!(hard_iface->wifi_flags & allowed_flags);
}

/**
 * batadv_is_wifi_hardif() - check if the given hardif is a wifi interface
 * @hard_iface: the device to check
 *
 * Return: true if the net device is a 802.11 wireless device, false otherwise.
 */
bool Batman5Routing::batadv_is_wifi_hardif(batadv_hard_iface *hard_iface)
{
    if (hard_iface == NULL)
        return false;

    return hard_iface->wifi_flags != 0; // wifi_flags != 0 -> WiFi-Hard-Interface
}


/**
 * batadv_hardif_no_broadcast: Checks whether a packet needs to be (re)broadcasted on the given interface.
 * @if_outgoing: the outgoing interface checked and considered for (re)broadcast
 * @orig_addr: the originator of this packet
 * @orig_neigh: originator address of the forwarder we just got the packet from (NULL if we originated)
 * Return:
 *  BATADV_HARDIF_BCAST_NORECIPIENT: No neighbor on interface
 *  BATADV_HARDIF_BCAST_DUPFWD: Just one neighbor, but it is the forwarder
 *  BATADV_HARDIF_BCAST_DUPORIG: Just one neighbor, but it is the originator
 *  BATADV_HARDIF_BCAST_OK: Several neighbors, must broadcast
 */
int Batman5Routing::batadv_hardif_no_broadcast(batadv_hard_iface *if_outgoing, MACAddress orig_addr, MACAddress orig_neigh){
    int ret = BATADV_HARDIF_BCAST_OK;
    Neighbor_Table::iterator only_neigh_node;

    /* 0 neighbors -> no (re)broadcast */
    if(if_outgoing->neigh_table->empty()){
        return BATADV_HARDIF_BCAST_NORECIPIENT;
    }

    /* >1 neighbors -> (re)brodcast */
    if(if_outgoing->neigh_table->size() > 1){
        return BATADV_HARDIF_BCAST_OK;
    }

    if(if_outgoing->neigh_table->size() == 1)
        only_neigh_node = if_outgoing->neigh_table->begin();
    else
        return 99;

    /* 1 neighbor, is the originator -> no rebroadcast */
    if(!orig_addr.equals(MACAddress("00:00:00:00:00:00")) && only_neigh_node->second->hardif_neigh->orig.equals(orig_addr)){
        return BATADV_HARDIF_BCAST_DUPORIG;
    }
    /* 1 neighbor, is the one we received from -> no rebroadcast */
    else if(!orig_neigh.equals(MACAddress("00:00:00:00:00:00")) && only_neigh_node->second->hardif_neigh->orig.equals(orig_neigh)){
        return BATADV_HARDIF_BCAST_DUPFWD;
    }

    return ret;
}//batadv_hardif_no_broadcast


void Batman5Routing::batadv_primary_if_update_addr(batadv_hard_iface *oldif){
    batadv_hard_iface *primary_if;

    primary_if = batadv_primary_if_get_selected();
    if (primary_if == NULL){
        return;
    }
    primary_iface = primary_if;

//    batadv_dat_init_own_addr(primary_if);
//    batadv_bla_update_orig_address(primary_if, oldif);

}//batadv_primary_if_update_addr


void Batman5Routing::batadv_primary_if_select(batadv_hard_iface *new_hard_iface){
    batadv_hard_iface *curr_hard_iface;

    curr_hard_iface = primary_iface;

    if(new_hard_iface == NULL){
        return;
    }
    primary_iface = new_hard_iface;
    batadv_primary_if_update_addr(curr_hard_iface);

}//batadv_primary_if_select



/**
 * batadv_primary_if_get_selected() - Get reference to primary interface
 * @bat_priv: the bat priv with all the soft interface information
 *
 * Return: primary interface (with increased refcnt), otherwise NULL
 */
batadv_hard_iface *Batman5Routing::batadv_primary_if_get_selected(){

    return primary_iface;

}//batadv_primary_if_get_selected



void Batman5Routing::batadv_hardif_activate_interface(batadv_hard_iface *hard_iface){
    batadv_hard_iface *primary_if =NULL;

    if (hard_iface->if_status != BATADV_IF_INACTIVE){
        return;
    }
    hard_iface->if_status = BATADV_IF_TO_BE_ACTIVATED;

    /* the first active interface becomes our primary interface or
     * the next active interface after the old primary interface was removed */
    primary_if = batadv_primary_if_get_selected();
    if (primary_if == NULL){
        batadv_primary_if_select(hard_iface);
    }
//    std::cout <<hostModule->getFullName() << ": Interface activated: " << hard_iface->name << std::endl;

//    batadv_update_min_mtu();

    batadv_v_iface_activate(hard_iface);

}//batadv_hardif_activate_interface



void Batman5Routing::batadv_hardif_deactivate_interface(batadv_hard_iface *hard_iface){

    if (hard_iface->if_status != BATADV_IF_ACTIVE && hard_iface->if_status != BATADV_IF_TO_BE_ACTIVATED){
        return;
    }
//    std::cout << hostModule->getFullName() << ": elp_disable" << std::endl;
    batadv_v_elp_iface_disable(hard_iface);
//    std::cout << hostModule->getFullName() << ": ogm_disable" << std::endl;
    batadv_v_ogm_disable();
    hard_iface->if_status = BATADV_IF_INACTIVE;
//    std::cout << hostModule->getFullName() << "Interface deactivated: " << hard_iface->name << std::endl;

//    batadv_update_min_mtu();

}//batadv_hardif_deactivate_interface
















} /* namespace inet */
