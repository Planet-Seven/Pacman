#pragma once

#include <utility>

/** \class CPos
Used to represent a position on the game board.
*/
struct CPos
{
    ////////////////////////////////////////////////////////////////////////////////
    /// Default constructor, initializes the position to (0,0)
    CPos() = default;

    ////////////////////////////////////////////////////////////////////////////////
    /// Initializes the pos with x and y coordinates
    ///
    /// @param [in] x x coordinate
    /// @param [in] y y coordinate
    CPos(double x, double y) : x(x), y(y){};
    double x; ///< x coordinate
    double y; ///< y coordinate

    ////////////////////////////////////////////////////////////////////////////////
    /// Returns the position static-casted to two ints
    std::pair<int, int> getIntPos() const;

    ////////////////////////////////////////////////////////////////////////////////
    /// ROUNDS two position and compares them
    ///
    /// @param [in] rhs right operand
    bool operator==(const CPos &rhs) const;

    ////////////////////////////////////////////////////////////////////////////////
    /// subtracts the rhs from this. Useful for vector operations.
    ///
    /// @param [in] rhs right operand
    CPos operator-(const CPos &rhs);
};