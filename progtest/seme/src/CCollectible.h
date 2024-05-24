#pragma once
#include "CGameObject.h"

/** \class CCollectible
An abstract class that serves as the parent class of individual collectible items.
*/
class CCollectible : public CGameObject
{
public:
    ////////////////////////////////////////////////////////////////////////////////
    /// Constructor that creates the collectible at a specified position
    ///
    /// @param [in] pos position
    CCollectible(CPos pos) : pos(pos){};

    ////////////////////////////////////////////////////////////////////////////////
    /// Checks collision with the player and if a collision is detected, does the appropriate effect
    ///
    /// @param [in] renderer a pointer to the SDL_Renderer
    /// @param [in] deltaTime time since last frame
    virtual void update(CGameState &gamestate, double deltaTime) override;

protected:
    ////////////////////////////////////////////////////////////////////////////////
    ///  An abstract method, it's implementation will give the player the appropriate effect
    ///
    /// @param [out] gamestate a gamestate instance
    virtual void doEffect(CGameState &gamestate) = 0;

    CPos pos;               ///< the position of the collectible
    bool collected = false; ///< a bool that indicates if the collectible was collected by a player
};