#pragma once
constexpr int MAXWIDTH = 100;

/** \class CGameMap
A 2-d array and a set of constants to represent the game board.
*/
struct CGameMap
{

    int BOARDHEIGHT = 31; ///< configuration constant loaded from a config file. If no config file is present, or the loading fails, the default is used.
    int BOARDWIDTH = 28;  ///< configuration constant loaded from a config file. If no config file is present, or the loading fails, the default is used.
    int coinCount = 240;  ///< configuration constant loaded from a config file. If no config file is present, or the loading fails, the default is used.

    enum CMapObjects
    {
        W, ///< wall
        C, ///< coin
        P, ///< powerup
        S, ///< player start pos
        m, ///< Max start pos
        t, ///< manhaTtan start pos
        e, ///< Euclides start pos
        O  ///< empty
    };
    short unsigned int map[MAXWIDTH][MAXWIDTH] = {
        {W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W},
        {W, C, C, C, C, C, C, C, C, C, C, C, C, W, W, C, C, C, C, C, C, C, C, C, C, C, C, W},
        {W, C, W, W, W, W, C, W, W, W, W, W, C, W, W, C, W, W, W, W, W, C, W, W, W, W, C, W},
        {W, P, W, W, W, W, C, W, W, W, W, W, C, W, W, C, W, W, W, W, W, C, W, W, W, W, P, W},
        {W, C, W, W, W, W, C, W, W, W, W, W, C, W, W, C, W, W, W, W, W, C, W, W, W, W, C, W},
        {W, C, C, C, C, C, C, C, C, C, C, C, C, C, C, C, C, C, C, C, C, C, C, C, C, C, C, W},
        {W, C, W, W, W, W, C, W, W, C, W, W, W, W, W, W, W, W, C, W, W, C, W, W, W, W, C, W},
        {W, C, W, W, W, W, C, W, W, C, W, W, W, W, W, W, W, W, C, W, W, C, W, W, W, W, C, W},
        {W, C, C, C, C, C, C, W, W, C, C, C, C, W, W, C, C, C, C, W, W, C, C, C, C, C, C, W},
        {W, W, W, W, W, W, C, W, W, W, W, W, O, W, W, O, W, W, W, W, W, C, W, W, W, W, W, W},
        {W, W, W, W, W, W, C, W, W, W, W, W, O, W, W, O, W, W, W, W, W, C, W, W, W, W, W, W},
        {W, W, W, W, W, W, C, W, W, O, O, O, O, O, O, O, O, O, O, W, W, C, W, W, W, W, W, W},
        {W, W, W, W, W, W, C, W, W, O, W, W, W, O, O, W, W, W, O, W, W, C, W, W, W, W, W, W},
        {W, W, W, W, W, W, C, W, W, O, W, O, O, O, O, O, O, W, O, W, W, C, W, W, W, W, W, W},
        {O, O, O, O, O, O, C, O, O, O, O, O, m, t, e, O, O, O, O, O, O, C, O, O, O, O, O, O},
        {W, W, W, W, W, W, C, W, W, O, W, O, O, O, O, O, O, W, O, W, W, C, W, W, W, W, W, W},
        {W, W, W, W, W, W, C, W, W, O, W, W, W, O, O, W, W, W, O, W, W, C, W, W, W, W, W, W},
        {W, W, W, W, W, W, C, W, W, O, O, O, O, O, O, O, O, O, O, W, W, C, W, W, W, W, W, W},
        {W, W, W, W, W, W, C, W, W, O, W, W, W, W, W, W, W, W, O, W, W, C, W, W, W, W, W, W},
        {W, W, W, W, W, W, C, W, W, O, W, W, W, W, W, W, W, W, O, W, W, C, W, W, W, W, W, W},
        {W, C, C, C, C, C, C, C, C, C, C, C, C, W, W, C, C, C, C, C, C, C, C, C, C, C, C, W},
        {W, C, W, W, W, W, C, W, W, W, W, W, C, W, W, C, W, W, W, W, W, C, W, W, W, W, C, W},
        {W, C, W, W, W, W, C, W, W, W, W, W, C, W, W, C, W, W, W, W, W, C, W, W, W, W, C, W},
        {W, P, C, C, W, W, C, C, C, C, C, C, C, S, O, C, C, C, C, C, C, C, W, W, C, C, P, W},
        {W, W, W, C, W, W, C, W, W, C, W, W, W, W, W, W, W, W, C, W, W, C, W, W, C, W, W, W},
        {W, W, W, C, W, W, C, W, W, C, W, W, W, W, W, W, W, W, C, W, W, C, W, W, C, W, W, W},
        {W, C, C, C, C, C, C, W, W, C, C, C, C, W, W, C, C, C, C, W, W, C, C, C, C, C, C, W},
        {W, C, W, W, W, W, W, W, W, W, W, W, C, W, W, C, W, W, W, W, W, W, W, W, W, W, C, W},
        {W, C, W, W, W, W, W, W, W, W, W, W, C, W, W, C, W, W, W, W, W, W, W, W, W, W, C, W},
        {W, C, C, C, C, C, C, C, C, C, C, C, C, C, C, C, C, C, C, C, C, C, C, C, C, C, C, W},
        {W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W},
    }; ///< configuration constant loaded from a config file. If no config file is present, or the loading fails, the default is used. A 2-d array with enum constants is used to represent the game board.
};