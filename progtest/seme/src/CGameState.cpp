#include "CGameState.h"

// TODO - bound checking
bool CGameState::isNextMoveLegal()
{
    return true;
}

bool CGameState::isThisMoveLegal()
{
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