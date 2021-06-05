#include <iostream>
#include "TileCalc.h"
#include "TileCalcWidget.h"
#include <vector>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/box.hpp>
#include "linematch_test.hpp"
#include "viterbi_test.hpp"
namespace bg = boost::geometry;

typedef bg::model::d2::point_xy<double> Point2D;
typedef bg::model::box<Point2D> Box2D;

struct TestCaseData {
    double lon_;
    double lat_;
    int64_t morton_;
    int mortonx_;
    int mortony_;
    unsigned int tileid_lv10_;
    unsigned int tileid_lv13_;
};
typedef std::vector<TestCaseData> TestDataS;

void load_testcases(TestDataS &test_datas);
void ut_encodepackedtileid(const TestDataS &test_datas);
void ut_calctile(const TestDataS &test_datas);
void ut_decodetileid(const TestDataS &test_datas);

int main() {
    //begin-tile test
    unsigned int tilex, tiley;
    limap::tile::calc_tile(tilex, tiley, 180.0, 90.0, 10);
    unsigned int test_tileid = limap::tile::encode_packedtileid(10, tilex, tiley);
    double x1, y1, width;
    limap::tile::calc_tilebox(x1, y1, width, test_tileid);

    TestDataS test_datas;
    load_testcases(test_datas);

    ut_encodepackedtileid(test_datas);
    ut_calctile(test_datas);
    ut_decodetileid(test_datas);

    std::cout << "\n\n\n";
    typedef std::vector<limap::tile::TileInfo> TileInfoS;
    TileInfoS tiles;
    //limap::tile::get_tiles(tiles, 10, 114.29, 38.31, 118.68, 41.22);
    limap::tile::get_tiles(tiles, 10, -180.0, -90.0, 180.0, 90.0);
    //limap::tile::get_tiles(tiles, 10, .0, -90.0, 180.0, 90.0);
//    for (auto tile : tiles) {
//        unsigned int packedtileid = limap::tile::encode_packedtileid(tile.level_, tile.tilex_, tile.tiley_);
//        double lower_left_x, lower_left_y, tile_width;
//        limap::tile::calc_tilebox(lower_left_x, lower_left_y, tile_width, packedtileid);
//        std::cout << "POLYGON(("
//                  << lower_left_x << " " << lower_left_y << ", "
//                  << lower_left_x + tile_width << " " << lower_left_y << ", "
//                  << lower_left_x + tile_width << " " << lower_left_y + tile_width << ", "
//                  << lower_left_x << " " << lower_left_y + tile_width << ", "
//                  << lower_left_x << " " << lower_left_y << "))" << std::endl;
//    }
    //end-tile test

    //begin-linematch
    limap::match::LineMatchTest line_match_test;
    line_match_test.test("/home/ben/li-work/code/hdmap_util/data/test1.txt");
    //end-linematch

    std::cout << "\n\nbegin viterbi test---\n";
    limap::hmmmatch::ViterbiTest viterbi_test;
    viterbi_test.test();
    std::cout << "---viterbi test end\n";

    std::cout << "\n\nbegin HmmLinkMatchTest---\n";
    limap::hmmmatch::HmmLineMatchTest hmm_test;
    hmm_test.test("/home/ben/li-work/code/hdmap_util/data/test1.txt");
    std::cout << "---HmmLinkMatchTest end\n";

    return 0;
}

void ut_encodepackedtileid(const TestDataS &test_datas) {
    unsigned int packedtileid1 = limap::tile::encode_packedtileid(10, 13, 277);
    std::cout << "packedtileid1 = " << packedtileid1 << "\n";
    unsigned int packedtileid2 = limap::tile::encode_packedtileid(13, 104, 2223);
    std::cout << "packedtileid2 = " << packedtileid2 << "\n";
    assert(packedtileid1 == 67240563);
    assert(packedtileid2 == 545299690);

    for (auto data : test_datas) {
        unsigned int tilex, tiley;
        limap::tile::calc_tile(tilex, tiley, data.lon_, data.lat_, 10);
        unsigned int packedtileid = limap::tile::encode_packedtileid(10, tilex, tiley);
        double lower_left_lon, lower_left_lat, tile_width;
        limap::tile::calc_tilebox(lower_left_lon, lower_left_lat, tile_width, packedtileid);
        assert(data.lon_ >= lower_left_lon             and
               data.lat_ >= lower_left_lat             and
               data.lon_ < lower_left_lon + tile_width and
               data.lat_ < lower_left_lat + tile_width);

        limap::tile::calc_tile(tilex, tiley, data.lon_, data.lat_, 13);
        packedtileid = limap::tile::encode_packedtileid(13, tilex, tiley);
        limap::tile::calc_tilebox(lower_left_lon, lower_left_lat, tile_width, packedtileid);
        assert(data.lon_ >= lower_left_lon             and
               data.lat_ >= lower_left_lat             and
               data.lon_ < lower_left_lon + tile_width and
               data.lat_ < lower_left_lat + tile_width);
    }
}

void ut_calctile(const TestDataS &test_datas) {
    unsigned int tilex, tiley;
    limap::tile::calc_tile(tilex, tiley, 2.2945, 48.858222, 10);
    std::cout << "tilex = " << tilex << " , tiley = " << tiley << " when level = 10\n";
    assert(tilex == 13);
    assert(tiley == 277);
    limap::tile::calc_tile(tilex, tiley, 2.2945, 48.858222, 13);
    std::cout << "tilex = " << tilex << " , tiley = " << tiley << " when level = 13\n";
    assert(tilex == 104);
    assert(tiley == 2223);

    for (auto data : test_datas) {
        unsigned tilex, tiley;
        limap::tile::calc_tile(tilex, tiley, data.lon_, data.lat_, 10);
        unsigned int tileid = limap::tile::encode_tile(tilex, tiley);
        std::cout << "true value of lv10_tileid = " << data.tileid_lv10_
                  << "\ntest value of lv10_tileid = " << tileid << "\n";
        assert(tileid == data.tileid_lv10_);
        limap::tile::calc_tile(tilex, tiley, data.lon_, data.lat_, 13);
        tileid = limap::tile::encode_tile(tilex, tiley);
        assert(tileid == data.tileid_lv13_);
        std::cout << "true value of lv10_tileid = " << data.tileid_lv13_
                  << "\ntest value of lv10_tileid = " << tileid << "\n";
    }
}

void ut_decodetileid(const TestDataS &test_datas) {
    unsigned int eiffel_tower_lv10 = 131699;
    unsigned int eiffel_tower_lv13 = 8428778;
    unsigned int tilex, tiley;
    limap::tile::decode_tileid(tilex, tiley, eiffel_tower_lv10);
    std::cout << "tilex = " << tilex << " , tiley = " << tiley << " when level = 10\n";
    limap::tile::decode_tileid(tilex, tiley, eiffel_tower_lv13);
    std::cout << "tilex = " << tilex << " , tiley = " << tiley << " when level = 13\n";

    for (auto data : test_datas) {
        unsigned int tilex, tiley;
        limap::tile::decode_tileid(tilex, tiley, data.tileid_lv10_);;
        unsigned int tilex2, tiley2;
        limap::tile::calc_tile(tilex2, tiley2, data.lon_, data.lat_, 10);
        assert(tilex == tilex2);
        assert(tiley == tiley2);
        std::cout << "true value of lv10_tilex and tiley = " << tilex << ", " << tiley << "\n";
        std::cout << "test value of lv10_tilex and tiley = " << tilex2 << ", " << tiley2 << "\n";

        limap::tile::decode_tileid(tilex, tiley, data.tileid_lv13_);
        limap::tile::calc_tile(tilex2, tiley2, data.lon_, data.lat_, 13);
        assert(tilex == tilex2);
        assert(tiley == tiley2);
        std::cout << "true value of lv13_tilex and tiley = " << tilex << ", " << tiley << "\n";
        std::cout << "test value of lv13_tilex and tiley = " << tilex2 << ", " << tiley2 << "\n";
    }
}

void load_testcases(TestDataS &test_datas) {
    TestCaseData eiffel_tower;
    eiffel_tower.lon_ = 2.2945;
    eiffel_tower.lat_ = 48.858222;
    eiffel_tower.mortonx_ = 27374451;
    eiffel_tower.mortony_ = 582901293;
    eiffel_tower.morton_ = 579221254078012839;
    eiffel_tower.tileid_lv10_ = 131699;
    eiffel_tower.tileid_lv13_ = 8428778;
    test_datas.push_back(eiffel_tower);

    TestCaseData statue_of_liberty;
    statue_of_liberty.lon_ = -74.044444;
    statue_of_liberty.lat_ =  40.689167;
    statue_of_liberty.mortonx_ = -883384626;
    statue_of_liberty.mortony_ = 485440671;
    statue_of_liberty.morton_ = 5973384896724652798;
    statue_of_liberty.tileid_lv10_ = 1358190;
    statue_of_liberty.tileid_lv13_ = 86924190;
    test_datas.push_back(statue_of_liberty);

    TestCaseData sugarloaf_mountain;
    sugarloaf_mountain.lon_ = -43.157444;
    sugarloaf_mountain.lat_ = -22.948658;
    sugarloaf_mountain.mortonx_ = -514888362;
    sugarloaf_mountain.mortony_ = -273788154;
    sugarloaf_mountain.morton_ = 8983442095026671932;
    sugarloaf_mountain.tileid_lv10_ = 2042598;
    sugarloaf_mountain.tileid_lv13_ = 130726287;
    test_datas.push_back(sugarloaf_mountain);

    TestCaseData sydney_opera_house;
    sydney_opera_house.lon_ = 151.214189;
    sydney_opera_house.lat_ = -33.857529;
    sydney_opera_house.mortonx_ = 1804055545;
    sydney_opera_house.mortony_ = -403936054;
    sydney_opera_house.morton_ = 4354955230616876489;
    sydney_opera_house.tileid_lv10_ = 990202;
    sydney_opera_house.tileid_lv13_ = 63372939;
    test_datas.push_back(sydney_opera_house);

    TestCaseData near_the_millenium_dome;
    near_the_millenium_dome.lon_ = .0;
    near_the_millenium_dome.lat_ = 51.503;
    near_the_millenium_dome.mortonx_ = 0;
    near_the_millenium_dome.mortony_ = 614454724;
    near_the_millenium_dome.morton_ = 585611620934393888;
    near_the_millenium_dome.tileid_lv10_ = 133152;
    near_the_millenium_dome.tileid_lv13_ = 8521770;
    test_datas.push_back(near_the_millenium_dome);

    TestCaseData near_quito;
    near_quito.lon_ = -78.45;
    near_quito.lat_ = .0;
    near_quito.mortonx_ = -935944956;
    near_quito.mortony_ = 0;
    near_quito.morton_ = 5782627506097029136;
    near_quito.tileid_lv10_ = 1314817;
    near_quito.tileid_lv13_ = 84148305;
    test_datas.push_back(near_quito);
}
