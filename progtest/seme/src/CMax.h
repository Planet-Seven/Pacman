#pragma once

#include "CGhost.h"

/** \class CManhattan
A ghost class that will use the maximum vector norm for pathfinding
*/
class CMax : public CGhost
{
public:
    CMax(CPos pos) : CGhost(pos){};

    ////////////////////////////////////////////////////////////////////////////////
    /// Draw the ghost on the screen.
    virtual void draw(SDL_Renderer *renderer, CGameState &gamestate) override;

protected:
    ////////////////////////////////////////////////////////////////////////////////
    /// Find the next position to take.
    ///
    /// \param[in] gamestate a gamestate variable
    ///
    /// Find the next position the ghost will take using maximum vector norm.
    virtual double getNorm(CPos position) override;
    virtual CPos getGuardPos(CGameState &gamestate) override;
};