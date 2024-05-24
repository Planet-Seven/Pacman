#pragma once
#include "CGameState.h"

#include <SDL2/SDL.h>

/** \class CGameObject
An abstract class that serves as the base for all game objects (ghosts and collectibles).
*/
class CGameObject
{
public:
    ////////////////////////////////////////////////////////////////////////////////
    /// Default destructor
    virtual ~CGameObject() = default;

    ////////////////////////////////////////////////////////////////////////////////
    /// An abstract method, it's implementation will update the game object appropriately
    ///
    /// @param [in] gamestate a gamestate instance
    /// @param [in] deltaTime time since last frame
    virtual void update(CGameState &gamestate, double deltaTime) = 0;

    ////////////////////////////////////////////////////////////////////////////////
    /// An abstract method, it's implementation will draw the game object appropriately
    ///
    /// @param [in] renderer a pointer to the SDL_Renderer
    /// @param [in] gamestate a gamestate instance
    virtual void draw(SDL_Renderer *renderer, CGameState &gamestate) = 0;
};