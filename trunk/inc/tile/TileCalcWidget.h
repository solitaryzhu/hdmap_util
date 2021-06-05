#ifndef __TILE_CALC_WIDGET_H__
#define __TILE_CALC_WIDGET_H__

#include <vector>
#include "TileCalc.h"
using std::vector;

namespace limap {
namespace tile {

struct TileInfo {
    TileInfo():level_(0), tilex_(0), tiley_(0) {}
    unsigned int level_;
    unsigned int tilex_;
    unsigned int tiley_;
};

void get_tiles(vector<TileInfo> &tiles,
               unsigned int level,
               double lower_left_lon,
               double lower_left_lat,
               double upper_right_lon,
               double upper_right_lat);
}
}

#endif
