#pragma once

#include "CPos.h"
#include "CGameMap.h"
#include "CDirection.h"

#include <utility>
#include <vector>
#include <fstream>

/** \class CGameState
 A collection of variables and constants used as a context for other functions and methods.
*/
struct CGameState
{
    // configuration constants loaded from a config file. If no config file is present, or the loading fails, the defaults are used.
    int PLAYER_SPEED = 4;
    double POWER_UP_GHOST_SLOWDOWN = 0.75;

    int INITIAL_POWERUP_TIME = 5;
    int POWER_UP_TIME_DECREMENT = 1;

    int TIME_BETWEEN_GUARD_MODE = 30;
    int INITIAL_GUARD_TIME = 10;
    int GUARD_TIME_DECREMENT = 1;

    int WINDOW_SCALE = 30;
    int BOTTOM_PADDING = 100;
    int FONT_SIZE = 20;
    //------------------------------------------------------------------------------------------------------------------------------

    int level = 1;
    int score = 0;
    CPos playerPos;

    double powerUpTime = INITIAL_POWERUP_TIME;
    double powerUpRemaining = 0;

    double guardTime = INITIAL_GUARD_TIME;
    double guardTimeRemaining = 0;
    double nextGuard = TIME_BETWEEN_GUARD_MODE;

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
        powerup
    };

    std::vector<std::pair<int, int>> highscores;
    CScreen screen = CScreen::start;
    CDirection thisMove = CDirection::none; ///< player move that is currently being executed
    CDirection nextMove = CDirection::none; ///< player move that is next in line. It will either be cached or executed on the next update.
    CGameMode gamemode;                     ///< used to guide ghost behavior and enable eating interaction.
    CGameMap gameMap;                       ///< used to specify the postions of all game elements on game start

    int WINDOW_WIDTH = WINDOW_SCALE * gameMap.BOARDWIDTH;
    int WINDOW_HEIGHT = WINDOW_SCALE * gameMap.BOARDHEIGHT;

    bool isNextMoveLegal();
    bool isThisMoveLegal();
    bool isAMoveLegal(CDirection move, CPos pos);
    void updateMoves();
    void updatePos(double deltaTime);
    void saveScore();
    void loadConfig();

    void loadSpeed(std::ifstream &config);
    void loadPowerUpGhostSlowdown(std::ifstream &config);
    void loadInitialPowerUpTime(std::ifstream &config);
    void loadPowerUpTimeDecrement(std::ifstream &config);
    void loadTimeBetweenGuardMode(std::ifstream &config);
    void loadInitialGuardTime(std::ifstream &config);
    void loadGuardTimeDecrement(std::ifstream &config);
    void loadWindowScale(std::ifstream &config);
    void loadBottomPadding(std::ifstream &config);
    void loadFontSize(std::ifstream &config);
    void loadBoardWidth(std::ifstream &config);
    void loadBoardHeight(std::ifstream &config);
    void loadBoard(std::ifstream &config);
};