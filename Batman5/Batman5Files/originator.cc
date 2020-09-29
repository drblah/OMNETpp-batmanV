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

#include "../Batman5Files/originator.h"

namespace inet {

originator::originator() {}
originator::~originator() {}


/**
 *  batadv_orig_find: Find orig_node in originator table
 *  rewrite of batadv_orig_hash_find
 *  hash-table -> std::map
 */
Orig_node_ptr Batman5Routing::batadv_orig_find(MACAddress orig){
    Orig_node_ptr orig_node = NULL;
//    std::cout << "Starting batadv_orig_find, ";

    Originator_Table::iterator it = orig_table.find(orig);
    if (it != orig_table.end()){
        orig_node = it->second;
    }

    if(orig_node == NULL){
//        std::cout << hostModule->getFullName() << ": Orig_node = NULL!" << std::endl;
    }

    return orig_node;
}//batadv_orig_find


/**
 * batadv_orig_node_new:
 * Creates a new originator object and initialise all the generic fields.
 * The new object is not added to the originator list.
 */
Orig_node_ptr Batman5Routing::batadv_orig_node_new(MACAddress addr){
    EV << "Creating a new originator with mac " << addr.str() << std::endl;
    Orig_node_ptr orig_node(new batadv_orig_node());

    batadv_nc_init_orig(orig_node);

    orig_node->orig = addr;
    //batadv_dat_init_orig_node_addr(orig_node); // Distributed ARP Table
    orig_node->last_ttvn = 0;
    orig_node->tt_buff = new tt_changeList();
    orig_node->tt_buff_len = 0;
    orig_node->last_seen = simTime();
    simtime_t reset_time = simTime() - 1 - (BATADV_RESET_PROTECTION_MS/1000);
    orig_node->bcast_seqno_reset = reset_time;
    orig_node->ifinfo_list = std::vector<Orig_ifinfo_ptr>();
    orig_node->last_bonding_candidate = NULL;
    orig_node->capabilities = 0;
    orig_node->capa_initialized = 0;
    orig_node->last_bcast_seqno = 0;
    Neighbor_Table_ptr new_neigh_list(new Neighbor_Table());
    orig_node->neigh_list = new_neigh_list;
    orig_node->refcount = 0;

    return orig_node;
}//batadv_orig_node_new

/**
 * batadv_neigh_node_get_or_create:
 */
Neigh_node_ptr Batman5Routing::batadv_neigh_node_get_or_create(Orig_node_ptr orig_node, batadv_hard_iface *hard_iface, MACAddress neigh_addr){
    Neigh_node_ptr neigh_node = NULL;

    neigh_node = batadv_neigh_node_get(orig_node, hard_iface, neigh_addr);
    if (neigh_node != NULL)
        return neigh_node;

    return batadv_neigh_node_create(orig_node, hard_iface, neigh_addr);
}//batadv_neigh_node_get_or_create

/**
 * batadv_neigh_node_create: create a neigh node object
 */
Neigh_node_ptr Batman5Routing::batadv_neigh_node_create(Orig_node_ptr orig_node, batadv_hard_iface *hard_iface, MACAddress neigh_addr){
    Neigh_node_ptr neigh_node = NULL;
    Hardif_neigh_node_ptr hardif_neigh = NULL;

    neigh_node = batadv_neigh_node_get(orig_node, hard_iface, neigh_addr);
    if (neigh_node != NULL){
        return neigh_node;
    }

    hardif_neigh = batadv_hardif_neigh_get_or_create(hard_iface, neigh_addr, orig_node);
    if (hardif_neigh == NULL){
        return neigh_node; // always NULL
    }

  /* No neigh_node found but hardif_neigh exists -> Create a new neigh_node */
    Neigh_node_ptr new_neigh_node(new batadv_neigh_node());
    new_neigh_node->orig_node = orig_node;
    new_neigh_node->addr = neigh_addr;
    new_neigh_node->last_seen = simTime();
    new_neigh_node->hardif_neigh = hardif_neigh;
    new_neigh_node->if_incoming = hard_iface;
    new_neigh_node->refcount = 0;
    new_neigh_node->if_info_list = interface_info_list();
    //new neighbor-table-entry
    orig_node->neigh_list->insert(std::pair<MACAddress, Neigh_node_ptr>(new_neigh_node->addr, new_neigh_node));
    hard_iface->neigh_table->insert(std::pair<MACAddress, Neigh_node_ptr>(new_neigh_node->addr, new_neigh_node));

//    std::cout << "Creating new neighbor " << neigh_addr.str() << " for orig_node " << orig_node->orig.str() << " on interface " << hard_iface->name << std::endl;
    return new_neigh_node;

}//batadv_neigh_node_create

/**
 * batadv_neigh_node_get: retrieve a neighbour from the list
 * returns NULL-pointer when no entry was found
 */
Neigh_node_ptr Batman5Routing::batadv_neigh_node_get(Orig_node_ptr orig_node, batadv_hard_iface *hard_iface, MACAddress addr){
    Neigh_node_ptr tmp_neigh_node = NULL, res = NULL;
    Neighbor_Table::iterator it = orig_node->neigh_list->find(addr);

    if (it != orig_node->neigh_list->end()){
        tmp_neigh_node = it->second;

        if(tmp_neigh_node->if_incoming == hard_iface){
            res = tmp_neigh_node;
        }
    }
    return res;
}// batadv_neigh_node_get


/**
 * batadv_neigh_ifinfo_get() - find the ifinfo from an neigh_node
 * @neigh: the neigh node to be queried
 * @if_outgoing: the interface for which the ifinfo should be acquired
 *
 * Return: the requested neigh_ifinfo or NULL if not found
 */
Neigh_ifinfo_ptr Batman5Routing::batadv_neigh_ifinfo_get(Neigh_node_ptr neigh, batadv_hard_iface *if_outgoing){
    Neigh_ifinfo_ptr neigh_ifinfo = NULL, tmp_neigh_ifinfo = NULL;
    batadv_hard_iface *hard_iface = NULL;

    if (neigh->if_info_list.size() > 1){
        hard_iface = if_outgoing;
    }

    for (interface_info_list::iterator it = neigh->if_info_list.begin(); it != neigh->if_info_list.end(); it++){
        tmp_neigh_ifinfo = *it;

        if (tmp_neigh_ifinfo->if_outgoing != hard_iface)
            continue;

        neigh_ifinfo = tmp_neigh_ifinfo;
        break;
    }

    return neigh_ifinfo;
}

/**
 * batadv_neigh_ifinfo_new() - search and possibly create an neigh_ifinfo object
 * @neigh: the neigh node to be queried
 * @if_outgoing: the interface for which the ifinfo should be acquired
 *
 * Return: NULL in case of failure or the neigh_ifinfo object for the
 * if_outgoing interface otherwise. The object is created and added to the list if it does not exist.
 */
Neigh_ifinfo_ptr Batman5Routing::batadv_neigh_ifinfo_new(Neigh_node_ptr neigh, batadv_hard_iface *if_outgoing){
    Neigh_ifinfo_ptr neigh_ifinfo = NULL;

    neigh_ifinfo = batadv_neigh_ifinfo_get(neigh, if_outgoing);
    if (neigh_ifinfo != NULL)
        return neigh_ifinfo;

    // neigh_ifinfo not found -> create a new neigh_ifinfo
    Neigh_ifinfo_ptr new_neigh_ifinfo(new batadv_neigh_ifinfo());
    neigh_ifinfo = new_neigh_ifinfo;
    neigh_ifinfo->refcount = 0;
    neigh_ifinfo->last_ttl = 0;
    Neigh_ifinfo_bat_v_ptr new_neigh_ifinfo_bat_v(new batadv_neigh_ifinfo_bat_v());
    neigh_ifinfo->bat_v = new_neigh_ifinfo_bat_v;
    neigh_ifinfo->bat_v->last_seqno = 0;
    neigh_ifinfo->bat_v->metricValue = 0;
    neigh_ifinfo->if_outgoing = if_outgoing;

    // Add created neigh_ifinfo to list
    neigh->if_info_list.push_back(neigh_ifinfo);

    return neigh_ifinfo;
}// batadv_neigh_ifinfo_new


/**
 * batadv_hardif_neigh_get_or_create: retrieve or create a hardif neighbour node
 */
Hardif_neigh_node_ptr Batman5Routing::batadv_hardif_neigh_get_or_create(batadv_hard_iface *hard_iface, MACAddress neigh_addr, Orig_node_ptr orig_node){
    Hardif_neigh_node_ptr hardif_neigh = NULL;

    hardif_neigh = batadv_hardif_neigh_get(hard_iface, neigh_addr);
    if (hardif_neigh != NULL)
        return hardif_neigh;

    return batadv_hardif_neigh_create(hard_iface, neigh_addr, orig_node);
}//batadv_hardif_neigh_get_or_create

/**
 * batadv_hardif_neigh_get: retrieve a hardif neighbour from the list
 */
Hardif_neigh_node_ptr Batman5Routing::batadv_hardif_neigh_get(batadv_hard_iface *hard_iface, MACAddress neigh_addr){
    Hardif_neigh_node_ptr hardif_neigh = nullptr;

    for (Neighbor_Table::iterator it = hard_iface->neigh_table->begin(); it != hard_iface->neigh_table->end(); it++){
        if (it->second->hardif_neigh->addr == neigh_addr){
            hardif_neigh = it->second->hardif_neigh;
            break;
        }
    }
    return hardif_neigh;
}// batadv_hardif_neigh_get


/**
 * batadv_hardif_neigh_create: create a hardif neighbour node
 */
Hardif_neigh_node_ptr Batman5Routing::batadv_hardif_neigh_create(batadv_hard_iface *hard_iface, MACAddress neigh_addr, Orig_node_ptr orig_node){
    Hardif_neigh_node_ptr hardif_neigh;

    /* check if neighbor hasn't been added in the meantime */
    hardif_neigh = batadv_hardif_neigh_get(hard_iface, neigh_addr);
    if (hardif_neigh != NULL){
        return hardif_neigh;
    }

    /* No existing entry found -> Create new one */
    Hardif_neigh_node_ptr new_hardif_neigh(new batadv_hardif_neigh_node());
    hardif_neigh = new_hardif_neigh;
    hardif_neigh->addr = neigh_addr;
    hardif_neigh->orig = orig_node->orig;
    hardif_neigh->if_incoming = hard_iface;
    hardif_neigh->last_seen = simTime();

    Hardif_neigh_node_bat_v_ptr new_bat_v(new batadv_hardif_neigh_node_bat_v());
    hardif_neigh->bat_v = new_bat_v;
    hardif_neigh->bat_v->elp_interval = (double)hard_iface->bat_v.elp_interval / 1000; // uint32 in ms to simtime
    hardif_neigh->bat_v->elp_latest_seqno = hard_iface->bat_v.elp_seqno;
    hardif_neigh->bat_v->last_unicast_tx = 0;
    ewma_metric_init(&hardif_neigh->bat_v->metric);
    hardif_neigh->refcount = 0;

    return hardif_neigh;
}//batadv_hardif_neigh_create


/**
 * batadv_orig_ifinfo_get() - find the ifinfo from an orig_node
 * @orig_node: the orig node to be queried
 * @if_outgoing: the interface for which the ifinfo should be acquired
 * Return: the requested orig_ifinfo or NULL if not found.
 */
Orig_ifinfo_ptr Batman5Routing::batadv_orig_ifinfo_get(Orig_node_ptr orig_node, batadv_hard_iface *if_outgoing){
    Orig_ifinfo_ptr orig_ifinfo = NULL;

    for (std::vector<Orig_ifinfo_ptr>::iterator it = orig_node->ifinfo_list.begin(); it != orig_node->ifinfo_list.end(); it++){

        if((*it)->if_outgoing != if_outgoing){
            continue;
        }

        orig_ifinfo = (*it);
        break;
    }
    return orig_ifinfo;
}


/**
 * batadv_orig_ifinfo_new() - search and possibly create an orig_ifinfo object
 * @orig_node: the orig node to be queried
 * @if_outgoing: the interface for which the ifinfo should be acquired
 *
 * Return: NULL in case of failure or the orig_ifinfo object for the if_outgoing
 * interface otherwise. The object is created and added to the list if it does not exist.
 */
Orig_ifinfo_ptr Batman5Routing::batadv_orig_ifinfo_new(Orig_node_ptr orig_node, batadv_hard_iface *if_outgoing){

    Orig_ifinfo_ptr orig_ifinfo = NULL;
    simtime_t reset_time;

    orig_ifinfo = batadv_orig_ifinfo_get(orig_node, if_outgoing);
    if (orig_ifinfo != NULL)
        return orig_ifinfo;

    // orig_ifinfo not found -> create new orig_ifinfo
    Orig_ifinfo_ptr new_orig_ifinfo(new batadv_orig_ifinfo());
    orig_ifinfo = new_orig_ifinfo;
    reset_time = simTime() - 1;
    reset_time -= BATADV_RESET_PROTECTION_MS/1000;
    orig_ifinfo->batman_seqno_reset = reset_time;
    orig_ifinfo->if_outgoing = if_outgoing;
    orig_ifinfo->last_real_seqno = 0;
    orig_ifinfo->last_seqno_forwarded = 0;
    orig_ifinfo->last_ttl = 0;
    orig_ifinfo->refcount = 0;
    orig_ifinfo->router = NULL;

    // Add new orig_ifinfo to list
    orig_node->ifinfo_list.push_back(orig_ifinfo);

    return orig_ifinfo;
}//batadv_orig_ifinfo_new


/**
 * batadv_orig_router_get() - router to the originator depending on iface
 * @orig_node: the orig node for the router
 * @if_outgoing: the interface where the payload packet has been received or the OGM should be sent to
 *
 * Return: the neighbor which should be router for this orig_node/iface.
 */
Neigh_node_ptr Batman5Routing::batadv_orig_router_get(Orig_node_ptr orig_node, batadv_hard_iface *iface_outgoing){
    Neigh_node_ptr router = NULL;
    batadv_hard_iface *hard_iface = NULL;
//    std::cout << hostModule->getFullName() << ": Starting batadv_orig_router_get, ifinfo_list.size: " << orig_node->ifinfo_list.size() << ", " << std::endl;

    if (orig_node->ifinfo_list.size() > 1){
        hard_iface = iface_outgoing;
    }

    for (origIfInfo_list::iterator it = orig_node->ifinfo_list.begin(); it != orig_node->ifinfo_list.end(); it++){
        if ((*it)->if_outgoing != hard_iface){
            continue;
        }

        router = (*it)->router;
        if((*it)->router == NULL){
//            std::cout << "(*it)->router == NULL" << std::endl;
        }
        break;
    }
    return router;
}// batadv_orig_router_get


/**
 * batadv_purge_neigh_ifinfo() - purge obsolete ifinfo entries from neighbor
 * @bat_priv: the bat priv with all the soft interface information
 * @neigh: orig node which is to be checked
 */
void Batman5Routing::batadv_purge_neigh_ifinfo(Neigh_node_ptr neigh){
    Neigh_ifinfo_ptr neigh_ifinfo;
    batadv_hard_iface *if_outgoing;

    /* for all ifinfo objects for this neighinator */
    for(interface_info_list::iterator it = neigh->if_info_list.begin(); it != neigh->if_info_list.end();){
        neigh_ifinfo = (*it);
        if_outgoing = neigh_ifinfo->if_outgoing;

        /* always keep the default interface */
        if (if_outgoing == BATADV_IF_DEFAULT){
            it++;
            continue;
        }

        /* don't purge if the interface is not (going) down */
        if (if_outgoing->if_status != BATADV_IF_INACTIVE &&
            if_outgoing->if_status != BATADV_IF_NOT_IN_USE &&
            if_outgoing->if_status != BATADV_IF_TO_BE_REMOVED){
                it++;
                continue;
        }

//        std::cout << "neighbor/ifinfo purge: neighbor " << neigh->addr << ", iface: " << if_outgoing->name << std::endl;
        it = neigh->if_info_list.erase(it);

    }//for



}//batadv_purge_neigh_ifinfo


bool Batman5Routing::batadv_purge_hardif_neigh(){
    batadv_hard_iface *hard_iface = NULL;
    Neigh_node_ptr neigh_node = NULL;
    simtime_t last_seen = 0;
    bool neigh_purged = false;

    for (int i=0; i < p_interfaceTable->getNumInterfaces(); i++){
       hard_iface = getHardif_by_ifID(p_interfaceTable->getInterface(i)->getInterfaceId());

       if (hard_iface->neigh_table->size() != 0){
           for (Neighbor_Table::iterator it = hard_iface->neigh_table->begin(); it != hard_iface->neigh_table->end();){
               neigh_node = it->second;
               last_seen = neigh_node->last_seen;

               if (batadv_has_timed_out(last_seen, purge_timeout)){
                   neigh_purged = true;

                   auto probeSearch = hard_iface->lastProbedThroughput.find(neigh_node->addr);
                   if(probeSearch != hard_iface->lastProbedThroughput.end()){
                       hard_iface->lastProbedThroughput.erase(probeSearch);
                   }

   //                std::cout << hostModule->getFullName() << ": Purging Neighbor: " << it->first.str() << " from " << hard_iface->name << " last_seen: " << last_seen.dbl() << std::endl;
                   it = hard_iface->neigh_table->erase(it);

               }
               else{
                   it++;
               }
           }//for NeighborTable
       }
    }//for interfaceTable
    return neigh_purged;
}//batadv_purge_hardif_neigh

/**
 * batadv_purge_orig_ifinfo() - purge obsolete ifinfo entries from originator
 * @orig_node: orig node which is to be checked
 *
 * Return: true if any ifinfo entry was purged, false otherwise.
 */
bool Batman5Routing::batadv_purge_orig_ifinfo(Orig_node_ptr orig_node){
    batadv_hard_iface *if_outgoing = NULL;
    bool ifinfo_purged = false;

    /* for all ifinfo objects for this originator */
    for (std::vector<Orig_ifinfo_ptr>::iterator it = orig_node->ifinfo_list.begin(); it != orig_node->ifinfo_list.end();){
        if_outgoing = (*it)->if_outgoing;

        /* always keep the default interface */
        if (if_outgoing == BATADV_IF_DEFAULT){
            it++;
            continue;
        }

        /* don't purge if the interface is not (going) down */
        if (if_outgoing->if_status != BATADV_IF_INACTIVE &&
            if_outgoing->if_status != BATADV_IF_NOT_IN_USE &&
            if_outgoing->if_status != BATADV_IF_TO_BE_REMOVED){
            it++;
            continue;
        }

//        std::cout << "router/ifinfo purge: originator " << orig_node->orig << ", iface: " << if_outgoing->name << std::endl;
        ifinfo_purged = true;

        /* Delete ifinfo_list entry */
        it = orig_node->ifinfo_list.erase(it);

        if (orig_node->last_bonding_candidate == (*it)) {
            orig_node->last_bonding_candidate = NULL;
        }
    }//for

    return ifinfo_purged;
}//batadv_purge_orig_ifinfo


/**
 * batadv_purge_orig_neighbors() - purges neighbors from originator
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: orig node which is to be checked
 *
 * Return: true if any neighbor was purged, false otherwise
 */
bool Batman5Routing::batadv_purge_orig_neighbors(Orig_node_ptr orig_node){
    Neigh_node_ptr neigh_node = NULL;
    bool neigh_purged = false;
    simtime_t last_seen = 0;
    batadv_hard_iface *if_incoming = NULL;
    int loop_count = 0; // loop counter
    Neighbor_Table::iterator it;
    Neighbor_Table::iterator hardif_it;
    batadv_hard_iface *hard_iface = NULL;

    /* for all neighbors towards this originator ... */
    for (it = orig_node->neigh_list->begin(); it != orig_node->neigh_list->end();){
//        std::cout << "Schleifendurchlauf: " << loop_count << " " << it->first.str() << std::endl;
        neigh_node = it->second;
        last_seen = neigh_node->last_seen;
        if_incoming = neigh_node->if_incoming;

        if (batadv_has_timed_out(last_seen, purge_timeout) ||
                    if_incoming->if_status == BATADV_IF_INACTIVE ||
                    if_incoming->if_status == BATADV_IF_NOT_IN_USE ||
                    if_incoming->if_status == BATADV_IF_TO_BE_REMOVED) {

            if(if_incoming->if_status == BATADV_IF_INACTIVE ||
               if_incoming->if_status == BATADV_IF_NOT_IN_USE ||
               if_incoming->if_status == BATADV_IF_TO_BE_REMOVED) {
//               std::cout << hostModule->getFullName() << ": neighbor purge: originator " << orig_node->orig << ", neighbor: " << neigh_node->addr << ", iface: " << if_incoming->name << std::endl;
               it++;
            }
            else{
//                std::cout << hostModule->getFullName() << "neighbor timeout: originator " << orig_node->orig << ", neighbor: " << neigh_node->addr << ", last_seen at: " << neigh_node->last_seen << std::endl;
                neigh_purged = true;
//                std::cout << hostModule->getFullName() << ": Deleting Neighbor: " << it->first.str() << " from " << orig_node->orig.str() << " last_seen: " << last_seen.dbl() << std::endl;
                it = orig_node->neigh_list->erase(it);

            }
        }//if
        else{
            /* only necessary if not the whole neighbor is to be deleted, but some interface has been removed.*/
            batadv_purge_neigh_ifinfo(neigh_node);
            it++;
        }//else
        loop_count++; // counting loops
    }//for
//    std::cout << "for-loop ended! neigh_purged = " << neigh_purged << std::endl;
    return neigh_purged;

}//batadv_purge_orig_neighbors


/**
 * batadv_find_best_neighbor() - finds the best neighbor after purging
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: orig node which is to be checked
 * @if_outgoing: the interface for which the metric should be compared
 *
 * Return: the current best neighbor, with refcount increased.
 */
Neigh_node_ptr Batman5Routing::batadv_find_best_neighbor(Orig_node_ptr orig_node, batadv_hard_iface *if_outgoing){
    Neigh_node_ptr best_neigh = NULL, neigh = NULL;

    for(Neighbor_Table::iterator it = orig_node->neigh_list->begin(); it != orig_node->neigh_list->end(); it++){
        neigh = it->second;

        if(best_neigh != NULL && (batadv_v_neigh_cmp(neigh, if_outgoing, best_neigh, if_outgoing) <= 0 )){
            continue;
        }

        best_neigh = neigh;
    }//for

    return best_neigh;

}//batadv_find_best_neighbor


/**
 * batadv_purge_orig_node() - purges obsolete information from an orig_node
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: orig node which is to be checked
 *
 * This function checks if the orig_node or substructures of it have become
 * obsolete, and purges this information if that's the case.
 *
 * Return: true if the orig_node is to be removed, false otherwise.
 */
bool Batman5Routing::batadv_purge_orig_node(Orig_node_ptr orig_node){
    Neigh_node_ptr best_neigh_node;
    batadv_hard_iface *hard_iface;
    bool changed_ifinfo, changed_neigh;

    if(orig_node == NULL){
        std::cout << "orig_node == NULL will be purged!" << std::endl;
        return true;
    }

    if (batadv_has_timed_out(orig_node->last_seen, purge_timeout_ogmv2)) {
//        std::cout << "Originator timeout: originator " << orig_node->orig << ",  last_seen " << orig_node->last_seen << std::endl;
        return true;
    }//if(batadv_has_timed_out)

    changed_ifinfo = batadv_purge_orig_ifinfo(orig_node);
    changed_neigh  = batadv_purge_orig_neighbors(orig_node);
    if (!changed_ifinfo && !changed_neigh){
        return false;
    }

    /* first for NULL ... */
    best_neigh_node = batadv_find_best_neighbor(orig_node, BATADV_IF_DEFAULT);
    batadv_update_route(orig_node, BATADV_IF_DEFAULT, best_neigh_node);

    /* ... then for all other interfaces. */
    for (int i=0; i < p_interfaceTable->getNumInterfaces(); i++){
        hard_iface = getHardif_by_ifID(p_interfaceTable->getInterface(i)->getInterfaceId());

        if(hard_iface->if_status != BATADV_IF_ACTIVE){
            continue;
        }

        best_neigh_node = batadv_find_best_neighbor(orig_node, hard_iface);
        batadv_update_route(orig_node, hard_iface, best_neigh_node);

    }//for

    return false;

}//batadv_purge_orig_node

// Function called by originator_table_timeout
void Batman5Routing::batadv_purge_orig(){
    _batadv_purge_orig();
}//batadv_purge_orig


/**
 * batadv_purge_orig_ref() - Purge all outdated originators
 */
void Batman5Routing::batadv_purge_orig_ref(){
    _batadv_purge_orig();
}//batadv_purge_orig_ref



void Batman5Routing::_batadv_purge_orig(){
    if (orig_table.empty()){
        return;
    }
    Orig_node_ptr orig_node = NULL;

    /* for all origins... */
    for (Originator_Table::iterator it = orig_table.begin(); it != orig_table.end();){
        if (batadv_purge_orig_node(it->second)) {
            orig_node = it->second;
//            batadv_gw_node_delete(orig_node);
            batadv_tt_global_del_orig(orig_node, -1, "originator timed out");
            it = orig_table.erase(it);
        }//if
        else{
//            batadv_frag_purge_orig(it->second, batadv_frag_check_entry);
            it++;
        }//else
    }//for

//    batadv_gw_election();

}//_batadv_purge_orig












} /* namespace inet */
