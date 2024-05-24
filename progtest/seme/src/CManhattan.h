#pragma once

#include "CGhost.h"

/** \class CManhattan
A ghost class that will use the manhattan vector norm for pathfinding
*/
class CManhattan : public CGhost
{
public:
    ////////////////////////////////////////////////////////////////////////////////
    /// Constructor that creates Manhattan at the specified position.
    ///
    /// @param [in] pos isnitial position
    CManhattan(CPos pos) : CGhost(pos){};

    ////////////////////////////////////////////////////////////////////////////////
    /// Draws Manhattan on the screen according to his current position.
    ///
    /// @param [in] renderer a pointer to the SDL_Renderer
    /// @param [in] gamestate a gamestate instance
    virtual void draw(SDL_Renderer *renderer, CGameState &gamestate) override;

protected:
    ////////////////////////////////////////////////////////////////////////////////
    /// Returns the Manhattan norm of a position that is interpreted as a vector
    ///
    /// @param[in] position a 2-d vector
    virtual double getNorm(CPos position) override;

    ////////////////////////////////////////////////////////////////////////////////
    /// Returns the position Manhattan wants to take when he's in guard mode.
    ///
    /// @param[in] gamestate a gamestate instance
    virtual CPos getGuardPos(CGameState &gamestate) override;
};