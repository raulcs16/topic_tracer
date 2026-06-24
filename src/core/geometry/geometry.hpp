#pragma once
#include <vector>

namespace tt {

struct Point {
    double x = 0.0;
    double y = 0.0;
};

// Use a type alias for Path to keep it clean
using Path = std::vector<Point>;

struct Line {
    Point start;
    Point end;
    Path bends;
};

struct Rect {
    double x = 0.0;
    double y = 0.0;
    double width = 0.0;
    double height = 0.0;

    // Pro-tip: Helper to get the center Point
    Point center() const { return {x + width / 2.0, y + height / 2.0}; }
};

struct BoundingBox {
    double min_x;
    double max_x;
    double min_y;
    double max_y;
};

} // namespace tt