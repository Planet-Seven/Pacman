#include "CCollectible.h"

void CCollectible::update(CGameState &gamestate)
{
    if (gamestate.playerPos == pos)
    {
        doEffect(gamestate);
        collected = true;
    }
}

void CCoin::doEffect(CGameState &gamestate)
{
    if (!collected)
        gamestate.gameMap.coinCount -= 1;
}
void CCoin::draw()
{
}

void CPowerUp::doEffect(CGameState &gamestate)
{
}
void CPowerUp::draw()
{
}