#pragma once

#include "CGhost.h"

/** \class CManhattan
A ghost class that will use the manhattan vector norm for pathfinding
*/
class CManhattan : public CGhost
{
public:
    CManhattan(CPos pos) : CGhost(pos){};
    ////////////////////////////////////////////////////////////////////////////////
    /// Draw the ghost on the screen.
    virtual void draw(SDL_Renderer *renderer, CGameState &gamestate) override;

protected:
    ////////////////////////////////////////////////////////////////////////////////
    /// Find the next position to take.
    ///
    /// \param[in] gamestate a gamestate variable
    ///
    /// Find the next position the ghost will take using the mahattan vector norm.
    virtual double getNorm(CPos position) override;
    virtual CPos getGuardPos(CGameState &gamestate) override;
};