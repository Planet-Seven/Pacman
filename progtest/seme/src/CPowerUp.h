#pragma once

#include "CCollectible.h"

/** \class CPowerUp
A class that represents a power pelet that allows the player to eat the gosts for a limited time.
*/
class CPowerUp : public CCollectible
{
public:
    CPowerUp(CPos pos) : CCollectible(pos){};
    virtual void doEffect(CGameState &gamestate) override;
    virtual void draw(SDL_Renderer *renderer, CGameState &gamestate) override;
};