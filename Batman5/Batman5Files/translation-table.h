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

#ifndef INET_BATMAN5_BATMAN5FILES_TRANSLATION_TABLE_H_
#define INET_BATMAN5_BATMAN5FILES_TRANSLATION_TABLE_H_
#include "../../Batman5/routing/Batman5Routing.h"

namespace inet {

class TranslationTable {
public:
    TranslationTable();
    virtual ~TranslationTable();

    static int batadv_tt_tvlv_unicast_handler_v1(MACAddress src, MACAddress dst, void *tvlv_value, uint16_t tvlv_value_len, bool dst_is_my_MAC);


    bool batadv_send_tt_response(batadv_tvlv_tt_data *tt_data, MACAddress src, MACAddress dst, bool dst_is_my_MAC);
};

} /* namespace inet */

#endif /* INET_BATMAN5_BATMAN5FILES_TRANSLATION_TABLE_H_ */
