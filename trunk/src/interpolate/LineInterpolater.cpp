#include "LineInterpolater.h"

namespace limap {
namespace interpolate {

bool LineInterpolater::interpolate(LineString &out, const LineString &in, double interpolate_dist) {
  double degree_dist = meter2degree(interpolate_dist, in.front().get<0>());
  for (size_t i = 0; i < in.size() - 1; ++i) {
    auto beg_pt = in.at(i);
    auto end_pt = in.at(i + 1);
    interpolate_segment(out, beg_pt, end_pt, degree_dist);
  }
  assert(out.size() >= in.size());
  return (out.size() >= in.size());
}
//插值2个形点
void LineInterpolater::interpolate_segment(LineString &out, Point2D beg_pt, Point2D end_pt, double interpolate_dist) {
  double dist = bg::distance(beg_pt, end_pt);
  if (dist < interpolate_dist) {
    if (out.size() < 1 or not point_equal(out.back(), beg_pt)) {
      out.emplace_back(beg_pt);
    }
    out.emplace_back(end_pt);
    return;
  }
  Point2D mid_pt((beg_pt.get<0>() + end_pt.get<0>()) / 2.0,
                 (beg_pt.get<1>() + end_pt.get<1>()) / 2.0);
  interpolate_segment(out, beg_pt, mid_pt, interpolate_dist);
  interpolate_segment(out, mid_pt, end_pt, interpolate_dist);
}
//米单位转经纬度
double LineInterpolater::meter2degree(double interpolate_dist, double lat) {
  double degree_dist(.0);
  double circumference = M_2_PI * std::cos(M_2_PI / 360.0 * lat) * 6378140;
  degree_dist = 360.0 * interpolate_dist / circumference;
  return degree_dist;
}

bool LineInterpolater::point_equal(const Point2D &rhs, const Point2D &lhs, double threshold) {
  if (std::abs(rhs.get<0>() - lhs.get<0>()) < threshold and
      std::abs(rhs.get<1>() - lhs.get<1>()) < threshold) {
    return true;
  }
  return false;
}

}
}
