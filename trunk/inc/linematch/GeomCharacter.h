#ifndef __GEOM_CHARACTER_H__
#define __GEOM_CHARACTER_H__

namespace limap {
namespace match {

class GeomCharacter {
    friend bool operator==(const GeomCharacter &lhs, const GeomCharacter &rhs);
public:
    GeomCharacter(double inc_len, double disttoline, double slope, int convexity);
    GeomCharacter();
    //double Distance(const GeomCharacter &other) const;
public:
    //double Len() const;
    double IncLen() const;
    double Slope() const;
    void Convexity(int val);

private:
    //double len_;
    double inc_len_;
    double dist_to_line_;
    double slope_;
    int convexity_;
};

}
}

#endif
