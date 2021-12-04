

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector : public SimpleAnomalyDetector {
public:
    HybridAnomalyDetector();

    virtual ~HybridAnomalyDetector();

    void addCF(const TimeSeries &ts, const int &firstId, const int &secondId, const float &pearson);

    Point **toPoints(vector<float> x, vector<float> y);

    bool isAnomaly(float x, float y, const correlatedFeatures &cf);
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
