#include "TileCalcWidget.h"
#include <iostream>
#include <vector>

namespace limap {
namespace tile {

void get_tiles(vector<TileInfo> &tiles,
               unsigned int level,
               double lower_left_lon,
               double lower_left_lat,
               double upper_right_lon,
               double upper_right_lat) {
    if (lower_left_lon > upper_right_lon or lower_left_lat > upper_right_lat) {
        throw "invalid parameters";
    }
    if (lower_left_lon * upper_right_lon >= .0 and lower_left_lat * upper_right_lat >= .0 and
        upper_right_lat - lower_left_lat <= 90.0 and upper_right_lon - lower_left_lon <= 90.0) {
        //std::cout << "\t[" << lower_left_lon << " " << lower_left_lat << ", " << upper_right_lon << " " << upper_right_lat << "]\n";
        unsigned int lower_tilex, lower_tiley, upper_tilex, upper_tiley;
        limap::tile::calc_tile(lower_tilex, lower_tiley, lower_left_lon, lower_left_lat, level);
        limap::tile::calc_tile(upper_tilex, upper_tiley, upper_right_lon, upper_right_lat, level);
        if (lower_tilex > upper_tilex) {
            upper_tilex += 1 << (level+1);
        }
        if (lower_tiley > upper_tiley) {
            upper_tiley += 1 << level;
        }
        //std::cout << "\t" << lower_tilex << " " << lower_tiley << ", " << upper_tilex << " " << upper_tiley << "\n";
        for (unsigned int i = lower_tilex; i < upper_tilex; ++i) {
            for (unsigned int j = lower_tiley; j < upper_tiley; ++j) {
                TileInfo tile_info;
                tile_info.level_ = level;
                tile_info.tilex_ = i;
                tile_info.tiley_ = j;
                tiles.push_back(tile_info);
            }
        }
    }

    if (lower_left_lon * upper_right_lon < .0) {
        //std::cout << "[" << lower_left_lon << " " << lower_left_lat << ", " << upper_right_lon << " " << upper_right_lat << "]\n";
        get_tiles(tiles, level, lower_left_lon, lower_left_lat, 0, upper_right_lat);
        get_tiles(tiles, level, 0, lower_left_lat, upper_right_lon, upper_right_lat);
    } else if (lower_left_lat * upper_right_lat < .0) {
        //std::cout << "[" << lower_left_lon << " " << lower_left_lat << ", " << upper_right_lon << " " << upper_right_lat << "]\n";
        get_tiles(tiles, level, lower_left_lon, lower_left_lat, upper_right_lon, 0);
        get_tiles(tiles, level, lower_left_lon, 0, upper_right_lon, upper_right_lat);
    } else if (lower_left_lon < -90.0 and upper_right_lon > -90.0) {
        //std::cout << "[" << lower_left_lon << " " << lower_left_lat << ", " << upper_right_lon << " " << upper_right_lat << "]\n";
        get_tiles(tiles, level, lower_left_lon, lower_left_lat, -90.0, upper_right_lat);
        get_tiles(tiles, level, -90.0, lower_left_lat, upper_right_lon, upper_right_lat);
    } else if (lower_left_lon < 90.0 and upper_right_lon > 90.0) {
        //std::cout << "[" << lower_left_lon << " " << lower_left_lat << ", " << upper_right_lon << " " << upper_right_lat << "]\n";
        get_tiles(tiles, level, lower_left_lon, lower_left_lat, 90.0, upper_right_lat);
        get_tiles(tiles, level, 90.0, lower_left_lat, upper_right_lon, upper_right_lat);
    }
}
}
}
