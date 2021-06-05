#ifndef __PROJECT_WIDGET_H__
#define __PROJECT_WIDGET_H__

#include "Projection.h"

namespace limap {
namespace projection {

class ProjectionWidget {
public:
    ProjectionWidget(double dist_threshold, size_t beg_idx = 0, double real_threshold=1e-8);
    bool Project2Line(Point2D &proj_pt, size_t &proj_ptidx, Point2D pt, const LineString &line);

private:
    double dist_threshold_;
    PointProjector proj;
    size_t beg_idx_;
};

}
}

#endif
