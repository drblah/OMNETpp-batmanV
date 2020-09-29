//
// Generated file, do not edit! Created by nedtool 5.6 from inet/Batman5/controlpackages/BATMANV_ControlPackets.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include "BATMANV_ControlPackets_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

namespace inet {

// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec);

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// operator<< for std::vector<T>
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

EXECUTE_ON_STARTUP(
    omnetpp::cEnum *e = omnetpp::cEnum::find("inet::batadv_packettype");
    if (!e) omnetpp::enums.getInstance()->add(e = new omnetpp::cEnum("inet::batadv_packettype"));
    e->insert(BATADV_IV_OGM, "BATADV_IV_OGM");
    e->insert(BATADV_BCAST, "BATADV_BCAST");
    e->insert(BATADV_CODED, "BATADV_CODED");
    e->insert(BATADV_ELP, "BATADV_ELP");
    e->insert(BATADV_OGM2, "BATADV_OGM2");
    e->insert(BATADV_UNICAST, "BATADV_UNICAST");
    e->insert(BATADV_UNICAST_FRAG, "BATADV_UNICAST_FRAG");
    e->insert(BATADV_UNICAST_4ADDR, "BATADV_UNICAST_4ADDR");
    e->insert(BATADV_ICMP, "BATADV_ICMP");
    e->insert(BATADV_UNICAST_TVLV, "BATADV_UNICAST_TVLV");
)

EXECUTE_ON_STARTUP(
    omnetpp::cEnum *e = omnetpp::cEnum::find("inet::batadv_version");
    if (!e) omnetpp::enums.getInstance()->add(e = new omnetpp::cEnum("inet::batadv_version"));
    e->insert(BATMAN_IV, "BATMAN_IV");
    e->insert(BATMAN_V, "BATMAN_V");
)

EXECUTE_ON_STARTUP(
    omnetpp::cEnum *e = omnetpp::cEnum::find("inet::batadv_subtype");
    if (!e) omnetpp::enums.getInstance()->add(e = new omnetpp::cEnum("inet::batadv_subtype"));
    e->insert(BATADV_P_DATA, "BATADV_P_DATA");
    e->insert(BATADV_P_DAT_DHT_GET, "BATADV_P_DAT_DHT_GET");
    e->insert(BATADV_P_DAT_DHT_PUT, "BATADV_P_DAT_DHT_PUT");
    e->insert(BATADV_P_DAT_CACHE_REPLY, "BATADV_P_DAT_CACHE_REPLY");
)

EXECUTE_ON_STARTUP(
    omnetpp::cEnum *e = omnetpp::cEnum::find("inet::batadv_iv_flags");
    if (!e) omnetpp::enums.getInstance()->add(e = new omnetpp::cEnum("inet::batadv_iv_flags"));
    e->insert(BATADV_NOT_BEST_NEXT_HOP, "BATADV_NOT_BEST_NEXT_HOP");
    e->insert(BATADV_PRIMARIES_FIRST_HOP, "BATADV_PRIMARIES_FIRST_HOP");
    e->insert(BATADV_DIRECTLINK, "BATADV_DIRECTLINK");
)

EXECUTE_ON_STARTUP(
    omnetpp::cEnum *e = omnetpp::cEnum::find("inet::batadv_icmp_messagetype");
    if (!e) omnetpp::enums.getInstance()->add(e = new omnetpp::cEnum("inet::batadv_icmp_messagetype"));
    e->insert(BATADV_ECHO_REPLY, "BATADV_ECHO_REPLY");
    e->insert(BATADV_DESTINATION_UNREACHABLE, "BATADV_DESTINATION_UNREACHABLE");
    e->insert(BATADV_ECHO_REQUEST, "BATADV_ECHO_REQUEST");
    e->insert(BATADV_TTL_EXCEEDED, "BATADV_TTL_EXCEEDED");
    e->insert(BATADV_PARAMETER_PROBLEM, "BATADV_PARAMETER_PROBLEM");
    e->insert(BATADV_TP, "BATADV_TP");
)

EXECUTE_ON_STARTUP(
    omnetpp::cEnum *e = omnetpp::cEnum::find("inet::batadv_icmp_tp_subtype");
    if (!e) omnetpp::enums.getInstance()->add(e = new omnetpp::cEnum("inet::batadv_icmp_tp_subtype"));
    e->insert(BATADV_TP_MSG, "BATADV_TP_MSG");
    e->insert(BATADV_TP_ACK, "BATADV_TP_ACK");
)

EXECUTE_ON_STARTUP(
    omnetpp::cEnum *e = omnetpp::cEnum::find("inet::batadv_mcast_flags");
    if (!e) omnetpp::enums.getInstance()->add(e = new omnetpp::cEnum("inet::batadv_mcast_flags"));
    e->insert(BATADV_MCAST_WANT_ALL_UNSNOOPABLES, "BATADV_MCAST_WANT_ALL_UNSNOOPABLES");
    e->insert(BATADV_MCAST_WANT_ALL_IPV4, "BATADV_MCAST_WANT_ALL_IPV4");
    e->insert(BATADV_MCAST_WANT_ALL_IPV6, "BATADV_MCAST_WANT_ALL_IPV6");
)

EXECUTE_ON_STARTUP(
    omnetpp::cEnum *e = omnetpp::cEnum::find("inet::batadv_tt_data_flags");
    if (!e) omnetpp::enums.getInstance()->add(e = new omnetpp::cEnum("inet::batadv_tt_data_flags"));
    e->insert(BATADV_TT_OGM_DIFF, "BATADV_TT_OGM_DIFF");
    e->insert(BATADV_TT_REQUEST, "BATADV_TT_REQUEST");
    e->insert(BATADV_TT_RESPONSE, "BATADV_TT_RESPONSE");
    e->insert(BATADV_TT_FULL_TABLE, "BATADV_TT_FULL_TABLE");
)

EXECUTE_ON_STARTUP(
    omnetpp::cEnum *e = omnetpp::cEnum::find("inet::batadv_vlan_flags");
    if (!e) omnetpp::enums.getInstance()->add(e = new omnetpp::cEnum("inet::batadv_vlan_flags"));
    e->insert(BATADV_VLAN_HAS_TAG, "BATADV_VLAN_HAS_TAG");
)

EXECUTE_ON_STARTUP(
    omnetpp::cEnum *e = omnetpp::cEnum::find("inet::batadv_bla_claimframe");
    if (!e) omnetpp::enums.getInstance()->add(e = new omnetpp::cEnum("inet::batadv_bla_claimframe"));
    e->insert(BATADV_CLAIM_TYPE_CLAIM, "BATADV_CLAIM_TYPE_CLAIM");
    e->insert(BATADV_CLAIM_TYPE_UNCLAIM, "BATADV_CLAIM_TYPE_UNCLAIM");
    e->insert(BATADV_CLAIM_TYPE_ANNOUNCE, "BATADV_CLAIM_TYPE_ANNOUNCE");
    e->insert(BATADV_CLAIM_TYPE_REQUEST, "BATADV_CLAIM_TYPE_REQUEST");
    e->insert(BATADV_CLAIM_TYPE_LOOPDETECT, "BATADV_CLAIM_TYPE_LOOPDETECT");
)

EXECUTE_ON_STARTUP(
    omnetpp::cEnum *e = omnetpp::cEnum::find("inet::batadv_tvlv_type");
    if (!e) omnetpp::enums.getInstance()->add(e = new omnetpp::cEnum("inet::batadv_tvlv_type"));
    e->insert(BATADV_TVLV_GW, "BATADV_TVLV_GW");
    e->insert(BATADV_TVLV_DAT, "BATADV_TVLV_DAT");
    e->insert(BATADV_TVLV_NC, "BATADV_TVLV_NC");
    e->insert(BATADV_TVLV_TT, "BATADV_TVLV_TT");
    e->insert(BATADV_TVLV_ROAM, "BATADV_TVLV_ROAM");
    e->insert(BATADV_TVLV_MCAST, "BATADV_TVLV_MCAST");
)

Register_Class(BATMANV_ControlPackets)

BATMANV_ControlPackets::BATMANV_ControlPackets(const char *name, short kind) : ::omnetpp::cPacket(name,kind)
{
    this->batadv_packettype = 0;
    this->batadv_version = 0;
}

BATMANV_ControlPackets::BATMANV_ControlPackets(const BATMANV_ControlPackets& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

BATMANV_ControlPackets::~BATMANV_ControlPackets()
{
}

BATMANV_ControlPackets& BATMANV_ControlPackets::operator=(const BATMANV_ControlPackets& other)
{
    if (this==&other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void BATMANV_ControlPackets::copy(const BATMANV_ControlPackets& other)
{
    this->batadv_packettype = other.batadv_packettype;
    this->batadv_version = other.batadv_version;
}

void BATMANV_ControlPackets::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->batadv_packettype);
    doParsimPacking(b,this->batadv_version);
}

void BATMANV_ControlPackets::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->batadv_packettype);
    doParsimUnpacking(b,this->batadv_version);
}

uint8_t BATMANV_ControlPackets::getBatadv_packettype() const
{
    return this->batadv_packettype;
}

void BATMANV_ControlPackets::setBatadv_packettype(uint8_t batadv_packettype)
{
    this->batadv_packettype = batadv_packettype;
}

uint8_t BATMANV_ControlPackets::getBatadv_version() const
{
    return this->batadv_version;
}

void BATMANV_ControlPackets::setBatadv_version(uint8_t batadv_version)
{
    this->batadv_version = batadv_version;
}

class BATMANV_ControlPacketsDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    BATMANV_ControlPacketsDescriptor();
    virtual ~BATMANV_ControlPacketsDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(BATMANV_ControlPacketsDescriptor)

BATMANV_ControlPacketsDescriptor::BATMANV_ControlPacketsDescriptor() : omnetpp::cClassDescriptor("inet::BATMANV_ControlPackets", "omnetpp::cPacket")
{
    propertynames = nullptr;
}

BATMANV_ControlPacketsDescriptor::~BATMANV_ControlPacketsDescriptor()
{
    delete[] propertynames;
}

bool BATMANV_ControlPacketsDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<BATMANV_ControlPackets *>(obj)!=nullptr;
}

const char **BATMANV_ControlPacketsDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *BATMANV_ControlPacketsDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int BATMANV_ControlPacketsDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount() : 2;
}

unsigned int BATMANV_ControlPacketsDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *BATMANV_ControlPacketsDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "batadv_packettype",
        "batadv_version",
    };
    return (field>=0 && field<2) ? fieldNames[field] : nullptr;
}

int BATMANV_ControlPacketsDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='b' && strcmp(fieldName, "batadv_packettype")==0) return base+0;
    if (fieldName[0]=='b' && strcmp(fieldName, "batadv_version")==0) return base+1;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *BATMANV_ControlPacketsDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "uint8_t",
        "uint8_t",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : nullptr;
}

const char **BATMANV_ControlPacketsDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *BATMANV_ControlPacketsDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int BATMANV_ControlPacketsDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    BATMANV_ControlPackets *pp = (BATMANV_ControlPackets *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *BATMANV_ControlPacketsDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    BATMANV_ControlPackets *pp = (BATMANV_ControlPackets *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string BATMANV_ControlPacketsDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    BATMANV_ControlPackets *pp = (BATMANV_ControlPackets *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getBatadv_packettype());
        case 1: return ulong2string(pp->getBatadv_version());
        default: return "";
    }
}

bool BATMANV_ControlPacketsDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    BATMANV_ControlPackets *pp = (BATMANV_ControlPackets *)object; (void)pp;
    switch (field) {
        case 0: pp->setBatadv_packettype(string2ulong(value)); return true;
        case 1: pp->setBatadv_version(string2ulong(value)); return true;
        default: return false;
    }
}

const char *BATMANV_ControlPacketsDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *BATMANV_ControlPacketsDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    BATMANV_ControlPackets *pp = (BATMANV_ControlPackets *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(BATMANV_TVLVPackets)

BATMANV_TVLVPackets::BATMANV_TVLVPackets(const char *name, short kind) : ::omnetpp::cPacket(name,kind)
{
    this->batadv_tvlv_type = 0;
    this->version = 0;
    this->length = 0;
}

BATMANV_TVLVPackets::BATMANV_TVLVPackets(const BATMANV_TVLVPackets& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

BATMANV_TVLVPackets::~BATMANV_TVLVPackets()
{
}

BATMANV_TVLVPackets& BATMANV_TVLVPackets::operator=(const BATMANV_TVLVPackets& other)
{
    if (this==&other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void BATMANV_TVLVPackets::copy(const BATMANV_TVLVPackets& other)
{
    this->batadv_tvlv_type = other.batadv_tvlv_type;
    this->version = other.version;
    this->length = other.length;
}

void BATMANV_TVLVPackets::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->batadv_tvlv_type);
    doParsimPacking(b,this->version);
    doParsimPacking(b,this->length);
}

void BATMANV_TVLVPackets::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->batadv_tvlv_type);
    doParsimUnpacking(b,this->version);
    doParsimUnpacking(b,this->length);
}

uint8_t BATMANV_TVLVPackets::getBatadv_tvlv_type() const
{
    return this->batadv_tvlv_type;
}

void BATMANV_TVLVPackets::setBatadv_tvlv_type(uint8_t batadv_tvlv_type)
{
    this->batadv_tvlv_type = batadv_tvlv_type;
}

uint8_t BATMANV_TVLVPackets::getVersion() const
{
    return this->version;
}

void BATMANV_TVLVPackets::setVersion(uint8_t version)
{
    this->version = version;
}

uint16_t BATMANV_TVLVPackets::getLength() const
{
    return this->length;
}

void BATMANV_TVLVPackets::setLength(uint16_t length)
{
    this->length = length;
}

class BATMANV_TVLVPacketsDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    BATMANV_TVLVPacketsDescriptor();
    virtual ~BATMANV_TVLVPacketsDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(BATMANV_TVLVPacketsDescriptor)

BATMANV_TVLVPacketsDescriptor::BATMANV_TVLVPacketsDescriptor() : omnetpp::cClassDescriptor("inet::BATMANV_TVLVPackets", "omnetpp::cPacket")
{
    propertynames = nullptr;
}

BATMANV_TVLVPacketsDescriptor::~BATMANV_TVLVPacketsDescriptor()
{
    delete[] propertynames;
}

bool BATMANV_TVLVPacketsDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<BATMANV_TVLVPackets *>(obj)!=nullptr;
}

const char **BATMANV_TVLVPacketsDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *BATMANV_TVLVPacketsDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int BATMANV_TVLVPacketsDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount() : 3;
}

unsigned int BATMANV_TVLVPacketsDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *BATMANV_TVLVPacketsDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "batadv_tvlv_type",
        "version",
        "length",
    };
    return (field>=0 && field<3) ? fieldNames[field] : nullptr;
}

int BATMANV_TVLVPacketsDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='b' && strcmp(fieldName, "batadv_tvlv_type")==0) return base+0;
    if (fieldName[0]=='v' && strcmp(fieldName, "version")==0) return base+1;
    if (fieldName[0]=='l' && strcmp(fieldName, "length")==0) return base+2;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *BATMANV_TVLVPacketsDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "uint8_t",
        "uint8_t",
        "uint16_t",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : nullptr;
}

const char **BATMANV_TVLVPacketsDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *BATMANV_TVLVPacketsDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int BATMANV_TVLVPacketsDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    BATMANV_TVLVPackets *pp = (BATMANV_TVLVPackets *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *BATMANV_TVLVPacketsDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    BATMANV_TVLVPackets *pp = (BATMANV_TVLVPackets *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string BATMANV_TVLVPacketsDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    BATMANV_TVLVPackets *pp = (BATMANV_TVLVPackets *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getBatadv_tvlv_type());
        case 1: return ulong2string(pp->getVersion());
        case 2: return ulong2string(pp->getLength());
        default: return "";
    }
}

bool BATMANV_TVLVPacketsDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    BATMANV_TVLVPackets *pp = (BATMANV_TVLVPackets *)object; (void)pp;
    switch (field) {
        case 0: pp->setBatadv_tvlv_type(string2ulong(value)); return true;
        case 1: pp->setVersion(string2ulong(value)); return true;
        case 2: pp->setLength(string2ulong(value)); return true;
        default: return false;
    }
}

const char *BATMANV_TVLVPacketsDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *BATMANV_TVLVPacketsDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    BATMANV_TVLVPackets *pp = (BATMANV_TVLVPackets *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(batadv_tvlv_gateway_data)

batadv_tvlv_gateway_data::batadv_tvlv_gateway_data(const char *name, short kind) : ::inet::BATMANV_TVLVPackets(name,kind)
{
    this->setBatadv_tvlv_type(BATADV_TVLV_GW);
    this->setVersion(BATADV_COMPAT_VERSION);
    this->setLength(8);
    this->setByteLength(12);

    this->bandwidth_down = 0;
    this->bandwidth_up = 0;
}

batadv_tvlv_gateway_data::batadv_tvlv_gateway_data(const batadv_tvlv_gateway_data& other) : ::inet::BATMANV_TVLVPackets(other)
{
    copy(other);
}

batadv_tvlv_gateway_data::~batadv_tvlv_gateway_data()
{
}

batadv_tvlv_gateway_data& batadv_tvlv_gateway_data::operator=(const batadv_tvlv_gateway_data& other)
{
    if (this==&other) return *this;
    ::inet::BATMANV_TVLVPackets::operator=(other);
    copy(other);
    return *this;
}

void batadv_tvlv_gateway_data::copy(const batadv_tvlv_gateway_data& other)
{
    this->bandwidth_down = other.bandwidth_down;
    this->bandwidth_up = other.bandwidth_up;
}

void batadv_tvlv_gateway_data::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::BATMANV_TVLVPackets::parsimPack(b);
    doParsimPacking(b,this->bandwidth_down);
    doParsimPacking(b,this->bandwidth_up);
}

void batadv_tvlv_gateway_data::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::BATMANV_TVLVPackets::parsimUnpack(b);
    doParsimUnpacking(b,this->bandwidth_down);
    doParsimUnpacking(b,this->bandwidth_up);
}

uint32_t batadv_tvlv_gateway_data::getBandwidth_down() const
{
    return this->bandwidth_down;
}

void batadv_tvlv_gateway_data::setBandwidth_down(uint32_t bandwidth_down)
{
    this->bandwidth_down = bandwidth_down;
}

uint32_t batadv_tvlv_gateway_data::getBandwidth_up() const
{
    return this->bandwidth_up;
}

void batadv_tvlv_gateway_data::setBandwidth_up(uint32_t bandwidth_up)
{
    this->bandwidth_up = bandwidth_up;
}

class batadv_tvlv_gateway_dataDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    batadv_tvlv_gateway_dataDescriptor();
    virtual ~batadv_tvlv_gateway_dataDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(batadv_tvlv_gateway_dataDescriptor)

batadv_tvlv_gateway_dataDescriptor::batadv_tvlv_gateway_dataDescriptor() : omnetpp::cClassDescriptor("inet::batadv_tvlv_gateway_data", "inet::BATMANV_TVLVPackets")
{
    propertynames = nullptr;
}

batadv_tvlv_gateway_dataDescriptor::~batadv_tvlv_gateway_dataDescriptor()
{
    delete[] propertynames;
}

bool batadv_tvlv_gateway_dataDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<batadv_tvlv_gateway_data *>(obj)!=nullptr;
}

const char **batadv_tvlv_gateway_dataDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *batadv_tvlv_gateway_dataDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int batadv_tvlv_gateway_dataDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount() : 2;
}

unsigned int batadv_tvlv_gateway_dataDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *batadv_tvlv_gateway_dataDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "bandwidth_down",
        "bandwidth_up",
    };
    return (field>=0 && field<2) ? fieldNames[field] : nullptr;
}

int batadv_tvlv_gateway_dataDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='b' && strcmp(fieldName, "bandwidth_down")==0) return base+0;
    if (fieldName[0]=='b' && strcmp(fieldName, "bandwidth_up")==0) return base+1;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *batadv_tvlv_gateway_dataDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "uint32_t",
        "uint32_t",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : nullptr;
}

const char **batadv_tvlv_gateway_dataDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *batadv_tvlv_gateway_dataDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int batadv_tvlv_gateway_dataDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_gateway_data *pp = (batadv_tvlv_gateway_data *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *batadv_tvlv_gateway_dataDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_gateway_data *pp = (batadv_tvlv_gateway_data *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string batadv_tvlv_gateway_dataDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_gateway_data *pp = (batadv_tvlv_gateway_data *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getBandwidth_down());
        case 1: return ulong2string(pp->getBandwidth_up());
        default: return "";
    }
}

bool batadv_tvlv_gateway_dataDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_gateway_data *pp = (batadv_tvlv_gateway_data *)object; (void)pp;
    switch (field) {
        case 0: pp->setBandwidth_down(string2ulong(value)); return true;
        case 1: pp->setBandwidth_up(string2ulong(value)); return true;
        default: return false;
    }
}

const char *batadv_tvlv_gateway_dataDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *batadv_tvlv_gateway_dataDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_gateway_data *pp = (batadv_tvlv_gateway_data *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(batadv_tvlv_dat)

batadv_tvlv_dat::batadv_tvlv_dat(const char *name, short kind) : ::inet::BATMANV_TVLVPackets(name,kind)
{
    this->setBatadv_tvlv_type(BATADV_TVLV_DAT);
    this->setVersion(BATADV_COMPAT_VERSION);
    this->setLength(0);
    this->setByteLength(4);
}

batadv_tvlv_dat::batadv_tvlv_dat(const batadv_tvlv_dat& other) : ::inet::BATMANV_TVLVPackets(other)
{
    copy(other);
}

batadv_tvlv_dat::~batadv_tvlv_dat()
{
}

batadv_tvlv_dat& batadv_tvlv_dat::operator=(const batadv_tvlv_dat& other)
{
    if (this==&other) return *this;
    ::inet::BATMANV_TVLVPackets::operator=(other);
    copy(other);
    return *this;
}

void batadv_tvlv_dat::copy(const batadv_tvlv_dat& other)
{
}

void batadv_tvlv_dat::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::BATMANV_TVLVPackets::parsimPack(b);
}

void batadv_tvlv_dat::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::BATMANV_TVLVPackets::parsimUnpack(b);
}

class batadv_tvlv_datDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    batadv_tvlv_datDescriptor();
    virtual ~batadv_tvlv_datDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(batadv_tvlv_datDescriptor)

batadv_tvlv_datDescriptor::batadv_tvlv_datDescriptor() : omnetpp::cClassDescriptor("inet::batadv_tvlv_dat", "inet::BATMANV_TVLVPackets")
{
    propertynames = nullptr;
}

batadv_tvlv_datDescriptor::~batadv_tvlv_datDescriptor()
{
    delete[] propertynames;
}

bool batadv_tvlv_datDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<batadv_tvlv_dat *>(obj)!=nullptr;
}

const char **batadv_tvlv_datDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *batadv_tvlv_datDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int batadv_tvlv_datDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 0+basedesc->getFieldCount() : 0;
}

unsigned int batadv_tvlv_datDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    return 0;
}

const char *batadv_tvlv_datDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    return nullptr;
}

int batadv_tvlv_datDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *batadv_tvlv_datDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    return nullptr;
}

const char **batadv_tvlv_datDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *batadv_tvlv_datDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int batadv_tvlv_datDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_dat *pp = (batadv_tvlv_dat *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *batadv_tvlv_datDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_dat *pp = (batadv_tvlv_dat *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string batadv_tvlv_datDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_dat *pp = (batadv_tvlv_dat *)object; (void)pp;
    switch (field) {
        default: return "";
    }
}

bool batadv_tvlv_datDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_dat *pp = (batadv_tvlv_dat *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *batadv_tvlv_datDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    return nullptr;
}

void *batadv_tvlv_datDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_dat *pp = (batadv_tvlv_dat *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(batadv_tvlv_nc)

batadv_tvlv_nc::batadv_tvlv_nc(const char *name, short kind) : ::inet::BATMANV_TVLVPackets(name,kind)
{
    this->setBatadv_tvlv_type(BATADV_TVLV_NC);
    this->setVersion(BATADV_COMPAT_VERSION);
    this->setLength(0);
    this->setByteLength(4);
}

batadv_tvlv_nc::batadv_tvlv_nc(const batadv_tvlv_nc& other) : ::inet::BATMANV_TVLVPackets(other)
{
    copy(other);
}

batadv_tvlv_nc::~batadv_tvlv_nc()
{
}

batadv_tvlv_nc& batadv_tvlv_nc::operator=(const batadv_tvlv_nc& other)
{
    if (this==&other) return *this;
    ::inet::BATMANV_TVLVPackets::operator=(other);
    copy(other);
    return *this;
}

void batadv_tvlv_nc::copy(const batadv_tvlv_nc& other)
{
}

void batadv_tvlv_nc::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::BATMANV_TVLVPackets::parsimPack(b);
}

void batadv_tvlv_nc::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::BATMANV_TVLVPackets::parsimUnpack(b);
}

class batadv_tvlv_ncDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    batadv_tvlv_ncDescriptor();
    virtual ~batadv_tvlv_ncDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(batadv_tvlv_ncDescriptor)

batadv_tvlv_ncDescriptor::batadv_tvlv_ncDescriptor() : omnetpp::cClassDescriptor("inet::batadv_tvlv_nc", "inet::BATMANV_TVLVPackets")
{
    propertynames = nullptr;
}

batadv_tvlv_ncDescriptor::~batadv_tvlv_ncDescriptor()
{
    delete[] propertynames;
}

bool batadv_tvlv_ncDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<batadv_tvlv_nc *>(obj)!=nullptr;
}

const char **batadv_tvlv_ncDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *batadv_tvlv_ncDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int batadv_tvlv_ncDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 0+basedesc->getFieldCount() : 0;
}

unsigned int batadv_tvlv_ncDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    return 0;
}

const char *batadv_tvlv_ncDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    return nullptr;
}

int batadv_tvlv_ncDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *batadv_tvlv_ncDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    return nullptr;
}

const char **batadv_tvlv_ncDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *batadv_tvlv_ncDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int batadv_tvlv_ncDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_nc *pp = (batadv_tvlv_nc *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *batadv_tvlv_ncDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_nc *pp = (batadv_tvlv_nc *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string batadv_tvlv_ncDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_nc *pp = (batadv_tvlv_nc *)object; (void)pp;
    switch (field) {
        default: return "";
    }
}

bool batadv_tvlv_ncDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_nc *pp = (batadv_tvlv_nc *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *batadv_tvlv_ncDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    return nullptr;
}

void *batadv_tvlv_ncDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_nc *pp = (batadv_tvlv_nc *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(batadv_tvlv_tt_data)

batadv_tvlv_tt_data::batadv_tvlv_tt_data(const char *name, short kind) : ::inet::BATMANV_TVLVPackets(name,kind)
{
    this->setBatadv_tvlv_type(BATADV_TVLV_TT);
    this->setVersion(BATADV_COMPAT_VERSION);
    this->setLength(4);

    this->tt_flags = 0;
    this->ttvn = 0;
    this->num_vlan = 0;
}

batadv_tvlv_tt_data::batadv_tvlv_tt_data(const batadv_tvlv_tt_data& other) : ::inet::BATMANV_TVLVPackets(other)
{
    copy(other);
}

batadv_tvlv_tt_data::~batadv_tvlv_tt_data()
{
}

batadv_tvlv_tt_data& batadv_tvlv_tt_data::operator=(const batadv_tvlv_tt_data& other)
{
    if (this==&other) return *this;
    ::inet::BATMANV_TVLVPackets::operator=(other);
    copy(other);
    return *this;
}

void batadv_tvlv_tt_data::copy(const batadv_tvlv_tt_data& other)
{
    this->tt_flags = other.tt_flags;
    this->ttvn = other.ttvn;
    this->num_vlan = other.num_vlan;
}

void batadv_tvlv_tt_data::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::BATMANV_TVLVPackets::parsimPack(b);
    doParsimPacking(b,this->tt_flags);
    doParsimPacking(b,this->ttvn);
    doParsimPacking(b,this->num_vlan);
}

void batadv_tvlv_tt_data::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::BATMANV_TVLVPackets::parsimUnpack(b);
    doParsimUnpacking(b,this->tt_flags);
    doParsimUnpacking(b,this->ttvn);
    doParsimUnpacking(b,this->num_vlan);
}

uint8_t batadv_tvlv_tt_data::getTt_flags() const
{
    return this->tt_flags;
}

void batadv_tvlv_tt_data::setTt_flags(uint8_t tt_flags)
{
    this->tt_flags = tt_flags;
}

uint8_t batadv_tvlv_tt_data::getTtvn() const
{
    return this->ttvn;
}

void batadv_tvlv_tt_data::setTtvn(uint8_t ttvn)
{
    this->ttvn = ttvn;
}

uint16_t batadv_tvlv_tt_data::getNum_vlan() const
{
    return this->num_vlan;
}

void batadv_tvlv_tt_data::setNum_vlan(uint16_t num_vlan)
{
    this->num_vlan = num_vlan;
}

class batadv_tvlv_tt_dataDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    batadv_tvlv_tt_dataDescriptor();
    virtual ~batadv_tvlv_tt_dataDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(batadv_tvlv_tt_dataDescriptor)

batadv_tvlv_tt_dataDescriptor::batadv_tvlv_tt_dataDescriptor() : omnetpp::cClassDescriptor("inet::batadv_tvlv_tt_data", "inet::BATMANV_TVLVPackets")
{
    propertynames = nullptr;
}

batadv_tvlv_tt_dataDescriptor::~batadv_tvlv_tt_dataDescriptor()
{
    delete[] propertynames;
}

bool batadv_tvlv_tt_dataDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<batadv_tvlv_tt_data *>(obj)!=nullptr;
}

const char **batadv_tvlv_tt_dataDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *batadv_tvlv_tt_dataDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int batadv_tvlv_tt_dataDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount() : 3;
}

unsigned int batadv_tvlv_tt_dataDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *batadv_tvlv_tt_dataDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "tt_flags",
        "ttvn",
        "num_vlan",
    };
    return (field>=0 && field<3) ? fieldNames[field] : nullptr;
}

int batadv_tvlv_tt_dataDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='t' && strcmp(fieldName, "tt_flags")==0) return base+0;
    if (fieldName[0]=='t' && strcmp(fieldName, "ttvn")==0) return base+1;
    if (fieldName[0]=='n' && strcmp(fieldName, "num_vlan")==0) return base+2;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *batadv_tvlv_tt_dataDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "uint8_t",
        "uint8_t",
        "uint16_t",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : nullptr;
}

const char **batadv_tvlv_tt_dataDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *batadv_tvlv_tt_dataDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int batadv_tvlv_tt_dataDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_tt_data *pp = (batadv_tvlv_tt_data *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *batadv_tvlv_tt_dataDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_tt_data *pp = (batadv_tvlv_tt_data *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string batadv_tvlv_tt_dataDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_tt_data *pp = (batadv_tvlv_tt_data *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getTt_flags());
        case 1: return ulong2string(pp->getTtvn());
        case 2: return ulong2string(pp->getNum_vlan());
        default: return "";
    }
}

bool batadv_tvlv_tt_dataDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_tt_data *pp = (batadv_tvlv_tt_data *)object; (void)pp;
    switch (field) {
        case 0: pp->setTt_flags(string2ulong(value)); return true;
        case 1: pp->setTtvn(string2ulong(value)); return true;
        case 2: pp->setNum_vlan(string2ulong(value)); return true;
        default: return false;
    }
}

const char *batadv_tvlv_tt_dataDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *batadv_tvlv_tt_dataDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_tt_data *pp = (batadv_tvlv_tt_data *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(batadv_tvlv_tt_vlan)

batadv_tvlv_tt_vlan::batadv_tvlv_tt_vlan(const char *name, short kind) : ::inet::batadv_tvlv_tt_data(name,kind)
{
    this->setNum_vlan(1);
    this->setByteLength(16);

    this->crc32 = 0;
    this->vid_vlan = 0;
    this->reserved_vlan = 0;
}

batadv_tvlv_tt_vlan::batadv_tvlv_tt_vlan(const batadv_tvlv_tt_vlan& other) : ::inet::batadv_tvlv_tt_data(other)
{
    copy(other);
}

batadv_tvlv_tt_vlan::~batadv_tvlv_tt_vlan()
{
}

batadv_tvlv_tt_vlan& batadv_tvlv_tt_vlan::operator=(const batadv_tvlv_tt_vlan& other)
{
    if (this==&other) return *this;
    ::inet::batadv_tvlv_tt_data::operator=(other);
    copy(other);
    return *this;
}

void batadv_tvlv_tt_vlan::copy(const batadv_tvlv_tt_vlan& other)
{
    this->crc32 = other.crc32;
    this->vid_vlan = other.vid_vlan;
    this->reserved_vlan = other.reserved_vlan;
}

void batadv_tvlv_tt_vlan::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::batadv_tvlv_tt_data::parsimPack(b);
    doParsimPacking(b,this->crc32);
    doParsimPacking(b,this->vid_vlan);
    doParsimPacking(b,this->reserved_vlan);
}

void batadv_tvlv_tt_vlan::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::batadv_tvlv_tt_data::parsimUnpack(b);
    doParsimUnpacking(b,this->crc32);
    doParsimUnpacking(b,this->vid_vlan);
    doParsimUnpacking(b,this->reserved_vlan);
}

int32_t batadv_tvlv_tt_vlan::getCrc32() const
{
    return this->crc32;
}

void batadv_tvlv_tt_vlan::setCrc32(int32_t crc32)
{
    this->crc32 = crc32;
}

int16_t batadv_tvlv_tt_vlan::getVid_vlan() const
{
    return this->vid_vlan;
}

void batadv_tvlv_tt_vlan::setVid_vlan(int16_t vid_vlan)
{
    this->vid_vlan = vid_vlan;
}

int16_t batadv_tvlv_tt_vlan::getReserved_vlan() const
{
    return this->reserved_vlan;
}

void batadv_tvlv_tt_vlan::setReserved_vlan(int16_t reserved_vlan)
{
    this->reserved_vlan = reserved_vlan;
}

class batadv_tvlv_tt_vlanDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    batadv_tvlv_tt_vlanDescriptor();
    virtual ~batadv_tvlv_tt_vlanDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(batadv_tvlv_tt_vlanDescriptor)

batadv_tvlv_tt_vlanDescriptor::batadv_tvlv_tt_vlanDescriptor() : omnetpp::cClassDescriptor("inet::batadv_tvlv_tt_vlan", "inet::batadv_tvlv_tt_data")
{
    propertynames = nullptr;
}

batadv_tvlv_tt_vlanDescriptor::~batadv_tvlv_tt_vlanDescriptor()
{
    delete[] propertynames;
}

bool batadv_tvlv_tt_vlanDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<batadv_tvlv_tt_vlan *>(obj)!=nullptr;
}

const char **batadv_tvlv_tt_vlanDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *batadv_tvlv_tt_vlanDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int batadv_tvlv_tt_vlanDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount() : 3;
}

unsigned int batadv_tvlv_tt_vlanDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *batadv_tvlv_tt_vlanDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "crc32",
        "vid_vlan",
        "reserved_vlan",
    };
    return (field>=0 && field<3) ? fieldNames[field] : nullptr;
}

int batadv_tvlv_tt_vlanDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='c' && strcmp(fieldName, "crc32")==0) return base+0;
    if (fieldName[0]=='v' && strcmp(fieldName, "vid_vlan")==0) return base+1;
    if (fieldName[0]=='r' && strcmp(fieldName, "reserved_vlan")==0) return base+2;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *batadv_tvlv_tt_vlanDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int32_t",
        "int16_t",
        "int16_t",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : nullptr;
}

const char **batadv_tvlv_tt_vlanDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *batadv_tvlv_tt_vlanDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int batadv_tvlv_tt_vlanDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_tt_vlan *pp = (batadv_tvlv_tt_vlan *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *batadv_tvlv_tt_vlanDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_tt_vlan *pp = (batadv_tvlv_tt_vlan *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string batadv_tvlv_tt_vlanDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_tt_vlan *pp = (batadv_tvlv_tt_vlan *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getCrc32());
        case 1: return long2string(pp->getVid_vlan());
        case 2: return long2string(pp->getReserved_vlan());
        default: return "";
    }
}

bool batadv_tvlv_tt_vlanDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_tt_vlan *pp = (batadv_tvlv_tt_vlan *)object; (void)pp;
    switch (field) {
        case 0: pp->setCrc32(string2long(value)); return true;
        case 1: pp->setVid_vlan(string2long(value)); return true;
        case 2: pp->setReserved_vlan(string2long(value)); return true;
        default: return false;
    }
}

const char *batadv_tvlv_tt_vlanDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *batadv_tvlv_tt_vlanDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_tt_vlan *pp = (batadv_tvlv_tt_vlan *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(batadv_tvlv_tt_change)

batadv_tvlv_tt_change::batadv_tvlv_tt_change(const char *name, short kind) : ::inet::batadv_tvlv_tt_data(name,kind)
{
    this->setByteLength(20);

    this->flags_change = 0;
    this->reserved16_change = 0;
    this->reserved8_change = 0;
    this->vid_change = 0;
}

batadv_tvlv_tt_change::batadv_tvlv_tt_change(const batadv_tvlv_tt_change& other) : ::inet::batadv_tvlv_tt_data(other)
{
    copy(other);
}

batadv_tvlv_tt_change::~batadv_tvlv_tt_change()
{
}

batadv_tvlv_tt_change& batadv_tvlv_tt_change::operator=(const batadv_tvlv_tt_change& other)
{
    if (this==&other) return *this;
    ::inet::batadv_tvlv_tt_data::operator=(other);
    copy(other);
    return *this;
}

void batadv_tvlv_tt_change::copy(const batadv_tvlv_tt_change& other)
{
    this->flags_change = other.flags_change;
    this->reserved16_change = other.reserved16_change;
    this->reserved8_change = other.reserved8_change;
    this->addr = other.addr;
    this->vid_change = other.vid_change;
}

void batadv_tvlv_tt_change::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::batadv_tvlv_tt_data::parsimPack(b);
    doParsimPacking(b,this->flags_change);
    doParsimPacking(b,this->reserved16_change);
    doParsimPacking(b,this->reserved8_change);
    doParsimPacking(b,this->addr);
    doParsimPacking(b,this->vid_change);
}

void batadv_tvlv_tt_change::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::batadv_tvlv_tt_data::parsimUnpack(b);
    doParsimUnpacking(b,this->flags_change);
    doParsimUnpacking(b,this->reserved16_change);
    doParsimUnpacking(b,this->reserved8_change);
    doParsimUnpacking(b,this->addr);
    doParsimUnpacking(b,this->vid_change);
}

int8_t batadv_tvlv_tt_change::getFlags_change() const
{
    return this->flags_change;
}

void batadv_tvlv_tt_change::setFlags_change(int8_t flags_change)
{
    this->flags_change = flags_change;
}

int16_t batadv_tvlv_tt_change::getReserved16_change() const
{
    return this->reserved16_change;
}

void batadv_tvlv_tt_change::setReserved16_change(int16_t reserved16_change)
{
    this->reserved16_change = reserved16_change;
}

int8_t batadv_tvlv_tt_change::getReserved8_change() const
{
    return this->reserved8_change;
}

void batadv_tvlv_tt_change::setReserved8_change(int8_t reserved8_change)
{
    this->reserved8_change = reserved8_change;
}

MACAddress& batadv_tvlv_tt_change::getAddr()
{
    return this->addr;
}

void batadv_tvlv_tt_change::setAddr(const MACAddress& addr)
{
    this->addr = addr;
}

int16_t batadv_tvlv_tt_change::getVid_change() const
{
    return this->vid_change;
}

void batadv_tvlv_tt_change::setVid_change(int16_t vid_change)
{
    this->vid_change = vid_change;
}

class batadv_tvlv_tt_changeDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    batadv_tvlv_tt_changeDescriptor();
    virtual ~batadv_tvlv_tt_changeDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(batadv_tvlv_tt_changeDescriptor)

batadv_tvlv_tt_changeDescriptor::batadv_tvlv_tt_changeDescriptor() : omnetpp::cClassDescriptor("inet::batadv_tvlv_tt_change", "inet::batadv_tvlv_tt_data")
{
    propertynames = nullptr;
}

batadv_tvlv_tt_changeDescriptor::~batadv_tvlv_tt_changeDescriptor()
{
    delete[] propertynames;
}

bool batadv_tvlv_tt_changeDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<batadv_tvlv_tt_change *>(obj)!=nullptr;
}

const char **batadv_tvlv_tt_changeDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *batadv_tvlv_tt_changeDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int batadv_tvlv_tt_changeDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 5+basedesc->getFieldCount() : 5;
}

unsigned int batadv_tvlv_tt_changeDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<5) ? fieldTypeFlags[field] : 0;
}

const char *batadv_tvlv_tt_changeDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "flags_change",
        "reserved16_change",
        "reserved8_change",
        "addr",
        "vid_change",
    };
    return (field>=0 && field<5) ? fieldNames[field] : nullptr;
}

int batadv_tvlv_tt_changeDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='f' && strcmp(fieldName, "flags_change")==0) return base+0;
    if (fieldName[0]=='r' && strcmp(fieldName, "reserved16_change")==0) return base+1;
    if (fieldName[0]=='r' && strcmp(fieldName, "reserved8_change")==0) return base+2;
    if (fieldName[0]=='a' && strcmp(fieldName, "addr")==0) return base+3;
    if (fieldName[0]=='v' && strcmp(fieldName, "vid_change")==0) return base+4;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *batadv_tvlv_tt_changeDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int8_t",
        "int16_t",
        "int8_t",
        "MACAddress",
        "int16_t",
    };
    return (field>=0 && field<5) ? fieldTypeStrings[field] : nullptr;
}

const char **batadv_tvlv_tt_changeDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *batadv_tvlv_tt_changeDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int batadv_tvlv_tt_changeDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_tt_change *pp = (batadv_tvlv_tt_change *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *batadv_tvlv_tt_changeDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_tt_change *pp = (batadv_tvlv_tt_change *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string batadv_tvlv_tt_changeDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_tt_change *pp = (batadv_tvlv_tt_change *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getFlags_change());
        case 1: return long2string(pp->getReserved16_change());
        case 2: return long2string(pp->getReserved8_change());
        case 3: {std::stringstream out; out << pp->getAddr(); return out.str();}
        case 4: return long2string(pp->getVid_change());
        default: return "";
    }
}

bool batadv_tvlv_tt_changeDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_tt_change *pp = (batadv_tvlv_tt_change *)object; (void)pp;
    switch (field) {
        case 0: pp->setFlags_change(string2long(value)); return true;
        case 1: pp->setReserved16_change(string2long(value)); return true;
        case 2: pp->setReserved8_change(string2long(value)); return true;
        case 4: pp->setVid_change(string2long(value)); return true;
        default: return false;
    }
}

const char *batadv_tvlv_tt_changeDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 3: return omnetpp::opp_typename(typeid(MACAddress));
        default: return nullptr;
    };
}

void *batadv_tvlv_tt_changeDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_tt_change *pp = (batadv_tvlv_tt_change *)object; (void)pp;
    switch (field) {
        case 3: return (void *)(&pp->getAddr()); break;
        default: return nullptr;
    }
}

Register_Class(batadv_tvlv_tt_vlan_change)

batadv_tvlv_tt_vlan_change::batadv_tvlv_tt_vlan_change(const char *name, short kind) : ::inet::batadv_tvlv_tt_data(name,kind)
{
    this->setNum_vlan(1);
    this->setByteLength(28);

    this->crc32 = 0;
    this->vid_vlan = 0;
    this->reserved_vlan = 0;
    this->flags = 0;
    this->reserved16_change = 0;
    this->reserved8_change = 0;
    this->vid_change = 0;
}

batadv_tvlv_tt_vlan_change::batadv_tvlv_tt_vlan_change(const batadv_tvlv_tt_vlan_change& other) : ::inet::batadv_tvlv_tt_data(other)
{
    copy(other);
}

batadv_tvlv_tt_vlan_change::~batadv_tvlv_tt_vlan_change()
{
}

batadv_tvlv_tt_vlan_change& batadv_tvlv_tt_vlan_change::operator=(const batadv_tvlv_tt_vlan_change& other)
{
    if (this==&other) return *this;
    ::inet::batadv_tvlv_tt_data::operator=(other);
    copy(other);
    return *this;
}

void batadv_tvlv_tt_vlan_change::copy(const batadv_tvlv_tt_vlan_change& other)
{
    this->crc32 = other.crc32;
    this->vid_vlan = other.vid_vlan;
    this->reserved_vlan = other.reserved_vlan;
    this->flags = other.flags;
    this->reserved16_change = other.reserved16_change;
    this->reserved8_change = other.reserved8_change;
    this->addr = other.addr;
    this->vid_change = other.vid_change;
}

void batadv_tvlv_tt_vlan_change::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::batadv_tvlv_tt_data::parsimPack(b);
    doParsimPacking(b,this->crc32);
    doParsimPacking(b,this->vid_vlan);
    doParsimPacking(b,this->reserved_vlan);
    doParsimPacking(b,this->flags);
    doParsimPacking(b,this->reserved16_change);
    doParsimPacking(b,this->reserved8_change);
    doParsimPacking(b,this->addr);
    doParsimPacking(b,this->vid_change);
}

void batadv_tvlv_tt_vlan_change::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::batadv_tvlv_tt_data::parsimUnpack(b);
    doParsimUnpacking(b,this->crc32);
    doParsimUnpacking(b,this->vid_vlan);
    doParsimUnpacking(b,this->reserved_vlan);
    doParsimUnpacking(b,this->flags);
    doParsimUnpacking(b,this->reserved16_change);
    doParsimUnpacking(b,this->reserved8_change);
    doParsimUnpacking(b,this->addr);
    doParsimUnpacking(b,this->vid_change);
}

int32_t batadv_tvlv_tt_vlan_change::getCrc32() const
{
    return this->crc32;
}

void batadv_tvlv_tt_vlan_change::setCrc32(int32_t crc32)
{
    this->crc32 = crc32;
}

int16_t batadv_tvlv_tt_vlan_change::getVid_vlan() const
{
    return this->vid_vlan;
}

void batadv_tvlv_tt_vlan_change::setVid_vlan(int16_t vid_vlan)
{
    this->vid_vlan = vid_vlan;
}

int16_t batadv_tvlv_tt_vlan_change::getReserved_vlan() const
{
    return this->reserved_vlan;
}

void batadv_tvlv_tt_vlan_change::setReserved_vlan(int16_t reserved_vlan)
{
    this->reserved_vlan = reserved_vlan;
}

int8_t batadv_tvlv_tt_vlan_change::getFlags() const
{
    return this->flags;
}

void batadv_tvlv_tt_vlan_change::setFlags(int8_t flags)
{
    this->flags = flags;
}

int16_t batadv_tvlv_tt_vlan_change::getReserved16_change() const
{
    return this->reserved16_change;
}

void batadv_tvlv_tt_vlan_change::setReserved16_change(int16_t reserved16_change)
{
    this->reserved16_change = reserved16_change;
}

int8_t batadv_tvlv_tt_vlan_change::getReserved8_change() const
{
    return this->reserved8_change;
}

void batadv_tvlv_tt_vlan_change::setReserved8_change(int8_t reserved8_change)
{
    this->reserved8_change = reserved8_change;
}

MACAddress& batadv_tvlv_tt_vlan_change::getAddr()
{
    return this->addr;
}

void batadv_tvlv_tt_vlan_change::setAddr(const MACAddress& addr)
{
    this->addr = addr;
}

int16_t batadv_tvlv_tt_vlan_change::getVid_change() const
{
    return this->vid_change;
}

void batadv_tvlv_tt_vlan_change::setVid_change(int16_t vid_change)
{
    this->vid_change = vid_change;
}

class batadv_tvlv_tt_vlan_changeDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    batadv_tvlv_tt_vlan_changeDescriptor();
    virtual ~batadv_tvlv_tt_vlan_changeDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(batadv_tvlv_tt_vlan_changeDescriptor)

batadv_tvlv_tt_vlan_changeDescriptor::batadv_tvlv_tt_vlan_changeDescriptor() : omnetpp::cClassDescriptor("inet::batadv_tvlv_tt_vlan_change", "inet::batadv_tvlv_tt_data")
{
    propertynames = nullptr;
}

batadv_tvlv_tt_vlan_changeDescriptor::~batadv_tvlv_tt_vlan_changeDescriptor()
{
    delete[] propertynames;
}

bool batadv_tvlv_tt_vlan_changeDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<batadv_tvlv_tt_vlan_change *>(obj)!=nullptr;
}

const char **batadv_tvlv_tt_vlan_changeDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *batadv_tvlv_tt_vlan_changeDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int batadv_tvlv_tt_vlan_changeDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 8+basedesc->getFieldCount() : 8;
}

unsigned int batadv_tvlv_tt_vlan_changeDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<8) ? fieldTypeFlags[field] : 0;
}

const char *batadv_tvlv_tt_vlan_changeDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "crc32",
        "vid_vlan",
        "reserved_vlan",
        "flags",
        "reserved16_change",
        "reserved8_change",
        "addr",
        "vid_change",
    };
    return (field>=0 && field<8) ? fieldNames[field] : nullptr;
}

int batadv_tvlv_tt_vlan_changeDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='c' && strcmp(fieldName, "crc32")==0) return base+0;
    if (fieldName[0]=='v' && strcmp(fieldName, "vid_vlan")==0) return base+1;
    if (fieldName[0]=='r' && strcmp(fieldName, "reserved_vlan")==0) return base+2;
    if (fieldName[0]=='f' && strcmp(fieldName, "flags")==0) return base+3;
    if (fieldName[0]=='r' && strcmp(fieldName, "reserved16_change")==0) return base+4;
    if (fieldName[0]=='r' && strcmp(fieldName, "reserved8_change")==0) return base+5;
    if (fieldName[0]=='a' && strcmp(fieldName, "addr")==0) return base+6;
    if (fieldName[0]=='v' && strcmp(fieldName, "vid_change")==0) return base+7;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *batadv_tvlv_tt_vlan_changeDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int32_t",
        "int16_t",
        "int16_t",
        "int8_t",
        "int16_t",
        "int8_t",
        "MACAddress",
        "int16_t",
    };
    return (field>=0 && field<8) ? fieldTypeStrings[field] : nullptr;
}

const char **batadv_tvlv_tt_vlan_changeDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *batadv_tvlv_tt_vlan_changeDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int batadv_tvlv_tt_vlan_changeDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_tt_vlan_change *pp = (batadv_tvlv_tt_vlan_change *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *batadv_tvlv_tt_vlan_changeDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_tt_vlan_change *pp = (batadv_tvlv_tt_vlan_change *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string batadv_tvlv_tt_vlan_changeDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_tt_vlan_change *pp = (batadv_tvlv_tt_vlan_change *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getCrc32());
        case 1: return long2string(pp->getVid_vlan());
        case 2: return long2string(pp->getReserved_vlan());
        case 3: return long2string(pp->getFlags());
        case 4: return long2string(pp->getReserved16_change());
        case 5: return long2string(pp->getReserved8_change());
        case 6: {std::stringstream out; out << pp->getAddr(); return out.str();}
        case 7: return long2string(pp->getVid_change());
        default: return "";
    }
}

bool batadv_tvlv_tt_vlan_changeDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_tt_vlan_change *pp = (batadv_tvlv_tt_vlan_change *)object; (void)pp;
    switch (field) {
        case 0: pp->setCrc32(string2long(value)); return true;
        case 1: pp->setVid_vlan(string2long(value)); return true;
        case 2: pp->setReserved_vlan(string2long(value)); return true;
        case 3: pp->setFlags(string2long(value)); return true;
        case 4: pp->setReserved16_change(string2long(value)); return true;
        case 5: pp->setReserved8_change(string2long(value)); return true;
        case 7: pp->setVid_change(string2long(value)); return true;
        default: return false;
    }
}

const char *batadv_tvlv_tt_vlan_changeDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 6: return omnetpp::opp_typename(typeid(MACAddress));
        default: return nullptr;
    };
}

void *batadv_tvlv_tt_vlan_changeDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_tt_vlan_change *pp = (batadv_tvlv_tt_vlan_change *)object; (void)pp;
    switch (field) {
        case 6: return (void *)(&pp->getAddr()); break;
        default: return nullptr;
    }
}

Register_Class(batadv_tvlv_roam_adv)

batadv_tvlv_roam_adv::batadv_tvlv_roam_adv(const char *name, short kind) : ::inet::BATMANV_TVLVPackets(name,kind)
{
    this->setBatadv_tvlv_type(BATADV_TVLV_ROAM);
    this->setVersion(BATADV_COMPAT_VERSION);
    this->setLength(8);
    this->setByteLength(12);

    this->vid = 0;
}

batadv_tvlv_roam_adv::batadv_tvlv_roam_adv(const batadv_tvlv_roam_adv& other) : ::inet::BATMANV_TVLVPackets(other)
{
    copy(other);
}

batadv_tvlv_roam_adv::~batadv_tvlv_roam_adv()
{
}

batadv_tvlv_roam_adv& batadv_tvlv_roam_adv::operator=(const batadv_tvlv_roam_adv& other)
{
    if (this==&other) return *this;
    ::inet::BATMANV_TVLVPackets::operator=(other);
    copy(other);
    return *this;
}

void batadv_tvlv_roam_adv::copy(const batadv_tvlv_roam_adv& other)
{
    this->client = other.client;
    this->vid = other.vid;
}

void batadv_tvlv_roam_adv::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::BATMANV_TVLVPackets::parsimPack(b);
    doParsimPacking(b,this->client);
    doParsimPacking(b,this->vid);
}

void batadv_tvlv_roam_adv::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::BATMANV_TVLVPackets::parsimUnpack(b);
    doParsimUnpacking(b,this->client);
    doParsimUnpacking(b,this->vid);
}

MACAddress& batadv_tvlv_roam_adv::getClient()
{
    return this->client;
}

void batadv_tvlv_roam_adv::setClient(const MACAddress& client)
{
    this->client = client;
}

uint16_t batadv_tvlv_roam_adv::getVid() const
{
    return this->vid;
}

void batadv_tvlv_roam_adv::setVid(uint16_t vid)
{
    this->vid = vid;
}

class batadv_tvlv_roam_advDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    batadv_tvlv_roam_advDescriptor();
    virtual ~batadv_tvlv_roam_advDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(batadv_tvlv_roam_advDescriptor)

batadv_tvlv_roam_advDescriptor::batadv_tvlv_roam_advDescriptor() : omnetpp::cClassDescriptor("inet::batadv_tvlv_roam_adv", "inet::BATMANV_TVLVPackets")
{
    propertynames = nullptr;
}

batadv_tvlv_roam_advDescriptor::~batadv_tvlv_roam_advDescriptor()
{
    delete[] propertynames;
}

bool batadv_tvlv_roam_advDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<batadv_tvlv_roam_adv *>(obj)!=nullptr;
}

const char **batadv_tvlv_roam_advDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *batadv_tvlv_roam_advDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int batadv_tvlv_roam_advDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount() : 2;
}

unsigned int batadv_tvlv_roam_advDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *batadv_tvlv_roam_advDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "client",
        "vid",
    };
    return (field>=0 && field<2) ? fieldNames[field] : nullptr;
}

int batadv_tvlv_roam_advDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='c' && strcmp(fieldName, "client")==0) return base+0;
    if (fieldName[0]=='v' && strcmp(fieldName, "vid")==0) return base+1;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *batadv_tvlv_roam_advDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "MACAddress",
        "uint16_t",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : nullptr;
}

const char **batadv_tvlv_roam_advDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *batadv_tvlv_roam_advDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int batadv_tvlv_roam_advDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_roam_adv *pp = (batadv_tvlv_roam_adv *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *batadv_tvlv_roam_advDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_roam_adv *pp = (batadv_tvlv_roam_adv *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string batadv_tvlv_roam_advDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_roam_adv *pp = (batadv_tvlv_roam_adv *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getClient(); return out.str();}
        case 1: return ulong2string(pp->getVid());
        default: return "";
    }
}

bool batadv_tvlv_roam_advDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_roam_adv *pp = (batadv_tvlv_roam_adv *)object; (void)pp;
    switch (field) {
        case 1: pp->setVid(string2ulong(value)); return true;
        default: return false;
    }
}

const char *batadv_tvlv_roam_advDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 0: return omnetpp::opp_typename(typeid(MACAddress));
        default: return nullptr;
    };
}

void *batadv_tvlv_roam_advDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_roam_adv *pp = (batadv_tvlv_roam_adv *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getClient()); break;
        default: return nullptr;
    }
}

Register_Class(batadv_tvlv_mcast)

batadv_tvlv_mcast::batadv_tvlv_mcast(const char *name, short kind) : ::inet::BATMANV_TVLVPackets(name,kind)
{
    this->setBatadv_tvlv_type(BATADV_TVLV_MCAST);
    this->setVersion(BATADV_COMPAT_VERSION);
    this->setLength(4);
    this->setByteLength(8);

    this->flags = 0;
    this->reserved16 = 0;
    this->reserved8 = 0;
}

batadv_tvlv_mcast::batadv_tvlv_mcast(const batadv_tvlv_mcast& other) : ::inet::BATMANV_TVLVPackets(other)
{
    copy(other);
}

batadv_tvlv_mcast::~batadv_tvlv_mcast()
{
}

batadv_tvlv_mcast& batadv_tvlv_mcast::operator=(const batadv_tvlv_mcast& other)
{
    if (this==&other) return *this;
    ::inet::BATMANV_TVLVPackets::operator=(other);
    copy(other);
    return *this;
}

void batadv_tvlv_mcast::copy(const batadv_tvlv_mcast& other)
{
    this->flags = other.flags;
    this->reserved16 = other.reserved16;
    this->reserved8 = other.reserved8;
}

void batadv_tvlv_mcast::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::BATMANV_TVLVPackets::parsimPack(b);
    doParsimPacking(b,this->flags);
    doParsimPacking(b,this->reserved16);
    doParsimPacking(b,this->reserved8);
}

void batadv_tvlv_mcast::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::BATMANV_TVLVPackets::parsimUnpack(b);
    doParsimUnpacking(b,this->flags);
    doParsimUnpacking(b,this->reserved16);
    doParsimUnpacking(b,this->reserved8);
}

int8_t batadv_tvlv_mcast::getFlags() const
{
    return this->flags;
}

void batadv_tvlv_mcast::setFlags(int8_t flags)
{
    this->flags = flags;
}

int16_t batadv_tvlv_mcast::getReserved16() const
{
    return this->reserved16;
}

void batadv_tvlv_mcast::setReserved16(int16_t reserved16)
{
    this->reserved16 = reserved16;
}

int8_t batadv_tvlv_mcast::getReserved8() const
{
    return this->reserved8;
}

void batadv_tvlv_mcast::setReserved8(int8_t reserved8)
{
    this->reserved8 = reserved8;
}

class batadv_tvlv_mcastDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    batadv_tvlv_mcastDescriptor();
    virtual ~batadv_tvlv_mcastDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(batadv_tvlv_mcastDescriptor)

batadv_tvlv_mcastDescriptor::batadv_tvlv_mcastDescriptor() : omnetpp::cClassDescriptor("inet::batadv_tvlv_mcast", "inet::BATMANV_TVLVPackets")
{
    propertynames = nullptr;
}

batadv_tvlv_mcastDescriptor::~batadv_tvlv_mcastDescriptor()
{
    delete[] propertynames;
}

bool batadv_tvlv_mcastDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<batadv_tvlv_mcast *>(obj)!=nullptr;
}

const char **batadv_tvlv_mcastDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *batadv_tvlv_mcastDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int batadv_tvlv_mcastDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount() : 3;
}

unsigned int batadv_tvlv_mcastDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *batadv_tvlv_mcastDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "flags",
        "reserved16",
        "reserved8",
    };
    return (field>=0 && field<3) ? fieldNames[field] : nullptr;
}

int batadv_tvlv_mcastDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='f' && strcmp(fieldName, "flags")==0) return base+0;
    if (fieldName[0]=='r' && strcmp(fieldName, "reserved16")==0) return base+1;
    if (fieldName[0]=='r' && strcmp(fieldName, "reserved8")==0) return base+2;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *batadv_tvlv_mcastDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int8_t",
        "int16_t",
        "int8_t",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : nullptr;
}

const char **batadv_tvlv_mcastDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *batadv_tvlv_mcastDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int batadv_tvlv_mcastDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_mcast *pp = (batadv_tvlv_mcast *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *batadv_tvlv_mcastDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_mcast *pp = (batadv_tvlv_mcast *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string batadv_tvlv_mcastDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_mcast *pp = (batadv_tvlv_mcast *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getFlags());
        case 1: return long2string(pp->getReserved16());
        case 2: return long2string(pp->getReserved8());
        default: return "";
    }
}

bool batadv_tvlv_mcastDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_mcast *pp = (batadv_tvlv_mcast *)object; (void)pp;
    switch (field) {
        case 0: pp->setFlags(string2long(value)); return true;
        case 1: pp->setReserved16(string2long(value)); return true;
        case 2: pp->setReserved8(string2long(value)); return true;
        default: return false;
    }
}

const char *batadv_tvlv_mcastDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *batadv_tvlv_mcastDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    batadv_tvlv_mcast *pp = (batadv_tvlv_mcast *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(batadv_elp_packet)

batadv_elp_packet::batadv_elp_packet(const char *name) : ::inet::BATMANV_ControlPackets(name)
{
    this->setBatadv_packettype(BATADV_ELP);
    this->setBatadv_version(BATADV_COMPAT_VERSION);
    this->setByteLength(sizeof(batadv_packettype) + sizeof(batadv_version) + orig.getAddressSize() + sizeof(seqno) + sizeof(elp_interval));

    this->seqno = 0;
    this->elp_interval = 0;
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

batadv_elp_packet::batadv_elp_packet(const batadv_elp_packet& other) : ::inet::BATMANV_ControlPackets(other)
{
    copy(other);
}

batadv_elp_packet::~batadv_elp_packet()
{
}

batadv_elp_packet& batadv_elp_packet::operator=(const batadv_elp_packet& other)
{
    if (this==&other) return *this;
    ::inet::BATMANV_ControlPackets::operator=(other);
    copy(other);
    return *this;
}

void batadv_elp_packet::copy(const batadv_elp_packet& other)
{
    this->orig = other.orig;
    this->seqno = other.seqno;
    this->elp_interval = other.elp_interval;
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
}

void batadv_elp_packet::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::BATMANV_ControlPackets::parsimPack(b);
    doParsimPacking(b,this->orig);
    doParsimPacking(b,this->seqno);
    doParsimPacking(b,this->elp_interval);
    doParsimPacking(b,this->x);
    doParsimPacking(b,this->y);
    doParsimPacking(b,this->z);
}

void batadv_elp_packet::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::BATMANV_ControlPackets::parsimUnpack(b);
    doParsimUnpacking(b,this->orig);
    doParsimUnpacking(b,this->seqno);
    doParsimUnpacking(b,this->elp_interval);
    doParsimUnpacking(b,this->x);
    doParsimUnpacking(b,this->y);
    doParsimUnpacking(b,this->z);
}

MACAddress& batadv_elp_packet::getOrig()
{
    return this->orig;
}

void batadv_elp_packet::setOrig(const MACAddress& orig)
{
    this->orig = orig;
}

uint32_t batadv_elp_packet::getSeqno() const
{
    return this->seqno;
}

void batadv_elp_packet::setSeqno(uint32_t seqno)
{
    this->seqno = seqno;
}

uint32_t batadv_elp_packet::getElp_interval() const
{
    return this->elp_interval;
}

void batadv_elp_packet::setElp_interval(uint32_t elp_interval)
{
    this->elp_interval = elp_interval;
}

double batadv_elp_packet::getX() const
{
    return this->x;
}

void batadv_elp_packet::setX(double x)
{
    this->x = x;
}

double batadv_elp_packet::getY() const
{
    return this->y;
}

void batadv_elp_packet::setY(double y)
{
    this->y = y;
}

double batadv_elp_packet::getZ() const
{
    return this->z;
}

void batadv_elp_packet::setZ(double z)
{
    this->z = z;
}

class batadv_elp_packetDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    batadv_elp_packetDescriptor();
    virtual ~batadv_elp_packetDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(batadv_elp_packetDescriptor)

batadv_elp_packetDescriptor::batadv_elp_packetDescriptor() : omnetpp::cClassDescriptor("inet::batadv_elp_packet", "inet::BATMANV_ControlPackets")
{
    propertynames = nullptr;
}

batadv_elp_packetDescriptor::~batadv_elp_packetDescriptor()
{
    delete[] propertynames;
}

bool batadv_elp_packetDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<batadv_elp_packet *>(obj)!=nullptr;
}

const char **batadv_elp_packetDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *batadv_elp_packetDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int batadv_elp_packetDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 6+basedesc->getFieldCount() : 6;
}

unsigned int batadv_elp_packetDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<6) ? fieldTypeFlags[field] : 0;
}

const char *batadv_elp_packetDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "orig",
        "seqno",
        "elp_interval",
        "x",
        "y",
        "z",
    };
    return (field>=0 && field<6) ? fieldNames[field] : nullptr;
}

int batadv_elp_packetDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='o' && strcmp(fieldName, "orig")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "seqno")==0) return base+1;
    if (fieldName[0]=='e' && strcmp(fieldName, "elp_interval")==0) return base+2;
    if (fieldName[0]=='x' && strcmp(fieldName, "x")==0) return base+3;
    if (fieldName[0]=='y' && strcmp(fieldName, "y")==0) return base+4;
    if (fieldName[0]=='z' && strcmp(fieldName, "z")==0) return base+5;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *batadv_elp_packetDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "MACAddress",
        "uint32_t",
        "uint32_t",
        "double",
        "double",
        "double",
    };
    return (field>=0 && field<6) ? fieldTypeStrings[field] : nullptr;
}

const char **batadv_elp_packetDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *batadv_elp_packetDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int batadv_elp_packetDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    batadv_elp_packet *pp = (batadv_elp_packet *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *batadv_elp_packetDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_elp_packet *pp = (batadv_elp_packet *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string batadv_elp_packetDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_elp_packet *pp = (batadv_elp_packet *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getOrig(); return out.str();}
        case 1: return ulong2string(pp->getSeqno());
        case 2: return ulong2string(pp->getElp_interval());
        case 3: return double2string(pp->getX());
        case 4: return double2string(pp->getY());
        case 5: return double2string(pp->getZ());
        default: return "";
    }
}

bool batadv_elp_packetDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    batadv_elp_packet *pp = (batadv_elp_packet *)object; (void)pp;
    switch (field) {
        case 1: pp->setSeqno(string2ulong(value)); return true;
        case 2: pp->setElp_interval(string2ulong(value)); return true;
        case 3: pp->setX(string2double(value)); return true;
        case 4: pp->setY(string2double(value)); return true;
        case 5: pp->setZ(string2double(value)); return true;
        default: return false;
    }
}

const char *batadv_elp_packetDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 0: return omnetpp::opp_typename(typeid(MACAddress));
        default: return nullptr;
    };
}

void *batadv_elp_packetDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    batadv_elp_packet *pp = (batadv_elp_packet *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getOrig()); break;
        default: return nullptr;
    }
}

Register_Class(batadv_elpProbe_packet)

batadv_elpProbe_packet::batadv_elpProbe_packet(const char *name) : ::inet::batadv_elp_packet(name)
{
    this->setByteLength(BATADV_ELP_MIN_PROBE_SIZE);

    take(&(this->elp_packet));
}

batadv_elpProbe_packet::batadv_elpProbe_packet(const batadv_elpProbe_packet& other) : ::inet::batadv_elp_packet(other)
{
    take(&(this->elp_packet));
    copy(other);
}

batadv_elpProbe_packet::~batadv_elpProbe_packet()
{
    drop(&(this->elp_packet));
}

batadv_elpProbe_packet& batadv_elpProbe_packet::operator=(const batadv_elpProbe_packet& other)
{
    if (this==&other) return *this;
    ::inet::batadv_elp_packet::operator=(other);
    copy(other);
    return *this;
}

void batadv_elpProbe_packet::copy(const batadv_elpProbe_packet& other)
{
    this->elp_packet = other.elp_packet;
    this->elp_packet.setName(other.elp_packet.getName());
}

void batadv_elpProbe_packet::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::batadv_elp_packet::parsimPack(b);
    doParsimPacking(b,this->elp_packet);
}

void batadv_elpProbe_packet::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::batadv_elp_packet::parsimUnpack(b);
    doParsimUnpacking(b,this->elp_packet);
}

batadv_elp_packet& batadv_elpProbe_packet::getElp_packet()
{
    return this->elp_packet;
}

void batadv_elpProbe_packet::setElp_packet(const batadv_elp_packet& elp_packet)
{
    this->elp_packet = elp_packet;
}

class batadv_elpProbe_packetDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    batadv_elpProbe_packetDescriptor();
    virtual ~batadv_elpProbe_packetDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(batadv_elpProbe_packetDescriptor)

batadv_elpProbe_packetDescriptor::batadv_elpProbe_packetDescriptor() : omnetpp::cClassDescriptor("inet::batadv_elpProbe_packet", "inet::batadv_elp_packet")
{
    propertynames = nullptr;
}

batadv_elpProbe_packetDescriptor::~batadv_elpProbe_packetDescriptor()
{
    delete[] propertynames;
}

bool batadv_elpProbe_packetDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<batadv_elpProbe_packet *>(obj)!=nullptr;
}

const char **batadv_elpProbe_packetDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *batadv_elpProbe_packetDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int batadv_elpProbe_packetDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount() : 1;
}

unsigned int batadv_elpProbe_packetDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND | FD_ISCOBJECT | FD_ISCOWNEDOBJECT,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *batadv_elpProbe_packetDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "elp_packet",
    };
    return (field>=0 && field<1) ? fieldNames[field] : nullptr;
}

int batadv_elpProbe_packetDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='e' && strcmp(fieldName, "elp_packet")==0) return base+0;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *batadv_elpProbe_packetDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "batadv_elp_packet",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : nullptr;
}

const char **batadv_elpProbe_packetDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *batadv_elpProbe_packetDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int batadv_elpProbe_packetDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    batadv_elpProbe_packet *pp = (batadv_elpProbe_packet *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *batadv_elpProbe_packetDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_elpProbe_packet *pp = (batadv_elpProbe_packet *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string batadv_elpProbe_packetDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_elpProbe_packet *pp = (batadv_elpProbe_packet *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getElp_packet(); return out.str();}
        default: return "";
    }
}

bool batadv_elpProbe_packetDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    batadv_elpProbe_packet *pp = (batadv_elpProbe_packet *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *batadv_elpProbe_packetDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 0: return omnetpp::opp_typename(typeid(batadv_elp_packet));
        default: return nullptr;
    };
}

void *batadv_elpProbe_packetDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    batadv_elpProbe_packet *pp = (batadv_elpProbe_packet *)object; (void)pp;
    switch (field) {
        case 0: return (void *)static_cast<omnetpp::cObject *>(&pp->getElp_packet()); break;
        default: return nullptr;
    }
}

Register_Class(batadv_iv_ogm_packet)

batadv_iv_ogm_packet::batadv_iv_ogm_packet(const char *name) : ::inet::BATMANV_ControlPackets(name)
{
    this->setBatadv_packettype(BATADV_IV_OGM);
    this->setBatadv_version(BATADV_COMPAT_VERSION);

    this->ttl = 0;
    this->flags = 0;
    this->seqno = 0;
    this->reserved = 0;
    this->tq = 0;
    this->tvlv_len = 0;
}

batadv_iv_ogm_packet::batadv_iv_ogm_packet(const batadv_iv_ogm_packet& other) : ::inet::BATMANV_ControlPackets(other)
{
    copy(other);
}

batadv_iv_ogm_packet::~batadv_iv_ogm_packet()
{
}

batadv_iv_ogm_packet& batadv_iv_ogm_packet::operator=(const batadv_iv_ogm_packet& other)
{
    if (this==&other) return *this;
    ::inet::BATMANV_ControlPackets::operator=(other);
    copy(other);
    return *this;
}

void batadv_iv_ogm_packet::copy(const batadv_iv_ogm_packet& other)
{
    this->ttl = other.ttl;
    this->flags = other.flags;
    this->seqno = other.seqno;
    this->orig = other.orig;
    this->prev_sender = other.prev_sender;
    this->reserved = other.reserved;
    this->tq = other.tq;
    this->tvlv_len = other.tvlv_len;
}

void batadv_iv_ogm_packet::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::BATMANV_ControlPackets::parsimPack(b);
    doParsimPacking(b,this->ttl);
    doParsimPacking(b,this->flags);
    doParsimPacking(b,this->seqno);
    doParsimPacking(b,this->orig);
    doParsimPacking(b,this->prev_sender);
    doParsimPacking(b,this->reserved);
    doParsimPacking(b,this->tq);
    doParsimPacking(b,this->tvlv_len);
}

void batadv_iv_ogm_packet::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::BATMANV_ControlPackets::parsimUnpack(b);
    doParsimUnpacking(b,this->ttl);
    doParsimUnpacking(b,this->flags);
    doParsimUnpacking(b,this->seqno);
    doParsimUnpacking(b,this->orig);
    doParsimUnpacking(b,this->prev_sender);
    doParsimUnpacking(b,this->reserved);
    doParsimUnpacking(b,this->tq);
    doParsimUnpacking(b,this->tvlv_len);
}

uint8_t batadv_iv_ogm_packet::getTtl() const
{
    return this->ttl;
}

void batadv_iv_ogm_packet::setTtl(uint8_t ttl)
{
    this->ttl = ttl;
}

uint8_t batadv_iv_ogm_packet::getFlags() const
{
    return this->flags;
}

void batadv_iv_ogm_packet::setFlags(uint8_t flags)
{
    this->flags = flags;
}

uint32_t batadv_iv_ogm_packet::getSeqno() const
{
    return this->seqno;
}

void batadv_iv_ogm_packet::setSeqno(uint32_t seqno)
{
    this->seqno = seqno;
}

MACAddress& batadv_iv_ogm_packet::getOrig()
{
    return this->orig;
}

void batadv_iv_ogm_packet::setOrig(const MACAddress& orig)
{
    this->orig = orig;
}

MACAddress& batadv_iv_ogm_packet::getPrev_sender()
{
    return this->prev_sender;
}

void batadv_iv_ogm_packet::setPrev_sender(const MACAddress& prev_sender)
{
    this->prev_sender = prev_sender;
}

uint8_t batadv_iv_ogm_packet::getReserved() const
{
    return this->reserved;
}

void batadv_iv_ogm_packet::setReserved(uint8_t reserved)
{
    this->reserved = reserved;
}

uint8_t batadv_iv_ogm_packet::getTq() const
{
    return this->tq;
}

void batadv_iv_ogm_packet::setTq(uint8_t tq)
{
    this->tq = tq;
}

uint16_t batadv_iv_ogm_packet::getTvlv_len() const
{
    return this->tvlv_len;
}

void batadv_iv_ogm_packet::setTvlv_len(uint16_t tvlv_len)
{
    this->tvlv_len = tvlv_len;
}

class batadv_iv_ogm_packetDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    batadv_iv_ogm_packetDescriptor();
    virtual ~batadv_iv_ogm_packetDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(batadv_iv_ogm_packetDescriptor)

batadv_iv_ogm_packetDescriptor::batadv_iv_ogm_packetDescriptor() : omnetpp::cClassDescriptor("inet::batadv_iv_ogm_packet", "inet::BATMANV_ControlPackets")
{
    propertynames = nullptr;
}

batadv_iv_ogm_packetDescriptor::~batadv_iv_ogm_packetDescriptor()
{
    delete[] propertynames;
}

bool batadv_iv_ogm_packetDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<batadv_iv_ogm_packet *>(obj)!=nullptr;
}

const char **batadv_iv_ogm_packetDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *batadv_iv_ogm_packetDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int batadv_iv_ogm_packetDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 8+basedesc->getFieldCount() : 8;
}

unsigned int batadv_iv_ogm_packetDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<8) ? fieldTypeFlags[field] : 0;
}

const char *batadv_iv_ogm_packetDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "ttl",
        "flags",
        "seqno",
        "orig",
        "prev_sender",
        "reserved",
        "tq",
        "tvlv_len",
    };
    return (field>=0 && field<8) ? fieldNames[field] : nullptr;
}

int batadv_iv_ogm_packetDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='t' && strcmp(fieldName, "ttl")==0) return base+0;
    if (fieldName[0]=='f' && strcmp(fieldName, "flags")==0) return base+1;
    if (fieldName[0]=='s' && strcmp(fieldName, "seqno")==0) return base+2;
    if (fieldName[0]=='o' && strcmp(fieldName, "orig")==0) return base+3;
    if (fieldName[0]=='p' && strcmp(fieldName, "prev_sender")==0) return base+4;
    if (fieldName[0]=='r' && strcmp(fieldName, "reserved")==0) return base+5;
    if (fieldName[0]=='t' && strcmp(fieldName, "tq")==0) return base+6;
    if (fieldName[0]=='t' && strcmp(fieldName, "tvlv_len")==0) return base+7;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *batadv_iv_ogm_packetDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "uint8_t",
        "uint8_t",
        "uint32_t",
        "MACAddress",
        "MACAddress",
        "uint8_t",
        "uint8_t",
        "uint16_t",
    };
    return (field>=0 && field<8) ? fieldTypeStrings[field] : nullptr;
}

const char **batadv_iv_ogm_packetDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *batadv_iv_ogm_packetDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int batadv_iv_ogm_packetDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    batadv_iv_ogm_packet *pp = (batadv_iv_ogm_packet *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *batadv_iv_ogm_packetDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_iv_ogm_packet *pp = (batadv_iv_ogm_packet *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string batadv_iv_ogm_packetDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_iv_ogm_packet *pp = (batadv_iv_ogm_packet *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getTtl());
        case 1: return ulong2string(pp->getFlags());
        case 2: return ulong2string(pp->getSeqno());
        case 3: {std::stringstream out; out << pp->getOrig(); return out.str();}
        case 4: {std::stringstream out; out << pp->getPrev_sender(); return out.str();}
        case 5: return ulong2string(pp->getReserved());
        case 6: return ulong2string(pp->getTq());
        case 7: return ulong2string(pp->getTvlv_len());
        default: return "";
    }
}

bool batadv_iv_ogm_packetDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    batadv_iv_ogm_packet *pp = (batadv_iv_ogm_packet *)object; (void)pp;
    switch (field) {
        case 0: pp->setTtl(string2ulong(value)); return true;
        case 1: pp->setFlags(string2ulong(value)); return true;
        case 2: pp->setSeqno(string2ulong(value)); return true;
        case 5: pp->setReserved(string2ulong(value)); return true;
        case 6: pp->setTq(string2ulong(value)); return true;
        case 7: pp->setTvlv_len(string2ulong(value)); return true;
        default: return false;
    }
}

const char *batadv_iv_ogm_packetDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 3: return omnetpp::opp_typename(typeid(MACAddress));
        case 4: return omnetpp::opp_typename(typeid(MACAddress));
        default: return nullptr;
    };
}

void *batadv_iv_ogm_packetDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    batadv_iv_ogm_packet *pp = (batadv_iv_ogm_packet *)object; (void)pp;
    switch (field) {
        case 3: return (void *)(&pp->getOrig()); break;
        case 4: return (void *)(&pp->getPrev_sender()); break;
        default: return nullptr;
    }
}

Register_Class(batadv_ogm2_packet)

batadv_ogm2_packet::batadv_ogm2_packet(const char *name) : ::inet::BATMANV_ControlPackets(name)
{
    this->setBatadv_packettype(BATADV_OGM2);
    this->setBatadv_version(BATADV_COMPAT_VERSION);
    this->setByteLength(20);

    this->ttl = 0;
    this->flags = 0;
    this->seqno = 0;
    this->tvlv_len = 0;
    this->metric = 0;
}

batadv_ogm2_packet::batadv_ogm2_packet(const batadv_ogm2_packet& other) : ::inet::BATMANV_ControlPackets(other)
{
    copy(other);
}

batadv_ogm2_packet::~batadv_ogm2_packet()
{
}

batadv_ogm2_packet& batadv_ogm2_packet::operator=(const batadv_ogm2_packet& other)
{
    if (this==&other) return *this;
    ::inet::BATMANV_ControlPackets::operator=(other);
    copy(other);
    return *this;
}

void batadv_ogm2_packet::copy(const batadv_ogm2_packet& other)
{
    this->ttl = other.ttl;
    this->flags = other.flags;
    this->seqno = other.seqno;
    this->orig = other.orig;
    this->tvlv_len = other.tvlv_len;
    this->metric = other.metric;
    this->prev_sender = other.prev_sender;
}

void batadv_ogm2_packet::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::BATMANV_ControlPackets::parsimPack(b);
    doParsimPacking(b,this->ttl);
    doParsimPacking(b,this->flags);
    doParsimPacking(b,this->seqno);
    doParsimPacking(b,this->orig);
    doParsimPacking(b,this->tvlv_len);
    doParsimPacking(b,this->metric);
    doParsimPacking(b,this->prev_sender);
}

void batadv_ogm2_packet::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::BATMANV_ControlPackets::parsimUnpack(b);
    doParsimUnpacking(b,this->ttl);
    doParsimUnpacking(b,this->flags);
    doParsimUnpacking(b,this->seqno);
    doParsimUnpacking(b,this->orig);
    doParsimUnpacking(b,this->tvlv_len);
    doParsimUnpacking(b,this->metric);
    doParsimUnpacking(b,this->prev_sender);
}

uint8_t batadv_ogm2_packet::getTtl() const
{
    return this->ttl;
}

void batadv_ogm2_packet::setTtl(uint8_t ttl)
{
    this->ttl = ttl;
}

uint8_t batadv_ogm2_packet::getFlags() const
{
    return this->flags;
}

void batadv_ogm2_packet::setFlags(uint8_t flags)
{
    this->flags = flags;
}

uint32_t batadv_ogm2_packet::getSeqno() const
{
    return this->seqno;
}

void batadv_ogm2_packet::setSeqno(uint32_t seqno)
{
    this->seqno = seqno;
}

MACAddress& batadv_ogm2_packet::getOrig()
{
    return this->orig;
}

void batadv_ogm2_packet::setOrig(const MACAddress& orig)
{
    this->orig = orig;
}

uint16_t batadv_ogm2_packet::getTvlv_len() const
{
    return this->tvlv_len;
}

void batadv_ogm2_packet::setTvlv_len(uint16_t tvlv_len)
{
    this->tvlv_len = tvlv_len;
}

uint32_t batadv_ogm2_packet::getMetric() const
{
    return this->metric;
}

void batadv_ogm2_packet::setMetric(uint32_t metric)
{
    this->metric = metric;
}

MACAddress& batadv_ogm2_packet::getPrev_sender()
{
    return this->prev_sender;
}

void batadv_ogm2_packet::setPrev_sender(const MACAddress& prev_sender)
{
    this->prev_sender = prev_sender;
}

class batadv_ogm2_packetDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    batadv_ogm2_packetDescriptor();
    virtual ~batadv_ogm2_packetDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(batadv_ogm2_packetDescriptor)

batadv_ogm2_packetDescriptor::batadv_ogm2_packetDescriptor() : omnetpp::cClassDescriptor("inet::batadv_ogm2_packet", "inet::BATMANV_ControlPackets")
{
    propertynames = nullptr;
}

batadv_ogm2_packetDescriptor::~batadv_ogm2_packetDescriptor()
{
    delete[] propertynames;
}

bool batadv_ogm2_packetDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<batadv_ogm2_packet *>(obj)!=nullptr;
}

const char **batadv_ogm2_packetDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *batadv_ogm2_packetDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int batadv_ogm2_packetDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 7+basedesc->getFieldCount() : 7;
}

unsigned int batadv_ogm2_packetDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
    };
    return (field>=0 && field<7) ? fieldTypeFlags[field] : 0;
}

const char *batadv_ogm2_packetDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "ttl",
        "flags",
        "seqno",
        "orig",
        "tvlv_len",
        "metric",
        "prev_sender",
    };
    return (field>=0 && field<7) ? fieldNames[field] : nullptr;
}

int batadv_ogm2_packetDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='t' && strcmp(fieldName, "ttl")==0) return base+0;
    if (fieldName[0]=='f' && strcmp(fieldName, "flags")==0) return base+1;
    if (fieldName[0]=='s' && strcmp(fieldName, "seqno")==0) return base+2;
    if (fieldName[0]=='o' && strcmp(fieldName, "orig")==0) return base+3;
    if (fieldName[0]=='t' && strcmp(fieldName, "tvlv_len")==0) return base+4;
    if (fieldName[0]=='m' && strcmp(fieldName, "metric")==0) return base+5;
    if (fieldName[0]=='p' && strcmp(fieldName, "prev_sender")==0) return base+6;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *batadv_ogm2_packetDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "uint8_t",
        "uint8_t",
        "uint32_t",
        "MACAddress",
        "uint16_t",
        "uint32_t",
        "MACAddress",
    };
    return (field>=0 && field<7) ? fieldTypeStrings[field] : nullptr;
}

const char **batadv_ogm2_packetDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *batadv_ogm2_packetDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int batadv_ogm2_packetDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    batadv_ogm2_packet *pp = (batadv_ogm2_packet *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *batadv_ogm2_packetDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_ogm2_packet *pp = (batadv_ogm2_packet *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string batadv_ogm2_packetDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_ogm2_packet *pp = (batadv_ogm2_packet *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getTtl());
        case 1: return ulong2string(pp->getFlags());
        case 2: return ulong2string(pp->getSeqno());
        case 3: {std::stringstream out; out << pp->getOrig(); return out.str();}
        case 4: return ulong2string(pp->getTvlv_len());
        case 5: return ulong2string(pp->getMetric());
        case 6: {std::stringstream out; out << pp->getPrev_sender(); return out.str();}
        default: return "";
    }
}

bool batadv_ogm2_packetDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    batadv_ogm2_packet *pp = (batadv_ogm2_packet *)object; (void)pp;
    switch (field) {
        case 0: pp->setTtl(string2ulong(value)); return true;
        case 1: pp->setFlags(string2ulong(value)); return true;
        case 2: pp->setSeqno(string2ulong(value)); return true;
        case 4: pp->setTvlv_len(string2ulong(value)); return true;
        case 5: pp->setMetric(string2ulong(value)); return true;
        default: return false;
    }
}

const char *batadv_ogm2_packetDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 3: return omnetpp::opp_typename(typeid(MACAddress));
        case 6: return omnetpp::opp_typename(typeid(MACAddress));
        default: return nullptr;
    };
}

void *batadv_ogm2_packetDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    batadv_ogm2_packet *pp = (batadv_ogm2_packet *)object; (void)pp;
    switch (field) {
        case 3: return (void *)(&pp->getOrig()); break;
        case 6: return (void *)(&pp->getPrev_sender()); break;
        default: return nullptr;
    }
}

Register_Class(batadv_unicast_packet)

batadv_unicast_packet::batadv_unicast_packet(const char *name) : ::inet::BATMANV_ControlPackets(name)
{
    this->setBatadv_packettype(BATADV_UNICAST);
    this->setBatadv_version(BATADV_COMPAT_VERSION);
    this->setByteLength(BATADV_UNICAST_HLEN);

    this->ttl = 0;
    this->ttvn = 0;
    this->decoded = false;
    this->packet_id = 0;
}

batadv_unicast_packet::batadv_unicast_packet(const batadv_unicast_packet& other) : ::inet::BATMANV_ControlPackets(other)
{
    copy(other);
}

batadv_unicast_packet::~batadv_unicast_packet()
{
}

batadv_unicast_packet& batadv_unicast_packet::operator=(const batadv_unicast_packet& other)
{
    if (this==&other) return *this;
    ::inet::BATMANV_ControlPackets::operator=(other);
    copy(other);
    return *this;
}

void batadv_unicast_packet::copy(const batadv_unicast_packet& other)
{
    this->ttl = other.ttl;
    this->ttvn = other.ttvn;
    this->dest = other.dest;
    this->src = other.src;
    this->h_source = other.h_source;
    this->h_dest = other.h_dest;
    this->decoded = other.decoded;
    this->packet_id = other.packet_id;
}

void batadv_unicast_packet::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::BATMANV_ControlPackets::parsimPack(b);
    doParsimPacking(b,this->ttl);
    doParsimPacking(b,this->ttvn);
    doParsimPacking(b,this->dest);
    doParsimPacking(b,this->src);
    doParsimPacking(b,this->h_source);
    doParsimPacking(b,this->h_dest);
    doParsimPacking(b,this->decoded);
    doParsimPacking(b,this->packet_id);
}

void batadv_unicast_packet::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::BATMANV_ControlPackets::parsimUnpack(b);
    doParsimUnpacking(b,this->ttl);
    doParsimUnpacking(b,this->ttvn);
    doParsimUnpacking(b,this->dest);
    doParsimUnpacking(b,this->src);
    doParsimUnpacking(b,this->h_source);
    doParsimUnpacking(b,this->h_dest);
    doParsimUnpacking(b,this->decoded);
    doParsimUnpacking(b,this->packet_id);
}

uint8_t batadv_unicast_packet::getTtl() const
{
    return this->ttl;
}

void batadv_unicast_packet::setTtl(uint8_t ttl)
{
    this->ttl = ttl;
}

uint8_t batadv_unicast_packet::getTtvn() const
{
    return this->ttvn;
}

void batadv_unicast_packet::setTtvn(uint8_t ttvn)
{
    this->ttvn = ttvn;
}

MACAddress& batadv_unicast_packet::getDest()
{
    return this->dest;
}

void batadv_unicast_packet::setDest(const MACAddress& dest)
{
    this->dest = dest;
}

MACAddress& batadv_unicast_packet::getSrc()
{
    return this->src;
}

void batadv_unicast_packet::setSrc(const MACAddress& src)
{
    this->src = src;
}

MACAddress& batadv_unicast_packet::getH_source()
{
    return this->h_source;
}

void batadv_unicast_packet::setH_source(const MACAddress& h_source)
{
    this->h_source = h_source;
}

MACAddress& batadv_unicast_packet::getH_dest()
{
    return this->h_dest;
}

void batadv_unicast_packet::setH_dest(const MACAddress& h_dest)
{
    this->h_dest = h_dest;
}

bool batadv_unicast_packet::getDecoded() const
{
    return this->decoded;
}

void batadv_unicast_packet::setDecoded(bool decoded)
{
    this->decoded = decoded;
}

uint64_t batadv_unicast_packet::getPacket_id() const
{
    return this->packet_id;
}

void batadv_unicast_packet::setPacket_id(uint64_t packet_id)
{
    this->packet_id = packet_id;
}

class batadv_unicast_packetDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    batadv_unicast_packetDescriptor();
    virtual ~batadv_unicast_packetDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(batadv_unicast_packetDescriptor)

batadv_unicast_packetDescriptor::batadv_unicast_packetDescriptor() : omnetpp::cClassDescriptor("inet::batadv_unicast_packet", "inet::BATMANV_ControlPackets")
{
    propertynames = nullptr;
}

batadv_unicast_packetDescriptor::~batadv_unicast_packetDescriptor()
{
    delete[] propertynames;
}

bool batadv_unicast_packetDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<batadv_unicast_packet *>(obj)!=nullptr;
}

const char **batadv_unicast_packetDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *batadv_unicast_packetDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int batadv_unicast_packetDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 8+basedesc->getFieldCount() : 8;
}

unsigned int batadv_unicast_packetDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<8) ? fieldTypeFlags[field] : 0;
}

const char *batadv_unicast_packetDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "ttl",
        "ttvn",
        "dest",
        "src",
        "h_source",
        "h_dest",
        "decoded",
        "packet_id",
    };
    return (field>=0 && field<8) ? fieldNames[field] : nullptr;
}

int batadv_unicast_packetDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='t' && strcmp(fieldName, "ttl")==0) return base+0;
    if (fieldName[0]=='t' && strcmp(fieldName, "ttvn")==0) return base+1;
    if (fieldName[0]=='d' && strcmp(fieldName, "dest")==0) return base+2;
    if (fieldName[0]=='s' && strcmp(fieldName, "src")==0) return base+3;
    if (fieldName[0]=='h' && strcmp(fieldName, "h_source")==0) return base+4;
    if (fieldName[0]=='h' && strcmp(fieldName, "h_dest")==0) return base+5;
    if (fieldName[0]=='d' && strcmp(fieldName, "decoded")==0) return base+6;
    if (fieldName[0]=='p' && strcmp(fieldName, "packet_id")==0) return base+7;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *batadv_unicast_packetDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "uint8_t",
        "uint8_t",
        "MACAddress",
        "MACAddress",
        "MACAddress",
        "MACAddress",
        "bool",
        "uint64_t",
    };
    return (field>=0 && field<8) ? fieldTypeStrings[field] : nullptr;
}

const char **batadv_unicast_packetDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *batadv_unicast_packetDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int batadv_unicast_packetDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    batadv_unicast_packet *pp = (batadv_unicast_packet *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *batadv_unicast_packetDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_unicast_packet *pp = (batadv_unicast_packet *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string batadv_unicast_packetDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_unicast_packet *pp = (batadv_unicast_packet *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getTtl());
        case 1: return ulong2string(pp->getTtvn());
        case 2: {std::stringstream out; out << pp->getDest(); return out.str();}
        case 3: {std::stringstream out; out << pp->getSrc(); return out.str();}
        case 4: {std::stringstream out; out << pp->getH_source(); return out.str();}
        case 5: {std::stringstream out; out << pp->getH_dest(); return out.str();}
        case 6: return bool2string(pp->getDecoded());
        case 7: return uint642string(pp->getPacket_id());
        default: return "";
    }
}

bool batadv_unicast_packetDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    batadv_unicast_packet *pp = (batadv_unicast_packet *)object; (void)pp;
    switch (field) {
        case 0: pp->setTtl(string2ulong(value)); return true;
        case 1: pp->setTtvn(string2ulong(value)); return true;
        case 6: pp->setDecoded(string2bool(value)); return true;
        case 7: pp->setPacket_id(string2uint64(value)); return true;
        default: return false;
    }
}

const char *batadv_unicast_packetDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 2: return omnetpp::opp_typename(typeid(MACAddress));
        case 3: return omnetpp::opp_typename(typeid(MACAddress));
        case 4: return omnetpp::opp_typename(typeid(MACAddress));
        case 5: return omnetpp::opp_typename(typeid(MACAddress));
        default: return nullptr;
    };
}

void *batadv_unicast_packetDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    batadv_unicast_packet *pp = (batadv_unicast_packet *)object; (void)pp;
    switch (field) {
        case 2: return (void *)(&pp->getDest()); break;
        case 3: return (void *)(&pp->getSrc()); break;
        case 4: return (void *)(&pp->getH_source()); break;
        case 5: return (void *)(&pp->getH_dest()); break;
        default: return nullptr;
    }
}

Register_Class(batadv_unicast_4addr_packet)

batadv_unicast_4addr_packet::batadv_unicast_4addr_packet(const char *name) : ::inet::batadv_unicast_packet(name)
{
    this->setBatadv_packettype(BATADV_UNICAST_4ADDR);
    this->setByteLength(BATADV_UNICAST_4ADDR_HLEN);

    this->subtype = 0;
    this->reserved = 0;
}

batadv_unicast_4addr_packet::batadv_unicast_4addr_packet(const batadv_unicast_4addr_packet& other) : ::inet::batadv_unicast_packet(other)
{
    copy(other);
}

batadv_unicast_4addr_packet::~batadv_unicast_4addr_packet()
{
}

batadv_unicast_4addr_packet& batadv_unicast_4addr_packet::operator=(const batadv_unicast_4addr_packet& other)
{
    if (this==&other) return *this;
    ::inet::batadv_unicast_packet::operator=(other);
    copy(other);
    return *this;
}

void batadv_unicast_4addr_packet::copy(const batadv_unicast_4addr_packet& other)
{
    this->subtype = other.subtype;
    this->reserved = other.reserved;
}

void batadv_unicast_4addr_packet::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::batadv_unicast_packet::parsimPack(b);
    doParsimPacking(b,this->subtype);
    doParsimPacking(b,this->reserved);
}

void batadv_unicast_4addr_packet::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::batadv_unicast_packet::parsimUnpack(b);
    doParsimUnpacking(b,this->subtype);
    doParsimUnpacking(b,this->reserved);
}

uint8_t batadv_unicast_4addr_packet::getSubtype() const
{
    return this->subtype;
}

void batadv_unicast_4addr_packet::setSubtype(uint8_t subtype)
{
    this->subtype = subtype;
}

uint8_t batadv_unicast_4addr_packet::getReserved() const
{
    return this->reserved;
}

void batadv_unicast_4addr_packet::setReserved(uint8_t reserved)
{
    this->reserved = reserved;
}

class batadv_unicast_4addr_packetDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    batadv_unicast_4addr_packetDescriptor();
    virtual ~batadv_unicast_4addr_packetDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(batadv_unicast_4addr_packetDescriptor)

batadv_unicast_4addr_packetDescriptor::batadv_unicast_4addr_packetDescriptor() : omnetpp::cClassDescriptor("inet::batadv_unicast_4addr_packet", "inet::batadv_unicast_packet")
{
    propertynames = nullptr;
}

batadv_unicast_4addr_packetDescriptor::~batadv_unicast_4addr_packetDescriptor()
{
    delete[] propertynames;
}

bool batadv_unicast_4addr_packetDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<batadv_unicast_4addr_packet *>(obj)!=nullptr;
}

const char **batadv_unicast_4addr_packetDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *batadv_unicast_4addr_packetDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int batadv_unicast_4addr_packetDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount() : 2;
}

unsigned int batadv_unicast_4addr_packetDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *batadv_unicast_4addr_packetDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "subtype",
        "reserved",
    };
    return (field>=0 && field<2) ? fieldNames[field] : nullptr;
}

int batadv_unicast_4addr_packetDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "subtype")==0) return base+0;
    if (fieldName[0]=='r' && strcmp(fieldName, "reserved")==0) return base+1;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *batadv_unicast_4addr_packetDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "uint8_t",
        "uint8_t",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : nullptr;
}

const char **batadv_unicast_4addr_packetDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *batadv_unicast_4addr_packetDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int batadv_unicast_4addr_packetDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    batadv_unicast_4addr_packet *pp = (batadv_unicast_4addr_packet *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *batadv_unicast_4addr_packetDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_unicast_4addr_packet *pp = (batadv_unicast_4addr_packet *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string batadv_unicast_4addr_packetDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_unicast_4addr_packet *pp = (batadv_unicast_4addr_packet *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getSubtype());
        case 1: return ulong2string(pp->getReserved());
        default: return "";
    }
}

bool batadv_unicast_4addr_packetDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    batadv_unicast_4addr_packet *pp = (batadv_unicast_4addr_packet *)object; (void)pp;
    switch (field) {
        case 0: pp->setSubtype(string2ulong(value)); return true;
        case 1: pp->setReserved(string2ulong(value)); return true;
        default: return false;
    }
}

const char *batadv_unicast_4addr_packetDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *batadv_unicast_4addr_packetDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    batadv_unicast_4addr_packet *pp = (batadv_unicast_4addr_packet *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(batadv_frag_packet)

batadv_frag_packet::batadv_frag_packet(const char *name) : ::inet::BATMANV_ControlPackets(name)
{
    this->setBatadv_packettype(BATADV_UNICAST_FRAG);
    this->setBatadv_version(BATADV_COMPAT_VERSION);

    this->ttl = 0;
    this->seqno = 0;
    this->total_size = 0;
}

batadv_frag_packet::batadv_frag_packet(const batadv_frag_packet& other) : ::inet::BATMANV_ControlPackets(other)
{
    copy(other);
}

batadv_frag_packet::~batadv_frag_packet()
{
}

batadv_frag_packet& batadv_frag_packet::operator=(const batadv_frag_packet& other)
{
    if (this==&other) return *this;
    ::inet::BATMANV_ControlPackets::operator=(other);
    copy(other);
    return *this;
}

void batadv_frag_packet::copy(const batadv_frag_packet& other)
{
    this->ttl = other.ttl;
    this->dest = other.dest;
    this->orig = other.orig;
    this->seqno = other.seqno;
    this->total_size = other.total_size;
}

void batadv_frag_packet::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::BATMANV_ControlPackets::parsimPack(b);
    doParsimPacking(b,this->ttl);
    doParsimPacking(b,this->dest);
    doParsimPacking(b,this->orig);
    doParsimPacking(b,this->seqno);
    doParsimPacking(b,this->total_size);
}

void batadv_frag_packet::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::BATMANV_ControlPackets::parsimUnpack(b);
    doParsimUnpacking(b,this->ttl);
    doParsimUnpacking(b,this->dest);
    doParsimUnpacking(b,this->orig);
    doParsimUnpacking(b,this->seqno);
    doParsimUnpacking(b,this->total_size);
}

uint8_t batadv_frag_packet::getTtl() const
{
    return this->ttl;
}

void batadv_frag_packet::setTtl(uint8_t ttl)
{
    this->ttl = ttl;
}

MACAddress& batadv_frag_packet::getDest()
{
    return this->dest;
}

void batadv_frag_packet::setDest(const MACAddress& dest)
{
    this->dest = dest;
}

MACAddress& batadv_frag_packet::getOrig()
{
    return this->orig;
}

void batadv_frag_packet::setOrig(const MACAddress& orig)
{
    this->orig = orig;
}

uint16_t batadv_frag_packet::getSeqno() const
{
    return this->seqno;
}

void batadv_frag_packet::setSeqno(uint16_t seqno)
{
    this->seqno = seqno;
}

uint16_t batadv_frag_packet::getTotal_size() const
{
    return this->total_size;
}

void batadv_frag_packet::setTotal_size(uint16_t total_size)
{
    this->total_size = total_size;
}

class batadv_frag_packetDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    batadv_frag_packetDescriptor();
    virtual ~batadv_frag_packetDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(batadv_frag_packetDescriptor)

batadv_frag_packetDescriptor::batadv_frag_packetDescriptor() : omnetpp::cClassDescriptor("inet::batadv_frag_packet", "inet::BATMANV_ControlPackets")
{
    propertynames = nullptr;
}

batadv_frag_packetDescriptor::~batadv_frag_packetDescriptor()
{
    delete[] propertynames;
}

bool batadv_frag_packetDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<batadv_frag_packet *>(obj)!=nullptr;
}

const char **batadv_frag_packetDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *batadv_frag_packetDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int batadv_frag_packetDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 5+basedesc->getFieldCount() : 5;
}

unsigned int batadv_frag_packetDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<5) ? fieldTypeFlags[field] : 0;
}

const char *batadv_frag_packetDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "ttl",
        "dest",
        "orig",
        "seqno",
        "total_size",
    };
    return (field>=0 && field<5) ? fieldNames[field] : nullptr;
}

int batadv_frag_packetDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='t' && strcmp(fieldName, "ttl")==0) return base+0;
    if (fieldName[0]=='d' && strcmp(fieldName, "dest")==0) return base+1;
    if (fieldName[0]=='o' && strcmp(fieldName, "orig")==0) return base+2;
    if (fieldName[0]=='s' && strcmp(fieldName, "seqno")==0) return base+3;
    if (fieldName[0]=='t' && strcmp(fieldName, "total_size")==0) return base+4;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *batadv_frag_packetDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "uint8_t",
        "MACAddress",
        "MACAddress",
        "uint16_t",
        "uint16_t",
    };
    return (field>=0 && field<5) ? fieldTypeStrings[field] : nullptr;
}

const char **batadv_frag_packetDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *batadv_frag_packetDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int batadv_frag_packetDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    batadv_frag_packet *pp = (batadv_frag_packet *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *batadv_frag_packetDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_frag_packet *pp = (batadv_frag_packet *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string batadv_frag_packetDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_frag_packet *pp = (batadv_frag_packet *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getTtl());
        case 1: {std::stringstream out; out << pp->getDest(); return out.str();}
        case 2: {std::stringstream out; out << pp->getOrig(); return out.str();}
        case 3: return ulong2string(pp->getSeqno());
        case 4: return ulong2string(pp->getTotal_size());
        default: return "";
    }
}

bool batadv_frag_packetDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    batadv_frag_packet *pp = (batadv_frag_packet *)object; (void)pp;
    switch (field) {
        case 0: pp->setTtl(string2ulong(value)); return true;
        case 3: pp->setSeqno(string2ulong(value)); return true;
        case 4: pp->setTotal_size(string2ulong(value)); return true;
        default: return false;
    }
}

const char *batadv_frag_packetDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 1: return omnetpp::opp_typename(typeid(MACAddress));
        case 2: return omnetpp::opp_typename(typeid(MACAddress));
        default: return nullptr;
    };
}

void *batadv_frag_packetDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    batadv_frag_packet *pp = (batadv_frag_packet *)object; (void)pp;
    switch (field) {
        case 1: return (void *)(&pp->getDest()); break;
        case 2: return (void *)(&pp->getOrig()); break;
        default: return nullptr;
    }
}

Register_Class(batadv_bcast_packet)

batadv_bcast_packet::batadv_bcast_packet(const char *name) : ::inet::BATMANV_ControlPackets(name)
{
    this->setBatadv_packettype(BATADV_BCAST);
    this->setBatadv_version(BATADV_COMPAT_VERSION);
    this->setByteLength(BATADV_BCAST_HLEN);

    this->ttl = 0;
    this->reserved = 0;
    this->seqno = 0;
}

batadv_bcast_packet::batadv_bcast_packet(const batadv_bcast_packet& other) : ::inet::BATMANV_ControlPackets(other)
{
    copy(other);
}

batadv_bcast_packet::~batadv_bcast_packet()
{
}

batadv_bcast_packet& batadv_bcast_packet::operator=(const batadv_bcast_packet& other)
{
    if (this==&other) return *this;
    ::inet::BATMANV_ControlPackets::operator=(other);
    copy(other);
    return *this;
}

void batadv_bcast_packet::copy(const batadv_bcast_packet& other)
{
    this->ttl = other.ttl;
    this->reserved = other.reserved;
    this->seqno = other.seqno;
    this->orig = other.orig;
}

void batadv_bcast_packet::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::BATMANV_ControlPackets::parsimPack(b);
    doParsimPacking(b,this->ttl);
    doParsimPacking(b,this->reserved);
    doParsimPacking(b,this->seqno);
    doParsimPacking(b,this->orig);
}

void batadv_bcast_packet::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::BATMANV_ControlPackets::parsimUnpack(b);
    doParsimUnpacking(b,this->ttl);
    doParsimUnpacking(b,this->reserved);
    doParsimUnpacking(b,this->seqno);
    doParsimUnpacking(b,this->orig);
}

uint8_t batadv_bcast_packet::getTtl() const
{
    return this->ttl;
}

void batadv_bcast_packet::setTtl(uint8_t ttl)
{
    this->ttl = ttl;
}

uint8_t batadv_bcast_packet::getReserved() const
{
    return this->reserved;
}

void batadv_bcast_packet::setReserved(uint8_t reserved)
{
    this->reserved = reserved;
}

uint32_t batadv_bcast_packet::getSeqno() const
{
    return this->seqno;
}

void batadv_bcast_packet::setSeqno(uint32_t seqno)
{
    this->seqno = seqno;
}

MACAddress& batadv_bcast_packet::getOrig()
{
    return this->orig;
}

void batadv_bcast_packet::setOrig(const MACAddress& orig)
{
    this->orig = orig;
}

class batadv_bcast_packetDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    batadv_bcast_packetDescriptor();
    virtual ~batadv_bcast_packetDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(batadv_bcast_packetDescriptor)

batadv_bcast_packetDescriptor::batadv_bcast_packetDescriptor() : omnetpp::cClassDescriptor("inet::batadv_bcast_packet", "inet::BATMANV_ControlPackets")
{
    propertynames = nullptr;
}

batadv_bcast_packetDescriptor::~batadv_bcast_packetDescriptor()
{
    delete[] propertynames;
}

bool batadv_bcast_packetDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<batadv_bcast_packet *>(obj)!=nullptr;
}

const char **batadv_bcast_packetDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *batadv_bcast_packetDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int batadv_bcast_packetDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount() : 4;
}

unsigned int batadv_bcast_packetDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *batadv_bcast_packetDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "ttl",
        "reserved",
        "seqno",
        "orig",
    };
    return (field>=0 && field<4) ? fieldNames[field] : nullptr;
}

int batadv_bcast_packetDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='t' && strcmp(fieldName, "ttl")==0) return base+0;
    if (fieldName[0]=='r' && strcmp(fieldName, "reserved")==0) return base+1;
    if (fieldName[0]=='s' && strcmp(fieldName, "seqno")==0) return base+2;
    if (fieldName[0]=='o' && strcmp(fieldName, "orig")==0) return base+3;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *batadv_bcast_packetDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "uint8_t",
        "uint8_t",
        "uint32_t",
        "MACAddress",
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : nullptr;
}

const char **batadv_bcast_packetDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *batadv_bcast_packetDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int batadv_bcast_packetDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    batadv_bcast_packet *pp = (batadv_bcast_packet *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *batadv_bcast_packetDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_bcast_packet *pp = (batadv_bcast_packet *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string batadv_bcast_packetDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_bcast_packet *pp = (batadv_bcast_packet *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getTtl());
        case 1: return ulong2string(pp->getReserved());
        case 2: return ulong2string(pp->getSeqno());
        case 3: {std::stringstream out; out << pp->getOrig(); return out.str();}
        default: return "";
    }
}

bool batadv_bcast_packetDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    batadv_bcast_packet *pp = (batadv_bcast_packet *)object; (void)pp;
    switch (field) {
        case 0: pp->setTtl(string2ulong(value)); return true;
        case 1: pp->setReserved(string2ulong(value)); return true;
        case 2: pp->setSeqno(string2ulong(value)); return true;
        default: return false;
    }
}

const char *batadv_bcast_packetDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 3: return omnetpp::opp_typename(typeid(MACAddress));
        default: return nullptr;
    };
}

void *batadv_bcast_packetDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    batadv_bcast_packet *pp = (batadv_bcast_packet *)object; (void)pp;
    switch (field) {
        case 3: return (void *)(&pp->getOrig()); break;
        default: return nullptr;
    }
}

Register_Class(batadv_icmp_packet)

batadv_icmp_packet::batadv_icmp_packet(const char *name) : ::inet::BATMANV_ControlPackets(name)
{
    this->setBatadv_packettype(BATADV_ICMP);
    this->setBatadv_version(BATADV_COMPAT_VERSION);

    this->ttl = 0;
    this->msg_type = 0;
    this->uid = 0;
    this->reserved = 0;
    this->seqno = 0;
}

batadv_icmp_packet::batadv_icmp_packet(const batadv_icmp_packet& other) : ::inet::BATMANV_ControlPackets(other)
{
    copy(other);
}

batadv_icmp_packet::~batadv_icmp_packet()
{
}

batadv_icmp_packet& batadv_icmp_packet::operator=(const batadv_icmp_packet& other)
{
    if (this==&other) return *this;
    ::inet::BATMANV_ControlPackets::operator=(other);
    copy(other);
    return *this;
}

void batadv_icmp_packet::copy(const batadv_icmp_packet& other)
{
    this->ttl = other.ttl;
    this->msg_type = other.msg_type;
    this->dst = other.dst;
    this->orig = other.orig;
    this->uid = other.uid;
    this->reserved = other.reserved;
    this->seqno = other.seqno;
}

void batadv_icmp_packet::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::BATMANV_ControlPackets::parsimPack(b);
    doParsimPacking(b,this->ttl);
    doParsimPacking(b,this->msg_type);
    doParsimPacking(b,this->dst);
    doParsimPacking(b,this->orig);
    doParsimPacking(b,this->uid);
    doParsimPacking(b,this->reserved);
    doParsimPacking(b,this->seqno);
}

void batadv_icmp_packet::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::BATMANV_ControlPackets::parsimUnpack(b);
    doParsimUnpacking(b,this->ttl);
    doParsimUnpacking(b,this->msg_type);
    doParsimUnpacking(b,this->dst);
    doParsimUnpacking(b,this->orig);
    doParsimUnpacking(b,this->uid);
    doParsimUnpacking(b,this->reserved);
    doParsimUnpacking(b,this->seqno);
}

uint8_t batadv_icmp_packet::getTtl() const
{
    return this->ttl;
}

void batadv_icmp_packet::setTtl(uint8_t ttl)
{
    this->ttl = ttl;
}

uint8_t batadv_icmp_packet::getMsg_type() const
{
    return this->msg_type;
}

void batadv_icmp_packet::setMsg_type(uint8_t msg_type)
{
    this->msg_type = msg_type;
}

MACAddress& batadv_icmp_packet::getDst()
{
    return this->dst;
}

void batadv_icmp_packet::setDst(const MACAddress& dst)
{
    this->dst = dst;
}

MACAddress& batadv_icmp_packet::getOrig()
{
    return this->orig;
}

void batadv_icmp_packet::setOrig(const MACAddress& orig)
{
    this->orig = orig;
}

uint8_t batadv_icmp_packet::getUid() const
{
    return this->uid;
}

void batadv_icmp_packet::setUid(uint8_t uid)
{
    this->uid = uid;
}

uint8_t batadv_icmp_packet::getReserved() const
{
    return this->reserved;
}

void batadv_icmp_packet::setReserved(uint8_t reserved)
{
    this->reserved = reserved;
}

uint32_t batadv_icmp_packet::getSeqno() const
{
    return this->seqno;
}

void batadv_icmp_packet::setSeqno(uint32_t seqno)
{
    this->seqno = seqno;
}

class batadv_icmp_packetDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    batadv_icmp_packetDescriptor();
    virtual ~batadv_icmp_packetDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(batadv_icmp_packetDescriptor)

batadv_icmp_packetDescriptor::batadv_icmp_packetDescriptor() : omnetpp::cClassDescriptor("inet::batadv_icmp_packet", "inet::BATMANV_ControlPackets")
{
    propertynames = nullptr;
}

batadv_icmp_packetDescriptor::~batadv_icmp_packetDescriptor()
{
    delete[] propertynames;
}

bool batadv_icmp_packetDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<batadv_icmp_packet *>(obj)!=nullptr;
}

const char **batadv_icmp_packetDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *batadv_icmp_packetDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int batadv_icmp_packetDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 7+basedesc->getFieldCount() : 7;
}

unsigned int batadv_icmp_packetDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<7) ? fieldTypeFlags[field] : 0;
}

const char *batadv_icmp_packetDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "ttl",
        "msg_type",
        "dst",
        "orig",
        "uid",
        "reserved",
        "seqno",
    };
    return (field>=0 && field<7) ? fieldNames[field] : nullptr;
}

int batadv_icmp_packetDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='t' && strcmp(fieldName, "ttl")==0) return base+0;
    if (fieldName[0]=='m' && strcmp(fieldName, "msg_type")==0) return base+1;
    if (fieldName[0]=='d' && strcmp(fieldName, "dst")==0) return base+2;
    if (fieldName[0]=='o' && strcmp(fieldName, "orig")==0) return base+3;
    if (fieldName[0]=='u' && strcmp(fieldName, "uid")==0) return base+4;
    if (fieldName[0]=='r' && strcmp(fieldName, "reserved")==0) return base+5;
    if (fieldName[0]=='s' && strcmp(fieldName, "seqno")==0) return base+6;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *batadv_icmp_packetDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "uint8_t",
        "uint8_t",
        "MACAddress",
        "MACAddress",
        "uint8_t",
        "uint8_t",
        "uint32_t",
    };
    return (field>=0 && field<7) ? fieldTypeStrings[field] : nullptr;
}

const char **batadv_icmp_packetDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *batadv_icmp_packetDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int batadv_icmp_packetDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    batadv_icmp_packet *pp = (batadv_icmp_packet *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *batadv_icmp_packetDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_icmp_packet *pp = (batadv_icmp_packet *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string batadv_icmp_packetDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_icmp_packet *pp = (batadv_icmp_packet *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getTtl());
        case 1: return ulong2string(pp->getMsg_type());
        case 2: {std::stringstream out; out << pp->getDst(); return out.str();}
        case 3: {std::stringstream out; out << pp->getOrig(); return out.str();}
        case 4: return ulong2string(pp->getUid());
        case 5: return ulong2string(pp->getReserved());
        case 6: return ulong2string(pp->getSeqno());
        default: return "";
    }
}

bool batadv_icmp_packetDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    batadv_icmp_packet *pp = (batadv_icmp_packet *)object; (void)pp;
    switch (field) {
        case 0: pp->setTtl(string2ulong(value)); return true;
        case 1: pp->setMsg_type(string2ulong(value)); return true;
        case 4: pp->setUid(string2ulong(value)); return true;
        case 5: pp->setReserved(string2ulong(value)); return true;
        case 6: pp->setSeqno(string2ulong(value)); return true;
        default: return false;
    }
}

const char *batadv_icmp_packetDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 2: return omnetpp::opp_typename(typeid(MACAddress));
        case 3: return omnetpp::opp_typename(typeid(MACAddress));
        default: return nullptr;
    };
}

void *batadv_icmp_packetDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    batadv_icmp_packet *pp = (batadv_icmp_packet *)object; (void)pp;
    switch (field) {
        case 2: return (void *)(&pp->getDst()); break;
        case 3: return (void *)(&pp->getOrig()); break;
        default: return nullptr;
    }
}

Register_Class(batadv_icmp_tp_packet)

batadv_icmp_tp_packet::batadv_icmp_tp_packet(const char *name) : ::inet::batadv_icmp_packet(name)
{
    this->subtype = 0;
    for (unsigned int i=0; i<2; i++)
        this->session[i] = 0;
    this->timestamp = 0;
}

batadv_icmp_tp_packet::batadv_icmp_tp_packet(const batadv_icmp_tp_packet& other) : ::inet::batadv_icmp_packet(other)
{
    copy(other);
}

batadv_icmp_tp_packet::~batadv_icmp_tp_packet()
{
}

batadv_icmp_tp_packet& batadv_icmp_tp_packet::operator=(const batadv_icmp_tp_packet& other)
{
    if (this==&other) return *this;
    ::inet::batadv_icmp_packet::operator=(other);
    copy(other);
    return *this;
}

void batadv_icmp_tp_packet::copy(const batadv_icmp_tp_packet& other)
{
    this->subtype = other.subtype;
    for (unsigned int i=0; i<2; i++)
        this->session[i] = other.session[i];
    this->timestamp = other.timestamp;
}

void batadv_icmp_tp_packet::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::batadv_icmp_packet::parsimPack(b);
    doParsimPacking(b,this->subtype);
    doParsimArrayPacking(b,this->session,2);
    doParsimPacking(b,this->timestamp);
}

void batadv_icmp_tp_packet::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::batadv_icmp_packet::parsimUnpack(b);
    doParsimUnpacking(b,this->subtype);
    doParsimArrayUnpacking(b,this->session,2);
    doParsimUnpacking(b,this->timestamp);
}

uint8_t batadv_icmp_tp_packet::getSubtype() const
{
    return this->subtype;
}

void batadv_icmp_tp_packet::setSubtype(uint8_t subtype)
{
    this->subtype = subtype;
}

unsigned int batadv_icmp_tp_packet::getSessionArraySize() const
{
    return 2;
}

uint8_t batadv_icmp_tp_packet::getSession(unsigned int k) const
{
    if (k>=2) throw omnetpp::cRuntimeError("Array of size 2 indexed by %lu", (unsigned long)k);
    return this->session[k];
}

void batadv_icmp_tp_packet::setSession(unsigned int k, uint8_t session)
{
    if (k>=2) throw omnetpp::cRuntimeError("Array of size 2 indexed by %lu", (unsigned long)k);
    this->session[k] = session;
}

::omnetpp::simtime_t batadv_icmp_tp_packet::getTimestamp() const
{
    return this->timestamp;
}

void batadv_icmp_tp_packet::setTimestamp(::omnetpp::simtime_t timestamp)
{
    this->timestamp = timestamp;
}

class batadv_icmp_tp_packetDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    batadv_icmp_tp_packetDescriptor();
    virtual ~batadv_icmp_tp_packetDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(batadv_icmp_tp_packetDescriptor)

batadv_icmp_tp_packetDescriptor::batadv_icmp_tp_packetDescriptor() : omnetpp::cClassDescriptor("inet::batadv_icmp_tp_packet", "inet::batadv_icmp_packet")
{
    propertynames = nullptr;
}

batadv_icmp_tp_packetDescriptor::~batadv_icmp_tp_packetDescriptor()
{
    delete[] propertynames;
}

bool batadv_icmp_tp_packetDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<batadv_icmp_tp_packet *>(obj)!=nullptr;
}

const char **batadv_icmp_tp_packetDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *batadv_icmp_tp_packetDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int batadv_icmp_tp_packetDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount() : 3;
}

unsigned int batadv_icmp_tp_packetDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *batadv_icmp_tp_packetDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "subtype",
        "session",
        "timestamp",
    };
    return (field>=0 && field<3) ? fieldNames[field] : nullptr;
}

int batadv_icmp_tp_packetDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "subtype")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "session")==0) return base+1;
    if (fieldName[0]=='t' && strcmp(fieldName, "timestamp")==0) return base+2;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *batadv_icmp_tp_packetDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "uint8_t",
        "uint8_t",
        "simtime_t",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : nullptr;
}

const char **batadv_icmp_tp_packetDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *batadv_icmp_tp_packetDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int batadv_icmp_tp_packetDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    batadv_icmp_tp_packet *pp = (batadv_icmp_tp_packet *)object; (void)pp;
    switch (field) {
        case 1: return 2;
        default: return 0;
    }
}

const char *batadv_icmp_tp_packetDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_icmp_tp_packet *pp = (batadv_icmp_tp_packet *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string batadv_icmp_tp_packetDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_icmp_tp_packet *pp = (batadv_icmp_tp_packet *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getSubtype());
        case 1: return ulong2string(pp->getSession(i));
        case 2: return simtime2string(pp->getTimestamp());
        default: return "";
    }
}

bool batadv_icmp_tp_packetDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    batadv_icmp_tp_packet *pp = (batadv_icmp_tp_packet *)object; (void)pp;
    switch (field) {
        case 0: pp->setSubtype(string2ulong(value)); return true;
        case 1: pp->setSession(i,string2ulong(value)); return true;
        case 2: pp->setTimestamp(string2simtime(value)); return true;
        default: return false;
    }
}

const char *batadv_icmp_tp_packetDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *batadv_icmp_tp_packetDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    batadv_icmp_tp_packet *pp = (batadv_icmp_tp_packet *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(batadv_icmp_packet_rr)

batadv_icmp_packet_rr::batadv_icmp_packet_rr(const char *name) : ::inet::batadv_icmp_packet(name)
{
    this->rr_cur = 0;
}

batadv_icmp_packet_rr::batadv_icmp_packet_rr(const batadv_icmp_packet_rr& other) : ::inet::batadv_icmp_packet(other)
{
    copy(other);
}

batadv_icmp_packet_rr::~batadv_icmp_packet_rr()
{
}

batadv_icmp_packet_rr& batadv_icmp_packet_rr::operator=(const batadv_icmp_packet_rr& other)
{
    if (this==&other) return *this;
    ::inet::batadv_icmp_packet::operator=(other);
    copy(other);
    return *this;
}

void batadv_icmp_packet_rr::copy(const batadv_icmp_packet_rr& other)
{
    this->rr_cur = other.rr_cur;
    for (unsigned int i=0; i<BATADV_RR_LEN; i++)
        this->rr[i] = other.rr[i];
}

void batadv_icmp_packet_rr::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::batadv_icmp_packet::parsimPack(b);
    doParsimPacking(b,this->rr_cur);
    doParsimArrayPacking(b,this->rr,BATADV_RR_LEN);
}

void batadv_icmp_packet_rr::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::batadv_icmp_packet::parsimUnpack(b);
    doParsimUnpacking(b,this->rr_cur);
    doParsimArrayUnpacking(b,this->rr,BATADV_RR_LEN);
}

uint8_t batadv_icmp_packet_rr::getRr_cur() const
{
    return this->rr_cur;
}

void batadv_icmp_packet_rr::setRr_cur(uint8_t rr_cur)
{
    this->rr_cur = rr_cur;
}

unsigned int batadv_icmp_packet_rr::getRrArraySize() const
{
    return BATADV_RR_LEN;
}

MACAddress& batadv_icmp_packet_rr::getRr(unsigned int k)
{
    if (k>=BATADV_RR_LEN) throw omnetpp::cRuntimeError("Array of size BATADV_RR_LEN indexed by %lu", (unsigned long)k);
    return this->rr[k];
}

void batadv_icmp_packet_rr::setRr(unsigned int k, const MACAddress& rr)
{
    if (k>=BATADV_RR_LEN) throw omnetpp::cRuntimeError("Array of size BATADV_RR_LEN indexed by %lu", (unsigned long)k);
    this->rr[k] = rr;
}

class batadv_icmp_packet_rrDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    batadv_icmp_packet_rrDescriptor();
    virtual ~batadv_icmp_packet_rrDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(batadv_icmp_packet_rrDescriptor)

batadv_icmp_packet_rrDescriptor::batadv_icmp_packet_rrDescriptor() : omnetpp::cClassDescriptor("inet::batadv_icmp_packet_rr", "inet::batadv_icmp_packet")
{
    propertynames = nullptr;
}

batadv_icmp_packet_rrDescriptor::~batadv_icmp_packet_rrDescriptor()
{
    delete[] propertynames;
}

bool batadv_icmp_packet_rrDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<batadv_icmp_packet_rr *>(obj)!=nullptr;
}

const char **batadv_icmp_packet_rrDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *batadv_icmp_packet_rrDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int batadv_icmp_packet_rrDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount() : 2;
}

unsigned int batadv_icmp_packet_rrDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISCOMPOUND,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *batadv_icmp_packet_rrDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "rr_cur",
        "rr",
    };
    return (field>=0 && field<2) ? fieldNames[field] : nullptr;
}

int batadv_icmp_packet_rrDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='r' && strcmp(fieldName, "rr_cur")==0) return base+0;
    if (fieldName[0]=='r' && strcmp(fieldName, "rr")==0) return base+1;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *batadv_icmp_packet_rrDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "uint8_t",
        "MACAddress",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : nullptr;
}

const char **batadv_icmp_packet_rrDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *batadv_icmp_packet_rrDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int batadv_icmp_packet_rrDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    batadv_icmp_packet_rr *pp = (batadv_icmp_packet_rr *)object; (void)pp;
    switch (field) {
        case 1: return BATADV_RR_LEN;
        default: return 0;
    }
}

const char *batadv_icmp_packet_rrDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_icmp_packet_rr *pp = (batadv_icmp_packet_rr *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string batadv_icmp_packet_rrDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_icmp_packet_rr *pp = (batadv_icmp_packet_rr *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getRr_cur());
        case 1: {std::stringstream out; out << pp->getRr(i); return out.str();}
        default: return "";
    }
}

bool batadv_icmp_packet_rrDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    batadv_icmp_packet_rr *pp = (batadv_icmp_packet_rr *)object; (void)pp;
    switch (field) {
        case 0: pp->setRr_cur(string2ulong(value)); return true;
        default: return false;
    }
}

const char *batadv_icmp_packet_rrDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 1: return omnetpp::opp_typename(typeid(MACAddress));
        default: return nullptr;
    };
}

void *batadv_icmp_packet_rrDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    batadv_icmp_packet_rr *pp = (batadv_icmp_packet_rr *)object; (void)pp;
    switch (field) {
        case 1: return (void *)(&pp->getRr(i)); break;
        default: return nullptr;
    }
}

Register_Class(batadv_unicast_tvlv_packet)

batadv_unicast_tvlv_packet::batadv_unicast_tvlv_packet(const char *name) : ::inet::BATMANV_ControlPackets(name)
{
    this->setBatadv_packettype(BATADV_UNICAST_TVLV);
    this->setBatadv_version(BATADV_COMPAT_VERSION);

    this->ttl = 0;
    this->reserved = 0;
    this->tvlv_len = 0;
    this->align = 0;
}

batadv_unicast_tvlv_packet::batadv_unicast_tvlv_packet(const batadv_unicast_tvlv_packet& other) : ::inet::BATMANV_ControlPackets(other)
{
    copy(other);
}

batadv_unicast_tvlv_packet::~batadv_unicast_tvlv_packet()
{
}

batadv_unicast_tvlv_packet& batadv_unicast_tvlv_packet::operator=(const batadv_unicast_tvlv_packet& other)
{
    if (this==&other) return *this;
    ::inet::BATMANV_ControlPackets::operator=(other);
    copy(other);
    return *this;
}

void batadv_unicast_tvlv_packet::copy(const batadv_unicast_tvlv_packet& other)
{
    this->ttl = other.ttl;
    this->reserved = other.reserved;
    this->dst = other.dst;
    this->src = other.src;
    this->tvlv_len = other.tvlv_len;
    this->align = other.align;
}

void batadv_unicast_tvlv_packet::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::BATMANV_ControlPackets::parsimPack(b);
    doParsimPacking(b,this->ttl);
    doParsimPacking(b,this->reserved);
    doParsimPacking(b,this->dst);
    doParsimPacking(b,this->src);
    doParsimPacking(b,this->tvlv_len);
    doParsimPacking(b,this->align);
}

void batadv_unicast_tvlv_packet::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::BATMANV_ControlPackets::parsimUnpack(b);
    doParsimUnpacking(b,this->ttl);
    doParsimUnpacking(b,this->reserved);
    doParsimUnpacking(b,this->dst);
    doParsimUnpacking(b,this->src);
    doParsimUnpacking(b,this->tvlv_len);
    doParsimUnpacking(b,this->align);
}

uint8_t batadv_unicast_tvlv_packet::getTtl() const
{
    return this->ttl;
}

void batadv_unicast_tvlv_packet::setTtl(uint8_t ttl)
{
    this->ttl = ttl;
}

uint8_t batadv_unicast_tvlv_packet::getReserved() const
{
    return this->reserved;
}

void batadv_unicast_tvlv_packet::setReserved(uint8_t reserved)
{
    this->reserved = reserved;
}

MACAddress& batadv_unicast_tvlv_packet::getDst()
{
    return this->dst;
}

void batadv_unicast_tvlv_packet::setDst(const MACAddress& dst)
{
    this->dst = dst;
}

MACAddress& batadv_unicast_tvlv_packet::getSrc()
{
    return this->src;
}

void batadv_unicast_tvlv_packet::setSrc(const MACAddress& src)
{
    this->src = src;
}

uint16_t batadv_unicast_tvlv_packet::getTvlv_len() const
{
    return this->tvlv_len;
}

void batadv_unicast_tvlv_packet::setTvlv_len(uint16_t tvlv_len)
{
    this->tvlv_len = tvlv_len;
}

uint16_t batadv_unicast_tvlv_packet::getAlign() const
{
    return this->align;
}

void batadv_unicast_tvlv_packet::setAlign(uint16_t align)
{
    this->align = align;
}

class batadv_unicast_tvlv_packetDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    batadv_unicast_tvlv_packetDescriptor();
    virtual ~batadv_unicast_tvlv_packetDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(batadv_unicast_tvlv_packetDescriptor)

batadv_unicast_tvlv_packetDescriptor::batadv_unicast_tvlv_packetDescriptor() : omnetpp::cClassDescriptor("inet::batadv_unicast_tvlv_packet", "inet::BATMANV_ControlPackets")
{
    propertynames = nullptr;
}

batadv_unicast_tvlv_packetDescriptor::~batadv_unicast_tvlv_packetDescriptor()
{
    delete[] propertynames;
}

bool batadv_unicast_tvlv_packetDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<batadv_unicast_tvlv_packet *>(obj)!=nullptr;
}

const char **batadv_unicast_tvlv_packetDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *batadv_unicast_tvlv_packetDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int batadv_unicast_tvlv_packetDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 6+basedesc->getFieldCount() : 6;
}

unsigned int batadv_unicast_tvlv_packetDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<6) ? fieldTypeFlags[field] : 0;
}

const char *batadv_unicast_tvlv_packetDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "ttl",
        "reserved",
        "dst",
        "src",
        "tvlv_len",
        "align",
    };
    return (field>=0 && field<6) ? fieldNames[field] : nullptr;
}

int batadv_unicast_tvlv_packetDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='t' && strcmp(fieldName, "ttl")==0) return base+0;
    if (fieldName[0]=='r' && strcmp(fieldName, "reserved")==0) return base+1;
    if (fieldName[0]=='d' && strcmp(fieldName, "dst")==0) return base+2;
    if (fieldName[0]=='s' && strcmp(fieldName, "src")==0) return base+3;
    if (fieldName[0]=='t' && strcmp(fieldName, "tvlv_len")==0) return base+4;
    if (fieldName[0]=='a' && strcmp(fieldName, "align")==0) return base+5;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *batadv_unicast_tvlv_packetDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "uint8_t",
        "uint8_t",
        "MACAddress",
        "MACAddress",
        "uint16_t",
        "uint16_t",
    };
    return (field>=0 && field<6) ? fieldTypeStrings[field] : nullptr;
}

const char **batadv_unicast_tvlv_packetDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *batadv_unicast_tvlv_packetDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int batadv_unicast_tvlv_packetDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    batadv_unicast_tvlv_packet *pp = (batadv_unicast_tvlv_packet *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *batadv_unicast_tvlv_packetDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_unicast_tvlv_packet *pp = (batadv_unicast_tvlv_packet *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string batadv_unicast_tvlv_packetDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    batadv_unicast_tvlv_packet *pp = (batadv_unicast_tvlv_packet *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getTtl());
        case 1: return ulong2string(pp->getReserved());
        case 2: {std::stringstream out; out << pp->getDst(); return out.str();}
        case 3: {std::stringstream out; out << pp->getSrc(); return out.str();}
        case 4: return ulong2string(pp->getTvlv_len());
        case 5: return ulong2string(pp->getAlign());
        default: return "";
    }
}

bool batadv_unicast_tvlv_packetDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    batadv_unicast_tvlv_packet *pp = (batadv_unicast_tvlv_packet *)object; (void)pp;
    switch (field) {
        case 0: pp->setTtl(string2ulong(value)); return true;
        case 1: pp->setReserved(string2ulong(value)); return true;
        case 4: pp->setTvlv_len(string2ulong(value)); return true;
        case 5: pp->setAlign(string2ulong(value)); return true;
        default: return false;
    }
}

const char *batadv_unicast_tvlv_packetDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 2: return omnetpp::opp_typename(typeid(MACAddress));
        case 3: return omnetpp::opp_typename(typeid(MACAddress));
        default: return nullptr;
    };
}

void *batadv_unicast_tvlv_packetDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    batadv_unicast_tvlv_packet *pp = (batadv_unicast_tvlv_packet *)object; (void)pp;
    switch (field) {
        case 2: return (void *)(&pp->getDst()); break;
        case 3: return (void *)(&pp->getSrc()); break;
        default: return nullptr;
    }
}

} // namespace inet

