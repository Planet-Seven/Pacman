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
    void drawGhost(SDL_Renderer *renderer, CGameState &gamestate, int R, int G, int B);

protected:
    virtual double getNorm(CPos position) = 0;
    virtual CPos getGuardPos(CGameState &gamestate) = 0;
    void findPossibleMoves(CGameState &gamestate, std::vector<std::pair<CPos, CDirection>> &possibleMoves);
    void pickBestMove(CGameState &gamestate, std::vector<std::pair<CPos, CDirection>> &possibleMoves);
    void handlePlayerCollision(CGameState &gamestate);
    void setTargetPos(CGameState &gamestate);
    void getNextPos(CGameState &gamestate);
    void updatePos(CGameState &gamestate, double deltaTime, double speed);
    const CPos startPos;
    CPos currentPos; ///< the position the ghost right now
    CPos nextPos;    ///< the position the ghost will take next
    CPos targetPos;  ///< the position of the target, usually the player's position
    CDirection direction = CDirection::none;
};