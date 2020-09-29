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

#include "../Batman5Files/translation-table.h"

namespace inet {

TranslationTable::TranslationTable() {}
TranslationTable::~TranslationTable() {}

int Batman5Routing::batadv_tt_local_init(){
    if (tt_localTable.empty())
        return 0;

    tt_localTable.clear();
    if (!tt_localTable.empty())
        return -ENOMEM;

    return 0;
}

int Batman5Routing::batadv_tt_global_init(){
    if (tt_globalTable.empty())
        return 0;

    tt_globalTable.clear();
    if (!tt_globalTable.empty())
        return -ENOMEM;

    return 0;
}


/**
 * batadv_tt_len() - compute length in bytes of given number of tt changes
 * @changes_num: number of tt changes
 *
 * Return: computed length in bytes.
 */
static int batadv_tt_len(int changes_num)
{
    return changes_num * sizeof(batadv_tvlv_tt_change);
}//batadv_tt_len


/**
 * batadv_tt_entries() - compute the number of entries fitting in tt_len bytes
 * @tt_len: available space
 *
 * Return: the number of entries.
 */
static uint16_t batadv_tt_entries(uint16_t tt_len){
    return tt_len / batadv_tt_len(1);
}//batadv_tt_entries


/**
 * batadv_tt_local_commit_changes_nolock() - commit all pending local tt changes
 *  which have been queued in the time since the last commit
 * @bat_priv: the bat priv with all the soft interface information
 *
 * Caller must hold tt->commit_lock.
 */
void Batman5Routing::batadv_tt_local_commit_changes(){
    batadv_tt_local_commit_changes_nolock();
}//batadv_tt_local_commit_changes


/**
 * batadv_tt_local_commit_changes_nolock() - commit all pending local tt changes
 *  which have been queued in the time since the last commit
 * @bat_priv: the bat priv with all the soft interface information
 *
 * Caller must hold tt->commit_lock.
 */
void Batman5Routing::batadv_tt_local_commit_changes_nolock(){
//    std::cout << "local_changes: " << bat_priv_tt.local_changes << std::endl;
    if (bat_priv_tt.local_changes < 1)
    {
        if (!(bat_priv_tt.ogm_append_cnt - 1 != 0)){
            batadv_tt_tvlv_container_update();
        }
        return;
    }

    batadv_tt_local_set_flags((uint16_t)BATADV_TT_CLIENT_NEW, false, true);

    batadv_tt_local_purge_pending_clients();

    /* Increment the TTVN only once per OGM interval */
    bat_priv_tt.vn += 1;
//    std::cout << "Local changes committed, updating to ttvn " << bat_priv_tt.vn << std::endl;

    /* reset the sending counter */
    bat_priv_tt.ogm_append_cnt = BATADV_TT_OGM_APPEND_MAX;
    batadv_tt_tvlv_container_update();
}//batadv_tt_local_commit_changes_nolock


/**
 * batadv_tt_local_set_flags() - set or unset the specified flags on the local
 *  table and possibly count them in the TT size
 * @bat_priv: the bat priv with all the soft interface information
 * @flags: the flag to switch
 * @enable: whether to set or unset the flag
 * @count: whether to increase the TT size by the number of changed entries
 */
void Batman5Routing::batadv_tt_local_set_flags(uint16_t flags, bool enable, bool count) {

    batadv_tt_local_entry *localEntry = NULL;
    batadv_tt_common_entry tt_common_entry;

    for (TranslationTableLocal::iterator it = tt_localTable.begin(); it != tt_localTable.end(); it++){

        localEntry = it->second;
        tt_common_entry = localEntry->common;

        if (enable) {
            if ((tt_common_entry.flags & flags) == flags)
                continue;
            tt_common_entry.flags |= flags;
        }
        else {
            if (!(tt_common_entry.flags & flags))
                continue;
            tt_common_entry.flags &= ~flags;
        }

        if (!count)
            continue;
    }
}//batadv_tt_local_set_flags


/**
 * batadv_tt_tvlv_generate() - fill the tvlv buff with the tt entries from the
 *  specified tt hash
 * @bat_priv: the bat priv with all the soft interface information
 * @hash: hash table containing the tt entries
 * @tt_len: expected tvlv tt data buffer length in number of bytes
 * @tvlv_buff: pointer to the buffer to fill with the TT data
 * @valid_cb: function to filter tt change entries
 * @cb_data: data passed to the filter function as argument
 */
void Batman5Routing::batadv_tt_tvlv_generate(batadv_tvlv_tt_change *tvlv_buff, bool usingLocalTable, uint16_t tt_len, bool (Batman5Routing::*valid_cb)(void *, Orig_node_ptr), Orig_node_ptr cb_data){
    batadv_tt_common_entry *tt_common_entry;
    batadv_tvlv_tt_change *tt_change;
    uint16_t tt_tot, tt_num_entries = 0;

    bool valid_empty, valid_filled;

    tt_tot = batadv_tt_entries(tt_len);
    tt_change = tvlv_buff;

    // tt_localTable is used
    if(usingLocalTable){
        for(TranslationTableLocal::iterator it = tt_localTable.begin(); it != tt_localTable.end(); it++){
            tt_common_entry = &it->second->common;

            if (tt_tot == tt_num_entries)
               break;

            valid_empty = (this->*valid_cb)(NULL,NULL);
            valid_filled = (this->*valid_cb)(tt_common_entry, cb_data);
            if (valid_empty && !valid_filled)
               continue;

            tt_change->setAddr(tt_common_entry->addr);
            tt_change->setFlags_change(tt_common_entry->flags);
            tt_change->setVid_change(tt_common_entry->vid);

            tt_num_entries++;
            tt_change++;

        }
    }
    //tt_globalTable is used
    else{
        for(TranslationTableGlobal::iterator it = tt_globalTable.begin(); it != tt_globalTable.end(); it++){
            tt_common_entry = &it->second->common;

            if (tt_tot == tt_num_entries)
               break;

            valid_empty = (this->*valid_cb)(NULL,NULL);
            valid_filled = (this->*valid_cb)(tt_common_entry, cb_data);
            if (valid_empty && !valid_filled)
               continue;

           tt_change->setAddr(tt_common_entry->addr);
           tt_change->setFlags_change(tt_common_entry->flags);
           tt_change->setVid_change(tt_common_entry->vid);

           tt_num_entries++;
           tt_change++;

        }
    }



}//batadv_tt_tvlv_generate


/**
 * batadv_tt_local_valid() - verify that given tt entry is a valid one
 * @entry_ptr: to be checked local tt entry
 * @data_ptr: not used but definition required to satisfy the callback prototype
 *
 * Return: true if the entry is a valid, false otherwise.
 */
bool Batman5Routing::batadv_tt_local_valid(void *entry_ptr, Orig_node_ptr data_ptr){
    const batadv_tt_common_entry *tt_common_entry = (batadv_tt_common_entry*)entry_ptr;

    if (tt_common_entry->flags & BATADV_TT_CLIENT_NEW)
        return false;
    return true;
}//batadv_tt_local_valid


/**
 * batadv_send_my_tt_response() - send reply to tt request concerning this
 *  node's translation table
 * @bat_priv: the bat priv with all the soft interface information
 * @tt_data: tt data containing the tt request information
 * @req_src: mac address of tt request sender
 *
 * Return: true if tt request reply was sent, false otherwise.
 */
bool Batman5Routing::batadv_send_my_tt_response(batadv_tvlv_tt_data *tt_data, MACAddress req_src){

    batadv_tvlv_tt_data *tvlv_tt_data = NULL;
    batadv_hard_iface *primary_if = NULL;
    batadv_tvlv_tt_change *tt_change;
    Orig_node_ptr orig_node;
    uint8_t my_ttvn, req_ttvn;
    uint16_t tvlv_len;
    bool full_table;
    int32_t tt_len;

    char ttvn_flag = ((tt_data->getTt_flags() & BATADV_TT_FULL_TABLE) ? 'F' : '.');
    std::cout << "Received TT_REQUEST from " << req_src.str() << "for ttvn: " << tt_data->getTtvn() << " (me) [" << ttvn_flag << "]" << std::endl;

    my_ttvn = (uint8_t)(bat_priv_tt.vn);
    req_ttvn = tt_data->getTtvn();

    orig_node = batadv_orig_find(req_src);
    if (!orig_node)
        return true;

    primary_if = batadv_primary_if_get_selected();
    if (!primary_if)
        return true;

    /* If the full table has been explicitly requested or the gap
     * is too big send the whole local translation table
     */
    if (tt_data->getTt_flags() & BATADV_TT_FULL_TABLE || my_ttvn != req_ttvn || !bat_priv_tt.last_changeset.empty())
        full_table = true;
    else
        full_table = false;

    /* TT fragmentation hasn't been implemented yet, so send as many
     * TT entries fit a single packet as possible only
     */
    if (!full_table) {
        tt_len = bat_priv_tt.last_changeset_len;
        tvlv_len = batadv_tt_prepare_tvlv_local_data(&tvlv_tt_data, &tt_change, &tt_len);
        if (!tt_len || !tvlv_len)
            return true;

        /* Copy the last orig_node's OGM buffer */
        tt_changeList::iterator it = bat_priv_tt.last_changeset.begin();
        tt_change = (*it);;
    }
    else {
        req_ttvn = (uint8_t)(bat_priv_tt.vn);

        /* allocate the tvlv, put the tt_data and all the tt_vlan_data
         * in the initial part
         */
        tt_len = -1;
        tvlv_len = batadv_tt_prepare_tvlv_local_data( &tvlv_tt_data, &tt_change, &tt_len);
        if (!tt_len || !tvlv_len)
            return true;

        /* fill the rest of the tvlv with the real TT entries */
        bool (Batman5Routing::*function_ptr)(void *, Orig_node_ptr) = &Batman5Routing::batadv_tt_local_valid;
        batadv_tt_tvlv_generate(tt_change, true, tt_len, function_ptr, NULL);
    }

    tvlv_tt_data->setTt_flags(BATADV_TT_RESPONSE);
    tvlv_tt_data->setTtvn(req_ttvn);

    if (full_table){
        uint8_t write_flag = tvlv_tt_data->getTt_flags() | BATADV_TT_FULL_TABLE;
        tvlv_tt_data->setTt_flags(write_flag);
    }

    std::cout <<"Sending TT_RESPONSE to " << orig_node->orig << " [" << (full_table ? 'F' : '.') << "]  (ttvn: " << req_ttvn << ")" << std::endl;

    batadv_tvlv_unicast_send(get_hardif_MAC(primary_if->name.c_str()), req_src, BATADV_TVLV_TT, 1, tvlv_tt_data, tvlv_len);


    /* The packet was for this host, so it doesn't need to be re-routed */
    return true;
}//batadv_send_my_tt_response


/**
 * batadv_send_other_tt_response() - send reply to tt request concerning another
 *  node's translation table
 * @bat_priv: the bat priv with all the soft interface information
 * @tt_data: tt data containing the tt request information
 * @req_src: mac address of tt request sender
 * @req_dst: mac address of tt request recipient
 *
 * Return: true if tt request reply was sent, false otherwise.
 */
bool Batman5Routing::batadv_send_other_tt_response(batadv_tvlv_tt_data *tt_data, MACAddress req_src, MACAddress req_dst){
    Orig_node_ptr req_dst_orig_node;
    Orig_node_ptr res_dst_orig_node = NULL;
    batadv_tvlv_tt_change *tt_change;
    batadv_tvlv_tt_data *tvlv_tt_data = NULL;
    batadv_tvlv_tt_vlan *tt_vlan;
    bool ret = false, full_table;
    uint8_t orig_ttvn, req_ttvn;
    uint16_t tvlv_len;
    int32_t tt_len;

    char tt_flags = ((tt_data->getTt_flags() & BATADV_TT_FULL_TABLE) ? 'F' : '.');
    std::cout << "Received TT_REQUEST from " << req_src.str() << " for ttvn: " << tt_data->getTtvn() << "(" << req_dst.str() << ") [" << tt_flags << "]" << std::endl;

    /* Let's get the orig node of the REAL destination */
    req_dst_orig_node = batadv_orig_find(req_dst);
    if (!req_dst_orig_node)
        return ret;

    res_dst_orig_node = batadv_orig_find(req_src);
    if (!res_dst_orig_node)
        return ret;

    orig_ttvn = (uint8_t)req_dst_orig_node->last_ttvn;
    req_ttvn = tt_data->getTtvn();

    tt_vlan = (batadv_tvlv_tt_vlan *)(tt_data + 1);
    /* this node doesn't have the requested data */
    if (orig_ttvn != req_ttvn || !batadv_tt_global_check_crc(req_dst_orig_node, tt_vlan,tt_data->getNum_vlan()))
        return ret;

    /* If the full table has been explicitly requested */
    if (tt_data->getTt_flags() & BATADV_TT_FULL_TABLE || !req_dst_orig_node->tt_buff->empty())
        full_table = true;
    else
        full_table = false;

    /* TT fragmentation hasn't been implemented yet, so send as many
     * TT entries fit a single packet as possible only
     */
    if (!full_table) {
        tt_len = req_dst_orig_node->tt_buff_len;

        tvlv_len = batadv_tt_prepare_tvlv_global_data(req_dst_orig_node, &tvlv_tt_data, &tt_change, &tt_len);
        if (!tt_len)
            return ret;

        /* Copy the last orig_node's OGM buffer */
        std::memcpy(tt_change, req_dst_orig_node->tt_buff, req_dst_orig_node->tt_buff_len);

    }
    else {
        /* allocate the tvlv, put the tt_data and all the tt_vlan_data
         * in the initial part */
        tt_len = -1;
        tvlv_len = batadv_tt_prepare_tvlv_global_data(req_dst_orig_node, &tvlv_tt_data, &tt_change, &tt_len);
        if (!tt_len)
            return ret;

        /* fill the rest of the tvlv with the real TT entries */
        bool (Batman5Routing::*function_ptr)(void *, Orig_node_ptr) = &Batman5Routing::batadv_tt_global_valid;
        batadv_tt_tvlv_generate(tt_change, false, tt_len, function_ptr, req_dst_orig_node);
    }

    /* Don't send the response, if larger than fragmented packet. */
    tt_len = sizeof(batadv_unicast_tvlv_packet) + tvlv_len;
    if (tt_len > ETH_DATA_LEN) {
        std::cout << "Ignoring TT_REQUEST from " << res_dst_orig_node->orig.str() << ". Response size exceeds max packet size." << std::endl;
        return ret;
    }



    tvlv_tt_data->setTt_flags(BATADV_TT_RESPONSE);
    tvlv_tt_data->setTtvn(req_ttvn);

    if (full_table){
        uint8_t full_table_flag = tvlv_tt_data->getTt_flags() | BATADV_TT_FULL_TABLE;
        tvlv_tt_data->setTt_flags(full_table_flag);
    }

    std::cout << "Sending TT_RESPONSE " << res_dst_orig_node->orig.str() << " for " << req_dst_orig_node->orig.str() << "[" << (full_table ? 'F' : '.') << "] (ttvn: " << req_ttvn << ")" << std::endl;

    batadv_tvlv_unicast_send(req_dst_orig_node->orig, req_src, BATADV_TVLV_TT, 1, tvlv_tt_data, tvlv_len);

    ret = true;
    return ret;
}//batadv_send_other_tt_response


/**
 * batadv_tt_global_check_crc() - check if all the CRCs are correct
 * @orig_node: originator for which the CRCs have to be checked
 * @tt_vlan: pointer to the first tvlv VLAN entry
 * @num_vlan: number of tvlv VLAN entries
 *
 * Return: true if all the received CRCs match the locally stored ones, false
 * otherwise
 */
bool Batman5Routing::batadv_tt_global_check_crc(Orig_node_ptr orig_node, batadv_tvlv_tt_vlan *tt_vlan, uint16_t num_vlan){
//    batadv_orig_node_vlan *vlan;
//    batadv_tvlv_tt_vlan *tt_vlan_tmp;
    int i, orig_num_vlan;

    /* check if each received CRC matches the locally stored one */
    for (i = 0; i < num_vlan; i++) {
//        tt_vlan_tmp = tt_vlan + i;

        /* if orig_node is a backbone node for this VLAN, don't check
         * the CRC as we ignore all the global entries over it */
//        if (batadv_bla_is_backbone_gw_orig(orig_node->bat_priv, orig_node->orig, tt_vlan_tmp->vid_vlan))
//            continue;

//        vlan = batadv_orig_node_vlan_get(orig_node, ntohs(tt_vlan_tmp->vid));
//        if (!vlan)
//            return false;
//
//        crc = vlan->tt.crc;
//
//        if (crc != tt_vlan_tmp->crc))
//            return false;
    }

    /* check if any excess VLANs exist locally for the originator
     * which are not mentioned in the TVLV from the originator.
     */
    orig_num_vlan = 1;

    if (orig_num_vlan > num_vlan)
        return false;

    return true;
}//batadv_tt_global_check_crc


/**
 * batadv_tt_prepare_tvlv_global_data() - prepare the TVLV TT header to send
 *  within a TT Response directed to another node
 * @orig_node: originator for which the TT data has to be prepared
 * @tt_data: uninitialised pointer to the address of the TVLV buffer
 * @tt_change: uninitialised pointer to the address of the area where the TT
 *  changed can be stored
 * @tt_len: pointer to the length to reserve to the tt_change. if -1 this
 *  function reserves the amount of space needed to send the entire global TT
 *  table. In case of success the value is updated with the real amount of
 *  reserved bytes
 * Allocate the needed amount of memory for the entire TT TVLV and write its
 * header made up by one tvlv_tt_data object and a series of tvlv_tt_vlan_data
 * objects, one per active VLAN served by the originator node.
 *
 * Return: the size of the allocated buffer or 0 in case of failure.
 */
uint16_t Batman5Routing::batadv_tt_prepare_tvlv_global_data(Orig_node_ptr orig_node, batadv_tvlv_tt_data **tt_data, batadv_tvlv_tt_change **tt_change, int32_t *tt_len){
    uint16_t num_vlan = 1;
    uint16_t num_entries = 0;
    uint16_t change_offset;
    uint16_t tvlv_len;
    batadv_tvlv_tt_vlan *tt_vlan;
//    batadv_orig_node_vlan *vlan;
    uint8_t *tt_change_ptr;

    change_offset = sizeof(**tt_data);
    change_offset += num_vlan * sizeof(*tt_vlan);

    /* if tt_len is negative, allocate the space needed by the full table */
    if (*tt_len < 0)
        *tt_len = batadv_tt_len(num_entries);

    tvlv_len = *tt_len;
    tvlv_len += change_offset;

//    *tt_data = kmalloc(tvlv_len, GFP_ATOMIC);
    if (!*tt_data) {
        *tt_len = 0;
        return tvlv_len;
    }

    (*tt_data)->setTt_flags(BATADV_NO_FLAGS);
    (*tt_data)->setTtvn(orig_node->last_ttvn);
    (*tt_data)->setNum_vlan(num_vlan);

    tt_vlan = (batadv_tvlv_tt_vlan *)(*tt_data + 1);
    tt_vlan->setVid_vlan(1);
    tt_vlan->setCrc32(1);

    tt_change_ptr = (uint8_t *)*tt_data + change_offset;
    *tt_change = (batadv_tvlv_tt_change *)tt_change_ptr;

    return tvlv_len;
}//batadv_tt_prepare_tvlv_global_data


bool Batman5Routing::batadv_tt_global_valid(void *entry_ptr, Orig_node_ptr data_ptr){
    batadv_tt_common_entry *tt_common_entry = (batadv_tt_common_entry*)entry_ptr;
    batadv_tt_global_entry *tt_global_entry;
    Orig_node_ptr orig_node = data_ptr;

    if (tt_common_entry->flags & BATADV_TT_CLIENT_ROAM || tt_common_entry->flags & BATADV_TT_CLIENT_TEMP)
        return false;

    tt_global_entry = new batadv_tt_global_entry();
    tt_global_entry->common = *tt_common_entry;

    return batadv_tt_global_entry_has_orig(tt_global_entry, orig_node);
}//batadv_tt_global_valid


/**
 * batadv_tt_global_entry_has_orig() - check if a TT global entry is also
 *  handled by a given originator
 * @entry: the TT global entry to check
 * @orig_node: the originator to search in the list
 *
 * find out if an orig_node is already in the list of a tt_global_entry.
 *
 * Return: true if found, false otherwise
 */
bool Batman5Routing::batadv_tt_global_entry_has_orig(batadv_tt_global_entry *entry, Orig_node_ptr orig_node){
    batadv_tt_orig_list_entry *orig_entry;
    bool found = false;

    orig_entry = batadv_tt_global_orig_entry_find(entry, orig_node);
    if (orig_entry) {
        found = true;
    }

    return found;
}//batadv_tt_global_entry_has_orig


/**
 * batadv_tt_global_orig_entry_find() - find a TT orig_list_entry
 * @entry: the TT global entry where the orig_list_entry has to be
 *  extracted from
 * @orig_node: the originator for which the orig_list_entry has to be found
 *
 * retrieve the orig_tt_list_entry belonging to orig_node from the
 * batadv_tt_global_entry list
 *
 * Return: it with an increased refcounter, NULL if not found
 */
batadv_tt_orig_list_entry *Batman5Routing::batadv_tt_global_orig_entry_find(batadv_tt_global_entry *entry, Orig_node_ptr orig_node){
    batadv_tt_orig_list_entry *tmp_orig_entry, *orig_entry = NULL;

    for(Originator_tt_list::iterator it = entry->orig_list.begin(); it != entry->orig_list.end(); it++){
        tmp_orig_entry = it->second;
        if (tmp_orig_entry->orig_node != orig_node)
            continue;

        orig_entry = tmp_orig_entry;
        break;
    }

    return orig_entry;
}//batadv_tt_global_orig_entry_find


/**
 * batadv_send_tt_response() - send reply to tt request
 * @bat_priv: the bat priv with all the soft interface information
 * @tt_data: tt data containing the tt request information
 * @req_src: mac address of tt request sender
 * @req_dst: mac address of tt request recipient
 *
 * Return: true if tt request reply was sent, false otherwise.
 */
bool Batman5Routing::batadv_send_tt_response(batadv_tvlv_tt_data *tt_data, MACAddress req_src, MACAddress req_dst){
    if (batadv_is_my_mac(req_dst))
        return batadv_send_my_tt_response(tt_data, req_src);

    return batadv_send_other_tt_response(tt_data, req_src, req_dst);
}//batadv_send_tt_response


/**
 * batadv_tt_tvlv_container_update() - update the translation table tvlv
 *  container after local tt changes have been committett_datad
 * @bat_priv: the bat priv with all the soft interface information
 */
void Batman5Routing::batadv_tt_tvlv_container_update(){

    batadv_tvlv_tt_data *tt_data;
    batadv_tvlv_tt_change *tt_change = new batadv_tvlv_tt_change();
    int tt_diff_entries_num = 0;
    int tt_diff_entries_count = 0;
    int tt_diff_len;//, tt_change_len = 0;
    uint16_t tvlv_len;

    tt_diff_entries_num = bat_priv_tt.local_changes;
    tt_diff_len = tt_diff_entries_num * tt_change->getByteLength();

    /* if we have too many changes for one packet don't send any
     * and wait for the tt table request which will be fragmented */
    if (tt_diff_len > primary_iface->mtu)
        tt_diff_len = 0;

    tvlv_len = batadv_tt_prepare_tvlv_local_data(&tt_data, &tt_change, &tt_diff_len);

    if (tvlv_len == 0)
        return;

    bat_priv_tt.vlan_expanded = true;

    tt_data->setTt_flags(BATADV_TT_OGM_DIFF);

    if (tt_diff_len == 0)
        return;

    bat_priv_tt.local_changes = 0;

    batadv_tvlv_tt_change* tmp_tt_change;
    tt_changeList last_change_set;
    for (unsigned int i = 0; i < bat_priv_tt.changes_list.size(); i++) {
        tmp_tt_change = bat_priv_tt.changes_list[i];
        if (tt_diff_entries_count < tt_diff_entries_num) {
            tmp_tt_change->setName("tt_change_" + tt_diff_entries_count);
            last_change_set.push_back(tmp_tt_change);
            tt_diff_entries_count++;
        }
    }//for

    /* Keep the buffer for possible tt_request */
    bat_priv_tt.last_changeset_len = 0;
    bat_priv_tt.last_changeset.clear();
//    tt_change_len = tt_diff_entries_count * tt_change->getByteLength();

    /* check whether this new OGM has no changes due to size problems */
    if (tt_diff_entries_count > 0) {
        /* if kmalloc() fails we will reply with the full table
         * instead of providing the diff */
        bat_priv_tt.last_changeset = last_change_set;
        bat_priv_tt.last_changeset_len = last_change_set.size();

        std::cout << "Container Register" << std::endl;
        batadv_tvlv_container_register(BATADV_TVLV_TT, 1, tt_data, tvlv_len);
    }

    delete tt_change;
    delete tt_data;
}//batadv_tt_tvlv_container_update


/**
 * batadv_tt_prepare_tvlv_local_data() - allocate and prepare the TT TVLV for
 *  this node
 * @bat_priv: the bat priv with all the soft interface information
 * @tt_data: uninitialised pointer to the address of the TVLV buffer
 * @tt_change: uninitialised pointer to the address of the area where the TT
 *  changes can be stored
 * @tt_len: pointer to the length to reserve to the tt_change. if -1 this
 *  function reserves the amount of space needed to send the entire local TT
 *  table. In case of success the value is updated with the real amount of
 *  reserved bytes
 *
 * Allocate the needed amount of memory for the entire TT TVLV and write its
 * header made up by one tvlv_tt_data object and a series of tvlv_tt_vlan_data
 * objects, one per active VLAN.
 *
 * Return: the size of the allocated buffer or 0 in case of failure.
 */
uint16_t Batman5Routing::batadv_tt_prepare_tvlv_local_data(batadv_tvlv_tt_data **tt_data, batadv_tvlv_tt_change **tt_change, int32_t *tt_len){
    batadv_tvlv_tt_vlan *tt_vlan = new batadv_tvlv_tt_vlan();;
    uint16_t num_vlan = 1;
    uint16_t num_entries = 0;
    uint16_t tvlv_len;
    int change_offset;


    (tt_vlan)->setTt_flags(BATADV_NO_FLAGS);
    (tt_vlan)->setTtvn(1);
    (tt_vlan)->setNum_vlan(num_vlan);

    tt_vlan->setVid_vlan(1);
    tt_vlan->setCrc32(1);

    (*tt_data) = tt_vlan;


    change_offset = sizeof(**tt_data);
    change_offset += num_vlan * sizeof(*tt_vlan);

    /* if tt_len is negative, allocate the space needed by the full table */
    if (*tt_len < 0)
        *tt_len = batadv_tt_len(num_entries);

    tvlv_len = *tt_len;
    tvlv_len += change_offset;

//    *tt_data = kmalloc(tvlv_len, GFP_ATOMIC);
    if (!*tt_data) {
        tvlv_len = 0;
        return tvlv_len;
    }

    return tvlv_len;
}//batadv_tt_prepare_tvlv_local_data


/**
 * batadv_handle_tt_response() - process incoming tt reply
 * @bat_priv: the bat priv with all the soft interface information
 * @tt_data: tt data containing the tt request information
 * @resp_src: mac address of tt reply sender
 * @num_entries: number of tt change entries appended to the tt data
 */
void Batman5Routing::batadv_handle_tt_response(batadv_tvlv_tt_data *tt_data, MACAddress resp_src, uint16_t num_entries){
    TT_req_node_ptr node;
    Orig_node_ptr orig_node = NULL;
    batadv_tvlv_tt_change *tt_change;
    uint8_t *tvlv_ptr = (uint8_t *)tt_data;
    uint16_t change_offset;

    char flag_string = (tt_data->getTt_flags() & BATADV_TT_FULL_TABLE) ? 'F' : '.';
    std::cout << "Received TT_RESPONSE from " << resp_src.str() << " for ttvn " << tt_data->getTtvn() << " t_size: " << num_entries << "[" << flag_string << "]" << std::endl;

    orig_node = batadv_orig_find(resp_src);
    if (!orig_node)
        return;

    change_offset = sizeof(batadv_tvlv_tt_vlan);
    change_offset *= tt_data->getNum_vlan();
    change_offset += sizeof(*tt_data);
    tvlv_ptr += change_offset;

    tt_change = (batadv_tvlv_tt_change *)tvlv_ptr;
    if (tt_data->getTt_flags() & BATADV_TT_FULL_TABLE) {
        batadv_tt_fill_gtable(tt_change, tt_data->getTtvn(), resp_src, num_entries);
    } else {
        batadv_tt_update_changes(orig_node, num_entries, tt_data->getTtvn(), tt_change);
    }

    /* Recalculate the CRC for this orig_node and store it */
    batadv_tt_global_update_crc(orig_node);

    /* Delete the tt_req_node from pending tt_requests list */

}//batadv_handle_tt_response


/**
 * batadv_tt_global_update_crc() - update all the global CRCs for this orig_node
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: the orig_node for which the CRCs have to be updated
 */
void Batman5Routing::batadv_tt_global_update_crc(Orig_node_ptr orig_node){
//    uint32_t crc;

    /* recompute the global CRC for each VLAN */
        /* if orig_node is a backbone node for this VLAN, don't compute
         * the CRC as we ignore all the global entries over it */
//        if (batadv_bla_is_backbone_gw_orig(bat_priv, orig_node->orig, vlan->vid))
//            continue;

//        crc = batadv_tt_global_crc(orig_node, 1);

//    }
}//batadv_tt_global_update_crc


void Batman5Routing::batadv_tt_fill_gtable(batadv_tvlv_tt_change *tt_change, uint8_t ttvn, MACAddress resp_src, uint16_t num_entries){
    Orig_node_ptr orig_node;

    orig_node = batadv_orig_find(resp_src);
    if (!orig_node)
        return;

    /* Purge the old table first.. */
    batadv_tt_global_del_orig(orig_node, -1, "Received full table");

    _batadv_tt_update_changes(orig_node, tt_change, num_entries, ttvn);

    orig_node->tt_buff->clear();
    orig_node->tt_buff_len = 0;
    orig_node->tt_buff = NULL;

    orig_node->last_ttvn = ttvn;
}//batadv_tt_fill_gtable


/**
 * batadv_tt_global_del_orig() - remove all the TT global entries belonging to
 *  the given originator matching the provided vid
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: the originator owning the entries to remove
 * @match_vid: the VLAN identifier to match. If negative all the entries will be
 *  removed
 * @message: debug message to print as "reason"
 */
void Batman5Routing::batadv_tt_global_del_orig(Orig_node_ptr orig_node, int32_t match_vid, const char *message){
    batadv_tt_global_entry *tt_global;
    batadv_tt_common_entry *tt_common_entry;
    unsigned short vid;

    for(TranslationTableGlobal::iterator it = tt_globalTable.begin(); it != tt_globalTable.end(); it++){
        tt_global = it->second;
        tt_common_entry = &tt_global->common;

        /* remove only matching entries */
        if (match_vid >= 0 && tt_common_entry->vid != match_vid)
            continue;

        batadv_tt_global_del_orig_node(tt_global, orig_node, message);

        if (tt_global->orig_list.empty()) {
            vid = tt_global->common.vid;
            std::cout << "Deleting global tt entry " << tt_global->common.addr.str() << "(vid: " << vid << "): " << message << std::endl;
        }//if
    }//for

    orig_node->capa_initialized |= BATADV_ORIG_CAPA_HAS_TT;
}


/**
 * batadv_tt_global_del_orig_node() - remove orig_node from a global tt entry
 * @bat_priv: the bat priv with all the soft interface information
 * @tt_global_entry: the global entry to remove the orig_node from
 * @orig_node: the originator announcing the client
 * @message: message to append to the log on deletion
 *
 * Remove the given orig_node and its according orig_entry from the given
 * global tt entry.
 */
void Batman5Routing::batadv_tt_global_del_orig_node(batadv_tt_global_entry *tt_global_entry, Orig_node_ptr orig_node, const char *message){
    batadv_tt_orig_list_entry *orig_entry;
    unsigned short vid;

    for (Originator_tt_list::iterator it = tt_global_entry->orig_list.begin(); it != tt_global_entry->orig_list.end(); it++){
        orig_entry = it->second;
        if (orig_entry->orig_node == orig_node) {
            vid = tt_global_entry->common.vid;
            std::cout << "Deleting " << orig_node->orig.str() << " from global tt entry " << tt_global_entry->common.addr.str() << "(vid: " << vid << "): " << message << std::endl;
            _batadv_tt_global_del_orig_entry(tt_global_entry, orig_entry);
        }//if
    }//for
}//batadv_tt_global_del_orig_node


/**
 * _batadv_tt_global_del_orig_entry() - remove and free an orig_entry
 * @tt_global_entry: the global entry to remove the orig_entry from
 * @orig_entry: the orig entry to remove and free
 *
 * Remove an orig_entry from its list in the given tt_global_entry and
 * free this orig_entry afterwards.
 *
 * Caller must hold tt_global_entry->list_lock and ensure orig_entry->list is
 * part of a list.
 */
void Batman5Routing::_batadv_tt_global_del_orig_entry(batadv_tt_global_entry *tt_global_entry, batadv_tt_orig_list_entry *orig_entry){
    Originator_tt_list::iterator it = tt_global_entry->orig_list.find(orig_entry->orig_node);
    tt_global_entry->orig_list.erase(it);
}


void Batman5Routing::batadv_tt_update_changes(Orig_node_ptr orig_node, uint16_t tt_num_changes, uint8_t ttvn, batadv_tvlv_tt_change *tt_change){
    _batadv_tt_update_changes(orig_node, tt_change, tt_num_changes, ttvn);

    batadv_tt_save_orig_buffer(orig_node, tt_change, batadv_tt_len(tt_num_changes));
    orig_node->last_ttvn = ttvn;
}//batadv_tt_update_changes


void Batman5Routing::batadv_tt_save_orig_buffer(Orig_node_ptr orig_node, batadv_tvlv_tt_change *tt_buff, uint16_t tt_buff_len){
    /* Replace the old buffer only if I received something in the
     * last OGM (the OGM could carry no changes) */

    if (tt_buff_len > 0) {
        orig_node->tt_buff->clear();
        orig_node->tt_buff_len = 0;

        orig_node->tt_buff->push_back(tt_buff);
        orig_node->tt_buff_len = tt_buff_len;

    }
}//batadv_tt_save_orig_buffer


void Batman5Routing::_batadv_tt_update_changes(Orig_node_ptr orig_node, batadv_tvlv_tt_change *tt_change, uint16_t tt_num_changes, uint8_t ttvn){
    int i;
    int roams;

    for (i = 0; i < tt_num_changes; i++) {
        if ((tt_change + i)->getTt_flags() & BATADV_TT_CLIENT_DEL) {
            roams = (tt_change + i)->getTt_flags() & BATADV_TT_CLIENT_ROAM;
            batadv_tt_global_del(orig_node, (tt_change + i)->getAddr(), (tt_change + i)->getVid_change(), "tt removed by changes", roams);
        }//if
        else{
            if (!batadv_tt_global_add(orig_node, (tt_change + i)->getAddr(), (tt_change + i)->getVid_change(), (tt_change + i)->getTt_flags(), ttvn))
                /* In case of problem while storing a
                 * global_entry, we stop the updating
                 * procedure without committing the
                 * ttvn change. This will avoid to send
                 * corrupted data on tt_request */
                return;
        }//else
    }//for
    orig_node->capa_initialized |= BATADV_ORIG_CAPA_HAS_TT;
}//_batadv_tt_update_changes


/**
 * batadv_tt_global_hash_find() - search the global table for a given client
 * @bat_priv: the bat priv with all the soft interface information
 * @addr: the mac address of the client to look for
 * @vid: VLAN identifier
 *
 * Return: a pointer to the corresponding tt_global_entry struct if the client
 * is found, NULL otherwise.
 */
batadv_tt_global_entry *Batman5Routing::batadv_tt_global_find(MACAddress addr, unsigned short vid){
    batadv_tt_global_entry *tt_global_entry = NULL;

    TranslationTableGlobal::iterator it = tt_globalTable.find(addr);
    if (it != tt_globalTable.end())
        tt_global_entry = it->second;

    return tt_global_entry;
}//batadv_tt_global_hash_find


/**
 * batadv_tt_local_hash_find() - search the local table for a given client
 * @bat_priv: the bat priv with all the soft interface information
 * @addr: the mac address of the client to look for
 * @vid: VLAN identifier
 *
 * Return: a pointer to the corresponding tt_local_entry struct if the client is
 * found, NULL otherwise.
 */
batadv_tt_local_entry *Batman5Routing::batadv_tt_local_find(MACAddress addr, unsigned short vid){
    batadv_tt_local_entry *tt_local_entry = NULL;

    TranslationTableLocal::iterator it = tt_localTable.find(addr);
    if(it != tt_localTable.end())
        tt_local_entry = it->second;
    return tt_local_entry;
}//batadv_tt_local_hash_find


/**
 * batadv_tt_global_add() - add a new TT global entry or update an existing one
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: the originator announcing the client
 * @tt_addr: the mac address of the non-mesh client
 * @vid: VLAN identifier
 * @flags: TT flags that have to be set for this non-mesh client
 * @ttvn: the tt version number ever announcing this non-mesh client
 *
 * Add a new TT global entry for the given originator. If the entry already
 * exists add a new reference to the given originator (a global entry can have
 * references to multiple originators) and adjust the flags attribute to reflect
 * the function argument.
 * If a TT local entry exists for this non-mesh client remove it.
 *
 * The caller must hold orig_node refcount.
 *
 * Return: true if the new entry has been added, false otherwise
 */
bool Batman5Routing::batadv_tt_global_add(Orig_node_ptr orig_node, MACAddress tt_addr, unsigned short vid, uint16_t flags, uint8_t ttvn){
    batadv_tt_global_entry *tt_global_entry;
    batadv_tt_local_entry *tt_local_entry;
    bool ret = false;
    batadv_tt_common_entry *common;
    uint16_t local_flags;

    /* ignore global entries from backbone nodes */
//    if (batadv_bla_is_backbone_gw_orig(bat_priv, orig_node->orig, vid))
//        return true;

    tt_global_entry = batadv_tt_global_find(tt_addr, vid);
    tt_local_entry = batadv_tt_local_find(tt_addr, vid);

    /* if the node already has a local client for this entry, it has to wait
     * for a roaming advertisement instead of manually messing up the global table */
    if ((flags & BATADV_TT_CLIENT_TEMP) && tt_local_entry && !(tt_local_entry->common.flags & BATADV_TT_CLIENT_NEW))
        return ret;

    if (!tt_global_entry) {
        common = &tt_global_entry->common;
        common->addr = tt_addr;
        common->vid = vid;

        common->flags = flags;
        tt_global_entry->roam_at = 0;
        /* node must store current time in case of roaming. This is
         * needed to purge this entry out on timeout (if nobody claims it) */
        if (flags & BATADV_TT_CLIENT_ROAM)
            tt_global_entry->roam_at = simTime();
        common->added_at = simTime();

        tt_global_entry->orig_list_count = 0;

//        hash_added = batadv_hash_add(bat_priv->tt.global_hash, batadv_compare_tt, batadv_choose_tt, common, &common->hash_entry);
        TranslationTableGlobal::iterator it = tt_globalTable.find(tt_global_entry->common.addr);
        it->second->common = *common;
        std::swap(tt_globalTable[common->addr], it->second);
        tt_globalTable.erase(it);

    }
    else {
        common = &tt_global_entry->common;
        /* If there is already a global entry, we can use this one for
         * our processing.
         * But if we are trying to add a temporary client then here are
         * two options at this point:
         * 1) the global client is not a temporary client: the global
         *    client has to be left as it is, temporary information
         *    should never override any already known client state
         * 2) the global client is a temporary client: purge the
         *    originator list and add the new one orig_entry
         */
        if (flags & BATADV_TT_CLIENT_TEMP) {
            if (!(common->flags & BATADV_TT_CLIENT_TEMP))
                return ret;
            if (batadv_tt_global_entry_has_orig(tt_global_entry, orig_node))
                goto out_remove;

            batadv_tt_global_del_orig_list(tt_global_entry);
            goto add_orig_entry;
        }

        /* if the client was temporary added before receiving the first
         * OGM announcing it, we have to clear the TEMP flag. Also,
         * remove the previous temporary orig node and re-add it
         * if required. If the orig entry changed, the new one which
         * is a non-temporary entry is preferred.
         */
        if (common->flags & BATADV_TT_CLIENT_TEMP) {
            batadv_tt_global_del_orig_list(tt_global_entry);
            common->flags &= ~BATADV_TT_CLIENT_TEMP;
        }

        /* the change can carry possible "attribute" flags like the
         * TT_CLIENT_TEMP, therefore they have to be copied in the
         * client entry
         */
        common->flags |= flags & (~BATADV_TT_SYNC_MASK);

        /* If there is the BATADV_TT_CLIENT_ROAM flag set, there is only
         * one originator left in the list and we previously received a
         * delete + roaming change for this originator.
         *
         * We should first delete the old originator before adding the
         * new one.
         */
        if (common->flags & BATADV_TT_CLIENT_ROAM) {
            batadv_tt_global_del_orig_list(tt_global_entry);
            common->flags &= ~BATADV_TT_CLIENT_ROAM;
            tt_global_entry->roam_at = 0;
        }
    }
add_orig_entry:
    /* add the new orig_entry (if needed) or update it */
    batadv_tt_global_orig_entry_add(tt_global_entry, orig_node, ttvn, flags & BATADV_TT_SYNC_MASK);

    std::cout << "Creating new global tt entry: " << common->addr.str() << " (vid: " << vid << ", via " << orig_node->orig.str() << ")" << std::endl;
    ret = true;

out_remove:
    /* Do not remove multicast addresses from the local hash on
     * global additions
     */
    if (tt_addr.isMulticast())
        return ret;

    /* remove address from local hash if present */
    local_flags = batadv_tt_local_remove(tt_addr, vid, "global tt received", flags & BATADV_TT_CLIENT_ROAM);
    tt_global_entry->common.flags |= local_flags & BATADV_TT_CLIENT_WIFI;

    if (!(flags & BATADV_TT_CLIENT_ROAM))
        /* this is a normal global add. Therefore the client is not in a
         * roaming state anymore.
         */
        tt_global_entry->common.flags &= ~BATADV_TT_CLIENT_ROAM;

    return ret;
}//batadv_tt_global_add


/**
 * batadv_tt_global_orig_entry_add() - add or update a TT orig entry
 * @tt_global: the TT global entry to add an orig entry in
 * @orig_node: the originator to add an orig entry for
 * @ttvn: translation table version number of this changeset
 * @flags: TT sync flags
 */
void Batman5Routing::batadv_tt_global_orig_entry_add(batadv_tt_global_entry *tt_global, Orig_node_ptr orig_node, int ttvn, uint8_t flags){
    batadv_tt_orig_list_entry *orig_entry;

    orig_entry = batadv_tt_global_orig_entry_find(tt_global, orig_node);
    if (orig_entry) {
        /* refresh the ttvn: the current value could be a bogus one that
         * was added during a "temporary client detection" */
        orig_entry->ttvn = ttvn;
        orig_entry->flags = flags;
        batadv_tt_global_sync_flags(tt_global);
        return;
    }


    orig_entry->orig_node = orig_node;
    orig_entry->ttvn = ttvn;
    orig_entry->flags = flags;
    tt_global->orig_list.insert(std::pair<Orig_node_ptr, batadv_tt_orig_list_entry*>(orig_node, orig_entry));

    tt_global->orig_list_count += 1;
}//batadv_tt_global_orig_entry_add


/**
 * batadv_tt_global_sync_flags() - update TT sync flags
 * @tt_global: the TT global entry to update sync flags in
 *
 * Updates the sync flag bits in the tt_global flag attribute with a logical
 * OR of all sync flags from any of its TT orig entries.
 */
void Batman5Routing::batadv_tt_global_sync_flags(batadv_tt_global_entry *tt_global){
    batadv_tt_orig_list_entry *orig_entry;
    uint16_t flags = BATADV_NO_FLAGS;

    for(Originator_tt_list::iterator it = tt_global->orig_list.begin(); it != tt_global->orig_list.end(); it++){
        orig_entry = it->second;
        flags |= orig_entry->flags;
    }

    flags |= tt_global->common.flags & (~BATADV_TT_SYNC_MASK);
    tt_global->common.flags = flags;
}//batadv_tt_global_sync_flags


/**
 * batadv_tt_local_remove() - logically remove an entry from the local table
 * @bat_priv: the bat priv with all the soft interface information
 * @addr: the MAC address of the client to remove
 * @vid: VLAN identifier
 * @message: message to append to the log on deletion
 * @roaming: true if the deletion is due to a roaming event
 *
 * Return: the flags assigned to the local entry before being deleted
 */
uint16_t Batman5Routing::batadv_tt_local_remove(MACAddress addr, unsigned short vid, const char *message, bool roaming){
    batadv_tt_local_entry *tt_local_entry;
    uint16_t flags, curr_flags = BATADV_NO_FLAGS;

    tt_local_entry = batadv_tt_local_find(addr, vid);
    if (!tt_local_entry)
        return curr_flags;

    curr_flags = tt_local_entry->common.flags;

    flags = BATADV_TT_CLIENT_DEL;
    /* if this global entry addition is due to a roaming, the node has to
     * mark the local entry as "roamed" in order to correctly reroute
     * packets later */
    if (roaming) {
        flags |= BATADV_TT_CLIENT_ROAM;
        /* mark the local client as ROAMed */
        tt_local_entry->common.flags |= BATADV_TT_CLIENT_ROAM;
    }

    if (!(tt_local_entry->common.flags & BATADV_TT_CLIENT_NEW)) {
        batadv_tt_local_set_pending(tt_local_entry, flags, message);
        return curr_flags;
    }
    /* if this client has been added right now, it is possible to
     * immediately purge it */
    batadv_tt_local_event(tt_local_entry, BATADV_TT_CLIENT_DEL);

    TranslationTableLocal::iterator it = tt_localTable.find(addr);
    tt_localTable.erase(it);

    return curr_flags;
}//batadv_tt_local_remove


/* deletes the orig list of a tt_global_entry */
void Batman5Routing::batadv_tt_global_del_orig_list(batadv_tt_global_entry *tt_global_entry){
    tt_global_entry->orig_list.clear();
}//batadv_tt_global_del_orig_list


void Batman5Routing::batadv_tt_global_free(batadv_tt_global_entry *tt_global, const char *message){
    std::cout << "Deleting global tt entry " << tt_global->common.addr.str()  << "(vid: " <<  tt_global->common.vid << "): " << message << std::endl;

    TranslationTableGlobal::iterator it = tt_globalTable.find(tt_global->common.addr);
    if(it != tt_globalTable.end())
        tt_globalTable.erase(it);
}



void Batman5Routing::batadv_tt_local_set_pending(batadv_tt_local_entry *tt_local_entry, uint16_t flags, const char *message){
    batadv_tt_local_event(tt_local_entry, flags);

    /* The local client has to be marked as "pending to be removed" but has
     * to be kept in the table in order to send it in a full table
     * response issued before the net ttvn increment (consistency check) */
    tt_local_entry->common.flags |= BATADV_TT_CLIENT_PENDING;

    std::cout << "Local tt entry (" << tt_local_entry->common.addr.str() << ", vid: " << tt_local_entry->common.vid << ") pending to be removed: " <<  message << std::endl;
}//batadv_tt_local_set_pending


/**
 * batadv_tt_global_del() - remove a client from the global table
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: an originator serving this client
 * @addr: the mac address of the client
 * @vid: VLAN identifier
 * @message: a message explaining the reason for deleting the client to print
 *  for debugging purpose
 * @roaming: true if the deletion has been triggered by a roaming event
 */
void Batman5Routing::batadv_tt_global_del(Orig_node_ptr orig_node, MACAddress addr, unsigned short vid, const char *message, bool roaming){
    batadv_tt_global_entry *tt_global_entry;
    batadv_tt_local_entry *local_entry = NULL;

    tt_global_entry = batadv_tt_global_find(addr, vid);
    if (!tt_global_entry)
        return;

    if (!roaming) {
        batadv_tt_global_del_orig_node(tt_global_entry, orig_node, message);

        if (tt_global_entry->orig_list.empty())
            batadv_tt_global_free(tt_global_entry, message);

        return;
    }

    /* if we are deleting a global entry due to a roam
     * event, there are two possibilities:
     * 1) the client roamed from node A to node B => if there
     *    is only one originator left for this client, we mark
     *    it with BATADV_TT_CLIENT_ROAM, we start a timer and we
     *    wait for node B to claim it. In case of timeout
     *    the entry is purged.
     *
     *    If there are other originators left, we directly delete
     *    the originator.
     * 2) the client roamed to us => we can directly delete
     *    the global entry, since it is useless now.
     */
    local_entry = batadv_tt_local_find(tt_global_entry->common.addr, vid);
    if (local_entry) {
        /* local entry exists, case 2: client roamed to us. */
        batadv_tt_global_del_orig_list(tt_global_entry);
        batadv_tt_global_free(tt_global_entry, message);
    }
    else {
        /* no local entry exists, case 1: check for roaming */
        batadv_tt_global_del_roaming(tt_global_entry, orig_node, message);
    }
}//batadv_tt_global_del


/* If the client is to be deleted, we check if it is the last origantor entry
 * within tt_global entry. If yes, we set the BATADV_TT_CLIENT_ROAM flag and the
 * timer, otherwise we simply remove the originator scheduled for deletion. */
void Batman5Routing::batadv_tt_global_del_roaming(batadv_tt_global_entry *tt_global_entry, Orig_node_ptr orig_node, const char *message){
    bool last_entry = true;
    batadv_tt_orig_list_entry *orig_entry;

    /* no local entry exists, case 1:
     * Check if this is the last one or if other entries exist. */

    for(Originator_tt_list::iterator it = tt_global_entry->orig_list.begin(); it != tt_global_entry->orig_list.end(); it++) {
        orig_entry = it->second;
        if (orig_entry->orig_node != orig_node) {
            last_entry = false;
            break;
        }
    }

    if (last_entry) {
        /* its the last one, mark for roaming. */
        tt_global_entry->common.flags |= BATADV_TT_CLIENT_ROAM;
        tt_global_entry->roam_at = simTime();
    } else {
        /* there is another entry, we can simply delete this
         * one and can still use the other one.
         */
        batadv_tt_global_del_orig_node(tt_global_entry, orig_node, message);
    }
}//batadv_tt_global_del_roaming


/**
 * batadv_tt_tvlv_unicast_handler_v1() - process incoming (unicast) tt tvlv
 *  container
 * @bat_priv: the bat priv with all the soft interface information
 * @src: mac address of tt tvlv sender
 * @dst: mac address of tt tvlv recipient
 * @tvlv_value: tvlv buffer containing the tt data
 * @tvlv_value_len: tvlv buffer length
 *
 * Return: NET_RX_DROP if the tt tvlv is to be re-routed, NET_RX_SUCCESS
 * otherwise.    out
 */
int Batman5Routing::batadv_tt_tvlv_unicast_handler_v1(MACAddress src, MACAddress dst, void *tvlv_value, uint16_t tvlv_value_len){

    batadv_tvlv_tt_data *tt_data;
    uint16_t tt_vlan_len, tt_num_entries;
    char tt_flag;
    bool ret;

    if (tvlv_value_len < sizeof(*tt_data))
        return NET_RX_SUCCESS;

    tt_data = (batadv_tvlv_tt_data *)tvlv_value;
    tvlv_value_len -= sizeof(*tt_data);

    tt_vlan_len = sizeof(batadv_tvlv_tt_vlan);
    tt_vlan_len *= tt_data->getNum_vlan();

    if (tvlv_value_len < tt_vlan_len)
        return NET_RX_SUCCESS;

    tvlv_value_len -= tt_vlan_len;
    tt_num_entries = batadv_tt_entries(tvlv_value_len);

    switch (tt_data->getTt_flags() & BATADV_TT_DATA_TYPE_MASK) {
    case BATADV_TT_REQUEST:
        /* If this node cannot provide a TT response the tt_request is forwarded */
        ret = batadv_send_tt_response(tt_data, src, dst);
        if (!ret) {
            if (tt_data->getTt_flags() & BATADV_TT_FULL_TABLE)
                tt_flag = 'F';
            else
                tt_flag = '.';

            std::cout << "Routing TT_REQUEST to " << dst.str() << " [" << tt_flag << "]" << std::endl;
            /* tvlv API will re-route the packet */
            return NET_RX_DROP;
        }
        break;
    case BATADV_TT_RESPONSE:
        if (batadv_is_my_mac(dst)) {
            batadv_handle_tt_response(tt_data, src, tt_num_entries);
            return NET_RX_SUCCESS;
        }

        if (tt_data->getTt_flags() & BATADV_TT_FULL_TABLE)
            tt_flag =  'F';
        else
            tt_flag = '.';

        std::cout << "Routing TT_RESPONSE to " << dst.str() << " [" << tt_flag << "]" <<std::endl;

        /* tvlv API will re-route the packet */
        return NET_RX_DROP;
    }

    return NET_RX_SUCCESS;
}//batadv_tt_tvlv_unicast_handler_v1


/**
 * batadv_tt_tvlv_ogm_handler_v1() - process incoming tt tvlv container
 * @bat_priv: the bat priv with all the soft interface information
 * @orig: the orig_node of the ogm
 * @flags: flags indicating the tvlv state (see batadv_tvlv_handler_flags)
 * @tvlv_value: tvlv buffer containing the gateway data
 * @tvlv_value_len: tvlv buffer length
 */
void Batman5Routing::batadv_tt_tvlv_ogm_handler_v1(Orig_node_ptr orig, uint8_t flags, void *tvlv_value, uint16_t tvlv_value_len){
    batadv_tvlv_tt_vlan *tt_vlan;
    batadv_tvlv_tt_change *tt_change;
    batadv_tvlv_tt_data *tt_data;
    uint16_t num_entries, num_vlan;

    if (tvlv_value_len < sizeof(*tt_data))
        return;

    tt_data = (batadv_tvlv_tt_data *)tvlv_value;
    tvlv_value_len -= sizeof(*tt_data);

    num_vlan = tt_data->getNum_vlan();

    if (tvlv_value_len < sizeof(*tt_vlan) * num_vlan)
        return;

    tt_vlan = (batadv_tvlv_tt_vlan *)(tt_data + 1);
    tt_change = (batadv_tvlv_tt_change *)(tt_vlan + num_vlan);
    tvlv_value_len -= sizeof(*tt_vlan) * num_vlan;

    num_entries = batadv_tt_entries(tvlv_value_len);

    batadv_tt_update_orig(orig, tt_vlan, num_vlan, tt_change, num_entries, tt_data->getTtvn());
}//batadv_tt_tvlv_ogm_handler_v1



/**
 * batadv_tt_update_orig() - update global translation table with new tt
 *  information received via ogms
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: the orig_node of the ogm
 * @tt_buff: pointer to the first tvlv VLAN entry
 * @tt_num_vlan: number of tvlv VLAN entries
 * @tt_change: pointer to the first entry in the TT buffer
 * @tt_num_changes: number of tt changes inside the tt buffer
 * @ttvn: translation table version number of this changeset
 */
void Batman5Routing::batadv_tt_update_orig(Orig_node_ptr orig_node, const void *tt_buff, uint16_t tt_num_vlan, batadv_tvlv_tt_change *tt_change, uint16_t tt_num_changes, uint8_t ttvn){
    uint8_t orig_ttvn = orig_node->last_ttvn;
    batadv_tvlv_tt_vlan *tt_vlan;
    bool full_table = true;
    bool has_tt_init;

    tt_vlan = (batadv_tvlv_tt_vlan *)tt_buff;
    has_tt_init = BATADV_ORIG_CAPA_HAS_TT & orig_node->capa_initialized;

    /* orig table not initialised AND first diff is in the OGM OR the ttvn
     * increased by one -> we can apply the attached changes */
    if ((!has_tt_init && ttvn == 1) || ttvn - orig_ttvn == 1) {
        /* the OGM could not contain the changes due to their size or
         * because they have already been sent BATADV_TT_OGM_APPEND_MAX times.
         * In this case send a tt request */
        if (!tt_num_changes) {
            full_table = false;
            goto request_table;
        }

        batadv_tt_update_changes(orig_node, tt_num_changes, ttvn, tt_change);

        /* Even if we received the precomputed crc with the OGM, we
         * prefer to recompute it to spot any possible inconsistency
         * in the global table
         */
        batadv_tt_global_update_crc(orig_node);

        /* The ttvn alone is not enough to guarantee consistency
         * because a single value could represent different states
         * (due to the wrap around). Thus a node has to check whether
         * the resulting table (after applying the changes) is still
         * consistent or not. E.g. a node could disconnect while its
         * ttvn is X and reconnect on ttvn = X + TTVN_MAX: in this case
         * checking the CRC value is mandatory to detect the
         * inconsistency
         */
        if (!batadv_tt_global_check_crc(orig_node, tt_vlan, tt_num_vlan))
            goto request_table;
    }
    else {
        /* if we missed more than one change or our tables are not
         * in sync anymore -> request fresh tt data */
        if (!has_tt_init || ttvn != orig_ttvn || !batadv_tt_global_check_crc(orig_node, tt_vlan, tt_num_vlan)) {
request_table:
            std::cout << "TT inconsistency for " << orig_node->orig.str() << ". Need to retrieve the correct information (ttvn: " << ttvn << " last_ttvn: " << orig_ttvn << "num_changes: " << tt_num_changes << ")" << std::endl;
            batadv_send_tt_request(orig_node, ttvn, tt_vlan, tt_num_vlan, full_table);
            return;
        }
    }
}//batadv_tt_update_orig


/**
 * batadv_send_tt_request() - send a TT Request message to a given node
 * @bat_priv: the bat priv with all the soft interface information
 * @dst_orig_node: the destination of the message
 * @ttvn: the version number that the source of the message is looking for
 * @tt_vlan: pointer to the first tvlv VLAN object to request
 * @num_vlan: number of tvlv VLAN entries
 * @full_table: ask for the entire translation table if true, while only for the
 *  last TT diff otherwise
 *
 * Return: true if the TT Request was sent, false otherwise
 */
bool Batman5Routing::batadv_send_tt_request(Orig_node_ptr dst_orig_node, uint8_t ttvn, batadv_tvlv_tt_vlan *tt_vlan, uint16_t num_vlan, bool full_table){
    batadv_tvlv_tt_data *tvlv_tt_data = NULL;
    TT_req_node_ptr tt_req_node = NULL;
    batadv_tvlv_tt_vlan *tt_vlan_req;
    batadv_hard_iface *primary_if;
    bool ret = false;
    int i, size;

    primary_if = batadv_primary_if_get_selected();
    if (!primary_if)
        return ret;

    /* The new tt_req will be issued only if I'm not waiting for a
     * reply from the same orig_node yet
     */
    tt_req_node = batadv_tt_req_node_new(dst_orig_node);
    if (!tt_req_node)
        return ret;

    size = sizeof(*tvlv_tt_data) + sizeof(*tt_vlan_req) * num_vlan;
    tvlv_tt_data = new batadv_tvlv_tt_data();
    if (!tvlv_tt_data){
        delete tvlv_tt_data;
        return ret;
    }

    tvlv_tt_data->setTt_flags(BATADV_TT_REQUEST);
    tvlv_tt_data->setTtvn(ttvn);
    tvlv_tt_data->setNum_vlan(num_vlan);

    /* send all the CRCs within the request. This is needed by intermediate
     * nodes to ensure they have the correct table before replying */
    tt_vlan_req = (batadv_tvlv_tt_vlan *)(tvlv_tt_data + 1);
    for (i = 0; i < num_vlan; i++) {
        tt_vlan_req->setVid_vlan(tt_vlan->getVid_vlan());
        tt_vlan_req->setCrc32(tt_vlan->getCrc32());

        tt_vlan_req++;
        tt_vlan++;
    }

    if (full_table){
        uint8_t write_flag = tvlv_tt_data->getTt_flags() | BATADV_TT_FULL_TABLE;
        tvlv_tt_data->setTt_flags(write_flag);
    }

    std::cout << "Sending TT_REQUEST to " <<  dst_orig_node->orig.str()  << " [" << (full_table ? 'F' : '.') << "]" << std::endl;

    batadv_tvlv_unicast_send(get_hardif_MAC(primary_if->name.c_str()), dst_orig_node->orig, BATADV_TVLV_TT, 1, tvlv_tt_data, size);
    ret = true;

    delete tvlv_tt_data;
    return ret;
}//batadv_send_tt_request


/**
 * batadv_tt_req_node_new() - search and possibly create a tt_req_node object
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: orig node this request is being issued for
 *
 * Return: the pointer to the new tt_req_node struct if no request
 * has already been issued for this orig_node, NULL otherwise.
 */
TT_req_node_ptr Batman5Routing::batadv_tt_req_node_new(Orig_node_ptr orig_node){
    TT_req_node_ptr tt_req_node_tmp = NULL;

    for(tt_req_node_list::iterator it = bat_priv_tt.req_list.begin(); it != bat_priv_tt.req_list.end(); it++) {
        tt_req_node_tmp = (*it);
        if (tt_req_node_tmp->addr.compareTo(orig_node->orig) && !batadv_has_timed_out(tt_req_node_tmp->issued_at, BATADV_TT_REQUEST_TIMEOUT))
            return NULL;
    }

    TT_req_node_ptr tt_req_node(new batadv_tt_req_node());
    if (!tt_req_node)
        return tt_req_node;

    tt_req_node->addr = orig_node->orig;
    tt_req_node->issued_at = simTime();

    bat_priv_tt.req_list.push_back(tt_req_node);

    return tt_req_node;
}//batadv_tt_req_node_new


/**
 * batadv_roam_tvlv_unicast_handler_v1() - process incoming tt roam tvlv
 *  container
 * @bat_priv: the bat priv with all the soft interface information
 * @src: mac address of tt tvlv sender
 * @dst: mac address of tt tvlv recipient
 * @tvlv_value: tvlv buffer containing the tt data
 * @tvlv_value_len: tvlv buffer length
 *
 * Return: NET_RX_DROP if the tt roam tvlv is to be re-routed, NET_RX_SUCCESS
 * otherwise.
 */
int Batman5Routing::batadv_roam_tvlv_unicast_handler_v1(MACAddress src, MACAddress dst, void *tvlv_value, uint16_t tvlv_value_len){
    batadv_tvlv_roam_adv *roaming_adv;
    Orig_node_ptr orig_node = NULL;

    /* If this node is not the intended recipient of the
     * roaming advertisement the packet is forwarded
     * (the tvlv API will re-route the packet). */
    if (!batadv_is_my_mac(dst))
        return NET_RX_DROP;

    if (tvlv_value_len < sizeof(*roaming_adv))
        return NET_RX_SUCCESS;

    orig_node = batadv_orig_find(src);
    if (!orig_node)
        return NET_RX_SUCCESS;

    roaming_adv = (batadv_tvlv_roam_adv *)tvlv_value;

    std::cout << "Received ROAMING_ADV from " << src.str() << " (client " << roaming_adv->getClient().str() << ")" << std::endl;

    batadv_tt_global_add(orig_node, roaming_adv->getClient(), roaming_adv->getVid(), BATADV_TT_CLIENT_ROAM, orig_node->last_ttvn + 1);

    return NET_RX_SUCCESS;
}//batadv_roam_tvlv_unicast_handler_v1


/* Purge out all the tt local entries marked with BATADV_TT_CLIENT_PENDING */
void Batman5Routing::batadv_tt_local_purge_pending_clients(){

    batadv_tt_common_entry *tt_common;


    if (tt_localTable.empty())
        return;

    for(TranslationTableLocal::iterator it = tt_localTable.begin(); it != tt_localTable.end();){
            tt_common = &it->second->common;
            if (!(tt_common->flags & BATADV_TT_CLIENT_PENDING)){
                it++;
                continue;
            }

            std::cout <<"Deleting local tt entry (" << tt_common->addr.str() << ", vid: " << tt_common->vid << " ): pending" << std::endl;

            it = tt_localTable.erase(it);

    }
}//batadv_tt_local_purge_pending_clients


/**
 * batadv_tt_init() - initialise the translation table internalsbatadv_tt_roam_node
 * @bat_priv: the bat priv with all the soft interface information
 *
 * Return: 0 on success or negative error number in case of failure.
 */
int Batman5Routing::batadv_tt_init(){
    int ret;

    ret = batadv_tt_local_init();
    if (ret < 0)
        return ret;

    ret = batadv_tt_global_init();
    if (ret < 0)
        return ret;

    void (Batman5Routing::*ogm_handler_function)(Orig_node_ptr, uint8_t, void*, uint16_t) = &Batman5Routing::batadv_tt_tvlv_ogm_handler_v1;
    int (Batman5Routing::*tvlv_unicast_handler_function)(MACAddress, MACAddress, void*, uint16_t) = &Batman5Routing::batadv_tt_tvlv_unicast_handler_v1;
    batadv_tvlv_handler_register(ogm_handler_function, tvlv_unicast_handler_function, BATADV_TVLV_TT, 1, BATADV_NO_FLAGS);

    int (Batman5Routing::*roam_unicast_handler_function)(MACAddress,MACAddress, void*,uint16_t) = &Batman5Routing::batadv_roam_tvlv_unicast_handler_v1;
    batadv_tvlv_handler_register(NULL, roam_unicast_handler_function, BATADV_TVLV_ROAM, 1, BATADV_NO_FLAGS);

    tt_purge = new cMessage("tt_purge");
    scheduleAt(tt_purge_timeout_interval+jitter,tt_purge);

    return 1;
}//batadv_tt_init


/**
 * batadv_tt_local_event() - store a local TT event (ADD/DEL)
 * @bat_priv: the bat priv with all the soft interface information
 * @tt_local_entry: the TT entry involved in the event
 * @event_flags: flags to store in the event structure
 */
void Batman5Routing::batadv_tt_local_event(batadv_tt_local_entry *tt_local_entry, uint8_t event_flags){
    batadv_tvlv_tt_change *tt_change_node, *entry;
    batadv_tt_common_entry *common = &tt_local_entry->common;
    uint8_t flags = common->flags | event_flags;
    bool event_removed = false;
    bool del_op_requested, del_op_entry;

    tt_change_node = new batadv_tvlv_tt_change();
    if (!tt_change_node)
        return;

    tt_change_node->setFlags_change(flags);
    tt_change_node->setAddr(common->addr);
    tt_change_node->setVid_change(common->vid);

    del_op_requested = flags & BATADV_TT_CLIENT_DEL;

    /* check for ADD+DEL or DEL+ADD events */
    for (tt_changeList::iterator it = bat_priv_tt.changes_list.begin(); it != bat_priv_tt.changes_list.end(); it++) {
        entry = (*it);
        if (!entry->getAddr().compareTo(common->addr))
            continue;

        /* DEL+ADD in the same orig interval have no effect and can be
         * removed to avoid silly behaviour on the receiver side. The
         * other way around (ADD+DEL) can happen in case of roaming of
         * a client still in the NEW state. Roaming of NEW clients is
         * now possible due to automatically recognition of "temporary" clients */
        del_op_entry = entry->getFlags_change() & BATADV_TT_CLIENT_DEL;
        if (!del_op_requested && del_op_entry)
            goto del;
        if (del_op_requested && !del_op_entry)
            goto del;

        /* this is a second add in the same originator interval. It
         * means that flags have been changed: update them! */
        if (!del_op_requested && !del_op_entry)
            entry->setFlags_change(flags);

        continue;
del:
        delete entry;
        delete tt_change_node;
        event_removed = true;
        if (event_removed)
            bat_priv_tt.local_changes--;
        else
            bat_priv_tt.local_changes++;
    }

    /* track the change in the OGMinterval list */
    bat_priv_tt.changes_list.push_back(tt_change_node);

    if (event_removed)
        bat_priv_tt.local_changes--;
    else
        bat_priv_tt.local_changes++;
}//batadv_tt_local_event


void Batman5Routing::batadv_tt_purge(){

    batadv_tt_local_purge(BATADV_TT_LOCAL_TIMEOUT);
    batadv_tt_global_purge();
    batadv_tt_req_purge();
    batadv_tt_roam_purge();

}//batadv_tt_purge


/**
 * batadv_tt_local_purge() - purge inactive tt local entries
 * @bat_priv: the bat priv with all the soft interface information
 * @timeout: parameter deciding whether a given tt local entry is considered
 *  inactive or not
 */
void Batman5Routing::batadv_tt_local_purge(int timeout){
    batadv_tt_local_purge_list(timeout);
}//batadv_tt_local_purge


/**
 * batadv_tt_local_purge_list() - purge inactive tt local entries
 * @bat_priv: the bat priv with all the soft interface information
 * @head: pointer to the list containing the local tt entries
 * @timeout: parameter deciding whether a given tt local entry is considered
 *  inactive or not
 */
void Batman5Routing::batadv_tt_local_purge_list(uint32_t timeout){
    batadv_tt_local_entry *tt_local_entry;

    for (TranslationTableLocal::iterator it = tt_localTable.begin(); it != tt_localTable.end(); it++){
        tt_local_entry = it->second;

        if (tt_local_entry->common.flags & BATADV_TT_CLIENT_NOPURGE)
            continue;

        /* entry already marked for deletion */
        if (tt_local_entry->common.flags & BATADV_TT_CLIENT_PENDING)
            continue;

        if (!batadv_has_timed_out(tt_local_entry->last_seen, timeout))
            continue;

        batadv_tt_local_set_pending(tt_local_entry, BATADV_TT_CLIENT_DEL, "timed out");
    }
}//batadv_tt_local_purge_list


void Batman5Routing::batadv_tt_global_purge(){
    char *msg = NULL;
    batadv_tt_global_entry *tt_global;


    for(TranslationTableGlobal::iterator it = tt_globalTable.begin(); it != tt_globalTable.end();){
        tt_global = it->second;

        if (!batadv_tt_global_to_purge(tt_global, &msg)){
            it++;
            continue;
        }

        std::cout << "Deleting global tt entry " << tt_global->common.addr.str() << "(vid: " << tt_global->common.vid << "): " << msg << std::endl;

        tt_global->orig_list.clear();
        tt_global->orig_list_count = 0;

        it = tt_globalTable.erase(it);

    }
}//batadv_tt_global_purge


bool Batman5Routing::batadv_tt_global_to_purge(batadv_tt_global_entry *tt_global, char **msg){
    bool purge = false;
    uint32_t roam_timeout = BATADV_TT_CLIENT_ROAM_TIMEOUT;
    uint32_t temp_timeout = BATADV_TT_CLIENT_TEMP_TIMEOUT;

    if ((tt_global->common.flags & BATADV_TT_CLIENT_ROAM) &&
        batadv_has_timed_out(tt_global->roam_at, roam_timeout)) {
        purge = true;
        std::string message = "Roaming timeout\n";
        strcpy(*msg, message.c_str());
    }

    if ((tt_global->common.flags & BATADV_TT_CLIENT_TEMP) &&
        batadv_has_timed_out(tt_global->common.added_at, temp_timeout)) {
        purge = true;
        std::string message = "Temporary client timeout\n";
        strcpy(*msg, message.c_str());
    }

    return purge;
}//batadv_tt_global_to_purge


void Batman5Routing::batadv_tt_req_purge(){
    TT_req_node_ptr node;

    for(tt_req_node_list::iterator it = bat_priv_tt.req_list.begin(); it != bat_priv_tt.req_list.end();){
        node = (*it);
        if (batadv_has_timed_out(node->issued_at, BATADV_TT_REQUEST_TIMEOUT)) {
            it = bat_priv_tt.req_list.erase(it);
        }
        else{
            it++;
        }
    }
}//batadv_tt_req_purge


void Batman5Routing::batadv_tt_roam_purge(){
    TT_roam_node_ptr node = NULL;

    for(tt_roam_node_list::iterator it = bat_priv_tt.roam_list.begin(); it != bat_priv_tt.roam_list.end();){

        node = (*it);
        if (!batadv_has_timed_out(node->first_time, BATADV_ROAMING_MAX_TIME)){
            it++;
            continue;
        }
        else{
            it = bat_priv_tt.roam_list.erase(it);
        }
    }

}//batadv_tt_roam_purge

} /* namespace inet */
