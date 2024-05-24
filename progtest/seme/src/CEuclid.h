#pragma once

#include "CGhost.h"

/** \class CEuclid
A ghost class that will use the euclidean vector norm for pathfinding
*/
class CEuclid : public CGhost
{
public:
    ////////////////////////////////////////////////////////////////////////////////
    /// Constructor that creates Euclid at the specified position.
    ///
    /// @param [in] pos initial position
    CEuclid(CPos pos) : CGhost(pos){};

    ////////////////////////////////////////////////////////////////////////////////
    /// Draws Euclid on the screen according to his current position.
    ///
    /// @param [in] renderer a pointer to the SDL_Renderer
    /// @param [in] gamestate a gamestate instance
    virtual void draw(SDL_Renderer *renderer, CGameState &gamestate) override;

protected:
    ////////////////////////////////////////////////////////////////////////////////
    /// Returns the standart (Euclidean) norm of a position that is interpreted as a vector
    ///
    /// @param[in] position a 2-d vector
    virtual double getNorm(CPos position) override;

    ////////////////////////////////////////////////////////////////////////////////
    /// Returns the position Euclid wants to take when he's in guard mode.
    ///
    /// @param[in] gamestate a gamestate instance
    virtual CPos getGuardPos(CGameState &gamestate) override;
};