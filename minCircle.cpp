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

// Function to return a unique circle that
// intersects three points
Circle circleFrom3Points(const Point& A, const Point& B, const Point& C)
{
    double newB = (B.x - A.x) * (B.x - A.x) + (B.y - A.y) * (B.y - A.y);
    double newC = (C.x - A.x) * (C.x - A.x) + (C.y - A.y) * (C.y - A.y);
    double newD = (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x);

    Point *center = new Point(((((C.y - A.y) * newB - (B.y - A.y) * newC) / (2 * newD))),
                              (((B.x - A.x) * newC - (C.x - A.x) * newB) / (2 * newD)));
    center->x += A.x;
    center->y += A.y;

    return Circle(*center, center->dist(A));
}

// Function to return the smallest circle
// that intersects 2 points
Circle circleFrom2Points(const Point &p1, const Point &p2)
{
    // Set the center to be the midpoint of A and p2
    Point *center = new Point(((p1.x + p2.x) / 2.0), ((p1.y + p2.y) / 2.0));

    // Set the radius to be half the distance between p1 and p2
    return Circle(*center, (p1.dist(p2) / 2.0));
}