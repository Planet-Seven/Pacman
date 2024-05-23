#include "CGameState.h"

#include <cmath>
#include <algorithm>
#include <vector>

constexpr double threshold = 0.05;

// TODO - bound checking
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

    if (move == CDirection::left)
        if ((intPos.first > 0 &&
             gameMap.map[intPos.second][intPos.first - 1] == gameMap.W &&
             pos.x - intPos.first < threshold) ||

            (pos.y - intPos.second > threshold))

            return false;

    if (move == CDirection::right)
        if ((intPos.first < BOARDWIDTH - 1 &&
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
        if ((intPos.second < BOARDHEIGHT - 1 &&
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
        playerPos.x += BOARDWIDTH;

    else if (playerPos.y < -1)
        playerPos.y += BOARDHEIGHT;

    else if (playerPos.x > BOARDWIDTH)
        playerPos.x -= BOARDWIDTH;

    else if (playerPos.y > BOARDHEIGHT)
        playerPos.y -= BOARDHEIGHT;
}

void CGameState::saveScore()
{
    highscores.push_back({score, level});
    std::sort(highscores.rbegin(), highscores.rend());
}