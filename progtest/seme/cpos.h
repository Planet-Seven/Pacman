#pragma once
struct CPos
{
    CPos() = default;
    CPos(double x, double y) : x(x), y(y){};
    double x;
    double y;
};