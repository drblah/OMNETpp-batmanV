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

package inet.examples.adhoc.batmanadv;

import inet.Batman5.node.Batman5Router;
import inet.networklayer.configurator.ipv4.IPv4NetworkConfigurator;
import inet.networklayer.configurator.ipv4.IPv4NodeConfigurator;
import inet.physicallayer.common.packetlevel.RadioMedium;
import inet.physicallayer.contract.packetlevel.IPropagation;
import inet.physicallayer.ieee80211.packetlevel.Ieee80211RadioMedium;
import inet.physicallayer.propagation.ConstantTimePropagation;


//
// TODO Auto-generated network
//
network Batmanadv
{
    @display("bgb=806,475");
    submodules:
        batman5Router: Batman5Router {
        	wlan[*].mgmtType = default("Ieee80211MgmtAdhoc");  // use adhoc management
            @display("p=199,208");
        }
        batman5Router1: Batman5Router {
        	wlan[*].mgmtType = default("Ieee80211MgmtAdhoc");  // use adhoc management
            @display("p=199,261");
        }
        radioMedium: RadioMedium {
            @display("p=603,184");
        }
        constantTimePropagation: ConstantTimePropagation {
            @display("p=586,294");
        }
        configurator: IPv4NetworkConfigurator {
            @display("p=581,378");
        }
}
