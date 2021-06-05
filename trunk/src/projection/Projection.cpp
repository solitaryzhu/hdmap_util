#include "Projection.h"

namespace limap {
namespace projection {

PointProjector::PointProjector(double real_threshold) :  real_threshold_(real_threshold) {}

bool PointProjector::Project2Line(Point2D &proj_pt, Point2D pt, Point2D beg_pt, Point2D end_pt) {
    Point2D w = pt;
    Point2D v = end_pt;
    bg::subtract_point(w, beg_pt);
    bg::subtract_point(v, beg_pt);

    double ratio = bg::dot_product(w, v);
    if (ratio + real_threshold_ < .0) {
        return false;
    }
    if (ratio < real_threshold_) {
        proj_pt = beg_pt;
        return true;
    }

    double max_ratio = bg::dot_product(v, v);
    if (ratio > max_ratio + real_threshold_) {
        return false;
    }
    if (ratio > max_ratio) {
        proj_pt = end_pt;
        return true;
    }

    proj_pt = beg_pt;
    bg::multiply_value(v, ratio / max_ratio);
    bg::add_point(proj_pt, v);

    return true;
}

}
}
