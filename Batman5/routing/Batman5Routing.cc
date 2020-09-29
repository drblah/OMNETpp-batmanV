//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// clear_bit
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 


#include "../../Batman5/routing/Batman5Routing.h"

namespace inet {
using namespace std;


Define_Module(Batman5Routing);

MACAddress Batman5Routing::get_hardif_MAC(const char *name){
    return p_interfaceTable->getInterfaceByName(name)->getMacAddress();
}//get_hardif_MAC

MACAddress Batman5Routing::get_hardif_MAC(const unsigned int id){
    return p_interfaceTable->getInterfaceById(id)->getMacAddress();
}//get_hardif_MAC


bool Batman5Routing::is_valid_ether_addr(MACAddress ether_addr){
    return (!ether_addr.isMulticast() && !ether_addr.isUnspecified() && !ether_addr.isBroadcast());
}//is_valid_ether_addr

void Batman5Routing::clear_bit(uint32_t bit_to_clear, uint32_t& number){
    number &= ~(1UL << bit_to_clear);
}//clear_bit

void Batman5Routing::set_bit(uint32_t bit_to_set, uint32_t& number){
    number |= 1UL << bit_to_set;
}//set_bit

/**
 * batadv_max_header_len() - calculate maximum encapsulation overhead for a payload packet
 * Return: the maximum encapsulation overhead in bytes.
 */
int Batman5Routing::batadv_max_header_len(){
    uint64_t header_len = 0;

    uint64_t packet_header_len = BATADV_UNICAST_HLEN;
    header_len = std::max(header_len, packet_header_len);
    packet_header_len = BATADV_UNICAST_4ADDR_HLEN;
    header_len = std::max(header_len, packet_header_len);
    packet_header_len = BATADV_BCAST_HLEN;
    header_len = std::max(header_len, packet_header_len);

    if (networkCoding){
       packet_header_len = BATADV_CODED_HLEN;
       header_len = std::max(header_len, packet_header_len);
    }

    return header_len + ETH_HLEN;
}//batadv_max_header_len

// This function was used to evaluate the batadv packet-implementation
void Batman5Routing::printPaketLength(cMessage *msg){
    std::string outs;

    //reset mutable variables
    bool showEncapsulatedPackets = true;

    for (cPacket *pk = dynamic_cast<cPacket *>(msg); showEncapsulatedPackets && pk; pk = pk->getEncapsulatedPacket()) {
        std::ostringstream out;
        out << pk->getClassName() << ":" << pk->getByteLength() << " bytes";
        if (outs.length())
            out << " <- " << outs;
        outs = out.str();
    }
    cout << outs << endl;;
}

/**
 * batadv_is_my_mac() - check if the given mac address belongs to any of the real interfaces of the host
 * @address: the address to check
 * Return: 'true' if the mac address was found, false otherwise.
 */
bool Batman5Routing::batadv_is_my_mac(MACAddress address){
//    std::cout << "batadv_is_my_mac with: " << address.str() << std::endl;

    if (address.equals(hostMAC)){
        return true;
    }
    else if (address.equals(wlan0MAC)){
        return true;
    }
    else if (address.equals(wlan1MAC)){
        return true;
    }
    else{
        return false;
    }
}//batadv_is_my_mac

/**
 * batadv_has_timed_out() - compares current time and timestamp + timeout
 * @timestamp:      base value to compare with
 * @timeout:        added to base value before comparing (in milliseconds)
 * Return: true if current time is after timestamp + timeout */
bool Batman5Routing::batadv_has_timed_out(simtime_t timestamp, uint32_t timeout){
    double timeout_dbl = (double)timeout/1000;
    if (simTime().dbl() > timestamp.dbl() + timeout_dbl)
        return true;
    else
        return false;
}//batadv_has_timed_out

batadv_hard_iface* Batman5Routing::getHardif_by_ifID(int if_ID){
    if(wlan0->if_id == if_ID){
        return wlan0;
    }
    else if (wlan1->if_id == if_ID){
        return wlan1;
    }
    else {
        cout << "Could not find Interface-ID " << if_ID << " in InterfaceTable. Interface -ID is unknown!" << endl;
        return NULL;
    }
}//getHardif_by_ifID

InterfaceEntry *Batman5Routing::getInterfaceByID(int if_ID){
    InterfaceEntry *entry = p_interfaceTable->getInterfaceById(if_ID);
    if(entry == NULL)
        cout << hostModule->getFullName() << ": InterfaceEntry is NULL! if_ID: " << if_ID << endl;
    return entry;
}//getInterfaceByID


int Batman5Routing::getRandomSeqno(){
    return (int) uniform(0, BATADV_EXPECTED_SEQNO_RANGE);
}//getRandomSeqno

uint64_t Batman5Routing::getRandomId(){
    return (uint64_t) intuniform(0, pow(2,16)-1);
}//getRandomId

/**
 * Received a packet for me
 * -> Check the type of the packet
 * -> Sending it upperLayerOut
 */
void Batman5Routing::batadv_interface_rx(cPacket *packet, Orig_node_ptr orig_node){
    if (dynamic_cast<batadv_unicast_packet *>(packet) != NULL){
        batadv_unicast_packet *uni_packet = dynamic_cast<batadv_unicast_packet *>(packet);
        L3Address srcAddr  = arp_table->getL3AddressFor(uni_packet->getSrc());
        L3Address destAddr = arp_table->getL3AddressFor(uni_packet->getDest());

        cPacket *decap_packet = uni_packet->decapsulate();
//        cout << hostModule->getFullName() << " received Unicast-Packet as final destination! " << " "  << endl;
        delete uni_packet;
        string fullName = decap_packet->getFullName();

        if(dynamic_cast<UDPPacket *>(decap_packet) != NULL){
            UDPPacket *udp_packet = dynamic_cast<UDPPacket *>(decap_packet);
//            cout << hostModule->getFullName() << " received UDP-Packet as final destination!" << endl;
            if(packet->getControlInfo() != NULL){
//                cout << hostModule->getFullName() << ": UDP-Packet mit ControlInfo " << packet->getControlInfo()->getFullName() << endl;
            }

            IPv4CtrlEncapsulation(udp_packet, destAddr, srcAddr);

            emit(received_UDP, udp_packet);
            emit(sent_packet_to_upper,udp_packet);
            numReceived_UDP += 1;
            send(udp_packet, "upperLayerOut", UDP_GATE);
//            cout << "UDP-Packet was received by Destination " << hostModule->getFullName() << endl << endl;
        }

        else if(fullName == "SYN" ||
                fullName == "SYN+ACK" ||
                fullName == "ACK" ||
                fullName.find("tcpseg") != string::npos ||
                fullName == "FIN") {
//            cout << hostModule->getFullName() << " received " << fullName << " from " << srcAddr.str() << endl;
            IPv4CtrlEncapsulation(decap_packet, destAddr, srcAddr, 6);
            send(decap_packet, "upperLayerOut", TCP_GATE);
        }
        else if(fullName.find("data") != string::npos){
//            cout << hostModule->getFullName() << ": Received Data from " << srcAddr.str() << endl;
            IPv4CtrlEncapsulation(decap_packet, destAddr, srcAddr, 6);
            send(decap_packet, "upperLayerOut", TCP_GATE);
        }

        else{
            cout << hostModule->getFullName() << " received unknown decapsulated packettype (" << decap_packet->getName() << ") as final destination!" << endl;
            delete decap_packet;
        }

    }
    else{
        cout << hostModule->getFullName() << " received unknwon packettype (" << packet->getName() << ") as final destination!" << endl;
    }

}//batadv_interface_rx


void Batman5Routing::initialize(int stage) {
    hostModule = this->getParentModule();
//    cout << hostModule->getFullName() << " Stage " << stage << endl;

    if(stage == INITSTAGE_LOCAL){
        //cout << "***** INITSTAGE LOCAL *****" << endl;

        cModule *wlan = this->getModuleByPath(".wlan[0]");

        mobility = dynamic_cast<MobilityBase*>(hostModule->getSubmodule("mobility"));
        if(mobility)
        {
//            cout << hostModule->getFullName() << ": " << mobility->getCurrentPosition() << endl;
        }

        p_interfaceTable = getModuleFromPar<IInterfaceTable>(par("interfaceTableModule"), this);
        p_routingTable = getModuleFromPar<IRoutingTable>(par("routingTableModule"), this);
//        p_networkProtocol = getModuleFromPar<INetfilter>(par("networkProtocolModule"), this);
        arp_table = getModuleFromPar<GlobalARP>(par("arpTableModule"),this);

        // Statistics
        received_OGMv2 = registerSignal("SuccessfulReceivedOGMv2");
        sent_OGMv2 = registerSignal("SuccessfulSendOGMv2");
        received_ELP = registerSignal("SuccessfulReceivedELP");
        sent_ELP = registerSignal("SuccessfulSendELP");
        sent_ELP_probes = registerSignal("SuccessfulSendELPProbes");
        received_UDP = registerSignal("ReceivedUDP");
        sent_UDP = registerSignal("SendUDP");
        noRouter_found_UDP = registerSignal("ForwardUDPnotPossible");
        forward_unicast = registerSignal("ForwardedUnicasts");

        received_packets_from_upper = registerSignal("packetsReceivedFromUp");
        received_packets_from_lower = registerSignal("packetsReceivedFromLow");
        sent_packet_to_upper = registerSignal("packetsSentUp");
        sent_packet_to_lower = registerSignal("packetsSentLow");

        nc_coding_Counter = 0;
        nc_coding_delete_Counter = 0;
        nc_decoding_Counter = 0;
        nc_decoding_delete_Counter = 0;

        this->elp_seqno = getRandomSeqno();
        this->ogmv2_ttl = par("ogmv2_ttl");
        this->bonding = par("bonding");
        this->elpProbing = par("elpProbing");
        this->networkCoding = par("networkCoding");
        this->fragmentation = par("fragmentation");
        this->hop_penalty = par("hop_penalty");
        this->tt_purge_timeout_interval = par("tt_purge_timeout");
        this->purge_timeout = par("purge_timeout");
        this->purge_timeout_ogmv2 = par("purge_timeout_ogmv2");
        this->elp_interval = par("elp_interval");
        this->elp_interval_packet = elp_interval.dbl()*1000;
        this->ogmv2_interval = par("ogmv2_interval");
        this->neigh_timeout_interval = par("neigh_timeout_interval");
        this->orig_timeout_interval = par("orig_timeout_interval");
        this->jitter = par("jitter");
        this->nc_work_interval = par("nc_work_interval");

        priv_tvlv = new batadv_priv_tvlv();

        priv_bat_v = new batadv_priv_bat_v();
        priv_bat_v->ogm_buff = OGMv2_buff();
        priv_bat_v->ogm_buff_len = 0;
        priv_bat_v->ogm_seqno = 0;

        wlan0 = NULL;
        wlan1 = NULL;
        primary_iface = NULL;

        bat_priv_tt = batadv_priv_tt();
        bat_priv_tt.local_changes = 1;
        bat_priv_tt.vn = 0;
        bat_priv_tt.last_changeset_len = 0;
        bat_priv_tt.ogm_append_cnt = 0;
        bat_priv_tt.vlan_expanded = true;

        // Reminders
        this->ELP_reminder_wlan0 = NULL;
        this->ELP_reminder_wlan1 = NULL;
        this->OGMv2_reminder = NULL;
        this->originatorTableTimeout = NULL;

    }
    else if(stage == INITSTAGE_PHYSICAL_ENVIRONMENT){

    }
    else if (stage == INITSTAGE_LINK_LAYER_2){
        //cout << "***** INITSTAGE LINK LAYER 2 *****" << endl;
    }
    else if (stage == INITSTAGE_NETWORK_LAYER_2){
        //cout << "***** INITSTAGE NETWORK LAYER 2 *****" << endl;
    }
    else if (stage == INITSTAGE_ROUTING_PROTOCOLS) {

        realValues = false; // default false, will be set in metric initialize (if used)
        initializeMetric();

        // Initialize Hard-interfaces
        if(p_interfaceTable->getInterfaceByName("wlan0") != NULL){
            wlan0 = new batadv_hard_iface();
            wlan0->if_num = 0;
            wlan0->if_id = p_interfaceTable->getInterfaceByName("wlan0")->getInterfaceId();
            wlan0->if_status = BATADV_IF_INACTIVE;
            wlan0->num_bcasts = 0;
            wlan0->wifi_flags = BATADV_HARDIF_WIFI_CFG80211_DIRECT;
            wlan0->refcount = 0;
            wlan0->mtu = p_interfaceTable->getInterfaceByName("wlan0")->getMTU();
            wlan0->name = "wlan0";
            wlan0->lastProbedThroughput.clear();
            Neighbor_Table_ptr new_neigh_table(&neigh_table);
            wlan0->neigh_table = new_neigh_table;
            wlan0->bat_v.elp_interval = elp_interval_packet;
            wlan0->bat_v.elp_seqno = elp_seqno;
            wlan0->bat_v.flags = 0;
            if(metricType.compare("throughput_overwrite") == 0){
                wlan0->bat_v.throughput_override = (uint32_t)par("throughputOverwrite");
            }
            else{
                wlan0->bat_v.throughput_override = 0;
            }
            wlan0MAC = getInterfaceByID(wlan0->if_id)->getMacAddress();
        }

        if(p_interfaceTable->getInterfaceByName("wlan1") != NULL){
            wlan1 = new batadv_hard_iface();
            wlan1->if_num = 1;
            wlan1->if_id = p_interfaceTable->getInterfaceByName("wlan1")->getInterfaceId();
            wlan1->if_status = BATADV_IF_INACTIVE;
            wlan1->num_bcasts = 0;
            wlan1->wifi_flags = BATADV_HARDIF_WIFI_CFG80211_DIRECT;
            wlan1->refcount = 0;
            wlan1->mtu = p_interfaceTable->getInterfaceByName("wlan1")->getMTU();
            wlan1->name = "wlan1";
            wlan1->lastProbedThroughput.clear();
            Neighbor_Table_ptr new_neigh_table(&neigh_table_wlan1);
            wlan1->neigh_table = new_neigh_table;
            wlan1->bat_v.elp_interval = elp_interval_packet;
            wlan1->bat_v.elp_seqno = elp_seqno;
            wlan1->bat_v.flags = 0;
            if(metricType.compare("throughput_overwrite") == 0){
                wlan0->bat_v.throughput_override = (uint32_t)par("throughputOverwrite");
            }
            else{
                wlan0->bat_v.throughput_override = 0;
            }
            wlan1MAC = getInterfaceByID(wlan1->if_id)->getMacAddress();
        }



    }
    else if (stage == INITSTAGE_LAST){

        if(p_interfaceTable->getInterfaceByName("wlan0") != NULL){
            batadv_hardif_activate_interface(this->wlan0);
        }

        if(p_interfaceTable->getInterfaceByName("wlan1") != NULL){
            batadv_hardif_activate_interface(this->wlan1);
        }

        hostIP = p_routingTable->getRouterIdAsGeneric().toIPv4();

        hostMAC = get_hardif_MAC(primary_iface->name.c_str());
//        cout << hostModule->getFullName() << ": " << hostIP << "  " << hostMAC.str() << endl;

        if(wlan0 != NULL){
//            cout << " wlan0 : " << getInterfaceByID(wlan0->if_id)->getIPv4Address().str() << "  " << wlan0MAC.str() << " " << endl;
        }
        if(wlan1 != NULL){
//            cout << " wlan1 : " << getInterfaceByID(wlan1->if_id)->getIPv4Address().str() << "  " << wlan1MAC.str() << endl << endl;
        }

        // enable ogmv2 (+originatorTableTimeout) and elp (+neighborTabeTimeout) processes
        if(p_interfaceTable->getInterfaceByName("wlan0") != NULL){
            batadv_v_iface_enable(wlan0);
        }
        if(p_interfaceTable->getInterfaceByName("wlan1") != NULL){
            batadv_v_iface_enable(wlan1);
        }

        batadv_tt_init();

        if(networkCoding){
            batadv_nc_init();   // handler register
            batadv_nc_mesh_init(); // init of bat_priv_nc
            batadv_nc_init_bat_priv(); // init of bat_priv_nc max/min values
        }


        // writing a txt-file with the position of the hosts every second
        this->writingTraces = par("writingTraces").boolValue();
        if(writingTraces){
            writeTraces = new cMessage("writingTracess");
            scheduleAt(simTime()+1, writeTraces);
            fileName = par("tracesFileName").stdstringValue();
        }
    }

}//initialize



void Batman5Routing::handleMessage(cMessage *msg) {

    if(msg->isSelfMessage()){
        if(msg == OGMv2_reminder){
//            cout << hostModule->getFullName() << ": OGMv2-Reminder" << endl;
            batadv_v_ogm_send();
            OGMv2_reminder = msg;
            scheduleAt(simTime()+ogmv2_interval+jitter,OGMv2_reminder);

        }
        else if(msg == ELP_reminder_wlan0){
            elp_reminder *reminder = dynamic_cast<elp_reminder *>(msg);
//            cout << hostModule->getFullName() << "ELP---Reminder on " << reminder->hard_iface->name << endl;
            batadv_v_elp_periodic_work(reminder->hard_iface);
            scheduleAt(simTime()+elp_interval+jitter,msg);
        }
        else if(msg == ELP_reminder_wlan1){
            elp_reminder *reminder = dynamic_cast<elp_reminder *>(msg);
            //cout << hostModule->getFullName() << "ELP---Reminder on " << reminder->hard_iface << endl;
            batadv_v_elp_periodic_work(reminder->hard_iface);
            scheduleAt(simTime()+elp_interval+jitter,msg);
        }
        else if(msg == tt_purge){
            //cout << hostModule->getFullName() << "TT-Purge " << endl;
            batadv_tt_purge();
            scheduleAt(simTime()+tt_purge_timeout_interval+jitter,msg);
        }
        else if(msg == hardiface_neighborTableTimeout){
            batadv_purge_hardif_neigh();
            hardiface_neighborTableTimeout = msg;
            scheduleAt(simTime()+neigh_timeout_interval,hardiface_neighborTableTimeout);
        }
        else if(msg == originatorTableTimeout){
            //cout << "Table timeout by " << hostModule->getFullName() << ": Checking tables for obsolete information!" << endl;
            batadv_purge_orig();
            originatorTableTimeout = msg;
            scheduleAt(simTime()+orig_timeout_interval,originatorTableTimeout);
        }
        else if(msg == periodic_nc_work){
//            cout << hostModule->getFullName() << ": Periodic NC Work" << endl;
            batadv_nc_worker();
            batadv_nc_start_timer(msg);
        }

        else if(msg == writeTraces){
            Coord myPosition = Coord(0,0,0);

            MobilityBase *mobility = dynamic_cast<MobilityBase*>(this->getParentModule()->getSubmodule("mobility"));
            if(mobility)
                myPosition = mobility->getCurrentPosition();

            cout << simTime() << ", " << myPosition.x << ", " << myPosition.y << endl;

            /**  Write position into .txt file */
            string filename = "results/RWP-Traces/";
            filename.append(fileName);
            filename.append("_");
            filename.append(to_string(this->getParentModule()->getIndex()));
            filename.append(".txt");

            fstream traces;
            traces.open(filename, ios::out|ios::app|ios::binary);
            traces << simTime() << "," << myPosition.x << "," << myPosition.y << endl;
            traces.close();
            /**  End: Write numReceived into .csv file */

            scheduleAt(simTime()+1, writeTraces);
        }

    }//isSelfMessage

    else if(string(msg->getArrivalGate()->getFullName()).find("lowerlayerIn") != string::npos){
        emit(received_packets_from_lower, dynamic_cast<cPacket *>(msg));

        // EthernetIIFrame Handler
        if(dynamic_cast<EthernetIIFrame *>(msg) != NULL){
//            cout << hostModule->getFullName() << " received an EthernetIIFrame "  << endl;
            if (dynamic_cast<EthernetIIFrame *>(msg)->getEtherType() == 0x4305){ //Check if ethernet-packet is a BATMAN-packet
                MACAddress ethFrame_src = dynamic_cast<EthernetIIFrame *>(msg)->getSrc(); // last sender of packet
                MACAddress ethFrame_dest = dynamic_cast<EthernetIIFrame *>(msg)->getDest();
                batadv_hard_iface *if_incoming = NULL;
                // Check for incoming interface
                if (msg->getArrivalGate()->getIndex() == 0){
                    if_incoming = wlan0;
//                    cout << hostModule->getFullName() << " received Ethernet-Frame over " << if_incoming->name << endl;
                }
                else if (msg->getArrivalGate()->getIndex() == 1){
                    if_incoming = wlan1;
//                    cout << hostModule->getFullName() << " received Ethernet-Frame over " << if_incoming->name << endl;
                }
                else{
                    cout << "if_incoming is unknown! Deleting packet!" << endl;
                    delete msg;
                    return;
                }

                cPacket* received_packet = dynamic_cast<EthernetIIFrame *>(msg)->decapsulate();

                if(dynamic_cast<batadv_ogm2_packet *>(received_packet) != NULL){
//                  cout << hostModule->getFullName() << " received ogmv2 packet"  << endl;
                    int ogmv2HandleCheck = batadv_v_ogm_packet_recv(dynamic_cast<batadv_ogm2_packet *>(received_packet), if_incoming, ethFrame_src, ethFrame_dest);
                    if (ogmv2HandleCheck == NET_RX_SUCCESS){
//                      cout << "NET_RX_SUCCESS" << endl;
                        emit(received_OGMv2, received_packet);
                        batadv_v_ogm_process(dynamic_cast<batadv_ogm2_packet *>(received_packet), 0, if_incoming, ethFrame_src);

                    }
                    else if (ogmv2HandleCheck == NET_RX_DROP){
//                      cout << "NET_RX_DROP" << endl;
                        batadv_ogm2_packet *packet = dynamic_cast<batadv_ogm2_packet *>(received_packet);
                        delete packet;
                    }
                    else{
//                        cout << "ogmv2HandleCheck failed!" << endl;
                        batadv_ogm2_packet *packet = dynamic_cast<batadv_ogm2_packet *>(received_packet);
                        delete packet;
                    }
                }
                else if(dynamic_cast<batadv_elp_packet *>(received_packet) != NULL){
//                      cout << hostModule->getFullName() << " received elp packet over interface " << if_incoming->name << endl;

                        if(dynamic_cast<batadv_elpProbe_packet *>(received_packet) != NULL){
//                            cout << "Received an ELP-Probe-Paket!" << endl;

                            handleELPProbePacket(dynamic_cast<batadv_elpProbe_packet *>(received_packet), if_incoming, ethFrame_src, ethFrame_dest);

                        }
                        else {
                            int elpHandleCheck = handleELPPacket(dynamic_cast<batadv_elp_packet *>(received_packet), if_incoming, ethFrame_src, ethFrame_dest);
                            // check if elp handle (inklusive neighbor update) succeded
                            if (elpHandleCheck == 0){
//                                EV << "ELP-Handle was successful!" << endl;
                            }
                            else {
                                EV << "ELP-Handle failed!" << endl;
                            }
                        }
                    }

                else if(dynamic_cast<batadv_unicast_packet *>(received_packet) != NULL){
//                    MACAddress uni_dest = dynamic_cast<batadv_unicast_packet *>(received_packet)->getDest(); // final destination of packet
//                    cout << hostModule->getFullName() << " received batadv_unicast packet from " << ethFrame_src.str() << " to " << uni_dest << ", Delay:" << simTime()-received_packet->getCreationTime() << endl;
                    batadv_recv_unicast_packet(dynamic_cast<batadv_unicast_packet *>(received_packet), if_incoming, ethFrame_src, ethFrame_dest);

                }

                else if(dynamic_cast<batadv_icmp_packet *>(received_packet) != NULL){
                    batadv_recv_icmp_packet(dynamic_cast<batadv_icmp_packet *>(received_packet), if_incoming, ethFrame_src, ethFrame_dest);
                }

                else if(dynamic_cast<batadv_coded_packet *>(received_packet) != NULL){
//                    cout << hostModule->getFullName() << ": Received Coded Packet!" << endl;
                    batadv_nc_recv_coded_packet(dynamic_cast<batadv_coded_packet *>(received_packet), if_incoming, ethFrame_src, ethFrame_dest);
                }

                else{
                    cout << hostModule->getFullName() << ": Received something in an Ethernet II Frame, but do not know what! " << received_packet->getFullName() << "!" << endl;
                    delete received_packet;
                }
            } // Batman-Ethernet-Frame Check
            delete msg;
        }// EthernetIIFrame Handler

        else // Unknown packet handler
        {
            cout << hostModule->getFullName() << " received something over *" << msg->getArrivalGate()->getName() << "* but does not know what -> '" << msg->getFullName() << "' " << msg->getFullPath() << endl;
            delete msg;
        }// Unknown packet handler
    } // strcmp(lowerLayerIn)

    else if (strcmp(msg->getArrivalGate()->getName(),"upperLayerIn") == 0){
//        cout << "Received Packet from upper layer " << endl;
        if(dynamic_cast<cPacket *>(msg) != NULL)
            emit(received_packets_from_upper, dynamic_cast<cPacket *>(msg));

        string fullName = msg->getFullName();

        if (dynamic_cast<RegisterTransportProtocolCommand *>(msg)) {
            //cout << hostModule->getFullName() << " received RegisterTransportProtocolCommand" << endl;
            //RegisterTransportProtocolCommand *rtpc = dynamic_cast<RegisterTransportProtocolCommand *>(msg);
            delete msg;
        }

        // UDP packet handler
        else if(dynamic_cast<UDPPacket *>(msg) != NULL) {
            UDPPacket *packet = dynamic_cast<UDPPacket *>(msg);
            INetworkProtocolControlInfo *udpProtocolCtrlInfo = dynamic_cast<INetworkProtocolControlInfo *>(packet->getControlInfo());
            udpProtocolCtrlInfo->setSourceAddress(hostIP);
            L3Address srcAddr  = udpProtocolCtrlInfo->getSourceAddress();
            L3Address destAddr = udpProtocolCtrlInfo->getDestinationAddress();
//            cout << "Transport-Protocol-ID: "<< udpProtocolCtrlInfo->getTransportProtocol() << endl;
            MACAddress destination = check_L3_to_MAC(destAddr); // cast L3Address to MACAddress
            MACAddress source = check_L3_to_MAC(srcAddr);
//            cout << hostModule->getFullName() << " received UDP-Packet with source " << srcAddr.str() << "<->" << source.str() << " and destination: " << destAddr.str() << "<->" << destination.str() << endl;

            numSend_UDP += 1;
            sendUDPUnicast(packet, destination, source);
        }// UDP packet handler

        else if(dynamic_cast<GenericAppMsg *>(msg) != NULL){
            cout << hostModule->getFullName() << ": Received GenericAppMsg" << endl;
            delete msg;
        }

        // Handle TCP packets
        else if(fullName == "SYN" ||
                fullName == "ACK" ||
                fullName == "SYN+ACK" ||
                fullName.find("tcpseg") != string::npos ||
                fullName == "FIN") {
           INetworkProtocolControlInfo *ProtocolCtrlInfo = dynamic_cast<INetworkProtocolControlInfo *>(msg->getControlInfo());
           MACAddress destination = check_L3_to_MAC(ProtocolCtrlInfo->getDestinationAddress());
           MACAddress source = hostMAC;
//           cout << hostModule->getFullName() << " received " << fullName << " to " << ProtocolCtrlInfo->getDestinationAddress() << " <-> " << destination.str() << endl;
           sendTCP(dynamic_cast<cPacket *>(msg), destination, source);
        } // TCP SYN

        else if(fullName.find("data") != string::npos){
            INetworkProtocolControlInfo *ProtocolCtrlInfo = dynamic_cast<INetworkProtocolControlInfo *>(msg->getControlInfo());
            MACAddress destination = check_L3_to_MAC(ProtocolCtrlInfo->getDestinationAddress());
            MACAddress source = hostMAC;
//            cout << hostModule->getFullName() << ": Received Data from UpperLayerIn!" << endl;
            sendTCP(dynamic_cast<cPacket *>(msg), destination, source);
        }

        else if(dynamic_cast<TCPCommand *>(msg) != NULL){
            cout << hostModule->getFullName() << ": Received TCPCommand" << endl;
            delete msg;
        }

        else
        {
            cout << hostModule->getFullName() << " received something over *" << msg->getArrivalGate()->getName() << "* but does not know what -> '" << msg->getName() << "' " << msg->getFullPath() << endl;
            cPacket *packet = dynamic_cast<cPacket *>(msg);
            cout << packet->getFullName() << " " << packet->getFullPath() << endl;
            delete msg;
        }// Unknown packet handler

    } //strcmp 'upperLayerIn'

    // Unknown Gate handler
    else
    {
        cout << hostModule->getFullName() << " received something over *" << msg->getArrivalGate()->getName() << "* but Gate is not known! -> '" << msg->getFullName() << "'" << endl;
        delete msg;
    }// Unknown packet handler


}//handleMessage

std::string Batman5Routing::getForwarder(const std::string &_destination)
{
    batadv_hard_iface *hard_iface = NULL;
    Neigh_node_ptr neigh_node = NULL;
    Neigh_ifinfo_ptr n_ifinfo = NULL;
    simtime_t last_seen_secs;

    std::string forwarder = "";

    if (orig_table.size() != 0){
        for (Originator_Table::iterator it = orig_table.begin(); it != orig_table.end(); it++){

            for (int i=0; i < p_interfaceTable->getNumInterfaces(); i++){
                hard_iface = getHardif_by_ifID(p_interfaceTable->getInterface(i)->getInterfaceId());

                neigh_node = batadv_orig_router_get(it->second, hard_iface);
                if(neigh_node == NULL){
                    continue;
                }
                else if(_destination != neigh_node->addr.str()){
                    continue;
                }
                else{
                    forwarder = neigh_node->addr.str();
                }
            }
        }
    }

    return forwarder;
}

void Batman5Routing::print_orig_table(){
    batadv_hard_iface *hard_iface = NULL;
    Neigh_node_ptr neigh_node = NULL;
    Neigh_ifinfo_ptr n_ifinfo = NULL;
    simtime_t last_seen_secs;

    cout << "**************************************** Originator Table of "<< hostModule->getFullName() << " with " << orig_table.size() << " Entries *****************************************" << endl;
    cout << "|     Originator    |  Last seen  |    Metric    |   Next best Hop   |  outgoing interface  |  Potential Next Hops ...  " << endl;

    if (orig_table.size() != 0){
        for (Originator_Table::iterator it = orig_table.begin(); it != orig_table.end(); it++){

            for (int i=0; i < p_interfaceTable->getNumInterfaces(); i++){
                hard_iface = getHardif_by_ifID(p_interfaceTable->getInterface(i)->getInterfaceId());

                neigh_node = batadv_orig_router_get(it->second, hard_iface);
                if(neigh_node == NULL){
//                    cout << "neigh_node=NULL" << endl;
                    continue;
                }

                n_ifinfo = batadv_neigh_ifinfo_get(neigh_node, hard_iface);
                if(n_ifinfo == NULL){
//                    cout << "n_ifinfo=NULL" << endl;
                    continue;
                }

                last_seen_secs = simTime() - it->second->last_seen;

                cout << "  "
                     << setw(17) << it->second->orig
                     << "    "
                     << setw(7) << round(last_seen_secs.dbl() * 1000.0) / 1000.0
                     << "        "
                     << setw(7) << n_ifinfo->bat_v->metricValue << right
                     << "      "
                     << setw(17) << neigh_node->addr
                     << "          "
                     << setw(5) << neigh_node->if_incoming->name
                     << "            ";

                batadv_v_orig_print_neigh(it->second, hard_iface);

            }//for(hard_iface)
        }//for(orig_node)
    }//if(empty)
    else{
        cout << "**************************************** Originator Table is empty! *****************************************" << endl;
    }//else
}


void Batman5Routing::print_neigh_table(){
    batadv_hard_iface *hard_iface;

    cout << endl << "*********** Neighbor Table of "<< hostModule->getFullName() << " ***********" << endl;
    cout << "|      Neighbor     |  Last seen  |    Metric    |  Interface  |" << endl;

    for (int i=0; i < p_interfaceTable->getNumInterfaces(); i++){
        hard_iface = getHardif_by_ifID(p_interfaceTable->getInterface(i)->getInterfaceId());

        if (hard_iface->neigh_table->size() != 0){
            for (Neighbor_Table::iterator it = hard_iface->neigh_table->begin(); it != hard_iface->neigh_table->end(); it++){
                batadv_v_hardif_neigh_print(it->second->hardif_neigh);
            }
        }
        else{
            cout << "*********** Neighbor Table of "<< hard_iface->name << " is empty! ***********" << endl;
        }
    }
}

MACAddress Batman5Routing::check_L3_to_MAC(L3Address l3_address){
    MACAddress mac_Address;

    if (l3_address.getType() == L3Address::IPv4){
        // cast L3Address-IPv4 to MACAddress
        mac_Address = arp_table->resolveL3Address(l3_address, p_interfaceTable->getInterfaceById(primary_iface->if_id));
    }
    else if(l3_address.getType() == L3Address::MAC){
        // cast L3Address-MAC to MACAddress
        mac_Address = l3_address.toMAC();
    }
    else{
        cout << "Type: " << l3_address.getType() << " not supported! Translation is not possible!" << endl;
    }

//    cout << "Translated " << l3_address.str() << " to " << mac_Address.str() << endl;
    return mac_Address;
}//check_L3_to_MAC


void Batman5Routing::refreshDisplay() const{
/*    char buf[200];

    sprintf(buf, "OGMv2_rcvd: %ld  OGMv2_sent: %ld\n"
            "       ELP_rcvd: %ld       ELP_sent: %ld\n"
            "   ELP_Probes: %ld\n"
            "Forwarded Unicasts: %ld\n"
            "Send UDPs: %ld  Received UDPs: %ld\n",
            num_rcvd_OGMv2/(p_interfaceTable->getNumInterfaces()),
            num_send_OGMv2, num_rcvd_ELP/(p_interfaceTable->getNumInterfaces()),
            num_send_ELP,
            num_send_ELP_probes,
            num_forw_unicast,
            num_send_UDP, num_rcvd_UDP);

    getDisplayString().setTagArg("t", 0, buf);
*/
}//refreshDisplay


void Batman5Routing::finish(){
/*
 * Here it is possible to print the final neighbor and originator table
 */
/*
    cout << endl;
    print_neigh_table();
    cout << endl;
    print_orig_table();
    cout << endl;
*/
    if(networkCoding)
        batadv_nc_mesh_free();

    // cancelAndDelete() of all elp (+neighborTableTimeout) and ogmv2 (+OrigTableTimeout) selfMessages
    for (int i=0; i < p_interfaceTable->getNumInterfaces(); i++){
        batadv_hardif_deactivate_interface(getHardif_by_ifID(p_interfaceTable->getInterface(i)->getInterfaceId()));
    }
}//finisch

} //namespace
