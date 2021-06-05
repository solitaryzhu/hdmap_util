#ifndef __LIMAP_TILE_CALC_H__
#define __LIMAP_TILE_CALC_H__

namespace limap {
namespace tile {
/*
 * encode packed tile id
 * level - input : tile level
 * tilex - input : coord-x of tile
 * tiley - input : coord-y of tile
 * return packed tile id
*/
unsigned int encode_packedtileid(unsigned int level, unsigned int tilex, unsigned int tiley);

/*
 * decode packed tile id
 * level - output : tile level
 * tilex - output : coord-x of tile
 * tiley - output : coord-y of tile
 * packedtileid - input : packed tile id
*/
void decode_packedtileid(unsigned int &level, unsigned int &tilex, unsigned int &tiley, unsigned int packedtileid);

/*
 * calculate coordinates of tile on some level
 * tilex - output : coord-x of tile
 * tiley - output : coord-y of tile
 * lon - input : longitude of one point
 * lat - input : latitude of one point
 * level - input : the level of tile to calculate
*/
void calc_tile(unsigned int &tilex, unsigned int &tiley, double lon, double lat, unsigned int level);

/*
 * calculate coverage box of tile according to the packedtileid
 * lower_left_lon - output : longitude of lower left corner
 * lower_left_lat - output : latitude of lower left corner
 * tile_width - output : tile width
 * packedtileid - input : the packed tile id
 * return coverage box
*/
void calc_tilebox(double &lower_left_lon,  double &lower_left_lat, double &tile_width, unsigned int packedtileid);

/*
 * calculate the upper tileid by giving one tileid
 * for example : given 15-***-*** tile, return 14-***-*** parent tile
 * packedtileid - input : the packed tile id
 * return parent tile id
*/
unsigned int calc_upper_tileid(unsigned int packedtileid);

/*
 * calculate coord-x and coord-y of one tile
 * tilex - output : coord-x of tile
 * tiley - output : coord-y of tile
 * tileid - input : morton code of tile-x and tile-y
*/
void decode_tileid(unsigned int &tilex, unsigned int &tiley, unsigned int tileid);

/*
 * calculate morton code of tile-x and tile-y
 * tilex - input : coord-x of tile
 * tiley - input : coord-y of tile
*/
unsigned int encode_tile(unsigned int tilex, unsigned int tiley);
} // namespace tile
} // limap

#endif
