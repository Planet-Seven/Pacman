#pragma once

#include "CCollectible.h"

/** \class CCoin
A class that represents a coin that increases player score on collection.
*/
class CCoin : public CCollectible
{
public:
    CCoin(CPos pos) : CCollectible(pos){};
    virtual void doEffect(CGameState &gamestate) override;
    virtual void draw(SDL_Renderer *renderer, CGameState &gamestate) override;
};