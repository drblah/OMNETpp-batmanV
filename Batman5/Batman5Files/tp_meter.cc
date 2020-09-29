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

#include "../Batman5Files/tp_meter.h"

namespace inet {

tp_meter::tp_meter() {}
tp_meter::~tp_meter() {}


/**
 * batadv_tp_meter_recv() - main TP Meter receiving function
 * @skb: the buffer containing the received packet
 */
void Batman5Routing::batadv_tp_meter_recv(batadv_icmp_tp_packet *icmp){

    switch (icmp->getSubtype()) {
    case BATADV_TP_MSG:
        batadv_tp_recv_msg(icmp);
        break;
    case BATADV_TP_ACK:
        batadv_tp_recv_ack(icmp);
        break;
    default:
        std::cout << "Received unknown TP Metric packet type " << icmp->getSubtype() << std::endl;
    }
}// batadv_tp_meter_recv


/**
 * batadv_tp_recv_msg() - process a single data message
 * @bat_priv: the bat priv with all the soft interface information
 * @skb: the buffer containing the received packet
 *
 * Process a received TP MSG packet
 */
void Batman5Routing::batadv_tp_recv_msg(batadv_icmp_tp_packet *icmp){

}//batadv_tp_recv_msg


/**
 * batadv_tp_recv_ack() - ACK receiving function
 * @bat_priv: the bat priv with all the soft interface information
 * @skb: the buffer containing the received packet
 *
 * Process a received TP ACK packet
 */
void Batman5Routing::batadv_tp_recv_ack(batadv_icmp_tp_packet *icmp){

}//batadv_tp_recv_ack


} /* namespace inet */
