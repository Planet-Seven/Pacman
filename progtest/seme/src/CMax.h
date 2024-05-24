#pragma once

#include "CGhost.h"

/** \class CMax
A ghost class that will use the maximum vector norm for pathfinding
*/
class CMax : public CGhost
{
public:
    ////////////////////////////////////////////////////////////////////////////////
    /// Constructor that creates Max at the specified position.
    ///
    /// @param [in] pos isnitial position
    CMax(CPos pos) : CGhost(pos){};

    ////////////////////////////////////////////////////////////////////////////////
    /// Draws Max on the screen according to his current position.
    ///
    /// @param [in] renderer a pointer to the SDL_Renderer
    /// @param [in] gamestate a gamestate instance
    virtual void draw(SDL_Renderer *renderer, CGameState &gamestate) override;

protected:
    ////////////////////////////////////////////////////////////////////////////////
    /// Returns the max norm of a position that is interpreted as a vector
    ///
    /// @param[in] position a 2-d vector
    virtual double getNorm(CPos position) override;

    ////////////////////////////////////////////////////////////////////////////////
    /// Returns the position Max wants to take when he's in guard mode.
    ///
    /// @param[in] gamestate a gamestate instance
    virtual CPos getGuardPos(CGameState &gamestate) override;
};