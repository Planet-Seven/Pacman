#include "CCoin.h"

void CCoin::doEffect(CGameState &gamestate)
{
    if (!collected)
    {
        gamestate.gameMap.coinCount -= 1;
        gamestate.score += 1;
    }
}
void CCoin::draw(SDL_Renderer *renderer, CGameState &gamestate)
{
    if (!collected)
    {
        const double coinScale = 0.2;
        const double offset = (1 - coinScale) / 2;

        SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);

        SDL_Rect coin =
            {static_cast<int>(gamestate.WINDOW_WIDTH / static_cast<double>(gamestate.gameMap.BOARDWIDTH) * pos.x +
                              static_cast<int>(gamestate.WINDOW_WIDTH / static_cast<double>(gamestate.gameMap.BOARDWIDTH) * offset)),
             static_cast<int>(gamestate.WINDOW_HEIGHT / static_cast<double>(gamestate.gameMap.BOARDHEIGHT) * pos.y +
                              static_cast<int>(gamestate.WINDOW_WIDTH / static_cast<double>(gamestate.gameMap.BOARDWIDTH) * offset)),
             static_cast<int>(gamestate.WINDOW_WIDTH / static_cast<double>(gamestate.gameMap.BOARDWIDTH) * coinScale),
             static_cast<int>(gamestate.WINDOW_HEIGHT / static_cast<double>(gamestate.gameMap.BOARDHEIGHT) * coinScale)};
        SDL_RenderFillRect(renderer, &coin);
    }
}
