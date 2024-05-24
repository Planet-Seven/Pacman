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
    int PLAYER_SPEED = 4;                  ///< Tiles/second. Same speed is used for ghost in chase mode. Configuration constant loaded from a config file. If no config file is present, or the loading fails, the default is used.
    double POWER_UP_GHOST_SLOWDOWN = 0.75; ///< Multiplier for ghost speed while a power-up is active. Configuration constant loaded from a config file. If no config file is present, or the loading fails, the default is used.

    int INITIAL_POWERUP_TIME = 5;    ///< Seconds. Configuration constant loaded from a config file. If no config file is present, or the loading fails, the default is used.
    int POWER_UP_TIME_DECREMENT = 1; ///< Seconds, decrement on level increase. Configuration constant loaded from a config file. If no config file is present, or the loading fails, the default is used.

    int TIME_BETWEEN_GUARD_MODE = 30; ///< Seconds. configuration constant loaded from a config file. If no config file is present, or the loading fails, the default is used.
    int INITIAL_GUARD_TIME = 10;      ///< Seconds. configuration constant loaded from a config file. If no config file is present, or the loading fails, the default is used.
    int GUARD_TIME_DECREMENT = 1;     ///< Seconds, decrement on level increase. Configuration constant loaded from a config file. If no config file is present, or the loading fails, the default is used.

    int WINDOW_SCALE = 30;    ///< Window scale * board width is the window width in pixels. Same goes for height. Configuration constant loaded from a config file. If no config file is present, or the loading fails, the default is used.
    int BOTTOM_PADDING = 100; ///< Pixels. Configuration constant loaded from a config file. If no config file is present, or the loading fails, the default is used.
    int FONT_SIZE = 20;       ///< No idea what's the unit. Check TTF_OpenFont documentation. Configuration constant loaded from a config file. If no config file is present, or the loading fails, the default is used.

    int level = 1;  ///< current level
    int score = 0;  ///< current score
    CPos playerPos; ///< current position of the player

    double powerUpTime = INITIAL_POWERUP_TIME; ///< power up time corresponding to the current level
    double powerUpRemaining = 0;               ///< active power up time remaining

    double guardTime = INITIAL_GUARD_TIME;      ///< guard time corresponding to the current level
    double guardTimeRemaining = 0;              ///< active guard time remaining
    double nextGuard = TIME_BETWEEN_GUARD_MODE; ///< time until next guard mode

    /** \class CScreen
     enum class that represents the individual screens of the game
    */
    enum class CScreen
    {
        start,     ///< start screen, seen only when the game is first opened
        playing,   ///< playing screen, seen when the game is active
        gameOver,  ///< game over screen, seen, naturally, when the game is over
        scoreBoard ///< score boeard screen, shows the score leaderboard
    };

    /** \class CScreen
     enum class that represents the individual game modes
    */
    enum class CGameMode
    {
        chase,  ///< Ghosts are actively chasing the player.
        guard,  ///< Ghosts are guarding a sector.
        powerup ///< The player can eat the ghosts.
    };

    std::vector<std::pair<int, int>> highscores; ///< vector of high scores, sorted highest to lowest.
    CScreen screen = CScreen::start;             ///< currently active screen
    CDirection thisMove = CDirection::none;      ///< player move that is currently being executed
    CDirection nextMove = CDirection::none;      ///< player move that is next in line. It will either be cached or executed on the next update.
    CGameMode gamemode;                          ///< used to guide ghost behavior and enable eating interaction.
    CGameMap gameMap;                            ///< used to specify the postions of all game elements on game start

    int WINDOW_WIDTH = WINDOW_SCALE * gameMap.BOARDWIDTH;   ///< Window width in pixels
    int WINDOW_HEIGHT = WINDOW_SCALE * gameMap.BOARDHEIGHT; ///< Window height in pixels (bottom padding is not included)

    ////////////////////////////////////////////////////////////////////////////////
    /// Checks if the next player move is legal
    bool isNextMoveLegal();

    ////////////////////////////////////////////////////////////////////////////////
    /// Checks if the current player move is legal
    bool isThisMoveLegal();

    ////////////////////////////////////////////////////////////////////////////////
    /// Checks if a move from a position is legal
    ///
    /// @param [in] move the move being checked
    /// @param [in] pos move is from this position
    bool isAMoveLegal(CDirection move, CPos pos);

    ////////////////////////////////////////////////////////////////////////////////
    /// When possible, sets thisMove to nextMove
    void updateMoves();

    ////////////////////////////////////////////////////////////////////////////////
    /// Updates player's position according to deltaTime and player speed
    ///
    /// @param [in] deltaTime Time since last frame.
    void updatePos(double deltaTime);

    ////////////////////////////////////////////////////////////////////////////////
    /// Saves the current score to the highscores vector.
    void saveScore();

    ////////////////////////////////////////////////////////////////////////////////
    /// Loads all of the constants from the config file.
    void loadConfig();

    ////////////////////////////////////////////////////////////////////////////////
    /// Loads PLAYER_SPEED from an ifstream
    ///
    /// @param [in] config Ifstream that is being loaded
    void loadSpeed(std::ifstream &config);

    ////////////////////////////////////////////////////////////////////////////////
    /// Loads POWER_UP_GHOST_SLOWDOWN from an ifstream
    ///
    /// @param [in] config Ifstream that is being loaded
    void loadPowerUpGhostSlowdown(std::ifstream &config);

    ////////////////////////////////////////////////////////////////////////////////
    /// Loads INITIAL_POWER_UP_TIME from an ifstream
    ///
    /// @param [in] config Ifstream that is being loaded
    void loadInitialPowerUpTime(std::ifstream &config);

    ////////////////////////////////////////////////////////////////////////////////
    /// Loads POWER_UP_TIME_DECREMENT from an ifstream
    ///
    /// @param [in] config Ifstream that is being loaded
    void loadPowerUpTimeDecrement(std::ifstream &config);

    ////////////////////////////////////////////////////////////////////////////////
    /// Loads TIME_BETWEEN_GUARD_MODE from an ifstream
    ///
    /// @param [in] config Ifstream that is being loaded
    void loadTimeBetweenGuardMode(std::ifstream &config);

    ////////////////////////////////////////////////////////////////////////////////
    /// Loads INITIAL_GUARD_TIME from an ifstream
    ///
    /// @param [in] config Ifstream that is being loaded
    void loadInitialGuardTime(std::ifstream &config);

    ////////////////////////////////////////////////////////////////////////////////
    /// Loads GUARD_TIME_DECREMENT from an ifstream
    ///
    /// @param [in] config Ifstream that is being loaded
    void loadGuardTimeDecrement(std::ifstream &config);

    ////////////////////////////////////////////////////////////////////////////////
    /// Loads WINDOW_SCALE from an ifstream
    ///
    /// @param [in] config Ifstream that is being loaded
    void loadWindowScale(std::ifstream &config);

    ////////////////////////////////////////////////////////////////////////////////
    /// Loads BOTTOM_PADDING from an ifstream
    ///
    /// @param [in] config Ifstream that is being loaded
    void loadBottomPadding(std::ifstream &config);

    ////////////////////////////////////////////////////////////////////////////////
    /// Loads FONT_SIZE from an ifstream
    ///
    /// @param [in] config Ifstream that is being loaded
    void loadFontSize(std::ifstream &config);

    ////////////////////////////////////////////////////////////////////////////////
    /// Loads BOARD_WIDTH from an ifstream
    ///
    /// @param [in] config Ifstream that is being loaded
    void loadBoardWidth(std::ifstream &config);

    ////////////////////////////////////////////////////////////////////////////////
    /// Loads BOARD_HEIGHT from an ifstream
    ///
    /// @param [in] config Ifstream that is being loaded
    void loadBoardHeight(std::ifstream &config);

    ////////////////////////////////////////////////////////////////////////////////
    /// Loads the game map from an ifstream
    ///
    /// @param [in] config Ifstream that is being loaded
    void loadBoard(std::ifstream &config);
};