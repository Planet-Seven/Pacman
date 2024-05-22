#pragma once

#include <utility>

/** \class CPos
Used to represent a position on the game board.
*/
struct CPos
{
    CPos() = default;
    CPos(double x, double y) : x(x), y(y){};
    double x; ///< x coordinate
    double y; ///< y coordinate

    std::pair<int, int> getIntPos() const;
    bool operator==(const CPos &rhs) const;
    CPos operator-(const CPos &rhs);
};