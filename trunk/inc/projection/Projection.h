#ifndef __LIMAP_PROJECTION_H__
#define __LIMAP_PROJECTION_H__

#include <boost/geometry.hpp>

namespace limap {
namespace projection {

namespace bg = boost::geometry;
typedef bg::model::point<double, 2, bg::cs::cartesian> Point2D;
typedef bg::model::linestring<Point2D> LineString;

class PointProjector {
public:
    PointProjector(double real_threshold = 1e-8);
    /*
     * proj_pt - output : project point on line if projection is successful
     * pt - input : the point which is used to projected onto line
     * beg_pt - input : the begin point of line
     * end_pt - input : the end point of line
     * return true if project point between beg_pt and end_pt, or return false
    */
    bool Project2Line(Point2D &proj_pt, Point2D pt, Point2D beg_pt, Point2D end_pt);

private:
    const double real_threshold_;
};

}
}

#endif
