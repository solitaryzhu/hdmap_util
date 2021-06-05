#include "LineMatch.h"
#include "Projection.h"
#include <boost/geometry/algorithms/distance.hpp>

namespace limap {
namespace match {

bool LineMatcher::Match(const LineString &base_line, const LineString &match_line) {
    return true;
}

void LineMatcher::calc_characters(vector<GeomCharacter> &geom_chars, const LineString &match_line, const LineString &base_line) {
    geom_chars.clear();
    Point2D pre_pt = match_line.at(0);
    size_t pre_pt_index(0);
    for (size_t i = 0; i < match_line.size(); ++i) {
        const Point2D &cur_pt = match_line.at(i);
        double inc_len = bg::distance(pre_pt, cur_pt);
        if (not geom_chars.empty()) {
            inc_len += geom_chars.back().IncLen();
        }

        double dist_to_line(.0);
        if (not calc_distance(dist_to_line, cur_pt, base_line, pre_pt_index)) {
            throw("error while projecting point onto base_line");
        }

        double slope = calc_slope(cur_pt, pre_pt);

        GeomCharacter geom_char(inc_len, dist_to_line, slope, 0);
        geom_chars.emplace_back(geom_char);

        pre_pt = cur_pt;
    }

    for (size_t i = 1; i < geom_chars.size()-1; ++i) {
        const GeomCharacter &pre_char = geom_chars.at(i-1);
        const GeomCharacter &nxt_char = geom_chars.at(i+1);
        GeomCharacter &cur_char = geom_chars.at(i);
        double diff_slope1 = nxt_char.Slope() - cur_char.Slope();
        double diff_slope2 = cur_char.Slope() - pre_char.Slope();
        //TODO : adjust MagicNumber
        if (std::abs(diff_slope1 - diff_slope2) > 1e-6) {
            if (diff_slope1 > diff_slope2) {
                cur_char.Convexity(1);
            } else {
                cur_char.Convexity(-1);
            }
        }
    }
}

bool LineMatcher::calc_distance(double &dist, Point2D pt, const LineString &base_line, size_t &beg_idx) {
    bool ret(false);
    limap::projection::PointProjector point_proj;
    for (size_t i = beg_idx; i < base_line.size() - 1; ++i) {
        Point2D beg_pt = base_line.at(i);
        Point2D end_pt = base_line.at(i+1);
        Point2D proj_pt;
        if (point_proj.Project2Line(proj_pt, pt, beg_pt, end_pt)) {
            beg_idx = i;
            dist = bg::distance(pt, proj_pt);
            ret = true;
            break;
        }
    }
    return ret;
}

double LineMatcher::calc_slope(Point2D cur_pt, Point2D pre_pt) {
    double slope(.0);
    double diffy = cur_pt.get<1>() - pre_pt.get<1>();
    double diffx = cur_pt.get<0>() - pre_pt.get<0>();
    //TODO : adjust MagicNumber
    if (std::abs(diffx) > 1e-4) {
        slope = diffy / diffx;
    }
    return slope;
}

}
}
