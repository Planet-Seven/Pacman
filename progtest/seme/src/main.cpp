#include <cstdlib>
#include <vector>
#include <memory>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <fstream>
#include "CGhost.h"
#include "CCollectible.h"

////////////////////////////////////////////////////////////////////////////////
/// initializes a SDL window
void initializeWindow(CGameState &gamestate, SDL_Renderer *&renderer, SDL_Window *&window)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        std::cout << "Error initializing SDL." << std::endl;

    if (TTF_Init() != 0)
        std::cout << "Error initializing TTF." << std::endl;

    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        gamestate.WINDOW_WIDTH,
        gamestate.WINDOW_HEIGHT + gamestate.BOTTOM_PADDING,
        SDL_WINDOW_BORDERLESS);

    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

////////////////////////////////////////////////////////////////////////////////
/// destroys a SDL window
void destroyWindow(SDL_Renderer *renderer, SDL_Window *window)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void openFont(CGameState &gamestate, TTF_Font *&font)
{
    std::string fontPath = "assets/fonts/Pixelmania.ttf";
    font = TTF_OpenFont(fontPath.c_str(), gamestate.FONT_SIZE);
}

void loadHighScores(CGameState &gamestate)
{
    std::string score;
    std::string level;
    std::ifstream highscores("build/highscores.txt");
    if (highscores.is_open())
    {
        while (getline(highscores, score, ';') && getline(highscores, level))
        {
            gamestate.highscores.push_back({atoi(score.c_str()), atoi(level.c_str())});
        }
        highscores.close();
    }
    else
        std::cout << "An error occured while loading highscores";
}

void saveHighScores(CGameState &gamestate)
{
    std::ofstream highscores("build/highscores.txt");
    if (highscores.is_open())
    {
        highscores.clear();

        for (auto score : gamestate.highscores)
            highscores << score.first << ";" << score.second << std::endl;
    }
    else
        std::cout << "An error occured while saving highscores";
}

void closeFont(TTF_Font *font)
{
    TTF_CloseFont(font);
    TTF_Quit();
}

void loadGameObjects(CGameState &gamestate, std::vector<std::unique_ptr<CGameObject>> &gameObjects, int &coinCount)
{
    for (int i = 0; i < gamestate.gameMap.BOARDHEIGHT; i++)
        for (int j = 0; j < gamestate.gameMap.BOARDWIDTH; j++)

            switch (gamestate.gameMap.map[i][j])
            {
            case (gamestate.gameMap.CMapObjects::C):
                coinCount++;
                gameObjects.insert(gameObjects.begin(), std::unique_ptr<CGameObject>(new CCoin(CPos(j, i)))); // we insert to front because we want the ghosts to be rendered on top
                break;
            case (gamestate.gameMap.CMapObjects::P):
                gameObjects.insert(gameObjects.begin(), std::unique_ptr<CGameObject>(new CPowerUp(CPos(j, i))));
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
/// @param[in] gameObject a vector of unique pointers to game objects. Polymorphism applied here.
///
/// Setup is run once before the game loop starts. Initializes gameObjects.
void setup(CGameState &gamestate, std::vector<std::unique_ptr<CGameObject>> &gameObjects)
{
    std::vector<CPos> teleportPositions;
    int coinCount = 0;
    loadGameObjects(gamestate, gameObjects, coinCount);

    gamestate.gameMap.coinCount = coinCount;
    gamestate.nextMove = CDirection::none;
    gamestate.thisMove = CDirection::none;
    gamestate.nextGuard = gamestate.TIME_BETWEEN_GUARD_MODE;
}

////////////////////////////////////////////////////////////////////////////////
/// @param[in] gamestate a gamestate variable
/// @param[in] playing a boolean that maintains the game cycle.
///
/// Handles user input. Checks for escape and arrow keys.

void processInputPlayingScreen(CGameState &gamestate, std::vector<std::unique_ptr<CGameObject>> &gameObjects, bool &playing, const SDL_Event &event)
{
    if (event.key.keysym.sym == SDLK_UP)
        gamestate.nextMove = CDirection::up;
    else if (event.key.keysym.sym == SDLK_DOWN)
        gamestate.nextMove = CDirection::down;
    else if (event.key.keysym.sym == SDLK_LEFT)
        gamestate.nextMove = CDirection::left;
    else if (event.key.keysym.sym == SDLK_RIGHT)
        gamestate.nextMove = CDirection::right;
}

void processInputGameOverScreen(CGameState &gamestate, std::vector<std::unique_ptr<CGameObject>> &gameObjects, bool &playing, const SDL_Event &event)
{
    if (event.key.keysym.sym == SDLK_SPACE)
    {
        gameObjects.clear();
        gamestate.score = 0;
        gamestate.level = 1;
        setup(gamestate, gameObjects);
        gamestate.screen = CGameState::CScreen::playing;
    }
    else if (event.key.keysym.sym == SDLK_h)
    {
        gamestate.screen = CGameState::CScreen::scoreBoard;
    }
}

void processInputScoreBoardScreen(CGameState &gamestate, std::vector<std::unique_ptr<CGameObject>> &gameObjects, bool &playing, const SDL_Event &event)
{
    if (event.key.keysym.sym == SDLK_h)
        gamestate.screen = CGameState::CScreen::gameOver;
}

void handleKeyDown(CGameState &gamestate, std::vector<std::unique_ptr<CGameObject>> &gameObjects, bool &playing, const SDL_Event &event)
{
    if (gamestate.screen == CGameState::CScreen::start)
        gamestate.screen = CGameState::CScreen::playing;

    if (gamestate.screen == CGameState::CScreen::playing) // not including this in an else block allows the first arrow key press to be registered
        processInputPlayingScreen(gamestate, gameObjects, playing, event);

    else if (gamestate.screen == CGameState::CScreen::gameOver)
        processInputGameOverScreen(gamestate, gameObjects, playing, event);

    else if (gamestate.screen == CGameState::CScreen::scoreBoard)
        processInputScoreBoardScreen(gamestate, gameObjects, playing, event);

    if (event.key.keysym.sym == SDLK_ESCAPE) // break the game cycle
        playing = false;
}

void processInput(CGameState &gamestate, std::vector<std::unique_ptr<CGameObject>> &gameObjects, bool &playing)
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type)
    {
    case (SDL_QUIT):
        playing = false;
        break;

    case (SDL_KEYDOWN):
        handleKeyDown(gamestate, gameObjects, playing, event);
        break;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @param[in] gamestate a gamestate variable
/// @param[in] gameObjects a vector of unique pointers to game objects. Polymorphism applied here.
/// @param[in] lastFrameTime Time ellapsed from previous frame. Used for proper delta-time calculations
///
/// Runs once every frame. Used mainly to check for collisions and update game objects' and player's position.

void increaseLevel(CGameState &gamestate, std::vector<std::unique_ptr<CGameObject>> &gameObjects)
{
    gamestate.level++;

    if (gamestate.guardTime > 1)
        gamestate.guardTime -= gamestate.GUARD_TIME_DECREMENT;

    if (gamestate.powerUpTime > 1)
        gamestate.powerUpTime -= gamestate.POWER_UP_TIME_DECREMENT;

    gameObjects.clear();
    setup(gamestate, gameObjects);
}

void updatePowerUp(CGameState &gamestate, std::vector<std::unique_ptr<CGameObject>> &gameObjects, double deltaTime)
{
    if (gamestate.powerUpRemaining > 0)
    {
        gamestate.powerUpRemaining -= deltaTime;

        if (gamestate.powerUpRemaining <= 0)
            gamestate.gamemode = CGameState::CGameMode::chase;
    }
}

void updateGuard(CGameState &gamestate, std::vector<std::unique_ptr<CGameObject>> &gameObjects, double deltaTime)
{
    if (gamestate.guardTimeRemaining > 0)
    {
        gamestate.guardTimeRemaining -= deltaTime;

        if (gamestate.guardTimeRemaining <= 0)
            gamestate.gamemode = CGameState::CGameMode::chase;
    }

    if (gamestate.nextGuard < 0 && gamestate.gamemode != CGameState::CGameMode::powerup) // ghosts will not enter guard mode while a power up is active
    {
        gamestate.gamemode = CGameState::CGameMode::guard;
        gamestate.guardTimeRemaining = gamestate.guardTime;
        gamestate.nextGuard = gamestate.TIME_BETWEEN_GUARD_MODE;
    }
}

void updateGameModes(CGameState &gamestate, std::vector<std::unique_ptr<CGameObject>> &gameObjects, double deltaTime)
{
    updatePowerUp(gamestate, gameObjects, deltaTime);
    updateGuard(gamestate, gameObjects, deltaTime);
}

void updateGameObjects(CGameState &gamestate, std::vector<std::unique_ptr<CGameObject>> &gameObjects, double deltaTime)
{
    for (auto const &gameObject : gameObjects)
        gameObject->update(gamestate, deltaTime);
}

void updatePlaying(CGameState &gamestate, std::vector<std::unique_ptr<CGameObject>> &gameObjects, double deltaTime)
{
    gamestate.nextGuard -= deltaTime;

    if (gamestate.isThisMoveLegal())
        gamestate.updatePos(deltaTime);

    if (gamestate.isNextMoveLegal())
        gamestate.updateMoves();

    updateGameModes(gamestate, gameObjects, deltaTime);
    updateGameObjects(gamestate, gameObjects, deltaTime);
}

void update(CGameState &gamestate, std::vector<std::unique_ptr<CGameObject>> &gameObjects, int &lastFrameTime)
{
    if (gamestate.gameMap.coinCount == 0)
    {
        increaseLevel(gamestate, gameObjects);
    }

    double deltaTime = (SDL_GetTicks() - lastFrameTime) / 1000.0f;
    lastFrameTime = SDL_GetTicks();

    if (gamestate.screen == CGameState::CScreen::playing)
    {
        updatePlaying(gamestate, gameObjects, deltaTime);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @param[in] x x coordinate
/// @param[in] y y cooridnate
///
/// Draws a simple wall object.
void drawWall(int x, int y, SDL_Renderer *renderer, CGameState &gamestate)
{
    SDL_SetRenderDrawColor(renderer, 20, 20, 50, 255);

    SDL_Rect wall =
        {static_cast<int>(gamestate.WINDOW_WIDTH / static_cast<double>(gamestate.gameMap.BOARDWIDTH) * x),
         static_cast<int>(gamestate.WINDOW_HEIGHT / static_cast<double>(gamestate.gameMap.BOARDHEIGHT) * y),
         static_cast<int>(gamestate.WINDOW_WIDTH / static_cast<double>(gamestate.gameMap.BOARDWIDTH)),
         static_cast<int>(gamestate.WINDOW_HEIGHT / static_cast<double>(gamestate.gameMap.BOARDHEIGHT))};
    SDL_RenderFillRect(renderer, &wall);
}

////////////////////////////////////////////////////////////////////////////////
/// @param[in] gamestate a gamestate variable
///
/// Draws the playing board.
void drawMap(CGameState &gamestate, SDL_Renderer *renderer)
{
    for (int i = 0; i < gamestate.gameMap.BOARDHEIGHT; i++)
        for (int j = 0; j < gamestate.gameMap.BOARDWIDTH; j++)
            if (gamestate.gameMap.map[i][j] == gamestate.gameMap.CMapObjects::W)
                drawWall(j, i, renderer, gamestate);
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
            static_cast<int>(gamestate.WINDOW_WIDTH / static_cast<double>(gamestate.gameMap.BOARDWIDTH) * gamestate.playerPos.x),
            static_cast<int>(gamestate.WINDOW_HEIGHT / static_cast<double>(gamestate.gameMap.BOARDHEIGHT) * gamestate.playerPos.y),
            static_cast<int>(gamestate.WINDOW_WIDTH / (static_cast<double>(gamestate.gameMap.BOARDWIDTH))),
            static_cast<int>(gamestate.WINDOW_HEIGHT / (static_cast<double>(gamestate.gameMap.BOARDHEIGHT)))};
    SDL_RenderFillRect(renderer, &player);
}

void drawGameObjects(CGameState &gamestate, std::vector<std::unique_ptr<CGameObject>> &gameObjects, SDL_Renderer *renderer)
{
    for (auto const &gameObject : gameObjects)
        gameObject->draw(renderer, gamestate);
}

void drawText(SDL_Renderer *renderer, TTF_Font *font, std::string &text, int x, int y)
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

void drawStartOverlay(CGameState &gamestate, SDL_Renderer *renderer, TTF_Font *font)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 120);

    SDL_Rect background = {
        0,
        0,
        gamestate.WINDOW_HEIGHT,
        gamestate.WINDOW_WIDTH + gamestate.BOTTOM_PADDING};

    SDL_RenderFillRect(renderer, &background);

    std::string text = "PRESS ANY KEY TO START.";
    drawText(renderer, font, text, gamestate.WINDOW_WIDTH / 2, gamestate.WINDOW_HEIGHT / 2);
}

void drawGameOverOverlay(CGameState &gamestate, SDL_Renderer *renderer, TTF_Font *font)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 120);

    SDL_Rect background = {
        0,
        0,
        gamestate.WINDOW_HEIGHT,
        gamestate.WINDOW_WIDTH + gamestate.BOTTOM_PADDING};

    SDL_RenderFillRect(renderer, &background);

    std::string text = "GAME OVER.";
    std::string text2 = "SCORE " + std::to_string(gamestate.score);

    std::string text3 = "ESC                                     QUIT";
    std::string text4 = "SPACE          PLAY AGAIN";
    std::string text5 = "H      VIEW HIGH SCORES";

    drawText(renderer,
             font,
             text,
             gamestate.WINDOW_WIDTH / 2,
             gamestate.WINDOW_HEIGHT / 2 - gamestate.WINDOW_HEIGHT * 0.3);
    drawText(renderer,
             font,
             text2,
             gamestate.WINDOW_WIDTH / 2,
             gamestate.WINDOW_HEIGHT / 2 - gamestate.WINDOW_HEIGHT * 0.2);

    drawText(renderer,
             font, text3,
             gamestate.WINDOW_WIDTH / 2,
             gamestate.WINDOW_HEIGHT / 2);
    drawText(renderer,
             font,
             text4,
             gamestate.WINDOW_WIDTH / 2,
             gamestate.WINDOW_HEIGHT / 2 + gamestate.WINDOW_HEIGHT * 0.1);
    drawText(renderer,
             font,
             text5,
             gamestate.WINDOW_WIDTH / 2,
             gamestate.WINDOW_HEIGHT / 2 + gamestate.WINDOW_HEIGHT * 0.2);
}

void drawScores(CGameState &gamestate, SDL_Renderer *renderer, TTF_Font *font)
{
    int position = 1;
    for (auto score : gamestate.highscores)
    {
        if (position > 3)
            break;

        std::string scoreText = std::to_string(position) + ". SCORE " + std::to_string(score.first) + "    LEVEL " + std::to_string(score.second);
        position++;
        drawText(renderer,
                 font,
                 scoreText,
                 gamestate.WINDOW_WIDTH / 2,
                 gamestate.WINDOW_HEIGHT / 2 - gamestate.WINDOW_HEIGHT * 0.3 + gamestate.WINDOW_HEIGHT * (position * 0.1));
    }
}

void drawScoreBoardOverlay(CGameState &gamestate, SDL_Renderer *renderer, TTF_Font *font)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 120);

    SDL_Rect background = {
        0,
        0,
        gamestate.WINDOW_HEIGHT,
        gamestate.WINDOW_WIDTH + gamestate.BOTTOM_PADDING};

    SDL_RenderFillRect(renderer, &background);

    drawScores(gamestate, renderer, font);

    std::string text = "HIGH SCORES";
    std::string text2 = "ESC                                     QUIT";
    std::string text3 = "H                                          BACK";

    drawText(renderer,
             font,
             text,
             gamestate.WINDOW_WIDTH / 2,
             gamestate.WINDOW_HEIGHT / 2 - gamestate.WINDOW_HEIGHT * 0.3);
    drawText(renderer,
             font,
             text2,
             gamestate.WINDOW_WIDTH / 2,
             gamestate.WINDOW_HEIGHT / 2 + gamestate.WINDOW_HEIGHT * 0.3);
    drawText(renderer,
             font,
             text3,
             gamestate.WINDOW_WIDTH / 2,
             gamestate.WINDOW_HEIGHT / 2 + gamestate.WINDOW_HEIGHT * 0.4);
}

void drawGUI(CGameState &gamestate, SDL_Renderer *renderer, TTF_Font *font)
{
    std::string scoreText = "SCORE " + std::to_string(gamestate.score);
    std::string levelText = "LEVEL " + std::to_string(gamestate.level);
    drawText(renderer,
             font,
             scoreText,
             gamestate.WINDOW_WIDTH * 0.33,
             gamestate.WINDOW_HEIGHT + gamestate.BOTTOM_PADDING / 2);
    drawText(renderer,
             font,
             levelText,
             gamestate.WINDOW_WIDTH * 0.66,
             gamestate.WINDOW_HEIGHT + gamestate.BOTTOM_PADDING / 2);

    if (gamestate.screen == CGameState::CScreen::start)
        drawStartOverlay(gamestate, renderer, font);

    else if (gamestate.screen == CGameState::CScreen::gameOver)
        drawGameOverOverlay(gamestate, renderer, font);

    else if (gamestate.screen == CGameState::CScreen::scoreBoard)
        drawScoreBoardOverlay(gamestate, renderer, font);
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
    initializeWindow(gamestate, renderer, window);
    openFont(gamestate, font);
    loadHighScores(gamestate);
    setup(gamestate, gameObjects);

    bool playing = true;
    int lastFrameTime = 0;
    while (playing)
    {
        processInput(gamestate, gameObjects, playing);
        update(gamestate, gameObjects, lastFrameTime);
        draw(gamestate, gameObjects, renderer, font);
    }

    saveHighScores(gamestate);
    closeFont(font);
    destroyWindow(renderer, window);

    return 0;
}