#include "GeomCharacter.h"
#include <math.h>

namespace limap {
namespace match {

GeomCharacter::GeomCharacter(double inc_len, double disttoline, double slope, int convexity):
    inc_len_(inc_len), dist_to_line_(disttoline), slope_(slope), convexity_(convexity) {}

GeomCharacter::GeomCharacter():inc_len_(.0), dist_to_line_(1E7), slope_(.0), convexity_(.0) {}

//double GeomCharacter::Len() const {
//    return len_;
//}

double GeomCharacter::IncLen() const {
    return inc_len_;
}

double GeomCharacter::Slope() const {
    return slope_;
}

void GeomCharacter::Convexity(int val) {
    convexity_ = val;
}

//double GeomCharacter::Distance(const GeomCharacter &other) const {
//    return .0;
//}

bool operator==(const GeomCharacter &lhs, const GeomCharacter &rhs) {
    //TODO : adjust Threadhold number
    const double dist_threshold(2.0);
    const double slope_threshold(M_PI * 10 / 180.0);
    const int convexity_threshold(1);
    if (rhs.dist_to_line_ > dist_threshold or lhs.dist_to_line_ > dist_threshold) {
        return false;
    }
    if (std::abs(rhs.slope_ - lhs.slope_) > slope_threshold) {
        return false;
    }
    if (std::abs(rhs.convexity_ - lhs.convexity_) > convexity_threshold) {
        return false;
    }
    return true;
}

}
}
