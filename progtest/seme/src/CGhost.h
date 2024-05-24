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

    ////////////////////////////////////////////////////////////////////////////////
    /// Updates the ghost's position, checks collision with the player and gets his next move
    ///
    /// @param [in] renderer a pointer to the SDL_Renderer
    /// @param [in] deltaTime time since last frame
    virtual void update(CGameState &gamestate, double deltaTime) override;

    ////////////////////////////////////////////////////////////////////////////////
    /// Draws the ghost on his actual position as a rectangle of the color specified.
    ///
    /// @param [in] renderer a pointer to the SDL_Renderer
    /// @param [in] gamestate a gamestate instance
    /// @param [in] R red
    /// @param [in] G green
    /// @param [in] B blue
    void drawGhost(SDL_Renderer *renderer, CGameState &gamestate, int R, int G, int B);

protected:
    ////////////////////////////////////////////////////////////////////////////////
    /// An abstract method who's implementation will return a specific norm of a vector.
    ///
    /// @param[in] position a 2-d vector
    virtual double getNorm(CPos position) = 0;

    ////////////////////////////////////////////////////////////////////////////////
    /// An abstract method who's implementation will return the position the ghost will take in guard mode.
    ///
    /// @param[in] gamestate a gamestate instance
    virtual CPos getGuardPos(CGameState &gamestate) = 0;

    ////////////////////////////////////////////////////////////////////////////////
    /// Finds all moves that are legal and stores them in the possibleMoves vector
    ///
    /// @param[in] gamestate a gamestate instance
    /// @param[out] possibleMoves a gamestate instance
    void findPossibleMoves(CGameState &gamestate, std::vector<std::pair<CPos, CDirection>> &possibleMoves);

    ////////////////////////////////////////////////////////////////////////////////
    /// Pick the best move out of possible moves. This is the move the ghost will take next.
    ///
    /// @param[in] gamestate a gamestate instance
    /// @param[in] possibleMoves a gamestate instance
    void pickBestMove(CGameState &gamestate, std::vector<std::pair<CPos, CDirection>> &possibleMoves);

    ////////////////////////////////////////////////////////////////////////////////
    /// Handle player collision accordingly, depending on if a power up is active.
    ///
    /// @param[in, out] gamestate a gamestate instance
    void handlePlayerCollision(CGameState &gamestate);

    ////////////////////////////////////////////////////////////////////////////////
    /// Set target position accordingly, depending on if guard mode, power up or chase is currently active
    ///
    /// @param[in, out] gamestate a gamestate instance
    void setTargetPos(CGameState &gamestate);

    ////////////////////////////////////////////////////////////////////////////////
    /// Find the next position the ghost will take. Checks legal moves and picks the best one.
    ///
    /// @param[in] gamestate a gamestate instance
    void getNextPos(CGameState &gamestate);

    ////////////////////////////////////////////////////////////////////////////////
    /// Update the ghost's position based on the direction he's going, speed and delta time.
    ///
    /// @param[in] gamestate a gamestate instance
    /// @param[in] deltaTime time since last frame
    /// @param[in] speed the speed of the ghost
    void updatePos(CGameState &gamestate, double deltaTime, double speed);

    const CPos startPos;                     ///< the starting position of the ghost
    CPos currentPos;                         ///< the position the ghost right now
    CPos nextPos;                            ///< the position the ghost will take next
    CPos targetPos;                          ///< the position of the target, usually the player's position
    CDirection direction = CDirection::none; ///< the direction the ghost is moving
};