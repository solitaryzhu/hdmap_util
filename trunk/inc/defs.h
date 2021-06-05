#pragma once
#include <boost/geometry.hpp>

namespace limap {
namespace bg = boost::geometry;
typedef bg::model::point<double, 2, bg::cs::cartesian> Point2D;
typedef bg::model::linestring<Point2D> LineString;


}
