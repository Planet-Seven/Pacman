#include "CCollectible.h"

void CCollectible::update(CGameState &gamestate)
{
    if (gamestate.playerPos == pos)
    {
        doEffect(gamestate);
        collected = true;
    }
}

void CCoin::doEffect(CGameState &gamestate)
{
    if (!collected)
    {
        gamestate.gameMap.coinCount -= 1;
        gamestate.score += 1;
    }
}
void CCoin::draw(SDL_Renderer *renderer)
{
    if (!collected)
    {
        const double coinScale = 0.2;
        const double offset = (1 - coinScale) / 2;

        SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);

        SDL_Rect coin =
            {static_cast<int>(WINDOW_WIDTH / static_cast<double>(BOARDWIDTH) * pos.x + static_cast<int>(WINDOW_WIDTH / static_cast<double>(BOARDWIDTH) * offset)),
             static_cast<int>(WINDOW_HEIGHT / static_cast<double>(BOARDHEIGHT) * pos.y + static_cast<int>(WINDOW_WIDTH / static_cast<double>(BOARDWIDTH) * offset)),
             static_cast<int>(WINDOW_WIDTH / static_cast<double>(BOARDWIDTH) * coinScale),
             static_cast<int>(WINDOW_HEIGHT / static_cast<double>(BOARDHEIGHT) * coinScale)};
        SDL_RenderFillRect(renderer, &coin);
    }
}

void CPowerUp::doEffect(CGameState &gamestate)
{
}
void CPowerUp::draw(SDL_Renderer *renderer)
{
    if (!collected)
    {
        const double powerUpScale = 0.6;
        const double offset = (1 - powerUpScale) / 2;

        SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);

        SDL_Rect coin =
            {static_cast<int>(WINDOW_WIDTH / static_cast<double>(BOARDWIDTH) * pos.x + static_cast<int>(WINDOW_WIDTH / static_cast<double>(BOARDWIDTH) * offset)),
             static_cast<int>(WINDOW_HEIGHT / static_cast<double>(BOARDHEIGHT) * pos.y + static_cast<int>(WINDOW_WIDTH / static_cast<double>(BOARDWIDTH) * offset)),
             static_cast<int>(WINDOW_WIDTH / static_cast<double>(BOARDWIDTH) * powerUpScale),
             static_cast<int>(WINDOW_HEIGHT / static_cast<double>(BOARDHEIGHT) * powerUpScale)};
        SDL_RenderFillRect(renderer, &coin);
    }
}