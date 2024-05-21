#include "CPos.h"

#include "cmath"

std::pair<int, int> CPos::getIntPos() const
{
    return std::make_pair<int, int>(static_cast<int>(x), static_cast<int>(y));
}

bool CPos::operator==(const CPos &rhs) const
{
    return (round(rhs.x) == round(x) && round(rhs.y) == round(y));
}
