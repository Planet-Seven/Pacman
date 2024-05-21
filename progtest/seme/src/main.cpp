#include <cstdlib>
#include <vector>
#include <memory>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "CGhost.h"
#include "CCollectible.h"

////////////////////////////////////////////////////////////////////////////////
/// initializes a SDL window
void initializeWindow(SDL_Renderer *&renderer, SDL_Window *&window)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        std::cout << "Error initializing SDL." << std::endl;

    if (TTF_Init() != 0)
        std::cout << "Error initializing TTF." << std::endl;

    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT + BOTTOM_PADDING,
        SDL_WINDOW_BORDERLESS);

    renderer = SDL_CreateRenderer(window, -1, 0);
}

////////////////////////////////////////////////////////////////////////////////
/// destroys a SDL window
void destroyWindow(SDL_Renderer *renderer, SDL_Window *window)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void openFont(TTF_Font *&font)
{
    std::string fontPath = "assets/fonts/Pixelmania.ttf";
    font = TTF_OpenFont(fontPath.c_str(), FONT_SIZE);
}

void closeFont(TTF_Font *font)
{
    TTF_CloseFont(font);
    TTF_Quit();
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
                gameObjects.push_back(std::unique_ptr<CGameObject>(new CCoin(CPos(j, i))));
                break;
            case (gamestate.gameMap.CMapObjects::P):
                gameObjects.push_back(std::unique_ptr<CGameObject>(new CPowerUp(CPos(j, i))));
                break;
            case (gamestate.gameMap.CMapObjects::S):
                gamestate.playerPos = CPos(j, i);
                break;
            case (gamestate.gameMap.CMapObjects::m):
                gameObjects.push_back(std::unique_ptr<CGameObject>(new CMax(CPos(j, i))));
                break;
            case (gamestate.gameMap.CMapObjects::t):
                gameObjects.push_back(std::unique_ptr<CGameObject>(new CManhattan(CPos(j, i))));
                break;
            case (gamestate.gameMap.CMapObjects::e):
                gameObjects.push_back(std::unique_ptr<CGameObject>(new CEuclid(CPos(j, i))));
                break;
            }
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
            std::cout << gamestate.gameMap.coinCount << std::endl;
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

    for (auto const &gameObject : gameObjects)
        gameObject->update(gamestate);
    // TODO - update gameobjects
}

////////////////////////////////////////////////////////////////////////////////
/// @param[in] x x coordinate
/// @param[in] y y cooridnate
///
/// Draws a simple wall object.
void drawWall(int x, int y, SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 20, 20, 50, 255);

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
void drawMap(CGameState &gamestate, SDL_Renderer *renderer)
{
    for (int i = 0; i < BOARDHEIGHT; i++)
        for (int j = 0; j < BOARDWIDTH; j++)
            if (gamestate.gameMap.map[i][j] == gamestate.gameMap.CMapObjects::W)
                drawWall(j, i, renderer);
};

////////////////////////////////////////////////////////////////////////////////
/// @param[in] gamestate a gamestate variable
///
/// Draws the player.
void drawPlayer(CGameState &gamestate, SDL_Renderer *renderer)
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

void drawGameObjects(CGameState &gamestate, std::vector<std::unique_ptr<CGameObject>> &gameObjects, SDL_Renderer *renderer)
{
    for (auto const &gameObject : gameObjects)
        gameObject->draw(renderer);
}

void drawText(CGameState &gamestate, SDL_Renderer *renderer, TTF_Font *font, std::string &text, int x, int y)
{
    SDL_Color textColor = {255, 255, 255};
    int textWidth = 0;
    int textHeight = 0;

    SDL_Texture *fontTexture = nullptr;
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    fontTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    if (textSurface != nullptr)
    {
        textWidth = textSurface->w;
        textHeight = textSurface->h;
    }

    SDL_Rect dst = {
        x - textWidth / 2,
        y - textHeight / 2,
        textWidth,
        textHeight};
    SDL_RenderCopy(renderer, fontTexture, NULL, &dst);

    SDL_FreeSurface(textSurface); // cleanup
    SDL_DestroyTexture(fontTexture);
}

void drawGUI(CGameState &gamestate, SDL_Renderer *renderer, TTF_Font *font)
{
    std::string scoreText = "SCORE " + std::to_string(gamestate.score);
    std::string levelText = "LEVEL " + std::to_string(gamestate.level);
    drawText(gamestate,
             renderer,
             font,
             scoreText,
             WINDOW_WIDTH * 0.33,
             WINDOW_HEIGHT + BOTTOM_PADDING / 2);
    drawText(gamestate,
             renderer,
             font,
             levelText,
             WINDOW_WIDTH * 0.66,
             WINDOW_HEIGHT + BOTTOM_PADDING / 2);
}

////////////////////////////////////////////////////////////////////////////////
/// @param[in] gamestate a gamestate variable
/// @param[in] gameObjects a vector of unique pointers to game objects. Polymorphism applied here.
///
/// Used to handle rendering. Runs once every frame.
void draw(CGameState &gamestate, std::vector<std::unique_ptr<CGameObject>> &gameObjects, SDL_Renderer *renderer, TTF_Font *font)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    drawMap(gamestate, renderer);
    drawGameObjects(gamestate, gameObjects, renderer);
    drawPlayer(gamestate, renderer);
    drawGUI(gamestate, renderer, font);

    SDL_RenderPresent(renderer);
}

int main()
{

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    TTF_Font *font = nullptr;

    CGameState gamestate;
    std::vector<std::unique_ptr<CGameObject>> gameObjects;
    initializeWindow(renderer, window);
    openFont(font);
    setup(gamestate, gameObjects);

    bool playing = true;
    int lastFrameTime = 0;
    while (playing)
    {
        processInput(gamestate, playing);
        update(gamestate, gameObjects, lastFrameTime);
        draw(gamestate, gameObjects, renderer, font);
    }

    closeFont(font);
    destroyWindow(renderer, window);

    return 0;
}