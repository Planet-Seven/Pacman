#pragma once
#include "cgamestate.h"

class CGameObject
{
public:
    virtual ~CGameObject() = default;
    virtual void update(CGameState &gamestate) = 0;
    virtual void draw() = 0;

private:
};