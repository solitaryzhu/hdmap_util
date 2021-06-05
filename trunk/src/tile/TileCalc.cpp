#include "TileCalc.h"
#include <climits>
#include <cmath>

namespace limap {
namespace tile {

unsigned int encode_packedtileid(unsigned int level, unsigned int tilex, unsigned int tiley) {
    if (level > 15) {
        throw("level out of range");
    }

    int max_bit = tilex > tiley ? sizeof (tilex) * CHAR_BIT : sizeof (tiley) * CHAR_BIT;

    unsigned int ret(0);
    for (int i = 0; i < max_bit; ++i) {
        ret |= (tilex & (1U << i)) << i | (tiley & (1U << i)) << (i+1);
    }
    ret = (1 << (level + 16)) | ret;
    return ret;
}

void decode_packedtileid(unsigned int &level, unsigned int &tilex, unsigned int &tiley, unsigned int packedtileid) {
    level = 15;
    for (unsigned int i = 0; i < 16; ++i) {
        unsigned int level_place = (1U << (31 - i));
        if ((packedtileid & level_place) > 0) {
            level -= i;
            break;
        }
    }
    if (level > 15) {
        throw("level out of range");
    }

    tilex = 0;
    tiley = 0;
    for (unsigned int i = 0; i < 16 + level; ++i) {
        if (i % 2 == 0) {
            tilex |= (packedtileid & (1 << i)) >> (i>>1);
        } else {
            tiley |= (packedtileid & (1 << i)) >> ((i>>1) + 1);
        }
    }
}

void calc_tile(unsigned int &tilex, unsigned int &tiley, double lon, double lat, unsigned int level) {
    unsigned int lon_int = static_cast<unsigned int>(floor(lon / 180.0 * (1U << 31)));
    unsigned int lat_int = static_cast<unsigned int>(floor(lat / 180.0 * (1U << 31)));
    lat_int &= 0x7FFFFFFF; // set 0 on most significant bit
    tilex = (lon_int >> (31 - level)) & 0xFFFF;
    tiley = (lat_int >> (31 - level)) & 0xFFFF;
}

void decode_tileid(unsigned int &tilex, unsigned int &tiley, unsigned int tileid) {
    tilex = 0;
    tiley = 0;
    for (unsigned int i = 0; i < sizeof (tileid) * CHAR_BIT; ++i) {
        if (i % 2 == 0) {
            tilex |= (tileid & (1 << i)) >> (i>>1);
        } else {
            tiley |= (tileid & (1 << i)) >> ((i>>1) + 1);
        }
    }
}

unsigned int encode_tile(unsigned int tilex, unsigned int tiley) {
    int max_bit = tilex > tiley ? sizeof (tilex) * CHAR_BIT : sizeof (tiley) * CHAR_BIT;

    unsigned int ret(0);
    for (int i = 0; i < max_bit; ++i) {
        ret |= (tilex & (1U << i)) << i | (tiley & (1U << i)) << (i+1);
    }
    return ret;
}

unsigned int calc_upper_tileid(unsigned int packedtileid) {
    unsigned int curr_level, curr_tilex, curr_tiley;
    decode_packedtileid(curr_level, curr_tilex, curr_tiley, packedtileid);
    return encode_packedtileid(curr_level/2, curr_tilex/2, curr_tiley/2);
}

void calc_tilebox(double &lower_left_lon,  double &lower_left_lat, double &tile_width, unsigned int packedtileid) {
    unsigned int level, tilex, tiley;
    decode_packedtileid(level, tilex, tiley, packedtileid);

    if (level > 15 or level < 3) {
        throw("level out of range");
    }

    tile_width = 180.0 / (1 << level);
    unsigned int relative_tilex = tilex;
    unsigned int relative_tiley = tiley;
    relative_tilex &= ~(1 << level);
    relative_tilex &= ~(1 << (level - 1));
    relative_tiley &= ~(1 << (level - 1));
    lower_left_lon = tile_width * relative_tilex;
    lower_left_lat = tile_width * relative_tiley;

    unsigned int shift_flag = (packedtileid >> (2*level - 2)) & 0x7;
    if (shift_flag == 1) {
        lower_left_lon += 90.0;
    } else if (shift_flag == 2) {
        lower_left_lat -= 90.0;
    } else if (shift_flag == 3) {
        lower_left_lon += 90.0;
        lower_left_lat -= 90.0;
    } else if (shift_flag == 4) {
        lower_left_lon -= 180.0;
    } else if (shift_flag == 5) {
        lower_left_lon -= 90.0;
    } else if (shift_flag == 6) {
        lower_left_lon -= 180.0;
        lower_left_lat -= 90.0;
    } else if (shift_flag == 7) {
        lower_left_lon -= 90.0;
        lower_left_lat -= 90.0;
    }
}
}
}
