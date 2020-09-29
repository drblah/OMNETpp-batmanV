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

package inet.Batman5.node;
import inet.Batman5.routing.Batman5Routing;

import inet.networklayer.arp.ipv4.GlobalARP;
import inet.networklayer.configurator.ipv4.IPv4NetworkConfigurator;
import inet.networklayer.ipv4.IPv4;
import inet.networklayer.ipv4.IPv4NetworkLayer;
import inet.networklayer.ipv4.IPv4RoutingTable;
import inet.common.lifecycle.NodeStatus;
import inet.mobility.contract.IMobility;

import inet.transportlayer.contract.ITCP;
import inet.transportlayer.contract.IUDP;

import inet.networklayer.contract.IRoutingTable;
import inet.networklayer.common.InterfaceTable;
import inet.networklayer.contract.INetworkLayer;

import inet.linklayer.contract.IWirelessNic;
import inet.linklayer.ieee80211.mac.Ieee80211Mac;
import inet.linklayer.ethernet.switch.MACAddressTable;
import inet.linklayer.contract.IMACAddressTable;
import inet.linklayer.ieee80211.mac.contract.IRateControl;

import inet.physicallayer.contract.packetlevel.IRadio;

import inet.applications.contract.ITCPApp;
import inet.applications.contract.IUDPApp;
import inet.applications.udpapp.UDPBasicApp;
//import inet.applications.udpapp.UDPBroadcastApp;

import inet.visualizer.contract.INetworkRouteVisualizer;


module Batman5Router
{
    parameters:
        @networkNode;
        @labels(node,ethernet-node,wireless-node);
        @display("i=device/wifilaptop;bgl=2");
        //forwarding = true;
        int numRadios = default(1);
        int numUdpApps = default(0);
        int numTcpApps = default(0);
        bool hasUdp = default(numUdpApps > 0);
        bool hasTcp = default(numTcpApps > 0);
        string mobilityType = default(numRadios > 0 ? "StationaryMobility" : "");
        string networkLayerType = default("IPv4NetworkLayer");
        string routingTableType = default("IPv4RoutingTable");
        string visualizerType = default("");
        string arpModuleType = default("Global");
        string udpType = default(firstAvailableOrEmpty("UDP"));
        string tcpType = default(firstAvailableOrEmpty("TCP"));
        *.routingTableModule = default(routingTableType != "" ? absPath(".routingTable") : "");
        *.interfaceTableModule = default(absPath(".interfaceTable"));
//        networkLayer.proxyARP = default(false);

    gates:
        input radioIn[numRadios] @directIn;

    submodules:
        Batman5Routing: Batman5Routing {
            @display("p=205,288");
        }

         // linklayer
        interfaceTable: InterfaceTable {
            parameters:
                @display("p=511,155;is=s");
        }

        routingTable: <routingTableType> like IRoutingTable if routingTableType != "" {
            parameters:
                @display("p=511,242;is=s");
        }

//        networkLayer: <networkLayerType> like INetworkLayer {
//            parameters:
//                @display("p=458,278;q=queue");
//        }

        mobility: <mobilityType> like IMobility if mobilityType != "" {
            parameters:
                @display("p=511,352;is=s");
        }

        MACAddressTable: <"MACAddressTable"> like IMACAddressTable {
            @display("p=511,200;is=s");
        }

        udpApp[numUdpApps]: <> like IUDPApp {
            parameters:
                @display("p=311,29,row,60");
        }

        udp: <udpType> like IUDP if hasUdp {
            parameters:
                @display("p=311,148");
        }

        tcpApp[numTcpApps]: <> like ITCPApp {
            parameters:
                @display("p=94,29,row,60");
        }
        tcp: <tcpType> like ITCP if hasTcp {
            parameters:
                @display("p=94,148");
        }

        wlan[numRadios]: <default("Ieee80211Nic")> like IWirelessNic {
            parameters:
                @display("p=205,390,row,60;q=queue");
        }

        iPv4NetworkConfigurator: IPv4NetworkConfigurator {
            @display("p=511,309");
        }

        globalARP: GlobalARP {
            @display("p=94,286;is=s");
        }

        iPv4NetworkLayer: IPv4NetworkLayer {
            @display("p=511,86");
        }

    connections allowunconnected:

        for i=0..numUdpApps-1 {
            udpApp[i].udpOut --> udp.appIn++;
            udpApp[i].udpIn <-- udp.appOut++;
        }

        udp.ipOut --> Batman5Routing.upperLayerIn[0] if hasUdp;
        udp.ipIn <-- Batman5Routing.upperLayerOut[0] if hasUdp;

//		Batman5Routing.upperLayerOut[0] --> networkLayer.ifIn++;
//        Batman5Routing.upperLayerIn[0] <-- networkLayer.ifOut++;

        for i=0..numTcpApps-1 {
            tcpApp[i].tcpOut --> tcp.appIn++;
            tcpApp[i].tcpIn <-- tcp.appOut++;
        }
        tcp.ipOut --> Batman5Routing.upperLayerIn[1] if hasTcp;
        tcp.ipIn <-- Batman5Routing.upperLayerOut[1] if hasTcp;

        //      ARP-Table
        globalARP.netwOut --> Batman5Routing.arpIn;
        globalARP.netwIn <-- Batman5Routing.arpOut;

        for i=0..sizeof(radioIn)-1 {
            radioIn[i] --> { @display("m=s"); } --> wlan[i].radioIn;
            wlan[i].upperLayerOut --> Batman5Routing.lowerlayerIn[i];
            wlan[i].upperLayerIn <-- Batman5Routing.lowerlayerOut[i];
        }

}
