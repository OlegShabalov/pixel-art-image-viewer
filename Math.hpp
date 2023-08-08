#ifndef MATH_HPP
#define MATH_HPP

#include <algorithm>
#include <cmath>



inline double restrict(double value, double min, double max) {
    return std::min(max, std::max(min, value));
}
inline double restrictAbs(double value, double max) {
    return std::min(max, std::max(-max, value));
}

inline double maxD(double v1, double v2) {
    return std::max(v1, v2);
}
inline double minD(double v1, double v2) {
    return std::min(v1, v2);
}

inline int maxI(int v1, int v2) {
    return std::max(v1, v2);
}
inline int minI(int v1, int v2) {
    return std::min(v1, v2);
}

inline double powD(double x, double y) {
    return std::pow(x, y);
}



#endif // MATH_HPP
