#pragma once
#include "defs.h"

namespace limap {
namespace interpolate {

class LineInterpolater {
 public:
  bool interpolate(LineString &out, const LineString &in, double interpolate_dist);
 private:
  void interpolate_segment(LineString &out, Point2D beg_pt, Point2D end_pt, double interpolate_dist);
  double meter2degree(double interpolate_dist, double lat = 45.0);
  bool point_equal(const Point2D &rhs, const Point2D &lhs, double threshold = 1e-6);
};
}
}
