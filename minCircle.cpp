#include "minCircle.h"

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

    }
}

bool isInside(const Point &p, const Circle &d) {
    return p.dist(d.center) <= d.radius;
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
    recursiveFindMinCircle(points, boundary, size)
}