#include "CEuclid.h"

double CEuclid::getNorm(CPos position)
{
    return hypot(position.x, position.y);
}

void CEuclid::draw(SDL_Renderer *renderer, CGameState &gamestate)
{
    drawGhost(renderer, gamestate, 150, 0, 60);
}

CPos CEuclid::getGuardPos(CGameState &gamestate)
{
    return CPos(0, 0);
}