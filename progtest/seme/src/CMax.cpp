#include "CMax.h"

double CMax::getNorm(CPos position)
{
    return abs(position.x) < abs(position.y) ? abs(position.y) : abs(position.x);
}

void CMax::draw(SDL_Renderer *renderer, CGameState &gamestate)
{
    drawGhost(renderer, gamestate, 150, 60, 10);
}

CPos CMax::getGuardPos(CGameState &gamestate)
{
    return CPos(gamestate.gameMap.BOARDWIDTH, gamestate.gameMap.BOARDHEIGHT);
}