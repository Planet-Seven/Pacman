#pragma once

#include "CCollectible.h"

/** \class CPowerUp
A class that represents a power pelet that allows the player to eat the gosts for a limited time. It implements the abstract draw and doEffect methods.
*/
class CPowerUp : public CCollectible
{
public:
    ////////////////////////////////////////////////////////////////////////////////
    /// Constructor that creates the power up at a specified position
    ///
    /// @param [in] pos position
    CPowerUp(CPos pos) : CCollectible(pos){};

    ////////////////////////////////////////////////////////////////////////////////
    /// Draws the power up on the screen according to it's position.
    ///
    /// @param [in] renderer a pointer to the SDL_Renderer
    /// @param [in] gamestate a gamestate instance
    virtual void draw(SDL_Renderer *renderer, CGameState &gamestate) override;

private:
    ////////////////////////////////////////////////////////////////////////////////
    /// Implementation of the abstract doEffect, gives player the power up effect
    ///
    /// @param [out] gamestate a gamestate instance
    virtual void doEffect(CGameState &gamestate) override;
};