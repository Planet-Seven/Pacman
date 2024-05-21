#include <cstdlib>
#include <vector>
#include <memory>
#include <iostream>
#include <SDL2/SDL.h>
#include "CGhost.h"
#include "CCollectible.h"

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

////////////////////////////////////////////////////////////////////////////////
/// initializes a SDL window
void initializeWindow()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        std::cout << "Error initializing SDL." << std::endl;

    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_BORDERLESS);

    renderer = SDL_CreateRenderer(window, -1, 0);
}

////////////////////////////////////////////////////////////////////////////////
/// destroys a SDL window
void destroyWindow()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

////////////////////////////////////////////////////////////////////////////////
/// @param[in] gamestate a gamestate variable
/// @param[in] gameObject a vector of unique pointers to game objects. Polymorphism applied here.
///
/// Setup is run once before the game loop starts. Initializes gameObjects.
void setup(CGameState &gamestate, std::vector<std::unique_ptr<CGameObject>> &gameObjects)
{
    std::vector<CPos> teleportPositions;

    for (int i = 0; i < BOARDHEIGHT; i++)
        for (int j = 0; j < BOARDWIDTH; j++)

            switch (gamestate.gameMap.map[i][j])
            {
            case (gamestate.gameMap.CMapObjects::C):
                gameObjects.push_back(std::unique_ptr<CGameObject>(new CCoin(CPos(i, j))));
                break;
            case (gamestate.gameMap.CMapObjects::P):
                gameObjects.push_back(std::unique_ptr<CGameObject>(new CPowerUp(CPos(i, j))));
                break;
            case (gamestate.gameMap.CMapObjects::T):
                teleportPositions.push_back(CPos(i, j));
                break;
            case (gamestate.gameMap.CMapObjects::S):
                gamestate.playerPos = CPos(j, i);
                break;
            case (gamestate.gameMap.CMapObjects::m):
                gameObjects.push_back(std::unique_ptr<CGameObject>(new CMax(CPos(i, j))));
                break;
            case (gamestate.gameMap.CMapObjects::t):
                gameObjects.push_back(std::unique_ptr<CGameObject>(new CManhattan(CPos(i, j))));
                break;
            case (gamestate.gameMap.CMapObjects::e):
                gameObjects.push_back(std::unique_ptr<CGameObject>(new CEuclid(CPos(i, j))));
                break;
            }

    gameObjects.push_back(std::unique_ptr<CGameObject>(new CPortal(teleportPositions[0], teleportPositions[1])));
    gameObjects.push_back(std::unique_ptr<CGameObject>(new CPortal(teleportPositions[1], teleportPositions[0])));
}

////////////////////////////////////////////////////////////////////////////////
/// @param[in] gamestate a gamestate variable
/// @param[in] playing a boolean that maintains the game cycle.
///
/// Handles user input. Checks for escape and arrow keys.
void processInput(CGameState &gamestate, bool &playing)
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type)
    {
    case (SDL_QUIT):
        playing = false;
        break;

    case (SDL_KEYDOWN):
        if (event.key.keysym.sym == SDLK_ESCAPE)
            playing = false;
        if (event.key.keysym.sym == SDLK_UP)
            gamestate.nextMove = CGameState::CDirection::up;
        if (event.key.keysym.sym == SDLK_DOWN)
            gamestate.nextMove = CGameState::CDirection::down;
        if (event.key.keysym.sym == SDLK_LEFT)
            gamestate.nextMove = CGameState::CDirection::left;
        if (event.key.keysym.sym == SDLK_RIGHT)
            gamestate.nextMove = CGameState::CDirection::right;
        if (event.key.keysym.sym == SDLK_p)
            std::cout << "x: " << gamestate.playerPos.x << " y: " << gamestate.playerPos.y << std::endl;
        break;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @param[in] gamestate a gamestate variable
/// @param[in] gameObjects a vector of unique pointers to game objects. Polymorphism applied here.
/// @param[in] lastFrameTime Time ellapsed from previous frame. Used for proper delta-time calculations
///
/// Runs once every frame. Used mainly to check for collisions and update game objects' and player's position.
void update(CGameState &gamestate, std::vector<std::unique_ptr<CGameObject>> &gameObjects, int &lastFrameTime)
{
    double deltaTime = (SDL_GetTicks() - lastFrameTime) / 1000.0f;
    lastFrameTime = SDL_GetTicks();

    if (gamestate.isThisMoveLegal())
        gamestate.updatePos(deltaTime);

    if (gamestate.isNextMoveLegal())
        gamestate.updateMoves();

    // TODO - update gameobjects
}

////////////////////////////////////////////////////////////////////////////////
/// @param[in] x x coordinate
/// @param[in] y y cooridnate
///
/// Draws a simple wall object.
void drawWall(int x, int y)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_Rect wall =
        {static_cast<int>(WINDOW_WIDTH / static_cast<double>(BOARDWIDTH) * x),
         static_cast<int>(WINDOW_HEIGHT / static_cast<double>(BOARDHEIGHT) * y),
         static_cast<int>(WINDOW_WIDTH / static_cast<double>(BOARDWIDTH)),
         static_cast<int>(WINDOW_HEIGHT / static_cast<double>(BOARDHEIGHT))};
    SDL_RenderFillRect(renderer, &wall);
}

////////////////////////////////////////////////////////////////////////////////
/// @param[in] gamestate a gamestate variable
///
/// Draws the playing board.
void drawMap(CGameState &gamestate)
{
    for (int i = 0; i < BOARDHEIGHT; i++)
        for (int j = 0; j < BOARDWIDTH; j++)
            if (gamestate.gameMap.map[i][j] == gamestate.gameMap.CMapObjects::W)
                drawWall(j, i);
};

////////////////////////////////////////////////////////////////////////////////
/// @param[in] gamestate a gamestate variable
///
/// Draws the player.
void drawPlayer(CGameState &gamestate)
{
    SDL_SetRenderDrawColor(renderer, 180, 180, 0, 255);

    SDL_Rect player =
        {
            static_cast<int>(WINDOW_WIDTH / static_cast<double>(BOARDWIDTH) * gamestate.playerPos.x),
            static_cast<int>(WINDOW_HEIGHT / static_cast<double>(BOARDHEIGHT) * gamestate.playerPos.y),
            static_cast<int>(WINDOW_WIDTH / (static_cast<double>(BOARDWIDTH))),
            static_cast<int>(WINDOW_HEIGHT / (static_cast<double>(BOARDHEIGHT)))};
    SDL_RenderFillRect(renderer, &player);
}

////////////////////////////////////////////////////////////////////////////////
/// @param[in] gamestate a gamestate variable
/// @param[in] gameObjects a vector of unique pointers to game objects. Polymorphism applied here.
///
/// Used to handle rendering. Runs once every frame.
void draw(CGameState &gamestate, std::vector<std::unique_ptr<CGameObject>> &gameObjects)
{
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderClear(renderer);

    drawMap(gamestate);
    drawPlayer(gamestate);
    // TODO - draw gameobjects

    SDL_RenderPresent(renderer);
}

int main()
{
    CGameState gamestate;
    std::vector<std::unique_ptr<CGameObject>> gameObjects;
    initializeWindow();
    setup(gamestate, gameObjects);

    bool playing = true;
    int lastFrameTime = 0;
    while (playing)
    {
        processInput(gamestate, playing);
        update(gamestate, gameObjects, lastFrameTime);
        draw(gamestate, gameObjects);
    }

    destroyWindow();

    return 0;
}