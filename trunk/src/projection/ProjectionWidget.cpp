#include "ProjectionWidget.h"

namespace limap {
namespace projection {

ProjectionWidget::ProjectionWidget(double dist_threshold, size_t beg_idx, double real_threshold):
  dist_threshold_(dist_threshold), proj(real_threshold), beg_idx_(beg_idx) {}

bool ProjectionWidget::Project2Line(Point2D &proj_pt, size_t &proj_ptidx, Point2D pt, const LineString &line) {
  bool ret(false);
  for (proj_ptidx = beg_idx_; proj_ptidx < line.size() - 1; ++proj_ptidx) {
    const Point2D &beg_pt = line.at(proj_ptidx);
    const Point2D &end_pt = line.at(proj_ptidx + 1);

    if (proj.Project2Line(proj_pt, pt, beg_pt, end_pt)) {
      double dist = bg::distance(pt, proj_pt);
      if (dist < dist_threshold_) {
        ret = true;
        break;
      }
    }
  }

  return ret;
}

}
}
