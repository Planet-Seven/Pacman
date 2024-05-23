#include "CCollectible.h"

void CCollectible::update(CGameState &gamestate, double deltaTime)
{
    if (gamestate.playerPos == pos)
    {
        doEffect(gamestate);
        collected = true;
    }
}