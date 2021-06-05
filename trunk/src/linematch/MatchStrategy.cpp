#include "MatchStrategy.h"
#include "GeomCharacter.h"
namespace limap {
namespace match {

MatchStrategy::~MatchStrategy() {}

double LengthMatch::Match(const vector<GeomCharacter> &ss, const vector<GeomCharacter> &rs) {
    //init score-matrix
    vector<vector<double> > matrix(ss.size() + 1);
    for (size_t i = 0; i <= rs.size(); ++i) {
        matrix[0].push_back(.0);
    }
    for (size_t i = 1; i <= ss.size(); ++i) {
        matrix[i].push_back(.0);
    }
    for (size_t i = 1; i <= ss.size(); ++i) {
        for (size_t j = 1; j <= rs.size(); ++j) {
            matrix[i].push_back(.0);
        }
    }

    //fill score-matrix
    for (size_t i = 0; i < ss.size(); ++i) {
        for (size_t j = 0; j < rs.size(); ++j) {
            const GeomCharacter &s = ss.at(i);
            const GeomCharacter &r = rs.at(j);
            double valid_len(.0);
            if (r == s) {
                if (j == 0) {
                    valid_len = rs.at(j+1).IncLen() / 2.0;
                } else if (j == rs.size() - 1) {
                    valid_len = (rs.at(j).IncLen() - rs.at(j-1).IncLen()) / 2.0;

                } else {
                    valid_len = (rs.at(j+1).IncLen() - rs.at(j-1).IncLen()) / 2.0;
                }
            }
            double dist1 = valid_len + matrix[i][j];
            double dist2 = matrix[i][j+1];
            double dist3 = matrix[i+1][j];
            matrix[i+1][j+1] = std::max(std::max(dist1, dist2), dist3);
        }
    }
    return matrix[ss.size()][rs.size()];
}

EdrMatch::EdrMatch(double threshold):threshold_(threshold) {}

double EdrMatch::Match(const vector<GeomCharacter> &ss, const vector<GeomCharacter> &rs) {
    //init score-matrix
    vector<vector<double> > matrix(ss.size() + 1);
    for (size_t i = 0; i <= rs.size(); ++i) {
        matrix[0].push_back(double(i));
    }
    for (size_t i = 1; i <= ss.size(); ++i) {
        matrix[i].push_back(double(i));
    }
    for (size_t i = 1; i <= ss.size(); ++i) {
        for (size_t j = 1; j <= rs.size(); ++j) {
            matrix[i].push_back(.0);
        }
    }

    //fill score-matrix
    for (size_t i = 0; i < ss.size(); ++i) {
        for (size_t j = 0; j < rs.size(); ++j) {
            const GeomCharacter &s = ss.at(i);
            const GeomCharacter &r = rs.at(j);
            double dist(.0);
            if (not (s == r)) {
                dist = 1.0;
            }

            double dist1 = dist + matrix[i][j];
            double dist2 = matrix[i][j+1];
            double dist3 = matrix[i+1][j];
            matrix[i+1][j+1] = std::min(std::min(dist1, dist2), dist3);
        }
    }
    return matrix[ss.size()][rs.size()];
}

}
}
