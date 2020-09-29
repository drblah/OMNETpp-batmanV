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

#include "../Batman5Files/bat_v.h"

namespace inet {

bat_v::bat_v() {}
bat_v::~bat_v() {}


void Batman5Routing::batadv_v_iface_activate(batadv_hard_iface *hard_iface){

    /* B.A.T.M.A.N. V does not use any queuing mechanism, therefore it can
     * set the interface as ACTIVE right away, without any risk of race condition */
    if (hard_iface->if_status == BATADV_IF_TO_BE_ACTIVATED){
        hard_iface->if_status = BATADV_IF_ACTIVE;
    }

}//batadv_v_iface_activate

/**
 * batadv_v_elp_iface_disable() - release ELP interface private resources
 * @hard_iface: interface for which the resources have to be released
 */
void Batman5Routing::batadv_v_iface_disable(batadv_hard_iface *hard_iface){
    batadv_v_elp_iface_disable(hard_iface);
}


int Batman5Routing::batadv_v_iface_enable(batadv_hard_iface *hard_iface){
    int ret;
//    std::cout << "Start batadv_v_iface_enable" << std::endl;
    ret = batadv_v_elp_iface_enable(hard_iface);
    if (ret < 0){
        return ret;
    }

    ret = batadv_v_ogm_enable();
    if (ret < 0){
        batadv_v_elp_iface_disable(hard_iface);
    }

    return ret;
}


/**
 * batadv_v_hardif_neigh_print() - print a single ELP neighbour node
 * @seq: neighbour table seq_file struct
 * @hardif_neigh: hardif neighbour information
 */
void Batman5Routing::batadv_v_hardif_neigh_print(Hardif_neigh_node_ptr hardif_neigh){
    simtime_t last_secs = simTime() - hardif_neigh->last_seen;
    uint32_t metric = getEWMAMetric(&hardif_neigh->bat_v->metric);

    std::cout << "  "
         << std::setw(17) << hardif_neigh->addr
         << "      "
         << std::setw(7) << std::left << round( last_secs.dbl() * 1000.0 ) / 1000.0
         << "      "
         << std::setw(7) << std::right << metric
         << "          "
         << std::setw(5) << hardif_neigh->if_incoming->name << std::endl;

}//batadv_v_hardif_neigh_print


/**
 * batadv_v_orig_print_neigh() - print neighbors for the originator table
 * @orig_node: the orig_node for which the neighbors are printed
 * @if_outgoing: outgoing interface for these entries
 * @seq: debugfs table seq_file struct
 */
void Batman5Routing::batadv_v_orig_print_neigh(Orig_node_ptr orig_node, batadv_hard_iface *if_outgoing){
    Neigh_ifinfo_ptr n_ifinfo;

    for(Neighbor_Table::iterator it = orig_node->neigh_list->begin(); it != orig_node->neigh_list->end(); it++){
        n_ifinfo = batadv_neigh_ifinfo_get(it->second, if_outgoing);
        if(n_ifinfo == NULL)
            continue;

        std::cout  << it->second->addr << " (" << n_ifinfo->bat_v->metricValue << std::right << "), ";
    }
    std::cout << std::endl;
}


int Batman5Routing::batadv_v_neigh_cmp(Neigh_node_ptr neigh1, batadv_hard_iface *if_outgoing1,
                  Neigh_node_ptr neigh2, batadv_hard_iface *if_outgoing2)
{
    Neigh_ifinfo_ptr ifinfo1, ifinfo2;
    int64_t ret = 0;

    ifinfo1 = batadv_neigh_ifinfo_get(neigh1, if_outgoing1);
    if (ifinfo1 == NULL){
        return ret;
    }
    ifinfo2 = batadv_neigh_ifinfo_get(neigh2, if_outgoing2);
    if (ifinfo2 == NULL){
        return ret;
    }

    ret = static_cast<int64_t>(ifinfo1->bat_v->metricValue) - static_cast<int64_t>(ifinfo2->bat_v->metricValue);
    return ret;
}

/**
 * Neighbor is similar or better
 */
bool Batman5Routing::batadv_v_neigh_is_sob(Neigh_node_ptr neigh1, batadv_hard_iface *if_outgoing1,
                           Neigh_node_ptr neigh2, batadv_hard_iface *if_outgoing2)
{
    Neigh_ifinfo_ptr ifinfo1, ifinfo2;
    uint32_t threshold;
    bool ret = false;

    ifinfo1 = batadv_neigh_ifinfo_get(neigh1, if_outgoing1);
    if (!ifinfo1)
        return ret;

    ifinfo2 = batadv_neigh_ifinfo_get(neigh2, if_outgoing2);
    if (!ifinfo2)
        return ret;

    threshold = ifinfo1->bat_v->metricValue / 4;
    threshold = ifinfo1->bat_v->metricValue - threshold;

    // neigh2 have to be at least more than 1/4 of neigh1_throughput better
    ret = ifinfo2->bat_v->metricValue > threshold;

    return ret;
}


} /* namespace inet */
