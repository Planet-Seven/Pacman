#pragma once

#include "CGhost.h"

/** \class CEuclid
A ghost class that will use the euclidean vector norm for pathfinding
*/
class CEuclid : public CGhost
{
public:
    CEuclid(CPos pos) : CGhost(pos){};
    ////////////////////////////////////////////////////////////////////////////////
    /// Draw the ghost on the screen.
    virtual void draw(SDL_Renderer *renderer, CGameState &gamestate) override;

protected:
    ////////////////////////////////////////////////////////////////////////////////
    /// Find the next position to take.
    ///
    /// \param[in] gamestate a gamestate variable
    ///
    /// Find the next position the ghost will take using euclidean vector norm.
    virtual double getNorm(CPos position) override;
    virtual CPos getGuardPos(CGameState &gamestate) override;
};