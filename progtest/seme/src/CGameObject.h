#pragma once
#include "CGameState.h"

#include <SDL2/SDL.h>

/** \class CGameObject
An abstract class that serves as the base for all game objects (ghosts and collectibles).
*/
class CGameObject
{
public:
    virtual ~CGameObject() = default;
    virtual void update(CGameState &gamestate, double deltaTime) = 0;
    virtual void draw(SDL_Renderer *renderer, CGameState &gamestate) = 0;

private:
};