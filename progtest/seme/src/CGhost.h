#pragma once
#include "CGameObject.h"
#include "CDirection.h"

/** \class CGhost
An abstract class that serves as the parent class of individual ghost personalities
*/
class CGhost : public CGameObject
{
public:
    CGhost(CPos pos) : startPos(pos), currentPos(pos), nextPos(pos){};

    virtual void update(CGameState &gamestate, double deltaTime) override;

    ////////////////////////////////////////////////////////////////////////////////
    /// Find the next position to take.
    ///
    /// \param[in] gamestate a gamestate variable
    ///
    /// Find the next position the ghost will take using the appropriate vector norm.
    void getNextPos(CGameState &gamestate);
    virtual double getNorm(CPos position) = 0;
    void drawGhost(SDL_Renderer *renderer, CGameState &gamestate, int R, int G, int B);

protected:
    const CPos startPos;
    CPos currentPos; ///< the position the ghost right now
    CPos nextPos;    ///< the position the ghost will take next
    CPos targetPos;  ///< the position of the target, usually the player's position
    CDirection direction = CDirection::none;
};

/** \class CManhattan
A ghost class that will use the manhattan vector norm for pathfinding
*/
class CManhattan : public CGhost
{
public:
    CManhattan(CPos pos) : CGhost(pos){};

    ////////////////////////////////////////////////////////////////////////////////
    /// Find the next position to take.
    ///
    /// \param[in] gamestate a gamestate variable
    ///
    /// Find the next position the ghost will take using the mahattan vector norm.
    virtual double getNorm(CPos position) override;

    ////////////////////////////////////////////////////////////////////////////////
    /// Draw the ghost on the screen.
    virtual void draw(SDL_Renderer *renderer, CGameState &gamestate) override;
};

/** \class CEuclid
A ghost class that will use the euclidean vector norm for pathfinding
*/
class CEuclid : public CGhost
{
public:
    CEuclid(CPos pos) : CGhost(pos){};

    ////////////////////////////////////////////////////////////////////////////////
    /// Find the next position to take.
    ///
    /// \param[in] gamestate a gamestate variable
    ///
    /// Find the next position the ghost will take using euclidean vector norm.
    virtual double getNorm(CPos position) override;

    ////////////////////////////////////////////////////////////////////////////////
    /// Draw the ghost on the screen.
    virtual void draw(SDL_Renderer *renderer, CGameState &gamestate) override;
};

/** \class CManhattan
A ghost class that will use the maximum vector norm for pathfinding
*/
class CMax : public CGhost
{
public:
    CMax(CPos pos) : CGhost(pos){};

    ////////////////////////////////////////////////////////////////////////////////
    /// Find the next position to take.
    ///
    /// \param[in] gamestate a gamestate variable
    ///
    /// Find the next position the ghost will take using maximum vector norm.
    virtual double getNorm(CPos position) override;

    ////////////////////////////////////////////////////////////////////////////////
    /// Draw the ghost on the screen.
    virtual void draw(SDL_Renderer *renderer, CGameState &gamestate) override;
};