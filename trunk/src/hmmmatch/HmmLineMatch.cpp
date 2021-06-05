#include "HmmLineMatch.h"
#include "ProjectionWidget.h"
#include <cmath>
#include <boost/math/distributions/normal.hpp>
using limap::projection::ProjectionWidget;

namespace limap {
namespace hmmmatch {

LinkFeature::LinkFeature(uint32_t linkid, uint8_t direction, const LineString &geom):
    linkid_(linkid), direction_(direction), geom_(geom) {}

std::ostream &operator<<(std::ostream &out, const LinkFeature &feat) {
    out << feat.linkid_;
    return out;
}

LinkFeatureS HmmLinkMatcher::Match(const LinkFeatureS &links, const LineString &pts) {
    RealMatrix emit_prob;
    calc_emit_prob(emit_prob, links, pts);

    RealMatrix trans_prob;
    calc_trans_prob(trans_prob, links, pts);

    RealMatrix sub_emit_prob;
    LineString subpts;
    subpts.push_back(pts.front());
    calc_emit_prob(sub_emit_prob, links, subpts);
    RealVector start_prob;
    for (auto it : sub_emit_prob) {
        start_prob.push_back(it.front());
    }

    CommonViterbi<Point2D, LinkFeature> viterbi;
    LinkFeatureS match_rets = viterbi(pts, links, start_prob, trans_prob, emit_prob);

    return match_rets;
}

void HmmLinkMatcher::calc_emit_prob(RealMatrix &emit_prob, const LinkFeatureS &links, const LineString &pts) {
    emit_prob = RealMatrix(links.size(), RealVector(pts.size()));
    boost::math::normal_distribution<> nd(.0, 6.0);
    ProjectionWidget proj(10.0, 0);
    for (size_t j = 0; j < pts.size(); ++j) {
        auto pt = pts.at(j);
        for (size_t i = 0; i < links.size(); ++i) {
            auto link = links.at(i);
            double dist(.0);
            Point2D proj_pt;
            size_t proj_ptidx(0);
            if (proj.Project2Line(proj_pt, proj_ptidx, pt, link.geom_)) {
                dist = bg::distance(proj_pt, pt);
            } else {
                dist = std::max(bg::distance(pt, link.geom_.front()),
                                bg::distance(pt, link.geom_.back()));
            }
            emit_prob[i][j] = boost::math::pdf(nd, dist);
        }
    }
}

void HmmLinkMatcher::calc_trans_prob(RealMatrix &trans_prob, const LinkFeatureS &links, const LineString &pts) {
    trans_prob = RealMatrix(links.size(), RealVector(links.size()));

    typedef std::tuple<IntVector, LineString> Assist;
    std::vector<Assist> assists(pts.size());
    ProjectionWidget proj(10.0, 0);
    for (size_t i = 0; i < pts.size(); ++i) {
        for (size_t j = 0; j < links.size(); ++j) {
            size_t proj_ptidx(0);
            Point2D proj_pt;
            if (proj.Project2Line(proj_pt, proj_ptidx, pts.at(i), links.at(j).geom_)) {
                std::get<0>(assists[i]).push_back(j);
                std::get<1>(assists[i]).push_back(proj_pt);
            }
        }
    }

    for (size_t k = 0; k < assists.size() - 1; ++k) {
        auto assist1 = assists.at(k);
        auto assist2 = assists.at(k+1);
        double dist1 = bg::distance(pts.at(k), pts.at(k+1));

        for (size_t i = 0;  i < std::get<0>(assist1).size(); ++i) {
            auto link1 = std::get<0>(assist1).at(i);
            auto projpt1  = std::get<1>(assist1).at(i);

            for (size_t j = 0; j < std::get<0>(assist2).size(); ++j) {
                auto link2 = std::get<0>(assist2).at(j);
                auto projpt2 = std::get<1>(assist2).at(j);
                double dist2 = bg::distance(projpt1, projpt2);
                trans_prob[link1][link2] = calc_prob(6.0, std::abs(dist1 - dist2));
            }
        }
    }
}

double HmmLinkMatcher::calc_prob(double beta, double dist) {
    return 1.0 / beta * std::exp(-1.0 * dist / beta);
}

}
}
