#ifndef __VITERBI_TEST_HPP
#define __VITERBI_TEST_HPP

#include "../hmmmatch/CommonViterbi.h"
#include "../hmmmatch/HmmLineMatch.h"
#include "GisProj.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using std::cout;
using std::string;
using std::ifstream;
using std::stringstream;

namespace limap {
namespace hmmmatch {

class ViterbiTest {
public:
    void test() {
        vector<string> obs{"normal", "cold", "dizzy"};
        vector<string> states{"Healty", "Fever"};
        RealVector start_prob{0.6, 0.4};
        RealMatrix trans_prob{{0.7, 0.3}, {0.4, 0.6}};
        RealMatrix emit_prob{{0.5, 0.4, 0.1}, {0.1, 0.3, 0.6}};
        CommonViterbi<string, string> viterbi(CommonViterbi<string, string>::DEBUG_MODULE);
        cout << "begin to print trans_prob---\n";
        viterbi.print_matrix(trans_prob);
        cout << "---end of print trans_prob\n";
        cout << "begin to print emit_prob---\n";
        viterbi.print_matrix(emit_prob);
        cout << "---end of print emit_prob\n";
        vector<string> rets = viterbi(obs, states, start_prob, trans_prob, emit_prob);
        for (auto state : rets) {
            cout << state << "\t";
        }
    }
};

class HmmLineMatchTest {
public:
    void test(string wkt_file) {
        // same link test
        ifstream fin(wkt_file);
        stringstream ss;
        ss << fin.rdbuf();
        string wkt(ss.str());
        fin.close();

        limap::hmmmatch::LineString line_string;
        limap::hmmmatch::bg::read_wkt(wkt, line_string);

        limap::proj::ProjTransform<limap::hmmmatch::LineString> tr(21);
        limap::hmmmatch::LineString proj_line_string;
        tr.execute(proj_line_string, line_string);
        for (auto pt : proj_line_string) {
            cout << pt.get<0>() << ", " << pt.get<1>() << std::endl;
        }

        limap::hmmmatch::LineString input_line_string;
        for (auto pt : proj_line_string) {
            input_line_string.push_back(limap::hmmmatch::Point2D(pt.get<0>(), pt.get<1>()));
        }

        limap::hmmmatch::LinkFeatureS links;
        links.emplace_back(1, 2, input_line_string);
        limap::hmmmatch::HmmLinkMatcher matcher;
        auto match_rets = matcher.Match(links, input_line_string);
        for (auto ret : match_rets) {
            cout << ret.linkid_ << " " << ret.direction_ << "\n";
        }
        cout << "HmmLinkMatcher test done\n";
    }
};

}
}

#endif
