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

#ifndef INET_BATMAN5_BATMAN5FILES_HARD_INTERFACE_H_
#define INET_BATMAN5_BATMAN5FILES_HARD_INTERFACE_H_

#include "../../Batman5/routing/Batman5Routing.h"

namespace inet {

class hard_interface {
public:
    hard_interface();
    virtual ~hard_interface();


    /**
     * enum batadv_hard_if_state - State of a hard interface
     */
    enum batadv_hard_if_state {
        /*@BATADV_IF_NOT_IN_USE: interface is not used as slave interface of a batman-adv soft interface*/
        BATADV_IF_NOT_IN_USE,
        /* @BATADV_IF_TO_BE_REMOVED: interface will be removed from soft interface*/
        BATADV_IF_TO_BE_REMOVED,
        /** @BATADV_IF_INACTIVE: interface is deactivated */
        BATADV_IF_INACTIVE,
        /** @BATADV_IF_ACTIVE: interface is used */
        BATADV_IF_ACTIVE,
        /** @BATADV_IF_TO_BE_ACTIVATED: interface is getting activated */
        BATADV_IF_TO_BE_ACTIVATED,
        /* @BATADV_IF_I_WANT_YOU: interface is queued up (using sysfs) for being added as slave interface of a batman-adv soft interface */
        BATADV_IF_I_WANT_YOU,
    };

    /**
     * enum batadv_hard_if_bcast - broadcast avoidance options
     */
    enum batadv_hard_if_bcast {
        /** @BATADV_HARDIF_BCAST_OK: Do broadcast on according hard interface */
        BATADV_HARDIF_BCAST_OK = 0,
        /**
         * @BATADV_HARDIF_BCAST_NORECIPIENT: Broadcast not needed, there is no
         *  recipient
         */
        BATADV_HARDIF_BCAST_NORECIPIENT,
        /**
         * @BATADV_HARDIF_BCAST_DUPFWD: There is just the neighbor we got it
         *  from
         */
        BATADV_HARDIF_BCAST_DUPFWD,
        /** @BATADV_HARDIF_BCAST_DUPORIG: There is just the originator */
        BATADV_HARDIF_BCAST_DUPORIG,
    };

    /**
     * enum batadv_hard_if_cleanup - Cleanup modi for soft_iface after slave removal
     */
    enum batadv_hard_if_cleanup {
        /* @BATADV_IF_CLEANUP_KEEP: Don't automatically delete soft-interface*/
        BATADV_IF_CLEANUP_KEEP,
        /** @BATADV_IF_CLEANUP_AUTO: Delete soft-interface after last slave was removed */
        BATADV_IF_CLEANUP_AUTO,
    };

}; /* Class Hard-Interface */

} /* namespace inet */

#endif /* INET_BATMAN5_BATMAN5FILES_HARD_INTERFACE_H_ */
