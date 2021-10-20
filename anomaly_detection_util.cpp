/*
 * Created by:
 * Shaked Cohen, Shakedc159@gmail.com.
 * Noam Cohen, cohennoam48@gmail.com.
*/

#include "anomaly_detection_util.h"
#include <cmath>
#include "stdexcept"


/**
 * The function calculate the average of group of numbers.
 * @param x, size. the numbers group and the size of the group.
 * @return type Float, the average.
 * @throws invalid_argument.
 **/
float avr(float *x, float size) {

    //Check if the size is 0.
    if (size <= 0) {

        throw std::invalid_argument("size <= 0");
    }

    //Check if null pointer.
    if (x == nullptr) {

        throw std::invalid_argument("null pointer");
    }

    float sum = 0;
    for (int i = 0; i < size; i++) {

        sum += x[i];
    }
    return sum / size;
}

/**
 * The function returns the variance of X and Y.
 * @param x, size. the numbers group and "size" the size of the group.
 * @return type Float, the variance of X and Y.
 * @throws invalid_argument.
 **/
float var(float *x, int size) {

    //Check if the size is 0.
    if (size <= 0) {

        throw std::invalid_argument("size <= 0");
    }

    //Check if null pointer.
    if (x == nullptr) {

        throw std::invalid_argument("null pointer");
    }

    float newSize = size;
    float sum = 0;
    float mu = avr(x, newSize);

    //Calculate sum.
    for (int i = 0; i < size; i++) {

        sum += pow(x[i], 2);
    }

    return (sum / newSize) - pow(mu, 2);
}

/**
 * The function returns the covariance of X and Y, 𝑐𝑜𝑣(𝑋,𝑌)=𝐸(𝑋𝑌)−𝐸(𝑋)𝐸(𝑌)..
 * @param x, y, size. Two numbers group and the size of the groups.
 * @return type Float, the covariance of X and Y.
 * @throws invalid_argument.
 **/
float cov(float *x, float *y, int size) {

    //Check if the size is 0.
    if (size <= 0) {

        throw std::invalid_argument("size <= 0");
    }

    //Check if null pointer.
    if (x == nullptr || y == nullptr) {

        throw std::invalid_argument("null pointer");
    }

    float *newAry = new float[size];

    //Calculate E(X).
    float avrX = avr(x, size);

    //Calculate E(Y).
    float avrY = avr(y, size);

    //Calculate XY.
    for (int i = 0; i < size; i++) {

        newAry[i] = x[i] * y[i];
    }

    //Calculate E(XY).
    float avrXY = avr(newAry, size);

    //Deallocate.
    delete[](newAry);

    return avrXY - (avrX * avrY);
}

/**
 * The function returns the Pearson correlation coefficient of X and Y,Pearson = 𝑐𝑜𝑣(𝑋,𝑌)/𝜎𝑥𝜎𝑦.
 * @param x, y, size. Two numbers group, and the size of the groups.
 * @return type Float, the Pearson correlation coefficient of X and Y.
 * @throws invalid_argument.
 **/
float pearson(float *x, float *y, int size) {

    //Check if the size is 0.
    if (size <= 0) {

        throw std::invalid_argument("size <= 0");
    }

    //Check if null pointer.
    if (x == nullptr || y == nullptr) {

        throw std::invalid_argument("null pointer");
    }

    float sigmaX = sqrt(var(x, size));
    float sigmaY = sqrt(var(y, size));

    return cov(x, y, size) / (sigmaX * sigmaY);
}

/**
 * The function performs a linear regression and return s the line equation.
 * @param points, size. The group of points and the size of the group.
 * @return type Float, the line equation.
 * @throws invalid_argument.
 **/
Line linear_reg(Point **points, int size) {

    //Check if the size is 0.
    if (size <= 0) {

        throw std::invalid_argument("size <= 0");
    }

    //Check if null pointer.
    if (points == nullptr) {

        throw std::invalid_argument("null pointer");
    }

    float *x = new float[size];
    float *y = new float[size];

    for (int i = 0; i < size; i++) {

        x[i] = points[i]->x;
        y[i] = points[i]->y;
    }
    //Calculate 𝑦̅ and 𝑥̅.
    float avrX = avr(x, size);
    float avrY = avr(y, size);

    //Calculate a, a = 𝐶𝑂𝑉(𝑥,𝑦)/𝑉𝐴𝑅(𝑥).
    float a = cov(x, y, size) / var(x, size);

    //Calculate b, 𝑏= 𝑦̅  - 𝑎𝑥̅.
    float b = avrY - (a * avrX);

    //Deallocate.
    delete[](x);
    delete[](y);

    return Line(a, b);
}

/**
 * The function returns the deviation between point p and the line equation of the points.
 * @param points, p, size. group of points, point and the size of the paint's group.
 * @return type Float, the deviation between point p and the line equation of the points.
 **/
float dev(Point p, Point **points, int size) {

    //Calculate the line.
    Line line = linear_reg(points, size);

    return dev(p, line);
}

/**
 * The function returns the deviation between point p and the line.
 * @param l, p. the line and the point.
 * @return type Float, the deviation between point p and the line.
 **/
float dev(Point p, Line l) {

    //Calculate F(x).
    float y = (l.a * p.x) + l.b;

    //return deviation.
    return fabsf(y - p.y);
}


