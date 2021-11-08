
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {

}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {

}

/**
 * train the model on a given table.
 * @param ts TimeSeries on which we learn
 */
void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {
    int columns = ts.getColumnsSize();
    int rows = ts.getRowsSize();

    // finding correlation
    for (int firstId = 0; firstId < columns; ++firstId) {
        // init to the first
        float maxPearson = 0;
        const std::vector<float> &firstVec = ts.getColumn(firstId);
        int secondIdMax = -1;

        for (int secondId = firstId + 1; secondId < columns; ++secondId) {
            // init to the second
            const std::vector<float> &secondVec = ts.getColumn(secondId);

            // calculating the Pearson Value
            float pearsonValue = fabs(pearson(firstVec, secondVec, rows));

            // switching with max if needed
            if (pearsonValue > maxPearson) {
                maxPearson = pearsonValue;
                secondIdMax = secondId;
            }
        }

        // if not above pearsonThreshold we continue
        if (maxPearson < pearsonThreshold || secondIdMax == -1) {
            continue;
        }

        // ~ ~ second part ~ ~
        const std::vector<float> &secondVec = ts.getColumn(secondIdMax);

        // regression line
        Line regLine = linear_reg(firstVec, secondVec, rows);
        // correlation value
        float maxDev = calculateDeviation(firstVec, secondVec, rows, regLine);
        // adding to vector
        addCF(ts.getColumnName(firstId), ts.getColumnName(secondIdMax), maxPearson, regLine, maxDev);
    }
}

/**
 * calculate max deviation of a given 2 columns which represent x and y values and a line.
 * @param firstVec, secondVec x value and y value of the points as vec.
 * @param size the size of the vectors.
 * @param regLine the line to calculate max deviation.
 * @return the max deviation.
 */
float
SimpleAnomalyDetector::calculateDeviation(const std::vector<float> &firstVec, const std::vector<float> &secondVec,
                                          int size, const Line &regLine) {
    // calc max dev
    float maxDev = 0;
    for (int i = 0; i < size; ++i) {
        Point point(firstVec[i], secondVec[i]);
        float curDev = dev(point, regLine);
        dev(point, regLine);
        if (curDev >= maxDev) {
            maxDev = curDev;
        }
    }
    return maxDev;
}

/**
 * add correlated feature to the cf vector.
 * @param first, second the columns name as string.
 * @param pearson the correlation of the two columns.
 * @param regLine the regression line.
 * @param maxDev the max deviation of the two columns.
 */
void
SimpleAnomalyDetector::addCF(const string &first, const string &second, float pearson, Line regLine, float maxDev) {
    correlatedFeatures correlatedFeature;
    correlatedFeature.corrlation = pearson;
    correlatedFeature.feature1 = first;
    correlatedFeature.feature2 = second;
    correlatedFeature.lin_reg = regLine;
    correlatedFeature.threshold = maxDev * 1.1f;
    cf.push_back(correlatedFeature);
}

/**
 * detect false points on a given table.
 * @param ts the table to detect false points on.
 * @return a vector of all the false points (AnomalyReport).
 */
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) const {
    vector<AnomalyReport> result;

    // for every correlated Feature
    for (const correlatedFeatures &correlatedFeature: cf) {
        const std::vector<float> &firstVec = ts.getColumn(correlatedFeature.feature1);
        const std::vector<float> &secondVec = ts.getColumn(correlatedFeature.feature2);
        int size = ts.getRowsSize();

        Line regLine = correlatedFeature.lin_reg;
        float devThreshold = correlatedFeature.threshold;

        // looking for false points
        for (int i = 0; i < size; ++i) {
            Point point(firstVec[i], secondVec[i]);
            float curDev = dev(point, regLine);

            // if dev is greater than Threshold adding report
            if (curDev >= devThreshold) {
                result.emplace_back(correlatedFeature.feature1 + "-" + correlatedFeature.feature2, i + 1);
            }
        }
    }

    return result;
}
