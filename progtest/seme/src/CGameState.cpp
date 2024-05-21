#include "CGameState.h"

#include "cmath"
constexpr double threshold = 0.005;

// TODO - bound checking
bool CGameState::isNextMoveLegal()
{
    return isAMoveLegal(nextMove);
}

bool CGameState::isThisMoveLegal()
{
    return isAMoveLegal(thisMove);
}

bool CGameState::isAMoveLegal(CGameState::CDirection move)
{
    std::pair<int, int> intPlayerPos = playerPos.getIntPos();

    if (move == CDirection::left && playerPos.x - intPlayerPos.first < threshold)
        if ((intPlayerPos.first != 0 &&
             gameMap.map[intPlayerPos.second][intPlayerPos.first - 1] == gameMap.W) ||

            (playerPos.y - intPlayerPos.second > threshold))

            return false;

    if (move == CDirection::right)
        if ((intPlayerPos.first != BOARDWIDTH - 1 &&
             gameMap.map[intPlayerPos.second][intPlayerPos.first + 1] == gameMap.W) ||

            (playerPos.y - intPlayerPos.second > threshold))

            return false;

    if (move == CDirection::up && playerPos.y - intPlayerPos.second < threshold)
        if ((static_cast<int>(playerPos.y) != 0 &&
             gameMap.map[intPlayerPos.second - 1][intPlayerPos.first] == gameMap.W) ||

            (playerPos.x - intPlayerPos.first > threshold))

            return false;

    if (move == CDirection::down)
        if ((intPlayerPos.second != BOARDHEIGHT - 1 &&
             gameMap.map[intPlayerPos.second + 1][intPlayerPos.first] == gameMap.W) ||

            (playerPos.x - intPlayerPos.first > threshold))

            return false;

    return true;
}

void CGameState::updateMoves()
{
    thisMove = nextMove;
}

void CGameState::updatePos(double deltaTime)
{

    if (thisMove == CGameState::CDirection::up)
        playerPos.y -= PLAYER_SPEED * deltaTime;

    if (thisMove == CGameState::CDirection::down)
        playerPos.y += PLAYER_SPEED * deltaTime;

    if (thisMove == CGameState::CDirection::left)
        playerPos.x -= PLAYER_SPEED * deltaTime;

    if (thisMove == CGameState::CDirection::right)
        playerPos.x += PLAYER_SPEED * deltaTime;

    if (playerPos.x < -1)
        playerPos.x += BOARDWIDTH;

    if (playerPos.y < -1)
        playerPos.x += BOARDHEIGHT;

    if (playerPos.x > BOARDWIDTH)
        playerPos.x -= BOARDWIDTH;

    if (playerPos.y > BOARDHEIGHT)
        playerPos.x -= BOARDHEIGHT;
}
