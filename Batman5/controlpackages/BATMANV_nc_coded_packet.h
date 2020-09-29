#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#ifndef INET_BATMAN_ROUTING_BATMANV_NC_CODED_PACKET_M_H_
#define INET_BATMAN_ROUTING_BATMANV_NC_CODED_PACKET_M_H_

#include <omnetpp.h>

// dll export symbol
#ifndef INET_API
#  if defined(INET_EXPORT)
#    define INET_API  OPP_DLLEXPORT
#  elif defined(INET_IMPORT)
#    define INET_API  OPP_DLLIMPORT
#  else
#    define INET_API
#  endif
#endif

// cplusplus {{
#include "inet/common/INETDefs.h"
#include "inet/linklayer/common/MACAddress.h"
#include "../Batman5Files/batadv_definitions.h"

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

#define BATADV_COMPAT_VERSION 15
#define BATADV_TT_DATA_TYPE_MASK 0x0F
#define BATADV_OGM_HLEN 19
#define BATADV_OGM2_HLEN 20
#define BATADV_ELP_HLEN 16
#define BATADV_RR_LEN 16

#define BATADV_UNICAST_HLEN 10
#define BATADV_UNICAST_4ADDR_HLEN 18
#define BATADV_BCAST_HLEN 14
#define BATADV_CODED_HLEN 46
// }}

namespace inet {


class INET_API batadv_nc_packet //: public ::omnetpp::cPacket
{
    protected:
        uint64_t packet_id; //crc32 checksum
        simtime_t timestamp;
        Neigh_node_ptr neigh_node;
        batadv_unicast_packet *uni_packet;
        bool deleteUniPacket;
        NC_path_ptr nc_path; //pointer to path this nc packet is attached to

    private:
        void copy(const batadv_nc_packet& other);

      protected:
        // protected and unimplemented operator==(), to prevent accidental usage
        bool operator==(const batadv_nc_packet&);

      public:
        batadv_nc_packet(const char *name=nullptr);
        batadv_nc_packet(const batadv_nc_packet& other);
        virtual ~batadv_nc_packet();
        batadv_nc_packet& operator=(const batadv_nc_packet& other);
//        virtual batadv_nc_packet *dup() const override {return new batadv_nc_packet(*this);}
//        virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
//        virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

        // field getter/setter methods
        virtual uint32_t getPacket_id();
        virtual void setPacket_id(uint32_t packet_id);

        virtual bool getDeleteUniPacket();
        virtual void setDeleteUniPacket(bool deletePacket);

        virtual simtime_t getTimestamp();
        virtual void setTimestamp(simtime_t timestamp);

        virtual Neigh_node_ptr getNeigh_node();
        virtual void setNeigh_node(Neigh_node_ptr neigh_node);

        virtual batadv_unicast_packet *getUni_packet();
        virtual void setUni_packet(batadv_unicast_packet *uni_packet);

        virtual NC_path_ptr getNc_path();
        virtual void setNc_path(NC_path_ptr nc_path);

};
//inline void doParsimPacking(omnetpp::cCommBuffer *b, const batadv_nc_packet& obj) {obj.parsimPack(b);}
//inline void doParsimUnpacking(omnetpp::cCommBuffer *b, batadv_nc_packet& obj) {obj.parsimUnpack(b);}

class INET_API batadv_coded_packet : public ::omnetpp::cPacket
{
  protected:
    uint8_t batadv_packet_type = BATADV_CODED;
    uint8_t batadv_version = BATADV_COMPAT_VERSION;
    uint8_t ttl;
    uint8_t first_ttvn;
    MACAddress first_source;
    MACAddress first_orig_dest;
    uint32_t first_crc;

    MACAddress second_dest;
    MACAddress second_source;
    MACAddress second_orig_dest;
    uint32_t second_crc;
    uint8_t second_ttl;
    uint8_t second_ttvn;

    batadv_unicast_packet *packet1;
    batadv_unicast_packet *packet2;
    uint32_t coded_len;

  private:
    void copy(const batadv_coded_packet& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const batadv_coded_packet&);

  public:
    batadv_coded_packet(const char *name=nullptr);
    batadv_coded_packet(const batadv_coded_packet& other);
    virtual ~batadv_coded_packet();
    batadv_coded_packet& operator=(const batadv_coded_packet& other);
    virtual batadv_coded_packet *dup() const override {return new batadv_coded_packet(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual uint8_t getBatadv_packet_type();
    virtual void setBatadv_packet_type(uint8_t batadv_packet_type);
    virtual uint8_t getBatadv_version();
    virtual void setBatadv_version(uint8_t batadv_version);

    virtual uint8_t getTtl();
    virtual void setTtl(uint8_t ttl);
    virtual uint8_t getFirst_ttvn();
    virtual void setFirst_ttvn(uint8_t first_ttvn);

    virtual MACAddress& getFirst_source();
    virtual const MACAddress& getFirst_source() const {return const_cast<batadv_coded_packet*>(this)->getFirst_source();}
    virtual void setFirst_source(const MACAddress& first_source);

    virtual MACAddress& getFirst_orig_dest();
    virtual const MACAddress& getFirst_orig_dest() const {return const_cast<batadv_coded_packet*>(this)->getFirst_orig_dest();}
    virtual void setFirst_orig_dest(const MACAddress& first_orig_dest);

    virtual MACAddress& getSecond_source();
    virtual const MACAddress& getSecond_source() const {return const_cast<batadv_coded_packet*>(this)->getSecond_source();}
    virtual void setSecond_source(const MACAddress& second_source);

    virtual MACAddress& getSecond_dest();
    virtual const MACAddress& getSecond_dest() const {return const_cast<batadv_coded_packet*>(this)->getSecond_dest();}
    virtual void setSecond_dest(const MACAddress& second_dest);

    virtual MACAddress& getSecond_orig_dest();
    virtual const MACAddress& getSecond_orig_dest() const {return const_cast<batadv_coded_packet*>(this)->getSecond_orig_dest();}
    virtual void setSecond_orig_dest(const MACAddress& second_orig_dest);

    virtual uint8_t getSecond_ttl();
    virtual void setSecond_ttl(uint8_t second_ttl);
    virtual uint8_t getSecond_ttvn();
    virtual void setSecond_ttvn(uint8_t second_ttvn);

    virtual uint32_t getFirst_crc() const;
    virtual void setFirst_crc(uint32_t first_crc);
    virtual uint32_t getSecond_crc() const;
    virtual void setSecond_crc(uint32_t second_crc);

    virtual batadv_unicast_packet *getPacket1();
    virtual void setPacket1(batadv_unicast_packet *packet1);
    virtual batadv_unicast_packet *getPacket2();
    virtual void setPacket2(batadv_unicast_packet *packet2);

    virtual uint32_t getCoded_len() const;
    virtual void setCoded_len(uint32_t coded_len);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const batadv_coded_packet& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, batadv_coded_packet& obj) {obj.parsimUnpack(b);}



} /* namespace inet */

#endif /* INET_BATMAN_ROUTING_BATMANV_NC_CODED_PACKET_M_H_ */
