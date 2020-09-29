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

package inet.Batman5.routing;

// BATMAN V Routing Module
// Handles the routing and the datatraffic

simple Batman5Routing
{
    parameters:
        @display("i=block/routing");
        @class(Batman5Routing);
        
        // Statistics
        @signal[SuccessfulReceivedOGMv2](type=cPacket);
        @signal[SuccessfulSendOGMv2](type=cPacket);
        @signal[SuccessfulReceivedELP](type=cPacket);
        @signal[SuccessfulSendELP](type=cPacket);
        @signal[SuccessfulSendELPProbes](type=cPacket);
        @signal[ReceivedUDP](type=cPacket);
        @signal[SendUDP](type=cPacket);
        @signal[ForwardUDPnotPossible](type=cPacket);
        @signal[ForwardedUnicasts](type=cPacket);
        
        @signal[packetsSentUp](type=cPacket);
        @signal[packetsReceivedFromUp](type=cPacket);
        @signal[packetsSentLow](type=cPacket);
        @signal[packetsReceivedFromLow](type=cPacket);
        
        @statistic[rcvdOGMv2](title="Successful Received OGMv2s"; source=SuccessfulReceivedOGMv2; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        @statistic[sendOGMv2](title="Successful Send OGMv2s"; source=SuccessfulSendOGMv2; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        @statistic[rcvdELP](title="Successful Received ELP-Pakets"; source=SuccessfulReceivedELP; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        @statistic[sendELP](title="Successful Send ELP-Pakets"; source=SuccessfulSendELP; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        @statistic[sendELPProbes](title="Successful Send ELP-Probes"; source=SuccessfulSendELPProbes; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        @statistic[rcvdUDP](title="Successful Received UDP-Pakets"; source=ReceivedUDP; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        @statistic[sendUDP](title="Successful Send UDP-Pakets"; source=SendUDP; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        @statistic[noForwardUDP](title="UDP-Forward not possible"; source=ForwardUDPnotPossible; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        @statistic[ForwardedUnicasts](title="Forwarded Unicast-Pakets"; source=ForwardedUnicasts; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        
        @statistic[passedUpPk](title="packets passed to higher layer"; source=packetsSentUp; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        @statistic[sentDownPk](title="packets sent to lower layer"; source=packetsSentLow; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        @statistic[rcvdPkFromHL](title="packets received from higher layer"; source=packetsReceivedFromUp; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        @statistic[rcvdPkFromLL](title="packets received from lower layer"; source=packetsReceivedFromLow; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        
        
        // BATMAN V Variables
        int ogm_ttl = default(50);	// BATADV_TTL=50
        int ogmv2_ttl = default(50); // BATADV_TTL=50
        bool bonding = default(false);	// not implemented -> default(false)
        bool networkCoding = default(false); // if network coding should be activated or not; WARNING: NC is know not fully implemented -> default(false)
        bool fragmentation = default(false); // not implemented -> default(false)
        double hop_penalty = 15; // default Hop-penalty of OGMv2 Packets is 5.88% or 15, 255 is hop_penalty_max
        bool elpProbing = default(true); // if probing-packets for throughput estimation should be send, in BATMAN V ELP probes will always be sent
        int numELPProbes = default(2); // BATADV_ELP_PROBES_PER_NODE = 2
        int sizeELPProbes @unit(B) = default(200B); // BATADV_ELP_MIN_PROBE_SIZE = 200B
        double elp_interval		@unit(s) = default(0.5s);
        double ogmv2_interval 	@unit(s) = default(1s);
        int purge_timeout	@unit(ms) = default(200000ms); // BATADV_PURGE_TIMEOUT=200000
        int purge_timeout_ogmv2 @unit(ms) = default(2 * purge_timeout);
        int tt_purge_timeout @unit(ms) = default(5000ms); // BATADV_TT_WORK_PERIOD = 5000 /*5 seconds*/
        double orig_timeout_interval @unit(s) = default(1s); // BATADV_ORIG_WORK_PERIOD = 1000ms = 1s
        double neigh_timeout_interval @unit(s) = default(1s); // How often the age of the entries should be checked
        volatile double jitter 	@unit(s) = default(uniform (0.00s,0.05s)); // BATADV_JITTER = 0.02s
       	double nc_work_interval @unit(s) = default(0.01s); // from batadv_nc_start_timer: msecs_to_jiffies(10)
       	string metricType = default("throughput_overwrite");	// which metric should be used, throughput_overwrite, throughputMetric, distanceMetric, predictiveMobilityMetric or BATMobile
       	int throughputOverwrite @unit(kbps) = default(10Mbps); // only used with metricType = throughput_overwrite
       	bool writingTraces = default(false); //writing a file of the position data of each host while simulating
       	double predictionWidth = default(4); 	// predictionWidth for predictiveMobilityMetric and BATMobile in seconds
       	bool realValues = default(false);	// if information from other hosts or the global knowledge of the simulation will be used to estimate the distance between hosts; WARNING: not fully realValue not fully implemented
       	int distance_alpha = default(1); // Alpha of the distanceMetric
       	
        // OMNeT++ Variables
        string routingTableModule = default("ipv4routingTable");
        string interfaceTableModule = default("^.interfaceTable");
        string networkLayerType = default("IPv4NetworkLayer");
        string networkProtocolModule = default("^.networkLayer.ip");
        string arpTableModule = default("^.globalARP");
        string ownAddress = default("aa:aa:aa:aa:aa:aa");
		string node = default("");
		string tracesFileName = default("");
		
    gates:
        input upperLayerIn[2];
        output upperLayerOut[2];
        input lowerlayerIn[2];
        output lowerlayerOut[2];
        input arpIn;
        output arpOut;
}
