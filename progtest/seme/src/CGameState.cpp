#include "CGameState.h"

#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>
#include <iostream>

void CGameState::loadSpeed(std::ifstream &config)
{
    std::string valueName;
    getline(config, valueName, ':');
    if (valueName != "PLAYER_SPEED" || !(config >> PLAYER_SPEED))
        throw std::invalid_argument("unable to read player speed");

    getline(config, valueName); // flush the rest of the line
}
void CGameState::loadPowerUpGhostSlowdown(std::ifstream &config)
{
    std::string valueName;
    getline(config, valueName, ':');
    if (valueName != "POWER_UP_GHOST_SLOWDOWN" || !(config >> POWER_UP_GHOST_SLOWDOWN))
        throw std::invalid_argument("unable to read power up ghost slowdown");

    getline(config, valueName); // flush the rest of the line
}
void CGameState::loadInitialPowerUpTime(std::ifstream &config)
{
    std::string valueName;
    getline(config, valueName, ':');
    if (valueName != "INITIAL_POWERUP_TIME" || !(config >> INITIAL_POWERUP_TIME))
        throw std::invalid_argument("unable to read initial powerup time");

    getline(config, valueName); // flush the rest of the line
}
void CGameState::loadPowerUpTimeDecrement(std::ifstream &config)
{
    std::string valueName;
    getline(config, valueName, ':');
    if (valueName != "POWER_UP_TIME_DECREMENT" || !(config >> POWER_UP_TIME_DECREMENT))
        throw std::invalid_argument("unable to read power up time decrement");

    getline(config, valueName); // flush the rest of the line
}
void CGameState::loadTimeBetweenGuardMode(std::ifstream &config)
{
    std::string valueName;
    getline(config, valueName, ':');
    if (valueName != "TIME_BETWEEN_GUARD_MODE" || !(config >> TIME_BETWEEN_GUARD_MODE))
        throw std::invalid_argument("unable to read time between guard mode");

    getline(config, valueName); // flush the rest of the line
}
void CGameState::loadInitialGuardTime(std::ifstream &config)
{
    std::string valueName;
    getline(config, valueName, ':');
    if (valueName != "INITIAL_GUARD_TIME" || !(config >> INITIAL_GUARD_TIME))
        throw std::invalid_argument("unable to read initial guard time");

    getline(config, valueName); // flush the rest of the line
}
void CGameState::loadGuardTimeDecrement(std::ifstream &config)
{
    std::string valueName;
    getline(config, valueName, ':');
    if (valueName != "GUARD_TIME_DECREMENT" || !(config >> GUARD_TIME_DECREMENT))
        throw std::invalid_argument("unable to read guard time decrement");

    getline(config, valueName); // flush the rest of the line
}
void CGameState::loadWindowScale(std::ifstream &config)
{
    std::string valueName;
    getline(config, valueName, ':');
    if (valueName != "WINDOW_SCALE" || !(config >> WINDOW_SCALE))
        throw std::invalid_argument("unable to read window scale");

    getline(config, valueName); // flush the rest of the line

    WINDOW_WIDTH = WINDOW_SCALE * gameMap.BOARDWIDTH; // recalcuate window width and window height
    WINDOW_HEIGHT = WINDOW_SCALE * gameMap.BOARDHEIGHT;
}
void CGameState::loadBottomPadding(std::ifstream &config)
{
    std::string valueName;
    getline(config, valueName, ':');
    if (valueName != "BOTTOM_PADDING" || !(config >> BOTTOM_PADDING))
        throw std::invalid_argument("unable to read bottom padding");

    getline(config, valueName); // flush the rest of the line
}
void CGameState::loadFontSize(std::ifstream &config)
{
    std::string valueName;
    getline(config, valueName, ':');
    if (valueName != "FONT_SIZE" || !(config >> FONT_SIZE))
        throw std::invalid_argument("unable to read font size");

    getline(config, valueName); // flush the rest of the line
}
void CGameState::loadBoardWidth(std::ifstream &config)
{
    std::string valueName;
    getline(config, valueName, ':');
    if (valueName != "BOARD_WIDTH" || !(config >> gameMap.BOARDWIDTH))
        throw std::invalid_argument("unable to read board width");

    getline(config, valueName); // flush the rest of the line
}
void CGameState::loadBoardHeight(std::ifstream &config)
{
    std::string valueName;
    getline(config, valueName, ':');
    if (valueName != "BOARD_HEIGHT" || !(config >> gameMap.BOARDHEIGHT))
        throw std::invalid_argument("unable to read board height");

    getline(config, valueName); // flush the rest of the line
}

CGameMap::CMapObjects ASCIIToMapObject(char c)
{
    switch (c)
    {
    case 'W':
        return CGameMap::CMapObjects::W;
    case 'C':
        return CGameMap::CMapObjects::C;
    case 'S':
        return CGameMap::CMapObjects::S;
    case 'm':
        return CGameMap::CMapObjects::m;
    case 't':
        return CGameMap::CMapObjects::t;
    case 'e':
        return CGameMap::CMapObjects::e;
    case 'P':
        return CGameMap::CMapObjects::P;
    default:
        return CGameMap::CMapObjects::O;
    }
}

void CGameState::loadBoard(std::ifstream &config)
{
    std::string boardLine;
    if (!getline(config, boardLine)) // flush the first line
        throw std::invalid_argument("unable to read game board");

    for (int i = 0; i < gameMap.BOARDHEIGHT; i++)
    {
        if (!getline(config, boardLine))
            throw std::invalid_argument("unable to read game board");

        for (int j = 0; j < gameMap.BOARDWIDTH; j++)
            gameMap.map[i][j] = ASCIIToMapObject(boardLine[j]);
    }
}

void CGameState::loadConfig()
{
    std::ifstream config("./src/settings.conf");
    if (config.is_open())
    {
        try
        {
            loadSpeed(config);
            loadPowerUpGhostSlowdown(config);

            loadInitialPowerUpTime(config);
            loadPowerUpTimeDecrement(config);

            loadTimeBetweenGuardMode(config);
            loadInitialGuardTime(config);
            loadGuardTimeDecrement(config);

            loadWindowScale(config);
            loadBottomPadding(config);
            loadFontSize(config);

            loadBoardWidth(config);
            loadBoardHeight(config);

            loadBoard(config);
        }
        catch (std::invalid_argument &e)
        {
            std::cout << e.what() << " Please check the settings file." << std::endl;
        }

        config.close();
    }
    else
        std::cout << "Error loading settings. Using default settings instead." << std::endl;
}

bool CGameState::isNextMoveLegal()
{
    return isAMoveLegal(nextMove, playerPos);
}

bool CGameState::isThisMoveLegal()
{
    return isAMoveLegal(thisMove, playerPos);
}

bool CGameState::isAMoveLegal(CDirection move, CPos pos)
{
    std::pair<int, int> intPos = pos.getIntPos();
    const double threshold = 0.05;

    if (move == CDirection::left)
        if ((intPos.first > 0 &&
             gameMap.map[intPos.second][intPos.first - 1] == gameMap.W &&
             pos.x - intPos.first < threshold) ||

            (pos.y - intPos.second > threshold))

            return false;

    if (move == CDirection::right)
        if ((intPos.first < gameMap.BOARDWIDTH - 1 &&
             gameMap.map[intPos.second][intPos.first + 1] == gameMap.W) ||

            (pos.y - intPos.second > threshold))

            return false;

    if (move == CDirection::up)
        if ((intPos.second > 0 &&
             gameMap.map[intPos.second - 1][intPos.first] == gameMap.W &&
             pos.y - intPos.second < threshold) ||

            (pos.x - intPos.first > threshold))

            return false;

    if (move == CDirection::down)
        if ((intPos.second < gameMap.BOARDHEIGHT - 1 &&
             gameMap.map[intPos.second + 1][intPos.first] == gameMap.W) ||

            (pos.x - intPos.first > threshold))

            return false;

    return true;
}

void CGameState::updateMoves()
{
    thisMove = nextMove;
}

void CGameState::updatePos(double deltaTime)
{

    if (thisMove == CDirection::up)
        playerPos.y -= PLAYER_SPEED * deltaTime;

    else if (thisMove == CDirection::down)
        playerPos.y += PLAYER_SPEED * deltaTime;

    else if (thisMove == CDirection::left)
        playerPos.x -= PLAYER_SPEED * deltaTime;

    else if (thisMove == CDirection::right)
        playerPos.x += PLAYER_SPEED * deltaTime;

    if (playerPos.x < -1)
        playerPos.x += gameMap.BOARDWIDTH;

    else if (playerPos.y < -1)
        playerPos.y += gameMap.BOARDHEIGHT;

    else if (playerPos.x > gameMap.BOARDWIDTH)
        playerPos.x -= gameMap.BOARDWIDTH;

    else if (playerPos.y > gameMap.BOARDHEIGHT)
        playerPos.y -= gameMap.BOARDHEIGHT;
}

void CGameState::saveScore()
{
    highscores.push_back({score, level});
    std::sort(highscores.rbegin(), highscores.rend());
}