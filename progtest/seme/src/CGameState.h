#pragma once

#include "../config.h"

#include "CPos.h"
#include "CGameMap.h"
#include "CDirection.h"

#include <utility>
#include <vector>

/** \class CGameState
 A collection of variables and constants used as a context for other functions and methods.
*/
constexpr int WINDOW_WIDTH = WINDOW_SCALE * 28; // choose a multiple of 28
constexpr int WINDOW_HEIGHT = BOARDHEIGHT * (static_cast<double>(WINDOW_WIDTH) / BOARDWIDTH);

struct CGameState
{
    int level = 1;
    int score = 0;
    CPos playerPos;
    double powerUpTime = INITIAL_POWERUP_TIME;
    double powerUpRemaining = 0;

    enum class CScreen
    {
        start,
        playing,
        gameOver,
        scoreBoard
    };

    enum class CGameMode
    {
        chase,
        guard,
        powerup,
        lightsout
    };

    std::vector<std::pair<int, int>> highscores;
    CScreen screen = CScreen::start;
    CDirection thisMove = CDirection::none; ///< player move that is currently being executed
    CDirection nextMove = CDirection::none; ///< player move that is next in line. It will either be cached or executed on the next update.
    CGameMode gamemode;                     ///< used to guide ghost behavior and enable eating interaction.
    CGameMap gameMap;                       ///< used to specify the postions of all game elements on game start

    bool isNextMoveLegal();
    bool isThisMoveLegal();
    bool isAMoveLegal(CDirection move, CPos pos);
    void updateMoves();
    void updatePos(double deltaTime);
    void saveScore();
};