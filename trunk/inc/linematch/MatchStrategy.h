#ifndef __MATCH_STRATEGY_H__
#define __MATCH_STRATEGY_H__

#include <vector>


namespace limap {
namespace match {

class GeomCharacter;
using std::vector;

class MatchStrategy {
public:
    virtual ~MatchStrategy();
    virtual double Match(const vector<GeomCharacter> &ss, const vector<GeomCharacter> &rs) = 0;
};

class LengthMatch : public MatchStrategy {
public:
    double Match(const vector<GeomCharacter> &ss, const vector<GeomCharacter> &rs);
};

class EdrMatch : public MatchStrategy {
public:
    EdrMatch(double threshold);
    double Match(const vector<GeomCharacter> &ss, const vector<GeomCharacter> &rs);
protected:
    double threshold_;
};

}
}

#endif
