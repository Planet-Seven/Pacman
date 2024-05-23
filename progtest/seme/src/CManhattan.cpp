#include "CManhattan.h"

double CManhattan::getNorm(CPos position)
{
    return abs(position.x) + abs(position.y);
}

void CManhattan::draw(SDL_Renderer *renderer, CGameState &gamestate)
{
    drawGhost(renderer, gamestate, 60, 150, 10);
}

CPos CManhattan::getGuardPos(CGameState &gamestate)
{
    return CPos(0, gamestate.gameMap.BOARDHEIGHT);
}