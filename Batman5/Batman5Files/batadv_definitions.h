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
#include "inet/linklayer/common/MACAddress.h"
#include "inet/networklayer/contract/IInterfaceTable.h"

#include <iostream>
#include <memory>
#include "../../Batman5/controlpackages/BATMANV_ControlPackets_m.h"

/**
 * Addapted from batman-adv/net/batman-adv/main.h
 *               batman-adv/net/batman-adv/hard-interface.h
 *               uapi/linux/batman_adv.h
 * Version: 2018.0
 */

#ifndef INET_BATMAN_ROUTING_BATADV_DEFINITIONS_H_
#define INET_BATMAN_ROUTING_BATADV_DEFINITIONS_H_

#define BIT0  0000000000000001
#define BIT1  0000000000000010
#define BIT2  0000000000000100
#define BIT3  0000000000001000
#define BIT4  0000000000010000
#define BIT5  0000000000100000
#define BIT6  0000000001000000
#define BIT7  0000000010000000
#define BIT8  0000000100000000
#define BIT9  0000001000000000
#define BIT10 0000010000000000
#define BIT11 0000100000000000
#define BIT12 0001000000000000
#define BIT13 0010000000000000
#define BIT14 0100000000000000
#define BIT15 1000000000000000

#define NET_XMIT_SUCCESS 0
#define NET_XMIT_DROP 1
#define ENOMEM          12      /* Out of memory */
#define EINVAL          22      /* Invalid argument */
#define EREMOTE         66      /* Object is remote */
#define EINPROGRESS     115     /* Operation now in progress */
namespace inet {

/**
 ********************* Own Classes and definitions ********************
 */
class Batman5Routing;

class batadv_elp_packet;
typedef std::vector<batadv_elp_packet> ELP_buff;
class batadv_elpProbe_packet;
class batadv_ogm2_packet;
typedef std::vector<batadv_ogm2_packet> OGMv2_buff;
class batadv_iv_ogm_packet;
typedef std::vector<batadv_iv_ogm_packet> OGM_buff;
class batadv_tvlv_tt_data;
class batadv_orig_ifinfo;
typedef std::shared_ptr<batadv_orig_ifinfo> Orig_ifinfo_ptr;
typedef std::vector<Orig_ifinfo_ptr> origIfInfo_list;
class batadv_gw_node;
typedef std::vector<batadv_gw_node *> gwListObject;
class batadv_tvlv_tt_change;
typedef std::vector<batadv_tvlv_tt_change *> tt_changeList;
class batadv_neigh_ifinfo;
typedef std::shared_ptr<batadv_neigh_ifinfo> Neigh_ifinfo_ptr;
typedef std::vector<Neigh_ifinfo_ptr> interface_info_list;


/**
 ********************   Klassen     ********************
 */

/*   Definition of the B.A.T.M.A.N. V Neighbor-Table   */
class batadv_neigh_node;
typedef std::shared_ptr<batadv_neigh_node> Neigh_node_ptr;
typedef std::map<MACAddress, Neigh_node_ptr> Neighbor_Table;
typedef std::shared_ptr<Neighbor_Table> Neighbor_Table_ptr;

/*  Definition of the B.A.T.M.A.N. V Originator-Table   */
class batadv_orig_node;
typedef std::shared_ptr<batadv_orig_node> Orig_node_ptr;
typedef std::map<MACAddress, Orig_node_ptr> Originator_Table;
typedef std::shared_ptr<Originator_Table> Originator_Table_ptr;

class batadv_tt_orig_list_entry;
typedef std::map<Orig_node_ptr, batadv_tt_orig_list_entry*> Originator_tt_list;

class batadv_tvlv_container;
typedef std::shared_ptr<batadv_tvlv_container> TvlvContainer_ptr;
typedef std::map<uint8_t, TvlvContainer_ptr> Container_list;

class batadv_tvlv_handler;
typedef /*std::shared_ptr<*/ batadv_tvlv_handler* /*>*/ TvlvHandler_ptr;
typedef std::map<uint8_t, TvlvHandler_ptr> Handler_list;

class batadv_tt_req_node;
typedef std::shared_ptr<batadv_tt_req_node> TT_req_node_ptr;
typedef std::vector<TT_req_node_ptr>  tt_req_node_list;

class batadv_tt_roam_node;
typedef std::shared_ptr<batadv_tt_roam_node> TT_roam_node_ptr;
typedef std::vector<TT_roam_node_ptr>  tt_roam_node_list;

class batadv_nc_node;
typedef std::shared_ptr<batadv_nc_node> NC_node_ptr;
typedef std::map<MACAddress, NC_node_ptr> ncNodeTable;
typedef std::map<MACAddress, cPacket*> NC_Buffer;

class batadv_nc_path;
typedef std::shared_ptr<batadv_nc_path> NC_path_ptr;
typedef std::map<MACAddress, NC_path_ptr> ncPathList;

class batadv_nc_packet;
typedef std::shared_ptr<batadv_nc_packet> NC_packet_ptr;
typedef std::map<uint32_t, NC_packet_ptr> nc_packet_list;

/*  Definition of the B.A.T.M.A.N. V Translation-Tables   */
class batadv_tt_common_entry {
    public:
        /** @addr: mac address of non-mesh client */
        MACAddress addr;
        /** @vid: VLAN identifier */
        unsigned short vid;
        /** @flags: various state handling flags (see batadv_tt_client_flags) */
        uint16_t flags;
        /** @added_at: timestamp used for purging stale tt common entries */
        simtime_t added_at;
};
class batadv_tt_orig_list_entry {
    public:
        /** @orig_node: pointer to orig node announcing this non-mesh client */
        Orig_node_ptr orig_node;
        /*@ttvn: translation table version number which added the non-mesh client */
        uint8_t ttvn;
        /** @flags: per orig entry TT sync flags */
        uint8_t flags;
};
/*  Definition of the B.A.T.M.A.N. V Global Translation-Table   */
class batadv_tt_global_entry {
    public:
        /** @common: general translation table data */
        batadv_tt_common_entry common;
        /** @orig_list: list of orig nodes announcing this non-mesh client */
        Originator_tt_list orig_list;
        /** @orig_list_count: number of items in the orig_list */
        uint32_t orig_list_count;
        /** @roam_at: time at which TT_GLOBAL_ROAM was set */
        simtime_t roam_at;

};
typedef std::map<MACAddress, batadv_tt_global_entry*> TranslationTableGlobal;

/*  Definition of the B.A.T.M.A.N. V Local Translation-Table   */
class batadv_tt_local_entry {
    public:
        /** @common: general translation table data */
        batadv_tt_common_entry common;
        /** @last_seen: timestamp used for purging stale tt local entries */
        simtime_t last_seen;
};
typedef std::map<MACAddress, batadv_tt_local_entry*> TranslationTableLocal;

/*  Definition of the B.A.T.M.A.N. V Distributed-ARP-Table   */
class batadv_dat_entry {
    public:
        /** @ip: the IPv4 corresponding to this DAT/ARP entry */
        IPv4Address ip;
        /** @mac_addr: the MAC address associated to the stored IPv4 */
        MACAddress mac_addr;
        /** @last_update: time in jiffies when this entry was refreshed last time */
        simtime_t last_update;
};
typedef std::map<IPv4Address, batadv_dat_entry *> DAT;

struct ewma_Metric{
    uint32_t weight = 8;
    uint32_t factor = 10;
    uint32_t internal;
};

/********************************************************************** */

/*
 *  IEEE 802.3 Ethernet magic constants.
 *  The frame sizes omit the preamble and FCS/CRC (frame check sequence).
 *  Addapted from <linux/types.h>
 */
#define NET_IP_ALIGN 2      // Default Value in Linux Kernel
#define ETH_ALEN    6       /* Octets in one ethernet addr   */
#define ETH_HLEN    14      /* Total octets in header.   */
#define ETH_ZLEN    60      /* Min. octets in frame sans FCS */
#define ETH_DATA_LEN    1500        /* Max. octets in payload    */
#define ETH_FRAME_LEN   1514        /* Max. octets in frame sans FCS */
#define ETH_FCS_LEN 4       /* Octets in the FCS         */
#define ETH_P_BATMAN 0x4305 /* B.A.T.M.A.N.-Advanced Ethernet-packet [ NOT AN OFFICIALLY REGISTERED ID ] */

/* B.A.T.M.A.N. parameters */
#define BATADV_TQ_MAX_VALUE 255
#define BATADV_JITTER 20

/* Time To Live of broadcast messages */
#define BATADV_TTL 50

/* maximum sequence number age of broadcast messages */
#define BATADV_BCAST_MAX_AGE 64

#define BATADV_PURGE_TIMEOUT 200000 /* 200 seconds */
#define BATADV_TT_LOCAL_TIMEOUT 600000 /* in milliseconds */
#define BATADV_TT_CLIENT_ROAM_TIMEOUT 600000 /* in milliseconds */
#define BATADV_TT_CLIENT_TEMP_TIMEOUT 600000 /* in milliseconds */
#define BATADV_TT_WORK_PERIOD 5000 /* 5 seconds */
#define BATADV_ORIG_WORK_PERIOD 1000 /* 1 second */
#define BATADV_MCAST_WORK_PERIOD 500 /* 0.5 seconds */
#define BATADV_DAT_ENTRY_TIMEOUT (5 * 60000) /* 5 mins in milliseconds */
/* sliding packet range of received originator messages in sequence numbers
 * (should be a multiple of our word size)*/
#define BATADV_TQ_LOCAL_WINDOW_SIZE 64
/* milliseconds we have to keep pending tt_req */
#define BATADV_TT_REQUEST_TIMEOUT 3000

#define BATADV_TQ_GLOBAL_WINDOW_SIZE 5
#define BATADV_TQ_LOCAL_BIDRECT_SEND_MINIMUM 1
#define BATADV_TQ_LOCAL_BIDRECT_RECV_MINIMUM 1
#define BATADV_TQ_TOTAL_BIDRECT_LIMIT 1

/* B.A.T.M.A.N. V */
#define BATADV_THROUGHPUT_DEFAULT_VALUE 100 /* 10 Mbps = 100*100Kbps */
#define BATADV_THROUGHPUT_MAX_VALUE 0xFFFFFFFF
#define BATADV_ELP_PROBES_PER_NODE 2
#define BATADV_ELP_MIN_PROBE_SIZE 200 /* bytes */
#define BATADV_ELP_PROBE_MAX_TX_DIFF 100 /* milliseconds */
#define BATADV_ELP_MAX_AGE 64
#define BATADV_OGM_MAX_ORIGDIFF 5
#define BATADV_OGM_MAX_AGE 64

/* number of OGMs sent with the last tt diff */
#define BATADV_TT_OGM_APPEND_MAX 3

/* Time in which a client can roam at most ROAMING_MAX_COUNT times in
 * milliseconds
 */
#define BATADV_ROAMING_MAX_TIME 20000
#define BATADV_ROAMING_MAX_COUNT 5

#define BATADV_NO_FLAGS 0

#define BATADV_NULL_IFINDEX 0 /* dummy interface index used to avoid interface checks */

#define BATADV_NO_MARK 0

enum batadv_tvlv_handler_flags {
    BATADV_TVLV_HANDLER_OGM_CIFNOTFND = BIT0,
    BATADV_TVLV_HANDLER_OGM_CALLED = BIT1,
};

/* default interface for multi interface operation. The default interface is
 * used for communication which originated locally (i.e. is not forwarded)
 * or where special forwarding is not desired/necessary.
 */
#define BATADV_IF_DEFAULT   ((batadv_hard_iface *)NULL)

#define BATADV_NUM_WORDS BITS_TO_LONGS (BATADV_TQ_LOCAL_WINDOW_SIZE)

#define BATADV_LOG_BUF_LEN 8192   /* has to be a power of 2 */

/* number of packets to send for broadcasts on different interface types */
#define BATADV_NUM_BCASTS_DEFAULT 1
#define BATADV_NUM_BCASTS_WIRELESS 3
#define BATADV_NUM_BCASTS_MAX 3

/* length of the single packet used by the TP meter */
#define BATADV_TP_PACKET_LEN ETH_DATA_LEN

/* msecs after which an ARP_REQUEST is sent in broadcast as fallback */
#define ARP_REQ_DELAY 250
/* numbers of originator to contact for any PUT/GET DHT operation */
#define BATADV_DAT_CANDIDATES_NUM 3

/* BATADV_TQ_SIMILARITY_THRESHOLD - TQ points that a secondary metric can differ
 * at most from the primary one in order to be still considered acceptable
 */
#define BATADV_TQ_SIMILARITY_THRESHOLD 50

/* should not be bigger than 512 bytes or change the size of
 * forw_packet->direct_link_flags
 */
#define BATADV_MAX_AGGREGATION_BYTES 512
#define BATADV_MAX_AGGREGATION_MS 100

#define BATADV_BLA_PERIOD_LENGTH    10000   /* 10 seconds */
#define BATADV_BLA_BACKBONE_TIMEOUT (BATADV_BLA_PERIOD_LENGTH * 6)
#define BATADV_BLA_CLAIM_TIMEOUT    (BATADV_BLA_PERIOD_LENGTH * 10)
#define BATADV_BLA_WAIT_PERIODS     3
#define BATADV_BLA_LOOPDETECT_PERIODS   6
#define BATADV_BLA_LOOPDETECT_TIMEOUT   3000    /* 3 seconds */

#define BATADV_DUPLIST_SIZE         16
#define BATADV_DUPLIST_TIMEOUT      500 /* 500 ms */
/* don't reset again within 30 seconds */
#define BATADV_RESET_PROTECTION_MS  30000
#define BATADV_EXPECTED_SEQNO_RANGE 65536
#define BATADV_ELP_SEQNO_RANGE      4294967296

#define BATADV_NC_NODE_TIMEOUT      10000 /* Milliseconds */

/* BATADV_TP_MAX_NUM - maximum number of simultaneously active tp sessions  */
#define BATADV_TP_MAX_NUM 5

#define BATADV_ICMP_MAX_PACKET_SIZE 116  // max ICMP packet size

/* enum batadv_mesh_state - State of a soft interface   */
enum batadv_mesh_state {
    BATADV_MESH_INACTIVE,       // @BATADV_MESH_INACTIVE: soft interface is not yet running
    BATADV_MESH_ACTIVE,         // @BATADV_MESH_ACTIVE: interface is up and running
    BATADV_MESH_DEACTIVATING,   // @BATADV_MESH_DEACTIVATING: interface is getting shut down
};

#define BATADV_BCAST_QUEUE_LEN      256
#define BATADV_BATMAN_QUEUE_LEN     256

//  enum batadv_uev_action - action type of uevent  */
enum batadv_uev_action {
    BATADV_UEV_ADD = 0,     //  @BATADV_UEV_ADD: gateway was selected (after none was selected)
    BATADV_UEV_DEL,         //  @BATADV_UEV_DEL: selected gateway was removed and none is selected anymore
    BATADV_UEV_CHANGE,      //  @BATADV_UEV_CHANGE: a different gateway was selected as based gateway
    BATADV_UEV_LOOPDETECT,  //  @BATADV_UEV_LOOPDETECT: loop was detected which cannot be handled bybridge loop avoidance
};

/*  enum batadv_uev_type - Type of uevent    */
enum batadv_uev_type {
    BATADV_UEV_GW = 0,      //  @BATADV_UEV_GW: selected gateway was modified
    BATADV_UEV_BLA,         //  @BATADV_UEV_BLA: bridge loop avoidance event
};

/* Backlog congestion levels */
#define NET_RX_SUCCESS      0   /* keep 'em coming, baby */
#define NET_RX_DROP         1   /* packet dropped */

#define BATADV_GW_THRESHOLD 50

/** BATADV_TT_SYNC_MASK - bitmask of the flags that need to be kept in sync
 *  among the nodes. These flags are used to compute the global/local CRC  */
#define BATADV_TT_SYNC_MASK 0x00F0
/* Number of fragment chains for each orig_node */
#define BATADV_FRAG_BUFFER_COUNT 8
/* Maximum number of fragments for one packet */
#define BATADV_FRAG_MAX_FRAGMENTS 16
/* Maxumim size of each fragment */
#define BATADV_FRAG_MAX_FRAG_SIZE 1280
/* Time to keep fragments while waiting for rest of the fragments */
#define BATADV_FRAG_TIMEOUT 10000

#define BATADV_DAT_CANDIDATE_NOT_FOUND  0
#define BATADV_DAT_CANDIDATE_ORIG   1

/* Debug Messages */
#ifdef pr_fmt
#undef pr_fmt
#endif



/**
 ********************     Hard-Interface.h    ********************
**/

/*  enum batadv_hard_if_state - State of a hard interface    */
enum batadv_hard_if_state {
    BATADV_IF_NOT_IN_USE,       //  @BATADV_IF_NOT_IN_USE: interface is not used as slave interface of a batman-adv soft interface
    BATADV_IF_TO_BE_REMOVED,    //  @BATADV_IF_TO_BE_REMOVED: interface will be removed from soft interface
    BATADV_IF_INACTIVE,         //  @BATADV_IF_INACTIVE: interface is deactivated
    BATADV_IF_ACTIVE,           //  @BATADV_IF_ACTIVE: interface is used
    BATADV_IF_TO_BE_ACTIVATED,  //  @BATADV_IF_TO_BE_ACTIVATED: interface is getting activated
    BATADV_IF_I_WANT_YOU,       //  @BATADV_IF_I_WANT_YOU: interface is queued up (using sysfs) for being added as slave interface of a batman-adv soft interface
};

/*  enum batadv_hard_if_bcast - broadcast avoidance options   */
enum batadv_hard_if_bcast {
    BATADV_HARDIF_BCAST_OK = 0,         //  @BATADV_HARDIF_BCAST_OK: Do broadcast on according hard interface
    BATADV_HARDIF_BCAST_NORECIPIENT,    //  @BATADV_HARDIF_BCAST_NORECIPIENT: Broadcast not needed, there is no recipient
    BATADV_HARDIF_BCAST_DUPFWD,         //  @BATADV_HARDIF_BCAST_DUPFWD: There is just the neighbor we got it from
    BATADV_HARDIF_BCAST_DUPORIG,        //  @BATADV_HARDIF_BCAST_DUPORIG: There is just the originator
};

/*  enum batadv_hard_if_cleanup - Cleanup modi for soft_iface after slave remova    */
enum batadv_hard_if_cleanup {
    BATADV_IF_CLEANUP_KEEP,     //  @BATADV_IF_CLEANUP_KEEP: Don't automatically delete soft-interface
    BATADV_IF_CLEANUP_AUTO,     //  @BATADV_IF_CLEANUP_AUTO: Delete soft-interface after last slave was removed
};

/**
 ********************   types.h     ********************
 */

/*enum batadv_hard_iface_wifi_flags - Flags describing the wifi configuration of a batadv_hard_iface */
enum batadv_hard_iface_wifi_flags {
    /* @BATADV_HARDIF_WIFI_WEXT_DIRECT: it is a wext wifi device */
    BATADV_HARDIF_WIFI_WEXT_DIRECT = BIT0,
    /** @BATADV_HARDIF_WIFI_CFG80211_DIRECT: it is a cfg80211 wifi device */
    BATADV_HARDIF_WIFI_CFG80211_DIRECT = BIT1,
    /*@BATADV_HARDIF_WIFI_WEXT_INDIRECT: link device is a wext wifi device */
    BATADV_HARDIF_WIFI_WEXT_INDIRECT = BIT2,
    /* @BATADV_HARDIF_WIFI_CFG80211_INDIRECT: link device is a cfg80211 wifi device */
    BATADV_HARDIF_WIFI_CFG80211_INDIRECT = BIT3,
};


/**
 * enum batadv_orig_capabilities - orig node capabilities
 */
enum batadv_orig_capabilities {
    /* @BATADV_ORIG_CAPA_HAS_DAT: orig node has distributed arp table
     *  enabled */
    BATADV_ORIG_CAPA_HAS_DAT,
    /** @BATADV_ORIG_CAPA_HAS_NC: orig node has network coding enabled */
    BATADV_ORIG_CAPA_HAS_NC,
    /** @BATADV_ORIG_CAPA_HAS_TT: orig node has tt capability */
    BATADV_ORIG_CAPA_HAS_TT,
    /** @BATADV_ORIG_CAPA_HAS_MCAST: orig node has some multicast capability
     *  (= orig node announces a tvlv of type BATADV_TVLV_MCAST) */
    BATADV_ORIG_CAPA_HAS_MCAST,
};



enum batadv_v_hard_iface_flags {
    /* @BATADV_FULL_DUPLEX: tells if the connection over this link is full-duplex */
    BATADV_FULL_DUPLEX  = BIT0,
    /* @BATADV_WARNING_DEFAULT: tells whether we have warned the user that
     *  no throughput data is available for this interface and that default values are assumed */
    BATADV_WARNING_DEFAULT  = BIT1,
};

/**
 * enum batadv_dhcp_recipient - dhcp destination
 */
enum batadv_dhcp_recipient {
    /** @BATADV_DHCP_NO: packet is not a dhcp message */
    BATADV_DHCP_NO = 0,
    /** @BATADV_DHCP_TO_SERVER: dhcp message is directed to a server */
    BATADV_DHCP_TO_SERVER,
    /** @BATADV_DHCP_TO_CLIENT: dhcp message is directed to a client */
    BATADV_DHCP_TO_CLIENT,
};

/**
 * enum batadv_counters - indices for traffic counters
 */
enum batadv_counters {
    /** @BATADV_CNT_TX: transmitted payload traffic packet counter */
    BATADV_CNT_TX,
    /** @BATADV_CNT_TX_BYTES: transmitted payload traffic bytes counter */
    BATADV_CNT_TX_BYTES,
    /**
     * @BATADV_CNT_TX_DROPPED: dropped transmission payload traffic packet
     *  counter
     */
    BATADV_CNT_TX_DROPPED,
    /** @BATADV_CNT_RX: received payload traffic packet counter */
    BATADV_CNT_RX,
    /** @BATADV_CNT_RX_BYTES: received payload traffic bytes counter */
    BATADV_CNT_RX_BYTES,
    /** @BATADV_CNT_FORWARD: forwarded payload traffic packet counter */
    BATADV_CNT_FORWARD,
    /**
     * @BATADV_CNT_FORWARD_BYTES: forwarded payload traffic bytes counter
     */
    BATADV_CNT_FORWARD_BYTES,
    /**
     * @BATADV_CNT_MGMT_TX: transmitted routing protocol traffic packet
     *  counter
     */
    BATADV_CNT_MGMT_TX,
    /**
     * @BATADV_CNT_MGMT_TX_BYTES: transmitted routing protocol traffic bytes
     *  counter
     */
    BATADV_CNT_MGMT_TX_BYTES,
    /**
     * @BATADV_CNT_MGMT_RX: received routing protocol traffic packet counter
     */
    BATADV_CNT_MGMT_RX,
    /**
     * @BATADV_CNT_MGMT_RX_BYTES: received routing protocol traffic bytes
     *  counter
     */
    BATADV_CNT_MGMT_RX_BYTES,
    /** @BATADV_CNT_FRAG_TX: transmitted fragment traffic packet counter */
    BATADV_CNT_FRAG_TX,
    /**
     * @BATADV_CNT_FRAG_TX_BYTES: transmitted fragment traffic bytes counter
     */
    BATADV_CNT_FRAG_TX_BYTES,
    /** @BATADV_CNT_FRAG_RX: received fragment traffic packet counter */
    BATADV_CNT_FRAG_RX,
    /**
     * @BATADV_CNT_FRAG_RX_BYTES: received fragment traffic bytes counter
     */
    BATADV_CNT_FRAG_RX_BYTES,
    /** @BATADV_CNT_FRAG_FWD: forwarded fragment traffic packet counter */
    BATADV_CNT_FRAG_FWD,
    /**
     * @BATADV_CNT_FRAG_FWD_BYTES: forwarded fragment traffic bytes counter
     */
    BATADV_CNT_FRAG_FWD_BYTES,
    /**
     * @BATADV_CNT_TT_REQUEST_TX: transmitted tt req traffic packet counter
     */

    BATADV_CNT_TT_REQUEST_TX,
    /** @BATADV_CNT_TT_REQUEST_RX: received tt req traffic packet counter */
    BATADV_CNT_TT_REQUEST_RX,
    /**
     * @BATADV_CNT_TT_RESPONSE_TX: transmitted tt resp traffic packet
     *  counter
     */
    BATADV_CNT_TT_RESPONSE_TX,
    /**
     * @BATADV_CNT_TT_RESPONSE_RX: received tt resp traffic packet counter
     */
    BATADV_CNT_TT_RESPONSE_RX,
    /**
     * @BATADV_CNT_TT_ROAM_ADV_TX: transmitted tt roam traffic packet
     *  counter
     */
    BATADV_CNT_TT_ROAM_ADV_TX,
    /**
     * @BATADV_CNT_TT_ROAM_ADV_RX: received tt roam traffic packet counter
     */
    BATADV_CNT_TT_ROAM_ADV_RX,

#ifdef CONFIG_BATMAN_ADV_DAT
    /**
     * @BATADV_CNT_DAT_GET_TX: transmitted dht GET traffic packet counter
     */
    BATADV_CNT_DAT_GET_TX,
    /** @BATADV_CNT_DAT_GET_RX: received dht GET traffic packet counter */
    BATADV_CNT_DAT_GET_RX,
    /**
     * @BATADV_CNT_DAT_PUT_TX: transmitted dht PUT traffic packet counter
     */
    BATADV_CNT_DAT_PUT_TX,
    /** @BATADV_CNT_DAT_PUT_RX: received dht PUT traffic packet counter */
    BATADV_CNT_DAT_PUT_RX,
    /**
     * @BATADV_CNT_DAT_CACHED_REPLY_TX: transmitted dat cache reply traffic
     *  packet counter
     */
    BATADV_CNT_DAT_CACHED_REPLY_TX,
#endif

#ifdef CONFIG_BATMAN_ADV_NC
    /**
     * @BATADV_CNT_NC_CODE: transmitted nc-combined traffic packet counter
     */
    BATADV_CNT_NC_CODE,
    /**
     * @BATADV_CNT_NC_CODE_BYTES: transmitted nc-combined traffic bytes
     *  counter
     */
    BATADV_CNT_NC_CODE_BYTES,
    /**
     * @BATADV_CNT_NC_RECODE: transmitted nc-recombined traffic packet
     *  counter
     */
    BATADV_CNT_NC_RECODE,
    /**
     * @BATADV_CNT_NC_RECODE_BYTES: transmitted nc-recombined traffic bytes
     *  counter
     */
    BATADV_CNT_NC_RECODE_BYTES,
    /**
     * @BATADV_CNT_NC_BUFFER: counter for packets buffered for later nc
     *  decoding
     */
    BATADV_CNT_NC_BUFFER,
    /**
     * @BATADV_CNT_NC_DECODE: received and nc-decoded traffic packet counter
     */
    BATADV_CNT_NC_DECODE,
    /**
     * @BATADV_CNT_NC_DECODE_BYTES: received and nc-decoded traffic bytes
     *  counter
     */
    BATADV_CNT_NC_DECODE_BYTES,
    /**
     * @BATADV_CNT_NC_DECODE_FAILED: received and decode-failed traffic
     *  packet counter
     */
    BATADV_CNT_NC_DECODE_FAILED,
    /**
     * @BATADV_CNT_NC_SNIFFED: counter for nc-decoded packets received in
     *  promisc mode.
     */
    BATADV_CNT_NC_SNIFFED,
#endif
    /** @BATADV_CNT_NUM: number of traffic counters */
    BATADV_CNT_NUM,
};

/**
 * Host-Node
 */
class host_node {
    public:
        cModule *host;
        MACAddress host_mac;
        IPv4Address host_ip;
};

/**
 * struct batadv_hard_iface_bat_iv - per hard-interface B.A.T.M.A.N. IV data
 */
class batadv_hard_iface_bat_iv {
    public:
        /** @ogm_buff: buffer holding the OGM packet */
        OGM_buff ogm_buff;
        /** @ogm_buff_len: length of the OGM packet buffer */
        int ogm_buff_len;
        /** @ogm_seqno: OGM sequence number - used to identify each OGM */
        int ogm_seqno;
};

/**
 * struct batadv_hard_iface_bat_v - per hard-interface B.A.T.M.A.N. V data
 */
class batadv_hard_iface_bat_v {
    public:
        /** @elp_interval: time interval between two ELP transmissions */
        uint32_t elp_interval;
        /** @elp_seqno: current ELP sequence number */
        uint32_t elp_seqno;
        /** @elp_buffer: buffer of elp-packets */
        ELP_buff elp_buffer;
        /* @throughput_override: throughput override to disable link auto-detection */
        uint32_t throughput_override;
        /** @flags: interface specific flags */
        uint8_t flags;
};

/**
 * struct batadv_hard_iface - network device known to batman-adv
 */
class batadv_hard_iface {
    public:
        /** @if_num: identificator of the interface */
        unsigned int if_num;
        /** @if_status: status of the interface for batman-adv */
        batadv_hard_if_state if_status;
        /*@num_bcasts: number of payload re-broadcasts on this interface (ARQ) */
        uint8_t num_bcasts;
        /*@wifi_flags: flags whether this is (directly or indirectly) a wifi interface */
        uint32_t wifi_flags;
        /** @refcount: number of contexts the object is used */
        int refcount;
        std::string name;
        int mtu;
        int if_id;
        /** @bat_iv: per hard-interface B.A.T.M.A.N. IV data */
        //batadv_hard_iface_bat_iv bat_iv;
        /** @bat_v: per hard-interface B.A.T.M.A.N. V data */
        batadv_hard_iface_bat_v bat_v;
        /** @neigh_list: list of unique single hop neighbors via this interface */
        Neighbor_Table_ptr neigh_table;

        std::map<MACAddress,uint32_t> lastProbedThroughput;
        std::map<MACAddress,Coord> lastProbedPosition;
};

/**
 * struct batadv_hardif_neigh_node_bat_v - B.A.T.M.A.N. V private neighbor information
 */
class batadv_hardif_neigh_node_bat_v {
    public:
        /** @throughput: ewma link throughput towards this neighbor */
        ewma_Metric metric; // Exponentially Weighted Moving Average (EWMA)_throughput
        /** @elp_interval: time interval between two ELP transmissions */
        simtime_t elp_interval;
        /** @elp_latest_seqno: latest and best known ELP sequence number */
        uint32_t elp_latest_seqno;
        /**@last_unicast_tx: when the last unicast packet has been sent to this neighbor */
        simtime_t last_unicast_tx;
};
typedef std::shared_ptr<batadv_hardif_neigh_node_bat_v> Hardif_neigh_node_bat_v_ptr;

/**
 * struct batadv_hardif_neigh_node - unique neighbor per hard-interface
 */
class batadv_hardif_neigh_node {
    public:
        /** @addr: the MAC address of the neighboring interface */
        MACAddress addr;
        /*@orig: the address of the originator this neighbor node belongs to */
        MACAddress orig;
        /** @if_incoming: pointer to incoming hard-interface */
        batadv_hard_iface *if_incoming;
        /** @last_seen: when last packet via this neighbor was received */
        simtime_t last_seen;
        /** @bat_v: B.A.T.M.A.N. V private data */
        Hardif_neigh_node_bat_v_ptr bat_v;
        /** @refcount: number of contexts the object is used */
        int refcount;
};
typedef std::shared_ptr<batadv_hardif_neigh_node> Hardif_neigh_node_ptr;

/**
 * struct batadv_neigh_node - structure for single hops neighbors
 */
class batadv_neigh_node {
    public:
        /** @orig_node: pointer to corresponding orig_node */
        Orig_node_ptr orig_node;
        /** @addr: the MAC address of the neighboring interface */
        MACAddress addr;
        /** @ifinfo_list: list for routing metrics per outgoing interface */
        interface_info_list if_info_list;
        /** @if_incoming: pointer to incoming hard-interface */
        batadv_hard_iface *if_incoming;
        /** @last_seen: when last packet via this neighbor was received */
        simtime_t last_seen;
        /** @hardif_neigh: hardif_neigh of this neighbor */
        Hardif_neigh_node_ptr hardif_neigh;
        int refcount;
};

/**
 * struct batadv_orig_ifinfo - originator info per outgoing interface
 */
class batadv_orig_ifinfo {
    public:
        /** @if_outgoing: pointer to outgoing hard-interface */
        batadv_hard_iface *if_outgoing;
        /** @router: router that should be used to reach this originator */
        Neigh_node_ptr router;
        /** @last_real_seqno: last and best known sequence number */
        uint32_t last_real_seqno;
        /** @last_ttl: ttl of last received packet */
        uint8_t last_ttl;
        /** @last_seqno_forwarded: seqno of the OGM which was forwarded last */
        uint32_t last_seqno_forwarded;
        /** @batman_seqno_reset: time when the batman seqno window was reset */
        simtime_t batman_seqno_reset;
        /** @refcount: number of contexts the object is used */
        int refcount;
};

/**
 * struct batadv_orig_bat_iv - B.A.T.M.A.N. IV private orig_node members
 */
class batadv_orig_bat_iv {
    public:
        /**
         * @bcast_own: set of bitfields (one per hard-interface) where each one
         * counts the number of our OGMs this orig_node rebroadcasted "back" to
         * us  (relative to last_real_seqno). Every bitfield is
         * BATADV_TQ_LOCAL_WINDOW_SIZE bits long.
         */
        unsigned long *bcast_own;
        /** @bcast_own_sum: sum of bcast_own */
        uint8_t *bcast_own_sum;
};

/**
 * struct batadv_orig_node - structure for orig_list maintaining nodes of mesh
 */
class batadv_orig_node {
    public:
        /** @orig: originator ethernet address */
        MACAddress orig;
        /** @ifinfo_list: list for routers per outgoing interface */
        origIfInfo_list ifinfo_list;
        /** @last_bonding_candidate: pointer to last ifinfo of last used router */
        Orig_ifinfo_ptr last_bonding_candidate;
        /** @last_seen: time when last packet from this node was received */
        simtime_t last_seen;
        /* @bcast_seqno_reset: time when the broadcast seqno window was reset */
        simtime_t bcast_seqno_reset;
        /** @capabilities: announced capabilities of this originator */
        uint32_t capabilities;
        /** @capa_initialized: bitfield to remember whether a capability was initialized */
        uint32_t capa_initialized;
        /** @last_ttvn: last seen translation table version number */
        int32_t last_ttvn;
        /** @tt_buff: last tt changeset this node received from the orig node */
        tt_changeList  *tt_buff;
        /** @tt_buff_len: length of the last tt changeset this node received from the orig node */
        int16_t tt_buff_len;
        /** @last_bcast_seqno: last broadcast sequence number received by this host */
        uint32_t last_bcast_seqno;
        /**@neigh_list: list of potential next hop neighbor towards this orig node */
        Neighbor_Table_ptr neigh_list;
        /** @refcount: number of contexts the object is used */
        int refcount;

 //#ifdef CONFIG_BATMAN_ADV_NC
        /** @in_coding_list: list of nodes this orig can hear */
        ncNodeTable *in_coding_list;
        /** @out_coding_list: list of nodes that can hear this orig */
        ncNodeTable *out_coding_list;
        /** @bat_iv: B.A.T.M.A.N. IV private structure */
        //batadv_orig_bat_iv bat_iv;
};

/**
 * struct batadv_gw_node - structure for orig nodes announcing gw capabilities
 */
class batadv_gw_node {
    public:
        /** @orig_node: pointer to corresponding orig node */
        Orig_node_ptr orig_node;
        /** @bandwidth_down: advertised uplink download bandwidth */
        uint32_t bandwidth_down;
        /** @bandwidth_up: advertised uplink upload bandwidth */
        uint32_t bandwidth_up;
        /** @refcount: number of contexts the object is used */
        int refcount;
};

/**
 * struct batadv_neigh_ifinfo_bat_iv - neighbor information per outgoing
 *  interface for B.A.T.M.A.N. IV
 */
class batadv_neigh_ifinfo_bat_iv {
    public:
        /** @tq_recv: ring buffer of received TQ values from this neigh node */
        uint8_t tq_recv[BATADV_TQ_GLOBAL_WINDOW_SIZE];
        /** @tq_index: ring buffer index */
        uint8_t tq_index;
        /**@tq_avg: averaged tq of all tq values in the ring buffer (tq_recv)*/
        uint8_t tq_avg;
        /** @real_packet_count: counted result of real_bits */
        uint8_t real_packet_count;
};

/**
 * struct batadv_neigh_ifinfo_bat_v - neighbor information per outgoing
 *  interface for B.A.T.M.A.N. V
 */
class batadv_neigh_ifinfo_bat_v {
    public:
        /**@throughput: last throughput metric received from originator via this neigh */
        uint32_t metricValue;
        /** @last_seqno: last sequence number known for this neighbor */
        uint32_t last_seqno;

        std::vector<uint32_t> lastMetricValues;

/* Old batadv_neigh_ifinfo_bat_iv data for using network coding in this simulation */
        /** @tq_recv: ring buffer of received TQ values from this neigh node */
        uint8_t metric_recv[BATADV_TQ_GLOBAL_WINDOW_SIZE];
        /** @tq_index: ring buffer index */
        uint8_t metric_index;
        /**@tq_avg: averaged tq of all tq values in the ring buffer (tq_recv)*/
        uint8_t metric_avg;
        /** @real_packet_count: counted result of real_bits */
        uint8_t real_packet_count;
};
typedef std::shared_ptr<batadv_neigh_ifinfo_bat_v> Neigh_ifinfo_bat_v_ptr;

/**
 * struct batadv_neigh_ifinfo - neighbor information per outgoing interface
 */
class batadv_neigh_ifinfo {
    public:
        /** @if_outgoing: pointer to outgoing hard-interface */
        batadv_hard_iface *if_outgoing;
        /** @bat_iv: B.A.T.M.A.N. IV private structure */
        //batadv_neigh_ifinfo_bat_iv bat_iv;
        /** @bat_v: B.A.T.M.A.N. V private data */
        Neigh_ifinfo_bat_v_ptr bat_v;
        /** @last_ttl: last received ttl from this neigh node */
        uint8_t last_ttl;
        /** @refcount: number of contexts the object is used */
        int refcount;
};

/**
 * struct batadv_priv_tt - per mesh interface translation table data
 */
class batadv_priv_tt {
    public:
        /** @vn: translation table version number */typedef std::map<MACAddress, Neigh_node_ptr> Neighbor_Table;
        int vn;
        /** @ogm_append_cnt: counter of number of OGMs containing the local tt diff */
        int ogm_append_cnt;
        /** @local_changes: changes registered in an originator interval */
        int local_changes;
        /**@changes_list: tracks tt local changes within an originator interval */
        tt_changeList changes_list;
        /** @req_list: list of pending & unanswered tt_requests */
        tt_req_node_list req_list;
        /** @roam_list: list of the last roaming events of each client limiting
         *  the number of roaming events to avoid route flapping */
        tt_roam_node_list roam_list;
        /** @last_changeset: last tt changeset this host has generated */
        tt_changeList last_changeset;
        /** @last_changeset_len: length of last tt changeset this host has generated */
        int16_t last_changeset_len;

        bool vlan_expanded;
};

/**
 * struct batadv_priv_gw - per mesh interface gateway data
 */
class batadv_priv_gw {
    public:
        /** @curr_gw: pointer to currently selected gateway node */
        batadv_gw_node *curr_gw;
        /**@mode: gateway operation: off=0, client=1 or server=2 (see batadv_gw_modes) */
        int mode;
        /** @sel_class: gateway selection class (applies if gw_mode client) */
        int sel_class;
        /** @bandwidth_down: advertised uplink download bandwidth (if gw_mode server) */
        double bandwidth_down;
        /** @bandwidth_up: advertised uplink upload bandwidth (if gw_mode server) */
        double bandwidth_up;
        /** @reselect: bool indicating a gateway re-selection is in progress */
        bool reselect;
};

/**
 * struct batadv_priv_tvlv - per mesh interface tvlv data
 */
class batadv_priv_tvlv {
    public:
        /** @container_list: list of registered tvlv containers to be sent with each OGM */
        Container_list container_list;
        Handler_list handler_list;
};

/**
 * struct batadv_priv_nc - per mesh interface network coding private data
 */
class batadv_priv_nc {
    public:
        /** @min_tq: only consider neighbors for encoding if neigh_tq > min_tq */
        uint8_t min_tq;
        /** @max_fwd_delay: maximum packet forward delay to allow coding of packets */
        uint32_t max_fwd_delay;
        /** @max_buffer_time: buffer time for sniffed packets used to decoding */
        uint32_t max_buffer_time;
        /** @timestamp_fwd_flush: timestamp of last forward packet queue flush */
        simtime_t timestamp_fwd_flush;
        /** @timestamp_sniffed_purge: timestamp of last sniffed packet queue purge */
        simtime_t timestamp_sniffed_purge;
        /**
         * @coding_hash: Hash table used to buffer skbs while waiting for
         *  another incoming skb to code it with. Skbs are added to the buffer
         *  just before being forwarded in routing.c
         */
        NC_Buffer *coding_hash;
        /**
         * @decoding_hash: Hash table used to buffer skbs that might be needed
         *  to decode a received coded skb. The buffer is used for 1) skbs
         *  arriving on the soft-interface; 2) skbs overheard on the
         *  hard-interface; and 3) skbs forwarded by batman-adv.
         */
        NC_Buffer *decoding_hash;
};


/**
 * struct batadv_tp_unacked - unacked packet meta-information
 *
 * This struct is supposed to represent a buffer unacked packet. However, since
 * the purpose of the TP meter is to count the traffic only, there is no need to
 * store the entire sk_buff, the starting offset and the length are enough
 */
struct batadv_tp_unacked {
    /** @seqno: seqno of the unacked packet */
    uint32_t seqno;
    /** @len: length of the packet */
    uint16_t len;
};

/**
 * enum batadv_tp_meter_role - Modus in tp meter session
 */
enum batadv_tp_meter_role {
    /** @BATADV_TP_RECEIVER: Initialized as receiver */
    BATADV_TP_RECEIVER,
    /** @BATADV_TP_SENDER: Initialized as sender */
    BATADV_TP_SENDER
};

/*  enum batadv_tp_meter_reason - reason of a tp meter test run stop */
enum batadv_tp_meter_reason {
    BATADV_TP_REASON_COMPLETE           = 3,        //  @BATADV_TP_REASON_COMPLETE: sender finisbatadv_tvlv_containerhed tp run
    BATADV_TP_REASON_CANCEL             = 4,        //  @BATADV_TP_REASON_CANCEL: sender was stopped during run

    /* error status >= 128 */
    BATADV_TP_REASON_DST_UNREACHABLE    = 128,  //  @BATADV_TP_REASON_DST_UNREACHABLE: receiver could not be reached or didn't answer
    BATADV_TP_REASON_RESEND_LIMIT       = 129,  //  @BATADV_TP_REASON_RESEND_LIMIT: (unused) sender retry reached limit
    BATADV_TP_REASON_ALREADY_ONGOING    = 130,  //  @BATADV_TP_REASON_ALREADY_ONGOING: test to or from the same node already ongoing
    BATADV_TP_REASON_MEMORY_ERROR       = 131,  //  @BATADV_TP_REASON_MEMORY_ERROR: test was stopped due to low memory
    BATADV_TP_REASON_CANT_SEND          = 132,  //  @BATADV_TP_REASON_CANT_SEND: failed to send via outgoing interface
    BATADV_TP_REASON_TOO_MANY           = 133,  //  @BATADV_TP_REASON_TOO_MANY: too many ongoing sessions
};

/**
 * struct batadv_tp_vars - tp meter private variables per session
 */
struct batadv_tp_vars {
    /** @timer: timer for ack (receiver) and retry (sender) */
    simtime_t timer;
    /** @start_time: start time in jiffies */
    simtime_t start_time;
    /** @other_end: mac address of remote */
    MACAddress other_end;
    /** @role: receiver/sender modi */
    batadv_tp_meter_role role;
    /** @sending: sending binary semaphore: 1 if sending, 0 is not */
    int sending;
    /** @reason: reason for a stopped session */
    batadv_tp_meter_reason reason;
    /** @test_length: test length in milliseconds */
    simtime_t test_length;
    /** @session: TP session identifier */
    uint8_t session[2];
    /** @icmp_uid: local ICMP "socket" index */
    uint8_t icmp_uid;
    /* sender variables */
    /** @dec_cwnd: decimal part of the cwnd used during linear growth */
    uint16_t dec_cwnd;
    /** @cwnd: current size of the congestion window */
    uint32_t cwnd;
    /** @ss_threshold: Slow Start threshold. Once cwnd exceeds this value the
     *  connection switches to the Congestion Avoidance state */
    uint32_t ss_threshold;
    /** @last_acked: last acked byte */
    uint32_t last_acked;
    /** @last_sent: last sent byte, not yet acked */
    uint32_t last_sent;
    /** @tot_sent: amount of data sent/ACKed so far */
    uint32_t tot_sent;
    /** @dup_acks: duplicate ACKs counter */
    uint32_t dup_acks;
    /** @fast_recovery: true if in Fast Recovery mode */
    bool fast_recovery;
    /** @recover: last sent seqno when entering Fast Recovery */
    uint32_t recover;
    /** @rto: sender timeout */
    simtime_t rto;
    /** @srtt: smoothed RTT scaled by 2^3 */
    uint32_t srtt;
    /** @rttvar: RTT variation scaled by 2^2 */
    uint32_t rttvar;
    /** @prerandom_offset: offset inside the prerandom buffer */
    uint32_t prerandom_offset;
/* receiver variables */
    /** @last_recv: last in-order received packet */
    uint32_t last_recv;
    /** @last_recv_time: time time (jiffies) a msg was received */
    simtime_t last_recv_time;
    /** @refcount: number of context where the object is used */
    int refcount;
};

/**
 * struct batadv_priv_bat_v - B.A.T.M.A.N. V per soft-interface private data
 */
class batadv_priv_bat_v {
    public:
        /** @ogm_buff: buffer holding the OGM packet */
        OGMv2_buff ogm_buff;
        /** @ogm_buff_len: length of the OGM packet buffer */
        int ogm_buff_len;
        /** @ogm_seqno: OGM sequence number - used to identify each OGM */
        uint32_t ogm_seqno;
};

/**
 * struct batadv_tvlv_hdr - base tvlv header struct
 * @type: tvlv container type (see batadv_tvlv_type)
 * @version: tvlv container version
 * @len: tvlv container length
 */
class batadv_tvlv_hdr {
    public:
        uint8_t type;
        uint8_t version;
        uint16_t len;
};

/**
 * struct batadv_tvlv_container - container for tvlv appended to OGMs
 */
class batadv_tvlv_container {
    public:
        batadv_tvlv_hdr *tvlv_hdr;

};


/**
 * struct batadv_tt_change_node - structure for tt changes occurred
 */
class batadv_tt_change_node {
    public:
        /** @change: holds the actual translation table diff data */
        batadv_tvlv_tt_change *change; // without * there is an error
};

class batadv_tvlv_handler {
    public:
        /* @ogm_handler: handler callback which is given the tvlv payload to process on incoming OGM packets */
        void (Batman5Routing::*ogm_handler)(Orig_node_ptr, uint8_t, void*, uint16_t);

        /* @unicast_handler: handler callback which is given the tvlv payload to process on incoming unicast tvlv packets */
        int (Batman5Routing::*unicast_handler)(MACAddress src, MACAddress dst, void *tvlv_value, uint16_t tvlv_value_len);

        /** @type: tvlv type this handler feels responsible for */
        uint8_t type;

        /** @version: tvlv version this handler feels responsible for */
        uint8_t version;

        /** @flags: tvlv handler flags */
        uint8_t flags;

};

/**
 * struct batadv_tt_req_node - data to keep track of the tt requests in flight
 */
class batadv_tt_req_node {
    public:
        /*@addr: mac address address of the originator this request was sent to */
        MACAddress addr;
        /** @issued_at: timestamp used for purging stale tt requests */
        simtime_t issued_at;
};


/**
 * struct batadv_tt_roam_node - roaming client data
 */
class batadv_tt_roam_node {
    public:
        /** @addr: mac address of the client in the roaming phase */
        MACAddress addr;
        /** @counter: number of allowed roaming events per client within a single
         * OGM interval (changes are committed with each OGM) */
        unsigned int counter;
        /*@first_time: timestamp used for purging stale roaming node entries */
        simtime_t first_time;
};

class batadv_nc_node {
    public:
        /** @addr: the node's mac address */
        MACAddress addr;
        /** @refcount: number of contexts the object is used by */
        int refcount;
        /** @orig_node: pointer to corresponding orig node struct */
        Orig_node_ptr orig_node;
        /** @last_seen: timestamp of last ogm received from this node */
        simtime_t last_seen;
};

/**
 * struct batadv_nc_path - network coding path
 */
class batadv_nc_path {
    public:
        /** @packet_list: list of buffered packets for this path */
        nc_packet_list packet_list_coding;
        nc_packet_list packet_list_decoding;
        /** @next_hop: next hop (destination) of path */
        MACAddress next_hop;
        /** @prev_hop: previous hop (source) of path */
        MACAddress prev_hop;
        /** @last_valid: timestamp for last validation of path */
        simtime_t last_valid;
};

class batadv_dat_candidate {
    public:
        int type; // BATADV_DAT_CANDIDATE_NOT_FOUND or BATADV_DAT_CANDIDATE_ORIG
        Orig_node_ptr orig_node;
};

/**
 ********************   Packets/Messages    ********************
**/

class INET_API elp_reminder : public ::omnetpp::cPacket {
    public:
        batadv_hard_iface *hard_iface;
        int iface_id;
};

class INET_API batadv_forw_packet : public ::omnetpp::cPacket {
    public:
        simtime_t send_time; // execution time for delayed_work (packet sending)
        bool own; // bool for locally generated packets (local OGMs are re-scheduled after sending)
        uint16_t packet_len; //size of aggregated OGM packet
        uint32_t direct_link_flags; // direct link flags for aggregated OGM packets
        uint8_t num_packets; // counter for aggregated OGMv1 packets
        batadv_hard_iface *if_incoming; // pointer to incoming hard-iface or primary iface if locally generated packet
        batadv_hard_iface *if_outgoing; // packet where the packet should be sent to, or NULL if unspecified
};




/**
 ********************   batman_adv.h    ********************
**/
#define BATADV_NL_NAME "batadv"
#define BATADV_NL_MCAST_GROUP_TPMETER "tpmeter"

/**
 * enum batadv_tt_client_flags - TT client specific flags
 *
 * Bits from 0 to 7 are called _remote flags_ because they are sent on the wire.
 * Bits from 8 to 15 are called _local flags_ because they are used for local
 * computations only.
 * Bits from 4 to 7 - a subset of remote flags - are ensured to be in sync with
 * the other nodes in the network. To achieve this goal these flags are included
 * in the TT CRC computation.
 */
enum batadv_tt_client_flags {
    BATADV_TT_CLIENT_DEL     = BIT0,        //  @BATADV_TT_CLIENT_DEL: the client has to be deleted from the table
    BATADV_TT_CLIENT_ROAM    = BIT1,        //  @BATADV_TT_CLIENT_ROAM: the client roamed to/from another node and
                                            //      the new update telling its new real location has not been received/sent yet
    BATADV_TT_CLIENT_WIFI    = BIT4,        //  @BATADV_TT_CLIENT_WIFI: this client is connected through a wifi interface.
                                            //      This information is used by the "AP Isolation" feature
    BATADV_TT_CLIENT_ISOLA   = BIT5,        //  @BATADV_TT_CLIENT_ISOLA: this client is considered "isolated". This
                                            //      information is used by the Extended Isolation feature
    BATADV_TT_CLIENT_NOPURGE = BIT8,        //  @BATADV_TT_CLIENT_NOPURGE: this client should never be removed from the table
    BATADV_TT_CLIENT_NEW     = BIT9,        //  @BATADV_TT_CLIENT_NEW: this client has been added to the local table but has
                                            //      not been announced yet
    BATADV_TT_CLIENT_PENDING = BIT10,       //  @BATADV_TT_CLIENT_PENDING: this client is marked for removal but it is kept
                                            //      in the table for one more originator interval for consistency purposes
    BATADV_TT_CLIENT_TEMP    = BIT11,       //  @BATADV_TT_CLIENT_TEMP: this global client has been detected to be part of
                                            //      the network but no nnode has already announced it
};

/**
 * enum batadv_nl_attrs - batman-adv netlink attributes
 */
enum batadv_nl_attrs {
    BATADV_ATTR_UNSPEC,             //  @BATADV_ATTR_UNSPEC: unspecified attribute to catch errors
    BATADV_ATTR_VERSION,            //  @BATADV_ATTR_VERSION: batman-adv version string
    BATADV_ATTR_ALGO_NAME,          //  @BATADV_ATTR_ALGO_NAME: name of routing algorithm
    BATADV_ATTR_MESH_IFINDEX,       //  @BATADV_ATTR_MESH_IFINDEX: index of the batman-adv interface
    BATADV_ATTR_MESH_IFNAME,        //  @BATADV_ATTR_MESH_IFNAME: name of the batman-adv interface
    BATADV_ATTR_MESH_ADDRESS,       //  @BATADV_ATTR_MESH_ADDRESS: mac address of the batman-adv interface
    BATADV_ATTR_HARD_IFINDEX,       //  @BATADV_ATTR_HARD_IFINDEX: index of the non-batman-adv interface
    BATADV_ATTR_HARD_IFNAME,        //  @BATADV_ATTR_HARD_IFNAME: name of the non-batman-adv interface
    BATADV_ATTR_HARD_ADDRESS,       //  @BATADV_ATTR_HARD_ADDRESS: mac address of the non-batman-adv interface
    BATADV_ATTR_ORIG_ADDRESS,       //  @BATADV_ATTR_ORIG_ADDRESS: originator mac address
    BATADV_ATTR_TPMETER_RESULT,     //  @BATADV_ATTR_TPMETER_RESULT: result of run (see batadv_tp_meter_status)
    BATADV_ATTR_TPMETER_TEST_TIME,  //  @BATADV_ATTR_TPMETER_TEST_TIME: time (msec) the run took
    BATADV_ATTR_TPMETER_BYTES,      //  @BATADV_ATTR_TPMETER_BYTES: amount of acked bytes during run
    BATADV_ATTR_TPMETER_COOKIE,     //  @BATADV_ATTR_TPMETER_COOKIE: session cookie to match tp_meter session
    BATADV_ATTR_PAD,                //  @BATADV_ATTR_PAD: attribute used for padding for 64-bit alignment
    BATADV_ATTR_ACTIVE,             //  @BATADV_ATTR_ACTIVE: Flag indicating if the hard interface is active
    BATADV_ATTR_TT_ADDRESS,         //  @BATADV_ATTR_TT_ADDRESS: Client MAC address
    BATADV_ATTR_TT_TTVN,            //  @BATADV_ATTR_TT_TTVN: Translation table version
    BATADV_ATTR_TT_LAST_TTVN,       //  @BATADV_ATTR_TT_LAST_TTVN: Previous translation table version
    BATADV_ATTR_TT_CRC32,           //  @BATADV_ATTR_TT_CRC32: CRC32 over translation table
    BATADV_ATTR_TT_VID,             //  @BATADV_ATTR_TT_VID: VLAN ID
    BATADV_ATTR_TT_FLAGS,           //  @BATADV_ATTR_TT_FLAGS: Translation table client flags
    BATADV_ATTR_FLAG_BEST,          //  @BATADV_ATTR_FLAG_BEST: Flags indicating entry is the best
    BATADV_ATTR_LAST_SEEN_MSECS,    //  @BATADV_ATTR_LAST_SEEN_MSECS: Time in milliseconds since last seen
    BATADV_ATTR_NEIGH_ADDRESS,      //  @BATADV_ATTR_NEIGH_ADDRESS: Neighbour MAC address
    BATADV_ATTR_TQ,                 //  @BATADV_ATTR_TQ: TQ to neighbour
    BATADV_ATTR_THROUGHPUT,         //  @BATADV_ATTR_THROUGHPUT: Estimated throughput to Neighbour
    BATADV_ATTR_BANDWIDTH_UP,       //  @BATADV_ATTR_BANDWIDTH_UP: Reported uplink bandwidth
    BATADV_ATTR_BANDWIDTH_DOWN,     //  @BATADV_ATTR_BANDWIDTH_DOWN: Reported downlink bandwidth
    BATADV_ATTR_ROUTER,             //  @BATADV_ATTR_ROUTER: Gateway router MAC address
    BATADV_ATTR_BLA_OWN,            //  @BATADV_ATTR_BLA_OWN: Flag indicating own originator
    BATADV_ATTR_BLA_ADDRESS,        //  @BATADV_ATTR_BLA_ADDRESS: Bridge loop avoidance claim MAC address
    BATADV_ATTR_BLA_VID,            //  @BATADV_ATTR_BLA_VID: BLA VLAN ID
    BATADV_ATTR_BLA_BACKBONE,       //  @BATADV_ATTR_BLA_BACKBONE: BLA gateway originator MAC address
    BATADV_ATTR_BLA_CRC,            //  @BATADV_ATTR_BLA_CRC: BLA CRC

    /* add attributes above here, update the policy in netlink.c */

    __BATADV_ATTR_AFTER_LAST,                       //  @__BATADV_ATTR_AFTER_LAST: internal use
    NUM_BATADV_ATTR = __BATADV_ATTR_AFTER_LAST,     //  @NUM_BATADV_ATTR: total number of batadv_nl_attrs available
    BATADV_ATTR_MAX = __BATADV_ATTR_AFTER_LAST - 1  //  @BATADV_ATTR_MAX: highest attribute number currently defined
};

/*  enum batadv_nl_commands - supported batman-adv netlink commands  */
enum batadv_nl_commands {
    BATADV_CMD_UNSPEC,                  //  @BATADV_CMD_UNSPEC: unspecified command to catch errors
    BATADV_CMD_GET_MESH_INFO,           //  @BATADV_CMD_GET_MESH_INFO: Query basic information about batman-adv device
    BATADV_CMD_TP_METER,                //  @BATADV_CMD_TP_METER: Start a tp meter session
    BATADV_CMD_TP_METER_CANCEL,         //  @BATADV_CMD_TP_METER_CANCEL: Cancel a tp meter session
    BATADV_CMD_GET_ROUTING_ALGOS,       //  @BATADV_CMD_GET_ROUTING_ALGOS: Query the list of routing algorithms.
    BATADV_CMD_GET_HARDIFS,             //  @BATADV_CMD_GET_HARDIFS: Query list of hard interfaces
    BATADV_CMD_GET_TRANSTABLE_LOCAL,    //  @BATADV_CMD_GET_TRANSTABLE_LOCAL: Query list of local translations
    BATADV_CMD_GET_TRANSTABLE_GLOBAL,   //  @BATADV_CMD_GET_TRANSTABLE_GLOBAL: Query list of global translations
    BATADV_CMD_GET_ORIGINATORS,         //  @BATADV_CMD_GET_ORIGINATORS: Query list of originators
    BATADV_CMD_GET_NEIGHBORS,           //  @BATADV_CMD_GET_NEIGHBORS: Query list of neighbours
    BATADV_CMD_GET_GATEWAYS,            //  @BATADV_CMD_GET_GATEWAYS: Query list of gateways
    BATADV_CMD_GET_BLA_CLAIM,           //  @BATADV_CMD_GET_BLA_CLAIM: Query list of bridge loop avoidance claims
    BATADV_CMD_GET_BLA_BACKBONE,        //  @BATADV_CMD_GET_BLA_BACKBONE: Query list of bridge loop avoidance backbones

    /* add new commands above here */

    __BATADV_CMD_AFTER_LAST,                        //  @__BATADV_CMD_AFTER_LAST: internal use
    BATADV_CMD_MAX = __BATADV_CMD_AFTER_LAST - 1    //  @BATADV_CMD_MAX: highest used command number
};


class batadv_definitions {
public:
    batadv_definitions();
    virtual ~batadv_definitions();
};

} /* namespace inet */

#endif /* INET_BATMAN_ROUTING_BATADV_DEFINITIONS_H_ */
