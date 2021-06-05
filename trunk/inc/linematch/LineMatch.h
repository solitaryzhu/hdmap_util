#ifndef __LINE_MATCH_H__
#define __LINE_MATCH_H__

#include <vector>
#include <boost/geometry.hpp>
#include "GeomCharacter.h"

namespace limap {
namespace match {

using std::vector;
namespace bg = boost::geometry;
typedef bg::model::point<double, 2, bg::cs::cartesian> Point2D;
typedef bg::model::linestring<Point2D> LineString;

class LineMatcher {
public:
    bool Match(const LineString &base_line, const LineString &match_line);

private:
    void calc_characters(vector<GeomCharacter> &geom_chars, const LineString &match_line, const LineString &base_line);
    bool calc_distance(double &dist, Point2D pt, const LineString &base_line, size_t &beg_idx);
    double calc_slope(Point2D cur_pt, Point2D pre_pt);
    int calc_convexity();
    friend class LineMatchTest;
};

}
}

#endif
