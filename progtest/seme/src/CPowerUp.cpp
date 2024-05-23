#include "CPowerUp.h"

void CPowerUp::doEffect(CGameState &gamestate)
{
    if (!collected)
    {
        gamestate.gamemode = CGameState::CGameMode::powerup;
        gamestate.powerUpRemaining = gamestate.powerUpTime;
    }
}
void CPowerUp::draw(SDL_Renderer *renderer, CGameState &gamestate)
{
    if (!collected)
    {
        const double powerUpScale = 0.6;
        const double offset = (1 - powerUpScale) / 2;

        SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);

        SDL_Rect coin =
            {static_cast<int>(gamestate.WINDOW_WIDTH / static_cast<double>(gamestate.gameMap.BOARDWIDTH) * pos.x +                    // computing this on every re-render is not ideal, as it is always the same value
                              static_cast<int>(gamestate.WINDOW_WIDTH / static_cast<double>(gamestate.gameMap.BOARDWIDTH) * offset)), // ideally, the compiler should know to replace it at compile time (which it maybe does with -O),
             static_cast<int>(gamestate.WINDOW_HEIGHT / static_cast<double>(gamestate.gameMap.BOARDHEIGHT) * pos.y +                  // passing it as an argument or having it as an attribute is not very elegant.
                              static_cast<int>(gamestate.WINDOW_WIDTH / static_cast<double>(gamestate.gameMap.BOARDWIDTH) * offset)), // It should be revisited later and benchmarked.
             static_cast<int>(gamestate.WINDOW_WIDTH / static_cast<double>(gamestate.gameMap.BOARDWIDTH) * powerUpScale),
             static_cast<int>(gamestate.WINDOW_HEIGHT / static_cast<double>(gamestate.gameMap.BOARDHEIGHT) * powerUpScale)};
        SDL_RenderFillRect(renderer, &coin);
    }
}