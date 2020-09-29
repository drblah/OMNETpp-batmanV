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

#include "../../Batman5/controlpackages/BATMANV_nc_coded_packet.h"

#include <iostream>
#include <sstream>


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

//Register_Class(batadv_nc_packet)

batadv_nc_packet::batadv_nc_packet(const char *name) //: ::omnetpp::cPacket(name)
{
    this->packet_id = 0;
    this->timestamp = 0;
    this->deleteUniPacket = false;
    this->neigh_node = nullptr;
    this->uni_packet = nullptr;
    this->nc_path = nullptr;
}

batadv_nc_packet::batadv_nc_packet(const batadv_nc_packet& other) //: ::omnetpp::cPacket(other)
{
    copy(other);
}

batadv_nc_packet::~batadv_nc_packet()
{
    if(this->uni_packet != nullptr){
        std::cout << "Deleting uni_packet in deleted nc_packet!" << std::endl;
        delete this->uni_packet;
    }
    else{
        std::cout << "uni_packet == nullptr in deleted nc_packet!" << std::endl;
    }
}

batadv_nc_packet& batadv_nc_packet::operator=(const batadv_nc_packet& other)
{
    if (this==&other) return *this;
    //::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void batadv_nc_packet::copy(const batadv_nc_packet& other)
{
    this->packet_id = other.packet_id;
    this->deleteUniPacket = other.deleteUniPacket;
    this->timestamp = other.timestamp;
    this->neigh_node = other.neigh_node;
    this->uni_packet = other.uni_packet;
    this->nc_path = other.nc_path;
}

//void batadv_nc_packet::parsimPack(omnetpp::cCommBuffer *b) const
//{
//    ::omnetpp::cPacket::parsimPack(b);
//    doParsimPacking(b,this->packet_id);
//    doParsimPacking(b,this->timestamp);
//    doParsimPacking(b,this->neigh_node);
//    doParsimPacking(b,this->uni_packet);
//    doParsimPacking(b,this->nc_path);
//}
//
//void batadv_nc_packet::parsimUnpack(omnetpp::cCommBuffer *b)
//{
//    ::omnetpp::cPacket::parsimUnpack(b);
//    doParsimUnpacking(b,this->packet_id);
//    doParsimUnpacking(b,this->timestamp);
//    doParsimUnpacking(b,this->neigh_node);
//    doParsimUnpacking(b,this->uni_packet);
//    doParsimUnpacking(b,this->nc_path);
//}

uint32_t batadv_nc_packet::getPacket_id(){
    return this->packet_id;
}
void batadv_nc_packet::setPacket_id(uint32_t packet_id){
    this->packet_id = packet_id;
}

bool batadv_nc_packet::getDeleteUniPacket(){
    return this->deleteUniPacket;
}

void batadv_nc_packet::setDeleteUniPacket(bool deletePacket){
    this->deleteUniPacket = deletePacket;
}

simtime_t batadv_nc_packet::getTimestamp(){
    return this->timestamp;
}
void batadv_nc_packet::setTimestamp(simtime_t timestamp){
    this->timestamp = timestamp;
}

Neigh_node_ptr batadv_nc_packet::getNeigh_node(){
    return this->neigh_node;
}
void batadv_nc_packet::setNeigh_node(Neigh_node_ptr neigh_node){
    this->neigh_node = neigh_node;
}

batadv_unicast_packet *batadv_nc_packet::getUni_packet(){
    return this->uni_packet;
}
void batadv_nc_packet::setUni_packet(batadv_unicast_packet *uni_packet){
    this->uni_packet = uni_packet;
}

NC_path_ptr batadv_nc_packet::getNc_path(){
    return this->nc_path;
}
void batadv_nc_packet::setNc_path(NC_path_ptr nc_path){
    this->nc_path = nc_path;
}




Register_Class(batadv_coded_packet)

batadv_coded_packet::batadv_coded_packet(const char *name) : ::omnetpp::cPacket(name)
{
    this->setBatadv_packet_type(BATADV_CODED);
    this->setBatadv_version(BATADV_COMPAT_VERSION);
    this->setByteLength(BATADV_CODED_HLEN);

    this->ttl = 0;
    this->first_ttvn = 0;
//    this->first_source = MACAddress("00:00:00:00:00:00");
//    this->first_orig_dest = MACAddress("00:00:00:00:00:00");
    this->first_crc = 0;

//    this->second_dest = MACAddress("00:00:00:00:00:00");
//    this->second_source = MACAddress("00:00:00:00:00:00");
//    this->second_orig_dest = MACAddress("00:00:00:00:00:00");
    this->second_crc = 0;
    this->second_ttl = 0;
    this->second_ttvn = 0;

    this->packet1 = nullptr;
    this->packet2 = nullptr;
    this->coded_len = 0;
}

batadv_coded_packet::batadv_coded_packet(const batadv_coded_packet& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

batadv_coded_packet::~batadv_coded_packet(){}

batadv_coded_packet& batadv_coded_packet::operator=(const batadv_coded_packet& other)
{
    if (this==&other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void batadv_coded_packet::copy(const batadv_coded_packet& other)
{
    this->ttl = other.ttl;
    this->first_ttvn = other.first_ttvn;
    this->first_source = other.first_source;
    this->first_orig_dest = other.first_orig_dest;
    this->first_crc = 0;

    this->second_dest = other.second_dest;
    this->second_source = other.second_source;
    this->second_orig_dest = other.second_orig_dest;
    this->second_crc = other.second_crc;
    this->second_ttl = other.second_ttl;
    this->second_ttvn = other.second_ttvn;

    this->packet1 = other.packet1;
    this->packet2 = other.packet2;
    this->coded_len = other.coded_len;
}

void batadv_coded_packet::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->ttl);
    doParsimPacking(b,this->first_ttvn);
    doParsimPacking(b,this->first_source);
    doParsimPacking(b,this->first_orig_dest);
    doParsimPacking(b,this->first_crc);
    doParsimPacking(b,this->second_dest);
    doParsimPacking(b,this->second_source);
    doParsimPacking(b,this->second_orig_dest);
    doParsimPacking(b,this->second_crc);
    doParsimPacking(b,this->second_ttl);
    doParsimPacking(b,this->second_ttvn);
    doParsimPacking(b,this->packet1);
    doParsimPacking(b,this->packet2);
    doParsimPacking(b,this->coded_len);
}

void batadv_coded_packet::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->ttl);
    doParsimUnpacking(b,this->first_ttvn);
    doParsimUnpacking(b,this->first_source);
    doParsimUnpacking(b,this->first_orig_dest);
    doParsimUnpacking(b,this->first_crc);
    doParsimUnpacking(b,this->second_dest);
    doParsimUnpacking(b,this->second_source);
    doParsimUnpacking(b,this->second_orig_dest);
    doParsimUnpacking(b,this->second_crc);
    doParsimUnpacking(b,this->second_ttl);
    doParsimUnpacking(b,this->second_ttvn);
    doParsimUnpacking(b,this->packet1);
    doParsimUnpacking(b,this->packet2);
    doParsimUnpacking(b,this->coded_len);
}

uint8_t batadv_coded_packet::getBatadv_packet_type(){
    return this->batadv_packet_type;
}
void batadv_coded_packet::setBatadv_packet_type(uint8_t batadv_packet_type){
    this->batadv_packet_type = batadv_packet_type;
}
uint8_t batadv_coded_packet::getBatadv_version(){
    return this->batadv_version;
}
void batadv_coded_packet::setBatadv_version(uint8_t batadv_version){
    this->batadv_version = batadv_version;
}

uint8_t batadv_coded_packet::getTtl(){
    return this->ttl;
}
void batadv_coded_packet::setTtl(const uint8_t ttl){
    this->ttl = ttl;
}
uint8_t batadv_coded_packet::getFirst_ttvn(){
    return this->first_ttvn;
}
void batadv_coded_packet::setFirst_ttvn(const uint8_t first_ttvn){
    this->first_ttvn = first_ttvn;
}

MACAddress& batadv_coded_packet::getFirst_source()
{
    return this->first_source;
}
void batadv_coded_packet::setFirst_source(const MACAddress& first_source)
{
    this->first_source = first_source;
}
MACAddress& batadv_coded_packet::getFirst_orig_dest()
{
    return this->first_orig_dest;
}
void batadv_coded_packet::setFirst_orig_dest(const MACAddress& first_orig_dest)
{
    this->first_orig_dest = first_orig_dest;
}

MACAddress& batadv_coded_packet::getSecond_source()
{
    return this->second_source;
}
void batadv_coded_packet::setSecond_source(const MACAddress& second_source)
{
    this->second_source = second_source;
}
MACAddress& batadv_coded_packet::getSecond_dest()
{
    return this->second_dest;
}
void batadv_coded_packet::setSecond_dest(const MACAddress& second_dest)
{
    this->second_dest = second_dest;
}
MACAddress& batadv_coded_packet::getSecond_orig_dest()
{
    return this->second_orig_dest;
}
void batadv_coded_packet::setSecond_orig_dest(const MACAddress& second_orig_dest)
{
    this->second_orig_dest = second_orig_dest;
}

uint8_t batadv_coded_packet::getSecond_ttl(){
    return this->second_ttl;
}
void batadv_coded_packet::setSecond_ttl(const uint8_t second_ttl){
    this->second_ttl = second_ttl;
}
uint8_t batadv_coded_packet::getSecond_ttvn(){
    return this->second_ttvn;
}
void batadv_coded_packet::setSecond_ttvn(const uint8_t second_ttvn){
    this->second_ttvn = second_ttvn;
}

uint32_t batadv_coded_packet::getFirst_crc() const
{
    return this->first_crc;
}
void batadv_coded_packet::setFirst_crc(uint32_t first_crc)
{
    this->first_crc = first_crc;
}
uint32_t batadv_coded_packet::getSecond_crc() const
{
    return this->second_crc;
}
void batadv_coded_packet::setSecond_crc(uint32_t second_crc)
{
    this->second_crc = second_crc;
}

batadv_unicast_packet *batadv_coded_packet::getPacket1()
{
    return this->packet1;
}
void batadv_coded_packet::setPacket1(batadv_unicast_packet *packet1)
{
    this->packet1 = packet1;
}
batadv_unicast_packet *batadv_coded_packet::getPacket2(){
    return this->packet2;
}
void batadv_coded_packet::setPacket2(batadv_unicast_packet *packet2){
    this->packet2 = packet2;
}

uint32_t batadv_coded_packet::getCoded_len() const{
    return this->coded_len;
}
void batadv_coded_packet::setCoded_len(uint32_t coded_len){
    this->coded_len = coded_len;
}

} /* namespace inet */
