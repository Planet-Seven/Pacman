#pragma once

#include "cpos.h"
#include "cgamemap.h"

struct CGameState
{
    int level;
    CPos playerPos;

    enum class CGameMode
    {
        chase,
        guard,
        powerup,
        lightsout
    };

    enum class CDirection
    {
        none,
        up,
        left,
        down,
        right
    };

    CDirection thisMove = CDirection::none;
    CDirection nextMove = CDirection::none;
    CGameMode gamemode;
    CGameMap gameMap;
};