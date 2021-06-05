#ifndef __HMM_LINE_MATCH_H__
#define __HMM_LINE_MATCH_H__

#include <vector>
#include <boost/geometry.hpp>
using std::vector;
#include "CommonViterbi.h"

namespace limap {
namespace hmmmatch {

namespace bg = boost::geometry;
typedef bg::model::point<double, 2, bg::cs::cartesian> Point2D;
typedef bg::model::linestring<Point2D> LineString;

class LinkFeature {
    friend std::ostream &operator<<(std::ostream &, const LinkFeature &);
public:
    LinkFeature() = default;
    LinkFeature(uint32_t linkid, uint8_t direction, const LineString &geom);

public:
    uint32_t   linkid_;
    uint8_t    direction_;
    LineString geom_;
};

typedef vector<LinkFeature> LinkFeatureS;

class HmmLinkMatcher {
public:
    HmmLinkMatcher() = default;
    LinkFeatureS Match(const LinkFeatureS &links, const LineString &pts);

private:
    void calc_emit_prob(RealMatrix &emit_prob, const LinkFeatureS &links, const LineString &pts);
    void calc_trans_prob(RealMatrix &trans_prob, const LinkFeatureS &links, const LineString &pts);
private:
    double calc_prob(double beta, double dist);
//private:
//    void preprocess();
//    void postprocess();
};

}
}

#endif
