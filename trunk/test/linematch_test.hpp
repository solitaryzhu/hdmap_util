#ifndef __LINE_MATCH_TEST_HPP_
#define __LINE_MATCH_TEST_HPP_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;
#include "LineMatch.h"
#include "MatchStrategy.h"
#include "GisProj.h"

namespace limap {
namespace match {
class LineMatchTest {
 public:
  void test(string wkt_file) {
    // same link test
    ifstream fin(wkt_file);
    stringstream ss;
    ss << fin.rdbuf();
    string wkt(ss.str());
    fin.close();

    limap::proj::LineString line_string;
    limap::proj::bg::read_wkt(wkt, line_string);

    limap::proj::ProjTransform<limap::proj::LineString> tr(21);
    limap::proj::LineString proj_line_string;
    tr.execute(proj_line_string, line_string);
    for (auto pt : proj_line_string) {
      cout << pt.get<0>() << ", " << pt.get<1>() << endl;
    }

    limap::match::LineString input_line_string;
    for (auto pt : proj_line_string) {
      input_line_string.push_back(limap::match::Point2D(pt.get<0>(), pt.get<1>()));
    }

    limap::match::LineMatcher line_matcher;
    vector<limap::match::GeomCharacter> base_chars;
    vector<limap::match::GeomCharacter> match_chars;
    line_matcher.calc_characters(base_chars, input_line_string, input_line_string);
    line_matcher.calc_characters(match_chars, input_line_string, input_line_string);

    limap::match::MatchStrategy *match = new limap::match::LengthMatch();
    double match_length = match->Match(base_chars, match_chars);
    delete match;
    match = nullptr;
    cout << "Match Length is " << match_length << endl;
  }
};
}
}
#endif
