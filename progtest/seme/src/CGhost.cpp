#include "CGhost.h"
#include <vector>
#include <iostream>

void CGhost::updatePos(CGameState &gamestate, double deltaTime, double speed)
{
    if (direction == CDirection::up)
        currentPos.y -= speed * deltaTime;
    else if (direction == CDirection::down)
        currentPos.y += speed * deltaTime;
    else if (direction == CDirection::left)
        currentPos.x -= speed * deltaTime;
    else if (direction == CDirection::right)
        currentPos.x += speed * deltaTime;

    if (currentPos.x < -1)
        currentPos.x += BOARDWIDTH;
    else if (currentPos.y < -1)
        currentPos.y += BOARDHEIGHT;
    else if (currentPos.x > BOARDWIDTH)
        currentPos.x -= BOARDWIDTH;
    else if (currentPos.y > BOARDHEIGHT)
        currentPos.y -= BOARDHEIGHT;
}

void CGhost::setTargetPos(CGameState &gamestate)
{
    if (gamestate.gamemode == CGameState::CGameMode::guard)
        targetPos = getGuardPos();
    else
        targetPos = gamestate.playerPos;
}

void CGhost::handlePlayerCollision(CGameState &gamestate)
{
    if (gamestate.gamemode == CGameState::CGameMode::powerup)
    {
        gamestate.score += 400;
        currentPos = startPos;
    }
    else
    {
        gamestate.saveScore();
        gamestate.screen = CGameState::CScreen::gameOver;
    }
}

void CGhost::update(CGameState &gamestate, double deltaTime)
{
    double speed = PLAYER_SPEED;
    if (gamestate.gamemode == CGameState::CGameMode::powerup)
        speed *= 0.75;

    setTargetPos(gamestate);

    if (currentPos == gamestate.playerPos)
        handlePlayerCollision(gamestate);

    getNextPos(gamestate);
    updatePos(gamestate, deltaTime, speed);
}

void CGhost::findPossibleMoves(CGameState &gamestate, std::vector<std::pair<CPos, CDirection>> &possibleMoves)
{
    std::pair<int, int> intPos = currentPos.getIntPos();

    if (gamestate.isAMoveLegal(CDirection::up, currentPos) && direction != CDirection::down)
        possibleMoves.push_back({CPos(intPos.first, intPos.second - 1), CDirection::up});

    if (gamestate.isAMoveLegal(CDirection::down, currentPos) && direction != CDirection::up)
        possibleMoves.push_back({CPos(intPos.first, intPos.second + 1), CDirection::down});

    if (gamestate.isAMoveLegal(CDirection::left, currentPos) && direction != CDirection::right)
        possibleMoves.push_back({CPos(intPos.first - 1, intPos.second), CDirection::left});

    if (gamestate.isAMoveLegal(CDirection::right, currentPos) && direction != CDirection::left)
        possibleMoves.push_back({CPos(intPos.first + 1, intPos.second), CDirection::right});
}

void CGhost::pickBestMove(CGameState &gamestate, std::vector<std::pair<CPos, CDirection>> &possibleMoves)
{
    if (!possibleMoves.empty())
    {
        direction = possibleMoves[0].second;
        nextPos = possibleMoves[0].first;
    }
    else
        direction = CDirection::none;

    for (auto move : possibleMoves)
    {
        // while chasing, we want to pick the position closest to the player
        if (gamestate.gamemode == CGameState::CGameMode::chase &&
            getNorm(nextPos - gamestate.playerPos) > getNorm(move.first - gamestate.playerPos))
        {
            direction = move.second;
            nextPos = move.first;
        }
        // during a power up, we pick the opposite
        else if (gamestate.gamemode == CGameState::CGameMode::powerup &&
                 getNorm(nextPos - gamestate.playerPos) < getNorm(move.first - gamestate.playerPos))
        {
            direction = move.second;
            nextPos = move.first;
        }
    }
}
void CGhost::getNextPos(CGameState &gamestate)
{
    std::vector<std::pair<CPos, CDirection>> possibleMoves;

    findPossibleMoves(gamestate, possibleMoves);
    pickBestMove(gamestate, possibleMoves);
}

void CGhost::drawGhost(SDL_Renderer *renderer, CGameState &gamestate, int R, int G, int B)
{
    if (gamestate.gamemode != CGameState::CGameMode::powerup)
        SDL_SetRenderDrawColor(renderer, R, G, B, 255);
    else
        SDL_SetRenderDrawColor(renderer, 0, 0, 180, 255);

    SDL_Rect manhattan =
        {
            static_cast<int>(WINDOW_WIDTH / static_cast<double>(BOARDWIDTH) * currentPos.x),
            static_cast<int>(WINDOW_HEIGHT / static_cast<double>(BOARDHEIGHT) * currentPos.y),
            static_cast<int>(WINDOW_WIDTH / (static_cast<double>(BOARDWIDTH))),
            static_cast<int>(WINDOW_HEIGHT / (static_cast<double>(BOARDHEIGHT)))};
    SDL_RenderFillRect(renderer, &manhattan);
}

double CManhattan::getNorm(CPos position)
{
    return abs(position.x) + abs(position.y);
}

void CManhattan::draw(SDL_Renderer *renderer, CGameState &gamestate)
{
    drawGhost(renderer, gamestate, 60, 150, 10);
}

CPos CManhattan::getGuardPos()
{
    return CPos(0, BOARDHEIGHT);
}

double CEuclid::getNorm(CPos position)
{
    return hypot(position.x, position.y);
}

void CEuclid::draw(SDL_Renderer *renderer, CGameState &gamestate)
{
    drawGhost(renderer, gamestate, 150, 0, 60);
}

CPos CEuclid::getGuardPos()
{
    return CPos(0, 0);
}

double CMax::getNorm(CPos position)
{
    return abs(position.x) < abs(position.y) ? abs(position.y) : abs(position.x);
}

void CMax::draw(SDL_Renderer *renderer, CGameState &gamestate)
{
    drawGhost(renderer, gamestate, 150, 60, 10);
}

CPos CMax::getGuardPos()
{
    return CPos(BOARDHEIGHT, BOARDWIDTH);
}
