#include "minCircle.h"


bool isInside(const Point &p, const Circle &d) {
    return p.dist(d.center) <= d.radius;
}

Circle circleFrom2Points(Point *p1, Point *p2) {

}

bool isValid(Circle circle, vector<Point *> boundary) {
    // going throughout all points to check if inside circle
    for (Point *point: boundary) {
        if (!isInside(*point, circle)) {
            return false;
        }
    }
    return true;
}

Circle circleFrom2Points(Point *p1, Point *p2, Point *p3) {

}

Circle baseMinCircle(vector<Point *> boundary) {
    // if boundary is 0
    if (boundary.empty()) {
        return {{0, 0}, 0};
    }

    // if boundary is 1 then circle is the point
    if (boundary.size() == 1) {
        return {*boundary[0], 0};
    }

    // if boundary is 2
    if (boundary.size() == 2) {
        return circleFrom2Points(boundary[0], boundary[1]);
    }

    // if boundary is 3 - try to make circle from pair
    for (int first = 0; first < boundary.size(); ++first) {

        for (int second = first + 1; second < boundary.size(); ++second) {

            // calculating circle from pair
            Circle circle = circleFrom2Points(boundary[first], boundary[second]);

            // if the circle is valid return it
            if (isValid(circle, boundary)) {
                return circle;
            }
        }
    }

    // calculating cicle from 3 points
    return circleFrom2Points(boundary[0], boundary[1], boundary[2]);
}


Circle recursiveFindMinCircle(Point **points, vector<Point *> &boundary, size_t n) {
    // if boundary is 3 or n is 0 then we can know what is the circle
    if (boundary.size() == 3 || n == 0) {
        return baseMinCircle(boundary);
    }

    //saving last point
    Point *p = points[n - 1];

    // calling recursively
    Circle minimumCircle = recursiveFindMinCircle(points, boundary, n - 1);

    // if p is in minimumCircle
    if (isInside(*p, minimumCircle)) {
        return minimumCircle;
    }

    // adding p to boundary points and calculate again
    boundary.push_back(p);
    return recursiveFindMinCircle(points, boundary, n - 1);
}

Circle findMinCircle(Point **points, size_t size) {
    vector<Point *> boundary;
    boundary.reserve(3);
    recursiveFindMinCircle(points, boundary, size);
}