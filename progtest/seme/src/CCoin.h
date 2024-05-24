#pragma once

#include "CCollectible.h"

/** \class CCoin
A class that represents a coin that increases player score on collection.
*/
class CCoin : public CCollectible
{
public:
    ////////////////////////////////////////////////////////////////////////////////
    /// Constructor that creates the coin at a specified position
    ///
    /// @param [in] pos position
    CCoin(CPos pos) : CCollectible(pos){};

    ////////////////////////////////////////////////////////////////////////////////
    /// Draws the coin on the screen according to it's position.
    ///
    /// @param [in] renderer a pointer to the SDL_Renderer
    /// @param [in] gamestate a gamestate instance
    virtual void draw(SDL_Renderer *renderer, CGameState &gamestate) override;

private:
    ////////////////////////////////////////////////////////////////////////////////
    /// Implementation of the abstract doEffect, gives player a +1 score
    ///
    /// @param [out] gamestate a gamestate instance
    virtual void doEffect(CGameState &gamestate) override;
};