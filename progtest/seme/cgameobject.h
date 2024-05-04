#pragma once
#include "cgamestate.h"

/** \class CGameObject
An abstract class that serves as the base for all game objects (ghosts and collectibles).
*/
class CGameObject
{
public:
    virtual ~CGameObject() = default;
    virtual void update(CGameState &gamestate) = 0;
    virtual void draw() = 0;

private:
};