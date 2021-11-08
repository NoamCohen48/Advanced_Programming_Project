
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() = default;

SimpleAnomalyDetector::~SimpleAnomalyDetector() = default;

/**
 * Train the model on a given table.
 * @param ts TimeSeries on which we learn
 */
void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {

    // Get the columns and the rows
    int columns = ts.getColumnsSize();
    int rows = ts.getRowsSize();

    // Finding correlation
    for (int firstId = 0; firstId < columns; ++firstId) {

        // Init to the first
        float maxPearson = 0;
        int secondIdMax = -1;

        const std::vector<float> &firstVec = ts.getColumn(firstId);

        // For every column compare to the others columns
        for (int secondId = firstId + 1; secondId < columns; ++secondId) {

            // Init to the second
            const std::vector<float> &secondVec = ts.getColumn(secondId);

            // Calculating the Pearson Value
            float pearsonValue = fabs(pearson(firstVec, secondVec, rows));

            // Switching with max if needed
            if (pearsonValue > maxPearson) {
                maxPearson = pearsonValue;
                secondIdMax = secondId;
            }
        }

        // If not above pearsonThreshold we continue
        if (maxPearson < pearsonThreshold || secondIdMax == -1) {
            continue;
        }

        // Add the correlated column
        const std::vector<float> &secondVec = ts.getColumn(secondIdMax);

        // Regression line
        Line regLine = linear_reg(firstVec, secondVec, rows);
        // Correlation value
        float maxDev = calculateDeviation(firstVec, secondVec, rows, regLine);
        // Adding to vector
        addCF(ts.getColumnName(firstId), ts.getColumnName(secondIdMax), maxPearson, regLine, maxDev);
    }
}

/**
 * Calculate max deviation of a given 2 columns which represent x and y values and a line.
 * @param firstVec, secondVec x value and y value of the points as vec.
 * @param size the size of the vectors.
 * @param regLine the line to calculate max deviation.
 * @return the max deviation.
 */
float SimpleAnomalyDetector::calculateDeviation(const std::vector<float> &firstVec, const std::vector<float> &secondVec,
                                                int size, const Line &regLine) {
    float maxDev = 0;

    // Run through the line in the columns
    for (int i = 0; i < size; ++i) {
        Point point(firstVec[i], secondVec[i]);

        // Calculate the dev of the point
        float curDev = dev(point, regLine);

        // Save the max dev
        if (curDev >= maxDev) {
            maxDev = curDev;
        }
    }
    return maxDev;
}

/**
 * Add correlated feature to the correlated features vector.
 * @param firstColumn, secondColumn the columns name as string.
 * @param pearson the correlation of the two columns.
 * @param regLine the regression line.
 * @param maxDev the max deviation of the two columns.
 */
void SimpleAnomalyDetector::addCF(const string &firstColumn, const string &secondColumn, float pearson, Line regLine,
                                  float maxDev) {

    correlatedFeatures correlatedFeature;
    correlatedFeature.corrlation = pearson;
    correlatedFeature.feature1 = firstColumn;
    correlatedFeature.feature2 = secondColumn;
    correlatedFeature.lin_reg = regLine;
    correlatedFeature.threshold = maxDev * 1.1f;
    cf.push_back(correlatedFeature);
}

/**
 * Detect false points on a given table.
 * @param ts the table to detect false points on.
 * @return a vector of all the false points (AnomalyReport).
 */
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) const {
    vector<AnomalyReport> result;

    // For every correlated Feature
    for (const correlatedFeatures &correlatedFeature: cf) {

        const std::vector<float> &firstVec = ts.getColumn(correlatedFeature.feature1);
        const std::vector<float> &secondVec = ts.getColumn(correlatedFeature.feature2);
        int size = ts.getRowsSize();

        // Create regLine from correlated feature
        Line regLine = correlatedFeature.lin_reg;
        float devThreshold = correlatedFeature.threshold;

        // Check for false points
        for (int i = 0; i < size; ++i) {
            Point point(firstVec[i], secondVec[i]);
            float curDev = dev(point, regLine);

            // If dev is greater than Threshold, add the anomaly report
            if (curDev >= devThreshold) {
                result.emplace_back(correlatedFeature.feature1 + "-" + correlatedFeature.feature2, i + 1);
            }
        }
    }

    return result;
}
