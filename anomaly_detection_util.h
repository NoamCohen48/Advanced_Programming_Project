//
// Created by user on 12/10/2021.
//

#ifndef PROJECT1_ANOMALY_DETECTION_UTIL_H
#define PROJECT1_ANOMALY_DETECTION_UTIL_H

#include <cmath>
#include "stdexcept"
#include <vector>


// returns the variance of X and Y
float var(const std::vector<float> &x, int size);

// returns the covariance of X and Y
float cov(const std::vector<float> &x, float *y, int size);

// returns the Pearson correlation coefficient of X and Y
float pearson(const std::vector<float> &x, const std::vector<float> &y, int size);

class Line {
public:
    float a, b;

    Line() : a(0), b(0) {}

    Line(float a, float b) : a(a), b(b) {}

    float f(float x) {
        return a * x + b;
    }
};

class Point {
public:
    float x, y;

    Point(float x, float y) : x(x), y(y) {}

    double dist(const Point &other) const {
        return sqrt(pow(x - other.x, 2) + pow(y - other.y, 2));
    }
};

// performs a linear regression and return s the line equation
Line linear_reg(Point **points, int size);

Line linear_reg(const std::vector<float> &x, const std::vector<float> &y, int size);

// returns the deviation between point p and the line equation of the points
float dev(Point p, Point **points, int size);

// returns the deviation between point p and the line
float dev(Point p, const Line &l);


#endif //PROJECT1_ANOMALY_DETECTION_UTIL_H
