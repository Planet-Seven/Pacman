#pragma once

#include "CCollectible.h"

/** \class CPowerUp
A class that represents a power pelet that allows the player to eat the gosts for a limited time. It implements the abstract draw and doEffect methods.
*/
class CPowerUp : public CCollectible
{
public:
    CPowerUp(CPos pos) : CCollectible(pos){};
    virtual void draw(SDL_Renderer *renderer, CGameState &gamestate) override;

private:
    virtual void doEffect(CGameState &gamestate) override;
};