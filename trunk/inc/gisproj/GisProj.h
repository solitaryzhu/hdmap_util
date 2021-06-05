#ifndef __GIS_PROJ_H__
#define __GIS_PROJ_H__

#include <string>
#include <boost/geometry.hpp>
#include <boost/geometry/srs/transformation.hpp>

namespace limap {
namespace proj {

using std::string;
namespace bg = boost::geometry;
typedef bg::model::point<double, 2, bg::cs::geographic<bg::degree> > GeoPoint;
typedef bg::model::linestring<GeoPoint> LineString;

template<class T>
class ProjTransform {
public:
    ProjTransform(int zone):zone_(zone) {}
    void execute(T &dst, const T &src) {
        string src_proj_ini("+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs");
        char dst_proj_ini[1024] = {0};
        sprintf(dst_proj_ini, "+proj=utm +zone=%d +ellps=WGS84 +datum=WGS84 +units=m +no_defs", zone_);

        bg::srs::transformation<> tr{
            bg::srs::proj4(src_proj_ini),
            bg::srs::proj4(dst_proj_ini)};
        tr.forward(src, dst);
    }
private:
    int zone_;
};

}
}

#endif
