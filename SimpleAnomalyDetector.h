

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>

struct correlatedFeatures {
    string feature1, feature2;
    float corrlation;
    Line lin_reg;
    float threshold;
};

class SimpleAnomalyDetector : public TimeSeriesAnomalyDetector {
    vector<correlatedFeatures> cf;
    float pearsonThreshold = 0.7;

private:
    static float

    /**
     * Calculate max deviation of a given 2 columns which represent x and y values and a line.
     * @param firstVec, secondVec x value and y value of the points as vec.
     * @param size the size of the vectors.
     * @param regLine the line to calculate max deviation.
     * @return the max deviation.
     */
    calculateDeviation(const std::vector<float> &firstVec, const std::vector<float> &secondVec, int size,
                       const Line &regLine);

    /**
     * Add correlated feature to the correlated features vector.
     * @param firstColumn, secondColumn the columns name as string.
     * @param pearson the correlation of the two columns.
     * @param regLine the regression line.
     * @param maxDev the max deviation of the two columns.
     */
    void addCF(const string &firstColumn, const string &secondColumn, float pearson, Line regLine, float maxDev);

public:

    /**
     * Constructor
     */
    SimpleAnomalyDetector() = default;

    /**
     * Deconstruct
     */
    virtual ~SimpleAnomalyDetector() = default;

    /**
     * Train the model on a given table.
     * @param ts TimeSeries on which we learn
     */
    virtual void learnNormal(const TimeSeries &ts);

    /**
     * Detect false points on a given table.
     * @param ts the table to detect false points on.
     * @return a vector of all the false points (AnomalyReport).
     */
    virtual vector<AnomalyReport> detect(const TimeSeries &ts) const;

    /**
     * @return The correlated feature
     */
    vector<correlatedFeatures> getNormalModel() {
        return cf;
    }


};

#endif /* SIMPLEANOMALYDETECTOR_H_ */
