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

/**
 *  Implementation of "B.A.T.M.A.N. V" routing protocol in OMNeT++
 **/

#ifndef __INET_BATMAN5ROUTING_H_
#define __INET_BATMAN5ROUTING_H_

#include <omnetpp.h>
#include <iostream>
#include <iomanip>
#include <memory>
#include <string>

/***** BATMAN Includes *****/
#include "../../Batman5/controlpackages/BATMANV_ControlPackets_m.h"
#include "../../Batman5/controlpackages/BATMANV_nc_coded_packet.h"
#include "../Batman5Files/bat_v.h"
#include "../Batman5Files/bat_v_elp.h"
#include "../Batman5Files/bat_v_ogm.h"
#include "../Batman5Files/batadv_definitions.h"
#include "../Batman5Files/gateway_client.h"
#include "../Batman5Files/gateway_common.h"
#include "../Batman5Files/hard_interface.h"
#include "../Batman5Files/network_coding.h"
#include "../Batman5Files/originator.h"
#include "../Batman5Files/routing.h"
#include "../Batman5Files/send.h"
#include "../Batman5Files/tp_meter.h"
#include "../metric/Batman5Metric.h"

#include "inet/networklayer/common/IPSocket.h"
#include "inet/networklayer/contract/IInterfaceTable.h"
#include "inet/networklayer/contract/IL3AddressType.h"
#include "inet/networklayer/contract/IRoutingTable.h"
#include "inet/networklayer/contract/INetfilter.h"
#include "inet/networklayer/contract/ipv4/IPv4ControlInfo.h"
#include "inet/networklayer/ipv4/ICMPMessage.h"
#include "inet/networklayer/ipv4/IPv4Route.h"
#include "inet/networklayer/ipv4/IPv4Datagram.h"
#include "inet/networklayer/arp/ipv4/GlobalARP.h"
#include "inet/networklayer/contract/NetworkProtocolCommand_m.h"

#include "inet/transportlayer/contract/udp/UDPControlInfo.h"
#include "inet/transportlayer/contract/udp/UDPControlInfo_m.h"
#include "inet/transportlayer/udp/UDPPacket.h"

#include "inet/transportlayer/contract/tcp/TCPCommand_m.h"
#include "inet/transportlayer/tcp/TCPConnection.h"
#include "inet/transportlayer/tcp_common/TCPSegment_m.h"
#include "inet/networklayer/contract/INetworkProtocolControlInfo.h"
#include "inet/applications/tcpapp/GenericAppMsg_m.h"

#include "inet/common/INETDefs.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/lifecycle/NodeOperations.h"
#include "inet/common/misc/ThruputMeter.h"

#include "inet/linklayer/common/MACAddress.h"
#include "inet/linklayer/ethernet/Ethernet.h"
#include "inet/linklayer/ethernet/EtherFrame_m.h"
#include "inet/linklayer/common/Ieee802Ctrl.h"

#include "inet/mobility/base/MovingMobilityBase.h"
//#include "inet/mobility/single/TraceMobility.h"

#include "inet/physicallayer/common/packetlevel/RadioMedium.h"
#include "inet/physicallayer/common/packetlevel/MediumLimitCache.h"
#include "inet/physicallayer/contract/packetlevel/IRadioMedium.h"
#include "inet/physicallayer/contract/packetlevel/IMediumLimitCache.h"

using namespace omnetpp;

namespace inet {

class Batman5Routing : public cSimpleModule
{
  private:

/* Begin Statistics */
    double numSend_UDP = 0;
    double numReceived_UDP = 0;
    simsignal_t received_OGMv2;
    simsignal_t sent_OGMv2;
    simsignal_t received_ELP;
    simsignal_t sent_ELP;
    simsignal_t sent_ELP_probes;
    simsignal_t received_UDP;      //batadv_interface_rx in Batman5Routing.cc
    simsignal_t sent_UDP;      //sendUDPUnicast in send.cc
    simsignal_t noRouter_found_UDP;  //sendUDPUnicast in send.cc
    simsignal_t forward_unicast;  //batadv_send_unicast_packet in send.cc

    simsignal_t received_packets_from_upper;
    simsignal_t received_packets_from_lower;
    simsignal_t sent_packet_to_upper;
    simsignal_t sent_packet_to_lower;

    uint32_t nc_coding_Counter;
    uint32_t nc_coding_delete_Counter;
    uint32_t nc_decoding_Counter;
    uint32_t nc_decoding_delete_Counter;
/* End Statistics */

/* Begin Tables */
    IRoutingTable *p_routingTable;
    IInterfaceTable *p_interfaceTable;
    INetfilter *p_networkProtocol;
    GlobalARP *arp_table;

    Originator_Table orig_table;
    Neighbor_Table neigh_table;
    Neighbor_Table neigh_table_wlan1;

    TranslationTableLocal tt_localTable;
    TranslationTableGlobal tt_globalTable;

    ncNodeTable ncNode;
    ncPathList ncPath;
/* End Tables */

/* Begin bat_priv */
    batadv_priv_tvlv *priv_tvlv;
    batadv_priv_nc *bat_priv_nc;
    batadv_priv_tt bat_priv_tt;
    batadv_priv_bat_v *priv_bat_v;
/* End bat_priv */

/* Begin Hard-Interfaces */
    batadv_hard_iface *wlan0;
    batadv_hard_iface *wlan1;
    batadv_hard_iface *primary_iface;
/* End Hard-Interfaces */

/* Begin Messages */
    elp_reminder *ELP_reminder_wlan0;
    cMessage *neighborTableTimeout_wlan0;
    elp_reminder *ELP_reminder_wlan1;
    cMessage *hardiface_neighborTableTimeout;
    cMessage *OGMv2_reminder;
    cMessage *originatorTableTimeout;
    cMessage *tt_purge;
    cMessage *periodic_nc_work;
/* End Messages */

/* Begin Metric */
    std::string metricType;
    double hop_penalty;

    // Alpha for (distance/distance_max)^alpha
    int distance_alpha;

    // predictiveMobilityMetric
    double secondsInTheFuture;
    bool realValues;

/* End Metric   */

/* Begin Traces */
    bool writingTraces;
    cMessage *writeTraces;
    string fileName;
/* End Traces */


/* Begin Variables */
    int stationary_grid_rows;
    bool hasUDP;
    bool hasTCP;
    int UDP_GATE = 0;
    int TCP_GATE = 1;
    uint8_t ogmv2_ttl;
    bool bonding;
    bool elpProbing;
    bool networkCoding;
    bool fragmentation;
    uint32_t ogmv2_seqno;
    uint32_t last_ogmv2_seqno;
    uint32_t elp_seqno;
    uint32_t purge_timeout;
    uint32_t purge_timeout_ogmv2;
    uint32_t elp_interval_packet; // not simtime_t because it has to be send with each elp_packet
    simtime_t elp_interval;
    simtime_t ogmv2_interval;
    simtime_t neigh_timeout_interval;
    simtime_t orig_timeout_interval;
    simtime_t tt_purge_timeout_interval;
    simtime_t jitter;
    simtime_t nc_work_interval;
    cModule *hostModule;
    MobilityBase *mobility;
    MACAddress wlan0MAC;
    MACAddress wlan1MAC;
    MACAddress hostMAC;
    IPv4Address hostIP;
/* End Variables */

  public:
    std::string getForwarder(const std::string &_destination);
    MACAddress get_hardif_MAC(const char *name);
    void print_orig_table();
    void print_neigh_table();

  protected:

/** OMNeT++ Functions */
    void finish();
    void refreshDisplay() const;
    void initialize(int stage) override;
    virtual void handleMessage(cMessage *msg);
    virtual int numInitStages() const override { return NUM_INIT_STAGES; }


/**  Functions defined in Batman5Routing */
    int getRandomSeqno();
    uint64_t getRandomId();

    int batadv_max_header_len(void);
    void printPaketLength(cMessage *msg);
    void batadv_interface_rx(cPacket *packet, Orig_node_ptr orig_node);
    InterfaceEntry *getInterfaceByID(int if_ID);
    bool batadv_is_my_mac(MACAddress address);
    bool is_valid_ether_addr(MACAddress ether_addr);

    batadv_hard_iface* getHardif_by_ifID(int if_ID);
    MACAddress get_hardif_MAC(const unsigned int id);
    MACAddress check_L3_to_MAC(L3Address l3_Address);
    void clear_bit(uint32_t bit_to_clear, uint32_t& number);
    void set_bit(uint32_t bit_to_set, uint32_t& number);
    bool batadv_has_timed_out(simtime_t timestamp, uint32_t timeout);
    void handleUDPPacket(UDPPacket* packet,MACAddress src, MACAddress dest);
    batadv_unicast_packet* batadvUnicastEncapsulation(cPacket* packet, MACAddress dest, MACAddress src);


/** Echo Location Protocol (ELP) -> bat_v_elp-Class */
    int handleELPPacket(batadv_elp_packet* packet, batadv_hard_iface *if_incoming, MACAddress src, MACAddress dest);
    void handleELPProbePacket(batadv_elpProbe_packet* packet, batadv_hard_iface *if_incoming, MACAddress src, MACAddress dest);
    batadv_elp_packet* createELPPacket(batadv_hard_iface *hard_iface);
    batadv_elpProbe_packet* createELPProbePacket(batadv_hard_iface *hard_iface);
    void batadv_v_elp_neigh_update(MACAddress neigh_addr, batadv_hard_iface *if_incoming, batadv_elp_packet* elp_packet);
    void batadv_v_elp_periodic_work(batadv_hard_iface *hard_iface);
    bool batadv_v_elp_wifi_neigh_probe(Hardif_neigh_node_ptr neigh);
    void batadv_v_elp_throughput_metric_update(Hardif_neigh_node_ptr neigh_node);
    uint32_t batadv_v_elp_get_throughput(Hardif_neigh_node_ptr neigh);
    int batadv_v_elp_iface_enable(batadv_hard_iface *hard_iface);
    void batadv_v_elp_iface_disable(batadv_hard_iface *hard_iface);


/** Sending functions -> send-Class */
    void sendELPBroadcast(batadv_hard_iface *hard_iface);
    void sendELPUnicast(Hardif_neigh_node_ptr neigh);
    void sendUDPUnicast(UDPPacket *udp_packet, MACAddress dest, MACAddress src);
    void Ieee802CtrlEncapsulation(EthernetIIFrame *packet, MACAddress dest, batadv_hard_iface *hard_if);
    void IPv4CtrlEncapsulation(cPacket *packet, L3Address dest, L3Address src, uint8_t transportProtocolID = 17);
    void UDPDataIndicationEncapsulation(cPacket *packet, L3Address srcAddress, int srcPort, L3Address destAddress, int destPort);
    EthernetIIFrame* EthernetPacketEncapsulation(cPacket* packet, MACAddress Src, MACAddress Dest, int ethertype = ETH_P_BATMAN);
    int batadv_send_broadcast_packet(cPacket* packet, batadv_hard_iface *hard_iface);
    int batadv_send_packet(cPacket *packet, batadv_hard_iface *hard_iface,MACAddress dst_addr);
    int batadv_send_unicast_packet(cPacket *packet, Neigh_node_ptr neigh);
    int batadv_send_packet_to_orig(cPacket *packet, Orig_node_ptr orig_node, batadv_hard_iface *recv_if);
    bool batadv_send_prepare_unicast(cPacket *packet, Orig_node_ptr orig_node);
    bool batadv_send_prepare_unicast_4addr(cPacket *packet, Orig_node_ptr orig, int packet_subtype);
    int batadv_send_packet_unicast(cPacket *packet, int packet_type, int packet_subtype, Orig_node_ptr orig_node, unsigned short vid);
    bool batadv_send_push_fill_unicast(cPacket *packet, int hdr_size, Orig_node_ptr orig_node);
    int batadv_send_packet_via_tt_generic(cPacket *packet, int packet_type, int packet_subtype, MACAddress dst_hint, unsigned short vid);
    int batadv_send_packet_via_gw(cPacket *packet, unsigned short vid);
    batadv_forw_packet *batadv_forw_packet_alloc(batadv_hard_iface *if_incoming, batadv_hard_iface *if_outgoing, cPacket *packet);
    void batadv_forw_packet_bcasts_inc(batadv_forw_packet *forw_packet);
    void sendTCP(cPacket *packet, MACAddress dest, MACAddress src);


/** Originator Messages version 2 (OGMv2) -> bat_v_ogm-Class */
    batadv_ogm2_packet* createOGMv2Packet();
    Orig_node_ptr batadv_v_ogm_orig_get(MACAddress addr);
    void batadv_v_ogm_send();
    void batadv_v_ogm_send_to_if(batadv_ogm2_packet *ogmv2_packet, batadv_hard_iface *hard_iface);
    int batadv_v_ogm_enable();
    void batadv_v_ogm_disable();
    uint32_t batadv_v_forward_penalty(batadv_hard_iface *if_incoming, batadv_hard_iface *if_outgoing, uint32_t throughput);
    void batadv_v_ogm_forward(batadv_ogm2_packet *ogm_received, Orig_node_ptr orig_node, Neigh_node_ptr neigh_node, batadv_hard_iface *if_incoming, batadv_hard_iface *if_outgoing);
    int batadv_v_ogm_metric_update(batadv_ogm2_packet *ogm2, Orig_node_ptr orig_node, Neigh_node_ptr neigh_node, batadv_hard_iface *if_incoming, batadv_hard_iface *if_outgoing);
    bool batadv_v_ogm_route_update(MACAddress ethhdr, batadv_ogm2_packet *ogm2, Orig_node_ptr orig_node, Neigh_node_ptr neigh_node, batadv_hard_iface *if_incoming, batadv_hard_iface *if_outgoing);
    void batadv_v_ogm_process_per_outif(MACAddress ethhdr, batadv_ogm2_packet *ogm2, Orig_node_ptr orig_node, Neigh_node_ptr neigh_node, batadv_hard_iface *if_incoming, batadv_hard_iface *if_outgoing);
    bool batadv_v_ogm_aggr_packet(int buff_pos, int packet_len, uint16_t tvlv_len);
    void batadv_v_ogm_process(batadv_ogm2_packet *ogm_packet, int ogm_offset, batadv_hard_iface *if_incoming, MACAddress neigh);
    int batadv_v_ogm_packet_recv(batadv_ogm2_packet *ogm_packet, batadv_hard_iface *if_incoming, MACAddress src, MACAddress dest);


/** Originator-Tasks -> originator-Class */
    Orig_node_ptr batadv_orig_find(MACAddress orig);
    Orig_node_ptr batadv_orig_node_new(MACAddress addr);
    Neigh_node_ptr batadv_neigh_node_get_or_create(Orig_node_ptr orig_node, batadv_hard_iface *hard_iface, MACAddress neigh_addr);
    Neigh_node_ptr batadv_neigh_node_create(Orig_node_ptr orig_node, batadv_hard_iface *hard_iface, MACAddress neigh_addr);
    Neigh_node_ptr batadv_neigh_node_get(Orig_node_ptr orig_node, batadv_hard_iface *hard_iface, MACAddress addr);
    Hardif_neigh_node_ptr batadv_hardif_neigh_get_or_create(batadv_hard_iface *hard_iface, MACAddress neigh_addr, Orig_node_ptr orig_node);
    Hardif_neigh_node_ptr batadv_hardif_neigh_get(batadv_hard_iface *hard_iface, MACAddress neigh_addr);
    Hardif_neigh_node_ptr batadv_hardif_neigh_create(batadv_hard_iface *hard_iface, MACAddress neigh_addr, Orig_node_ptr orig_node);
    Orig_ifinfo_ptr batadv_orig_ifinfo_new(Orig_node_ptr orig_node, batadv_hard_iface *if_outgoing);
    Orig_ifinfo_ptr batadv_orig_ifinfo_get(Orig_node_ptr orig_node, batadv_hard_iface *if_outgoing);
    Neigh_node_ptr batadv_orig_router_get(Orig_node_ptr orig_node, batadv_hard_iface *if_outgoing);
    Neigh_ifinfo_ptr batadv_neigh_ifinfo_get(Neigh_node_ptr neigh, batadv_hard_iface *if_outgoing);
    Neigh_ifinfo_ptr batadv_neigh_ifinfo_new(Neigh_node_ptr neigh, batadv_hard_iface *if_outgoing);
    void batadv_purge_neigh_ifinfo(Neigh_node_ptr neigh);
    bool batadv_purge_orig_ifinfo(Orig_node_ptr orig_node);
    bool batadv_purge_orig_neighbors(Orig_node_ptr orig_node);
    bool batadv_purge_hardif_neigh();
    Neigh_node_ptr batadv_find_best_neighbor(Orig_node_ptr orig_node, batadv_hard_iface *if_outgoing);
    bool batadv_purge_orig_node(Orig_node_ptr orig_node);
    void batadv_purge_orig_ref();
    void batadv_purge_orig();
    void _batadv_purge_orig();
    int batadv_orig_hash_add_if(batadv_hard_iface *hard_iface, unsigned int max_if_num);
    int batadv_orig_hash_del_if(batadv_hard_iface *hard_iface, unsigned int max_if_num);


/** Routing-Tasks -> routing-Class */
    bool batadv_check_management_packet(cPacket *packet, batadv_hard_iface *hard_iface, int header_len, MACAddress src, MACAddress dest);
    bool batadv_window_protected(int32_t seq_num_diff, int32_t seq_old_max_diff, simtime_t *last_reset, bool *protection_started);
    void batadv_update_route(Orig_node_ptr orig_node, batadv_hard_iface *recv_if, Neigh_node_ptr neigh_node);
    void _batadv_update_route(Orig_node_ptr orig_node, batadv_hard_iface *recv_if, Neigh_node_ptr neigh_node);
    Neigh_node_ptr batadv_find_router(Orig_node_ptr orig_node, batadv_hard_iface *recv_if);
    int batadv_recv_my_icmp_packet(batadv_icmp_packet *icmp_packet);
    int batadv_recv_icmp_ttl_exceeded(batadv_icmp_packet *icmp_packet);
    int batadv_recv_icmp_packet(batadv_icmp_packet *icmp_packet, batadv_hard_iface *recv_if, MACAddress ethhdr_src, MACAddress ethhdr_dest);
    int batadv_check_unicast_packet(batadv_unicast_packet *unicast_packet, int hdr_size, MACAddress ethhdr_src, MACAddress ethhdr_dest);
    Orig_ifinfo_ptr batadv_last_bonding_get(Orig_node_ptr orig_node);
    void batadv_last_bonding_replace(Orig_node_ptr orig_node, Orig_ifinfo_ptr new_candidate);
    int batadv_route_unicast_packet(cPacket *packet, batadv_hard_iface *recv_if, MACAddress ethhdr_src, MACAddress ethhdr_dest);
    int batadv_recv_unhandled_unicast_packet(batadv_unicast_packet *unicast_packet, batadv_hard_iface *recv_if, MACAddress ethhdr_src, MACAddress ethhdr_dest);
    int batadv_recv_unicast_packet(batadv_unicast_packet *unicast_packet, batadv_hard_iface *recv_if, MACAddress ethhdr_src, MACAddress ethhdr_dest);
    int batadv_recv_unicast_tvlv(batadv_unicast_tvlv_packet *packet, batadv_hard_iface *recv_if, MACAddress ethhdr_src, MACAddress ethhdr_dest);
    int batadv_recv_frag_packet(batadv_frag_packet *packet, batadv_hard_iface *recv_if, MACAddress ethhdr_src, MACAddress ethhdr_dest);
    int batadv_recv_bcast_packet(batadv_bcast_packet *bcast_packet, batadv_hard_iface *recv_if, MACAddress ethhdr_src, MACAddress ethhdr_dest);


/** Hard-Interfaces -> hard_interface-Class */
    bool batadv_is_wifi_hardif(batadv_hard_iface *hard_iface);
    bool batadv_is_cfg80211_hardif(batadv_hard_iface *hard_iface);
    int batadv_hardif_no_broadcast(batadv_hard_iface *if_outgoing, MACAddress orig_addr, MACAddress orig_neigh);
    void batadv_primary_if_update_addr(batadv_hard_iface *oldif);
    void batadv_primary_if_select(batadv_hard_iface *new_hard_iface);
    batadv_hard_iface *batadv_primary_if_get_selected();
    void batadv_hardif_activate_interface(batadv_hard_iface *hard_iface);
    void batadv_hardif_deactivate_interface(batadv_hard_iface *hard_iface);


 /** Batman V Tasks -> bat_v-Class */
    void batadv_v_iface_activate(batadv_hard_iface *hard_iface);
    void batadv_v_hardif_neigh_print(Hardif_neigh_node_ptr hardif_neigh);
    void batadv_v_orig_print_neigh(Orig_node_ptr orig_node, batadv_hard_iface *if_outgoing);
    int batadv_v_neigh_cmp(Neigh_node_ptr neigh1, batadv_hard_iface *if_outgoing1, Neigh_node_ptr neigh2, batadv_hard_iface *if_outgoing2);
    int batadv_v_iface_enable(batadv_hard_iface *hard_iface);
    void batadv_v_iface_disable(batadv_hard_iface *req_listhard_iface);
    bool batadv_v_neigh_is_sob(Neigh_node_ptr neigh1, batadv_hard_iface *if_outgoing1, Neigh_node_ptr neigh2, batadv_hard_iface *if_outgoing2);


 /** TVLV-Tasks -> tvlv-Class */
    int batadv_tvlv_containers_process(bool ogm_source, Orig_node_ptr orig_node, MACAddress src, MACAddress dst, void *tvlv_value, uint16_t tvlv_value_len);
    void batadv_tvlv_unicast_send(MACAddress src, MACAddress dst, uint8_t type, uint8_t version, void *tvlv_value, uint16_t tvlv_value_len);
    uint16_t batadv_tvlv_container_ogm_append(OGMv2_buff &packet_buff, int packet_buff_len, int packet_min_len);
    uint16_t batadv_tvlv_container_list_size();
    bool batadv_tvlv_realloc_packet_buff(OGMv2_buff &packet_buff, int packet_buff_len, int min_packet_len, int additional_packet_len);
    TvlvContainer_ptr batadv_tvlv_container_get(uint8_t type, uint8_t version);
    void batadv_tvlv_container_remove(TvlvContainer_ptr tvlv);
    void batadv_tvlv_container_unregister(uint8_t type, uint8_t version);
    void batadv_tvlv_container_register(uint8_t type, uint8_t version, void *tvlv_value, uint16_t tvlv_value_len);
    TvlvHandler_ptr batadv_tvlv_handler_get(uint8_t type, uint8_t version);
    void batadv_tvlv_handler_register(void(Batman5Routing::*optr)(Orig_node_ptr orig, uint8_t flags, void *tvlv_value, uint16_t tvlv_value_len), int(Batman5Routing::*uptr)(MACAddress src, MACAddress dst, void *tvlv_value, uint16_t tvlv_value_len), uint8_t type, uint8_t version, uint8_t flags);
    void batadv_tvlv_handler_unregister(uint8_t type, uint8_t version);
    int batadv_tvlv_call_handler(TvlvHandler_ptr tvlv_handler, bool ogm_source, Orig_node_ptr orig_node, MACAddress src, MACAddress dst, void *tvlv_value, uint16_t tvlv_value_len);


 /** Translation-Table Tasks -> translation-table-Class */
    int batadv_tt_init();
    int batadv_tt_local_init();
    int batadv_tt_global_init();
    void batadv_tt_purge();
    void batadv_tt_local_purge(int timeout);
    void batadv_tt_global_purge();
    void batadv_tt_req_purge();
    void batadv_tt_roam_purge();
    void batadv_tt_local_purge_list(uint32_t timeout);
    bool batadv_tt_global_to_purge(batadv_tt_global_entry *tt_global, char **msg);
    void batadv_tt_local_commit_changes();
    void batadv_tt_local_commit_changes_nolock();
    void batadv_tt_local_set_flags(uint16_t flags, bool enable, bool count);
    void batadv_tt_tvlv_container_update();
    uint16_t batadv_tt_prepare_tvlv_local_data(batadv_tvlv_tt_data **tt_data, batadv_tvlv_tt_change **tt_change, int32_t *tt_len);
    void batadv_tt_local_purge_pending_clients();
    bool batadv_send_tt_response(batadv_tvlv_tt_data *tt_data, MACAddress req_src, MACAddress req_dst);
    int batadv_tt_tvlv_unicast_handler_v1(MACAddress src, MACAddress dst, void *tvlv_value, uint16_t tvlv_value_len);
    void batadv_tt_tvlv_ogm_handler_v1(Orig_node_ptr orig, uint8_t flags, void *tvlv_value, uint16_t tvlv_value_len);
    int batadv_roam_tvlv_unicast_handler_v1(MACAddress src, MACAddress dst, void *tvlv_value, uint16_t tvlv_value_len);
    bool batadv_send_my_tt_response(batadv_tvlv_tt_data *tt_data, MACAddress req_src);
    bool batadv_send_other_tt_response(batadv_tvlv_tt_data *tt_data, MACAddress req_src, MACAddress req_dst);
    void batadv_tt_tvlv_generate(batadv_tvlv_tt_change *tvlv_buff, bool usingLocalTable, uint16_t tt_len, bool (Batman5Routing::*valid_cb)(void *, Orig_node_ptr), Orig_node_ptr cb_data);
    bool batadv_tt_local_valid(void *entry_ptr, Orig_node_ptr data_ptr);
    bool batadv_tt_global_valid(void *entry_ptr, Orig_node_ptr data_ptr);
    bool batadv_tt_global_entry_has_orig(batadv_tt_global_entry *entry, Orig_node_ptr orig_node);
    batadv_tt_orig_list_entry *batadv_tt_global_orig_entry_find(batadv_tt_global_entry *entry, Orig_node_ptr orig_node);
    uint16_t batadv_tt_prepare_tvlv_global_data(Orig_node_ptr orig_node, batadv_tvlv_tt_data **tt_data, batadv_tvlv_tt_change **tt_change, int32_t *tt_len);
    bool batadv_tt_global_check_crc(Orig_node_ptr orig_node, batadv_tvlv_tt_vlan *tt_vlan, uint16_t num_vlan);
    void batadv_handle_tt_response(batadv_tvlv_tt_data *tt_data, MACAddress resp_src, uint16_t num_entries);
    void batadv_tt_update_changes(Orig_node_ptr orig_node, uint16_t tt_num_changes, uint8_t ttvn, batadv_tvlv_tt_change *tt_change);
    void _batadv_tt_update_changes(Orig_node_ptr orig_node, batadv_tvlv_tt_change *tt_change, uint16_t tt_num_changes, uint8_t ttvn);
    void batadv_tt_fill_gtable(batadv_tvlv_tt_change *tt_change, uint8_t ttvn, MACAddress resp_src, uint16_t num_entries);
    void batadv_tt_global_del_orig(Orig_node_ptr orig_node, int32_t match_vid, const char *message);
    void batadv_tt_global_update_crc(Orig_node_ptr orig_node);
    void batadv_tt_global_del_orig_node(batadv_tt_global_entry *tt_global_entry, Orig_node_ptr orig_node, const char *message);
    void _batadv_tt_global_del_orig_entry(batadv_tt_global_entry *tt_global_entry, batadv_tt_orig_list_entry *orig_entry);
    void batadv_tt_global_del(Orig_node_ptr orig_node, MACAddress addr, unsigned short vid, const char *message, bool roaming);
    bool batadv_tt_global_add(Orig_node_ptr orig_node, MACAddress tt_addr, unsigned short vid, uint16_t flags, uint8_t ttvn);
    void batadv_tt_global_del_orig_list(batadv_tt_global_entry *tt_global_entry);
    batadv_tt_global_entry *batadv_tt_global_find(MACAddress addr, unsigned short vid);
    batadv_tt_local_entry *batadv_tt_local_find(MACAddress addr, unsigned short vid);
    uint16_t batadv_tt_local_remove(MACAddress addr, unsigned short vid, const char *message, bool roaming);
    void batadv_tt_global_free(batadv_tt_global_entry *tt_global, const char *message);
    void batadv_tt_global_del_roaming(batadv_tt_global_entry *tt_global_entry, Orig_node_ptr orig_node, const char *message);
    void batadv_tt_local_set_pending(batadv_tt_local_entry *tt_local_entry, uint16_t flags, const char *message);
    void batadv_tt_save_orig_buffer(Orig_node_ptr orig_node, batadv_tvlv_tt_change *tt_buff, uint16_t tt_buff_len);
    void batadv_tt_global_orig_entry_add(batadv_tt_global_entry *tt_global, Orig_node_ptr orig_node, int ttvn, uint8_t flags);
    void batadv_tt_global_sync_flags(batadv_tt_global_entry *tt_global);
    void batadv_tt_update_orig(Orig_node_ptr orig_node, const void *tt_buff, uint16_t tt_num_vlan, batadv_tvlv_tt_change *tt_change, uint16_t tt_num_changes, uint8_t ttvn);
    bool batadv_send_tt_request(Orig_node_ptr dst_orig_node, uint8_t ttvn, batadv_tvlv_tt_vlan *tt_vlan, uint16_t num_vlan, bool full_table);
    TT_req_node_ptr batadv_tt_req_node_new(Orig_node_ptr orig_node);
    void batadv_tt_local_event(batadv_tt_local_entry *tt_local_entry, uint8_t event_flags);


 /** Throughput-Meter Tasks -> tp_meter-Class */
    void batadv_tp_meter_recv(batadv_icmp_tp_packet *icmp_packet);
    void batadv_tp_recv_msg(batadv_icmp_tp_packet *icmp);
    void batadv_tp_recv_ack(batadv_icmp_tp_packet *icmp);


 /** Network-Coding-Tasks -> network_coding-Class */
    int batadv_nc_init();
    void batadv_nc_start_timer(cMessage *msg);
    void batadv_nc_tvlv_container_update();
    void batadv_nc_status_update();
    void batadv_nc_tvlv_ogm_handler_v1(Orig_node_ptr orig, uint8_t flags, void *tvlv_value, uint16_t tvlv_value_len);
    int batadv_nc_mesh_init();
    void batadv_nc_init_bat_priv();
    void batadv_nc_init_orig(Orig_node_ptr orig_node);
    void batadv_nc_packet_free(NC_packet_ptr nc_packet, bool dropped);
    bool batadv_nc_to_purge_nc_node(NC_node_ptr nc_node);
    bool batadv_nc_to_purge_nc_path_coding(NC_path_ptr nc_path);
    bool batadv_nc_to_purge_nc_path_decoding(NC_path_ptr nc_path);
    void batadv_nc_purge_orig_nc_nodes(Orig_node_ptr orig_node, bool (Batman5Routing::*to_purge)(NC_node_ptr));
    void batadv_nc_purge_orig(Orig_node_ptr orig_node, bool (Batman5Routing::*to_purge)(NC_node_ptr));
    void batadv_nc_purge_orig_hash();
    void batadv_nc_purge_paths(bool (Batman5Routing::*to_purge)(NC_path_ptr));
    bool batadv_nc_hash_compare(NC_path_ptr nc_path1, MACAddress src, MACAddress dest);
    NC_path_ptr batadv_nc_hash_find(MACAddress src, MACAddress dest);
    void batadv_nc_send_packet(NC_packet_ptr nc_packet);
    bool batadv_nc_sniffed_purge(NC_path_ptr nc_path, NC_packet_ptr nc_packet);
    bool batadv_nc_fwd_flush(NC_path_ptr nc_path, NC_packet_ptr nc_packet);
    void batadv_nc_process_nc_paths(bool (Batman5Routing::*process_fn)(NC_path_ptr, NC_packet_ptr), bool coding);
    void batadv_nc_worker();
    bool batadv_can_nc_with_orig(Orig_node_ptr orig_node, batadv_ogm2_packet *ogm_packet);
    NC_node_ptr batadv_nc_find_nc_node(Orig_node_ptr orig_node, Orig_node_ptr orig_neigh_node, bool in_coding);
    NC_node_ptr batadv_nc_get_nc_node(Orig_node_ptr orig_node, Orig_node_ptr orig_neigh_node, bool in_coding);
    void batadv_nc_update_nc_node(Orig_node_ptr orig_node, Orig_node_ptr orig_neigh_node, batadv_ogm2_packet *ogm_packet, int is_single_hop_neigh);
    NC_path_ptr batadv_nc_get_path(MACAddress src, MACAddress dst);
    uint8_t batadv_nc_random_weight_tq(uint8_t tq);
    void batadv_nc_memxor(batadv_coded_packet* coded_packet, batadv_unicast_packet *packet1, batadv_unicast_packet *packet2/*char *dst, const char *src, unsigned int len*/);
    bool batadv_nc_code_packets(batadv_unicast_packet *uni_packet, NC_packet_ptr nc_packet, Neigh_node_ptr neigh_node);
    bool batadv_nc_packet_coding_possible(MACAddress dst, MACAddress src);
    NC_packet_ptr batadv_nc_path_search(NC_node_ptr in_nc_node, NC_node_ptr out_nc_node, MACAddress eth_dst);
    NC_packet_ptr batadv_nc_packet_src_search(MACAddress eth_dst, MACAddress eth_src, NC_node_ptr in_nc_node);
    void batadv_nc_packet_store_before_coding(batadv_unicast_packet *packet, MACAddress eth_dst_new);
    bool batadv_nc_packet_dst_search(batadv_unicast_packet *packet, Neigh_node_ptr neigh_node, MACAddress h_source);
    bool batadv_nc_packet_add_to_path(batadv_unicast_packet *packet, NC_path_ptr nc_path, Neigh_node_ptr neigh_node, uint32_t packet_id, bool coding);
    bool batadv_nc_packet_forward(batadv_unicast_packet *packet, Neigh_node_ptr neigh_node);
    void batadv_nc_packet_store_for_decoding(batadv_unicast_packet *packet);
    void batadv_nc_packet_store_sniffed_unicast(batadv_unicast_packet *packet);
    batadv_unicast_packet *batadv_nc_packet_decode_packet(batadv_coded_packet *packet, NC_packet_ptr nc_packet);
    NC_packet_ptr batadv_nc_find_decoding_packet(batadv_coded_packet *coded, MACAddress h_source, MACAddress h_dest);
    int batadv_nc_recv_coded_packet(batadv_coded_packet *packet, batadv_hard_iface *recv_if, MACAddress h_source, MACAddress h_dest);
    void batadv_nc_mesh_free();
    void release_coded_packet(batadv_coded_packet *packet);

 /** BitArray-Tasks -> bitarray-Class */
    bool batadv_test_bit(/*const unsigned long *seq_bits, */uint32_t last_seqno, uint32_t curr_seqno);
    bool batadv_bit_get_packet(/*unsigned long *seq_bits, */int32_t seq_num_diff, int set_mark);
    void batadv_set_bit(/*unsigned long *seq_bits, */ int32_t n);
    void batadv_bitmap_shift_left(/*unsigned long *seq_bits, */ int32_t n);

 /** Metric-Tasks -> metric-Class */
    uint32_t map(double x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max);
    double map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max);
    void initializeMetric();
    void metricMatchLinkCharacteristicts(Hardif_neigh_node_ptr hardif_neigh, batadv_ogm2_packet *ogm_packet);
    uint32_t calculatePathMetric(batadv_hard_iface *if_incoming, batadv_hard_iface *if_outgoing, uint32_t metric);
    uint32_t getLinkMetric(Hardif_neigh_node_ptr neigh);

    void ewma_metric_init(ewma_Metric *avg);
    uint32_t getEWMAMetric(ewma_Metric *avg);
    ewma_Metric *ewma_metric_add(ewma_Metric *avg, uint32_t value);

    uint32_t getReversePathMetric(Neigh_ifinfo_ptr neigh_ifinfo);
    void addReversePathMetric(Neigh_ifinfo_ptr neigh_ifinfo, uint32_t path_metric);

    // Throughput Metric
    uint32_t ewma_metric_read(ewma_Metric *avg);

    // Distance Metric
    cModule* getHostFromMac(MACAddress &_mac);
    cModule* getMobilityFromMAC(MACAddress &_mac);
    Coord getPositionFromMAC(MACAddress &_mac);
    double getDistanceToMAC(MACAddress &_mac);
    uint32_t getDistanceMetric(MACAddress &_mac);

    // predictiveMobility Metric
    uint32_t getPredictiveMobilityMetric(MACAddress &_mac);
    double getFutureDistanceToMAC(MACAddress &_mac);
    Coord getFuturePositionFromMAC(MACAddress &_mac, double futureSecs);

    // BATMobile
    uint32_t getScoredMetric(MACAddress &_mac);


};

} //namespace

#endif
