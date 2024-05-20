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
    if (move == CDirection::left && playerPos.x - floor(playerPos.x) < threshold)
        if (static_cast<int>(playerPos.x) != 0 &&
            gameMap.map[static_cast<int>(playerPos.y)][static_cast<int>(floor(playerPos.x) - 1)] == gameMap.W)
            return false;

    if (move == CDirection::right)
        if (static_cast<int>(playerPos.x) != BOARDWIDTH &&
            gameMap.map[static_cast<int>(playerPos.y)][static_cast<int>(playerPos.x) + 1] == gameMap.W)
            return false;

    if (move == CDirection::up && playerPos.y - floor(playerPos.y) < threshold)
        if (static_cast<int>(playerPos.y) != 0 &&
            gameMap.map[static_cast<int>(floor(playerPos.y) - 1)][static_cast<int>(playerPos.x)] == gameMap.W)
            return false;

    if (move == CDirection::down)
        if (static_cast<int>(playerPos.y) != BOARDHEIGHT &&
            gameMap.map[static_cast<int>(playerPos.y) + 1][static_cast<int>(playerPos.x)] == gameMap.W)
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
}