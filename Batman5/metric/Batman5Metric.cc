/*
 * metric.cpp
 *
 */

#include "Batman5Metric.h"

#include "../../Batman5/routing/Batman5Routing.h"

namespace inet {

/*
 * Re-maps a number x from one range to another
 * From: https://www.arduino.cc/reference/en/language/functions/math/map/
 */
uint32_t Batman5Routing::map(double x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max)
{
//    cout << "x: " << x << ", in_min: " << in_min << ", in_max: " << in_max << ", out_min: " << out_min << ", out_max: " << out_max << " => " << (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min << endl;
    return ((x - in_min) * (out_max - out_min) / (in_max - in_min)) + out_min;
}
double Batman5Routing::map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max)
{
//    cout << "x: " << x << ", in_min: " << in_min << ", in_max: " << in_max << ", out_min: " << out_min << ", out_max: " << out_max << " => " << (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min << endl;
    return ((double)((x - in_min) * (out_max - out_min)) / (in_max - in_min)) + out_min;
}

void Batman5Routing::initializeMetric(){

    /**
     * Check if metricType is a known type
     * if not set it to default hopPenalty
     */

    this->metricType = par("metricType").stdstringValue();

    if(metricType.compare("throughput_overwrite") == 0){
        return;
    }
    else if(metricType.compare("throughputMetric") == 0){
        return;
    }
    else if(metricType.compare("distanceMetric") == 0){
        this->realValues = par("realValues");
        this->distance_alpha = par("distance_alpha");
        return;
    }
    else if(metricType.compare("predictiveMobilityMetric") == 0){
        this->secondsInTheFuture = par("predictionWidth");
        this->realValues = par("realValues");
        this->distance_alpha = par("distance_alpha");
//        std::cout << secondsInTheFuture << " " << realValues << std::endl;
        return;
    }
    else if(metricType.compare("BATMobile") == 0){
        this->secondsInTheFuture = par("predictionWidth");
        this->realValues = par("realValues");
        this->distance_alpha = par("distance_alpha");
//        std::cout << secondsInTheFuture << " " << realValues << std::endl;
        return;
    }
    else{
        std::cout << "metricType " << metricType << " is not known! Using default metricType!" << std::endl;
        this->metricType = "hopPenalty";
        return;
    }

}//initializeMetric

void Batman5Routing::ewma_metric_init(ewma_Metric *avg){
    avg->factor = log2(10);
    avg->weight = log2(8);
    avg->internal = 0; // this is the metric value
}

/*
 * Determine Linkmetric
 */
uint32_t Batman5Routing::getLinkMetric(Hardif_neigh_node_ptr neigh){

    /**
     * Here the link metric to a neighbor node is determined
     * this function will be called in order of the ELP
     * batadv_v_elp_throughput_metric_update leads to this method
     *
     * Call the calculation matching the metricType
     */
    if(metricType.compare("throughput_overwrite") == 0){
        return batadv_v_elp_get_throughput(neigh);
    }
    else if(metricType.compare("throughputMetric") == 0){
        return batadv_v_elp_get_throughput(neigh);
    }
    else if(metricType.compare("distanceMetric") == 0){
        return getDistanceMetric(neigh->addr);
    }/*
    else if(metricType.compare("predictiveMobilityMetric") == 0){
            return getPredictiveMobilityMetric(neigh->addr);
    }
    else if(metricType.compare("BATMobile") == 0){
        return getScoredMetric(neigh->addr);
    }
    */
    else{
        return 0;
    }
}

/*
 * Determine Pathmetric
 */
void Batman5Routing::metricMatchLinkCharacteristicts(Hardif_neigh_node_ptr hardif_neigh, batadv_ogm2_packet *ogm_packet){

    /**
     * Here the ogm_metric can be configured to match link characteristics
     * e.g. using path_metric or link_metric
     */

    if(metricType.compare("throughput_overwrite") == 0){
        /* Update the received throughput metric to match the link characteristic:
         *  - If this OGM traveled one hop so far (emitted by single hop neighbor)
         *    the path throughput metric equals the link throughput.
         *  - For OGMs traversing more than one hop the path throughput metric is
         *    the smaller of the path throughput and the link throughput. */

        uint32_t link_metric = getEWMAMetric(&hardif_neigh->bat_v->metric);
        uint32_t path_metric = std::min(link_metric, ogm_packet->getMetric());
        ogm_packet->setMetric(path_metric);

    }
    else if(metricType.compare("throughputMetric") == 0){
        //
        uint32_t link_metric = getEWMAMetric(&hardif_neigh->bat_v->metric);
        uint32_t path_metric = std::min(link_metric, ogm_packet->getMetric());
        ogm_packet->setMetric(path_metric);

    }
    else if(metricType.compare("distanceMetric") == 0){

        uint32_t link_metric = ewma_metric_read(&hardif_neigh->bat_v->metric);
        uint32_t path_metric = ogm_packet->getMetric() - link_metric;
//        std::cout << ogm_packet->getMetric() << " - " << link_metric << " = " << path_metric << std::endl;
        ogm_packet->setMetric(path_metric);
    }
    else if(metricType.compare("predictiveMobilityMetric") == 0){

        uint32_t link_metric = ewma_metric_read(&hardif_neigh->bat_v->metric);
        uint32_t path_metric = ogm_packet->getMetric() - link_metric;
//        std::cout << ogm_packet->getMetric() << " - " << link_metric << " = " << path_metric << std::endl;
        ogm_packet->setMetric(path_metric);
    }
    else if(metricType.compare("BATMobile") == 0){

        uint32_t link_metric = getEWMAMetric(&hardif_neigh->bat_v->metric);
        double link_index = map(link_metric,0,BATADV_THROUGHPUT_MAX_VALUE-1,0,1);
        double ogm_index = map(ogm_packet->getMetric(),0,BATADV_THROUGHPUT_MAX_VALUE-1,0,1);
//        std::cout << hostModule->getFullName() << ": " << link_metric << " -> " << link_index << ", " << ogm_packet->getMetric() << " -> " << ogm_index << std::endl;

        double path_index = ogm_index * link_index; // Calculate path-metric

        uint32_t path_metric = map(path_index,0,1,0,BATADV_THROUGHPUT_MAX_VALUE-1);

//        std::cout << hostModule->getFullName() << ": " << ogm_index << " * " << link_index << " = " << path_index << " -> " << path_metric << std::endl;

        ogm_packet->setMetric(path_metric);
    }

}

/*
 * Determine forwarding Pathmetric
 */
uint32_t Batman5Routing::calculatePathMetric(batadv_hard_iface *if_incoming, batadv_hard_iface *if_outgoing, uint32_t metric){

    /**
     *  Here the path_metric is calculated with additional information
     *  and the Metric of the last OGMv2.
     *
     *  Call the calculation matching the metricType
     */

    if(metricType.compare("throughput_overwrite") == 0){
        return batadv_v_forward_penalty(if_incoming, if_outgoing, metric);
    }
    else if(metricType.compare("throughputMetric") == 0){
        return batadv_v_forward_penalty(if_incoming, if_outgoing, metric);
    }
    else if(metricType.compare("distanceMetric") == 0){
        return metric; // no penalty needed, using the direct metric from the ogmv2
    }
    else if(metricType.compare("predictiveMobilityMetric") == 0){
        return metric; // no penalty needed, using the direct metric from the ogmv2
    }
    else if(metricType.compare("BATMobile") == 0){
        return metric;
    }
    else{
        return 0;

    }
}

uint32_t Batman5Routing::getEWMAMetric(ewma_Metric *avg){
    if(metricType.compare("throughputMetric") == 0){
        return ewma_metric_read(avg);
    }
    else if(metricType.compare("distanceMetric") == 0){
        return BATADV_THROUGHPUT_MAX_VALUE - avg->internal;
    }
    else if(metricType.compare("predictiveMobilityMetric") == 0){
        return BATADV_THROUGHPUT_MAX_VALUE - avg->internal;
    }
    else{
        return avg->internal;
    }
}

uint32_t Batman5Routing::ewma_metric_read(ewma_Metric *avg){
    if(metricType.compare("throughputMetric") == 0){
        return avg->internal >> avg->factor;
    }
    else if(metricType.compare("distanceMetric") == 0){
        if(avg->internal == 0){
            // only initial value, no measured value in it for know
            // return the badest possible value for the link metric;
            return (BATADV_THROUGHPUT_MAX_VALUE-1)/ogmv2_ttl;
        }
        else{
            return avg->internal;
        }
    }
    else if(metricType.compare("predictiveMobilityMetric") == 0){
        if(avg->internal == 0){
            // only initial value, no measured value in it for know
            // return the badest possible value for the link metric;
            return (BATADV_THROUGHPUT_MAX_VALUE-1)/ogmv2_ttl;
        }
        else{
            return avg->internal;
        }
    }

}

/**
 * Using the same implementation of EWMA as in batman v
 */
ewma_Metric *Batman5Routing::ewma_metric_add(ewma_Metric *avg, uint32_t value){

//    std::cout << hostModule->getFullName() << ": Adding new value: " << value << " to ewma_Metric: " << getEWMAMetric(avg) << std::endl;
    if(metricType.compare("throughputMetric") == 0){
        avg->internal = avg->internal  ? (((avg->internal << avg->weight) - avg->internal) + (value << avg->factor)) >> avg->weight : (value << avg->factor);
    }
//    else if(metricType.compare("distanceMetric") == 0){
//        avg->internal = avg->internal  ? (((avg->internal << avg->weight) - avg->internal) + (value << avg->factor)) >> avg->weight : (value << avg->factor);
//    }
    else{
        avg->internal = value;
    }
    //    std::cout << " -> " << ewma_throughput_read(avg) << std::endl;
    return avg;
};


void Batman5Routing::addReversePathMetric(Neigh_ifinfo_ptr neigh_ifinfo, uint32_t path_metric){
//    std::cout << hostModule->getFullName() << " Before: ";
//    for (auto it = neigh_ifinfo->bat_v->lastMetricValues.begin(); it != neigh_ifinfo->bat_v->lastMetricValues.end(); it++){
//        std::cout << (*it) << ", ";
//    }
//    std::cout << std::endl;

    auto it = neigh_ifinfo->bat_v->lastMetricValues.begin();
    neigh_ifinfo->bat_v->lastMetricValues.insert(it,path_metric);

    if (neigh_ifinfo->bat_v->lastMetricValues.size() > 8){
        neigh_ifinfo->bat_v->lastMetricValues.pop_back();
    }

//    std::cout << " After: ";
//    for (auto it = neigh_ifinfo->bat_v->lastMetricValues.begin(); it != neigh_ifinfo->bat_v->lastMetricValues.end(); it++){
//        std::cout << (*it) << ", ";
//    }
//    std::cout << std::endl;

//    std::cout << "Vectorsize: " << neigh_ifinfo->bat_v->lastMetricValues.size() << " first value: " << neigh_ifinfo->bat_v->lastMetricValues.at(0) << " path_metric: " << path_metric << std::endl;
}


uint32_t Batman5Routing::getReversePathMetric(Neigh_ifinfo_ptr neigh_ifinfo){

    uint32_t meanMetric = 0;
    uint64_t accumulateMetric = 0;

    auto it_begin = neigh_ifinfo->bat_v->lastMetricValues.begin();
    auto it_end = neigh_ifinfo->bat_v->lastMetricValues.end();
    uint size = neigh_ifinfo->bat_v->lastMetricValues.size();

    for(auto it = it_begin; it != it_end; it++){
        accumulateMetric +=(*it);
    }

    meanMetric = accumulateMetric / size;

//    std::cout << accumulateMetric << " / " << size << " = " << meanMetric << std::endl;

//        std::cout << " Vector + Mean: ";
//        for (auto it = neigh_ifinfo->bat_v->lastMetricValues.begin(); it != neigh_ifinfo->bat_v->lastMetricValues.end(); it++){
//            std::cout << (*it) << ", ";
//        }
//        std::cout << meanMetric << std::endl;

    return meanMetric;
}



metric::metric() {}
metric::~metric() {}

} /* namespace inet */
