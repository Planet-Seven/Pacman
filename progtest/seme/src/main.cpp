#include <cstdlib>
#include <vector>
#include <memory>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <fstream>

#include "CMax.h"
#include "CManhattan.h"
#include "CEuclid.h"
#include "CCoin.h"
#include "CPowerUp.h"

/*! \mainpage About the project
 *
 * \section intro_sec Introduction
 *
 * This project was created as part of the Programming and Algorithmics 2 course, summer semester 2024 (B232) @ CTU, Prague. \n
 * \n
 * It is a simple PAC-MAN recreation. True to the original game, the individual ghosts all have different pathfinding algorithms. \n
 * They pick the player's position as a target (contrary to the original game, where this differed for individual ghosts) and use \n
 * different vector norms to calculate the optimal path. \n
 * \n
 * Three gamemodes are present. Chase, that is the default, when the player is being chased by the ghosts. Power-up, which occurs when \n
 * the player eats one of the power pellets and can then eat the ghosts, who turn blue. And finally, guard, when the ghosts stop following \n
 * the player and instead wander around a specific corridor. The duration of the latter two decreases with the player's progression through levels. \n
 *
 * \section controls_sec Controls
 *
 * While playing, the game is controlled with the arrow keys. \n
 * When a game is over, the user is prompted to play again (space), or enter the leaderboards (h). \n
 * In the leaderboard screen, the user can go back by pressing (h) again. \n
 * \n
 * To exit, press the (esc) key.
 *
 * \section conf_sec Config files
 *
 * The game is customizable via config files, that allow to change the map layout, effect duration and more. If no config file is loaded, default values are used. \n
 * \n
 * Here are the things you can modify: \n
 * \n
 * PLAYER_SPEED: Tiles/second. Same speed is used for ghost in chase mode. \n
 * POWER_UP_GHOST_SLOWDOWN: Multiplier for ghost speed while a power-up is active \n
 * INITIAL_POWERUP_TIME: 5; Seconds \n
 * POWER_UP_TIME_DECREMENT: Seconds, decrement on level increase \n
 * TIME_BETWEEN_GUARD_MODE: Seconds \n
 * INITIAL_GUARD_TIME: Seconds \n
 * GUARD_TIME_DECREMENT: Seconds, decrement on level increase \n
 * WINDOW_SCALE Window scale * board width is the window width in pixels. Same goes for height. \n
 * BOTTOM_PADDING In pixels \n
 * FONT_SIZE \n
 * BOARD_WIDTH Needed to correctly load the game board \n
 * BOARD_HEIGHT Needed to correctly load the game board \n
 * BOARD Game board \n
 *
 * \section sources_sec Sources
 *
 * [1] Creating a game loop with C & SDL2 (Tutorial). Available here: https://www.youtube.com/watch?v=XfZ6WrV5Z7Y&ab_channel=pikuma \n
 * [2] Stack overlflow, "How to render text in SDL2?". Available here: https://stackoverflow.com/questions/22886500/how-to-render-text-in-sdl2 \n
 * [3] CPP reference. Available here: https://en.cppreference.com/w/ \n
 * [4] Doxygen documentation. Available here: https://www.doxygen.nl/manual/index.html \n
 * [5] Introduction to Doxygen. Available here: https://www.youtube.com/watch?v=5G1zUpNFmEY&ab_channel=John%27sBasement \n
 */

////////////////////////////////////////////////////////////////////////////////
/// Initializes a SDL window.
///
/// @param [in] renderer the renderer that will be initialized
/// @param [in] window the window that will be initialized
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
/// Destroys a SDL window.
///
/// @param [in] renderer the renderer that will be destroyed
/// @param [in] window the window that will be destroyed
void destroyWindow(SDL_Renderer *renderer, SDL_Window *window)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

////////////////////////////////////////////////////////////////////////////////
/// Opens a true type font.
/// @param [in] gamestate A gamestate variable, used to extract the font size
/// @param [out] font A font pointer that the function initializes
void openFont(CGameState &gamestate, TTF_Font *&font)
{
    std::string fontPath = "assets/fonts/Pixelmania.ttf";
    font = TTF_OpenFont(fontPath.c_str(), gamestate.FONT_SIZE);
}

////////////////////////////////////////////////////////////////////////////////
/// Closes a true type font.
///
/// @param [out] font A font pointer that the function closes
void closeFont(TTF_Font *font)
{
    TTF_CloseFont(font);
    TTF_Quit();
}

////////////////////////////////////////////////////////////////////////////////
/// Loads high scores from a external file to a gamestate instance.
///
/// @param [out] gamestate a gamestate instance to be loaded to
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

////////////////////////////////////////////////////////////////////////////////
/// Saves the high scores that are held in the gamestate to an external file.
///
/// @param [in] gamestate a gamestate instance holding the high scores
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

////////////////////////////////////////////////////////////////////////////////
/// Loads game objects from the game board stored in the gamestate instance
///
/// @note This function also initializes the player position and coin count in the gamestate instance
///
/// @param [in] gamestate A gamestate instance that holds the game board
/// @param [out] gameObjects a vector of game objects that will be filled
void loadGameObjects(CGameState &gamestate, std::vector<std::unique_ptr<CGameObject>> &gameObjects)
{
    int coinCount = 0;

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

    gamestate.gameMap.coinCount = coinCount;
}

////////////////////////////////////////////////////////////////////////////////
/// Initializes game objects and other gamestate variables. Called once before the game loop starts and then on every level increase.
///
/// @param [out] gamestate a gamestate instance
/// @param [out] gameObject a vector of unique pointers to game objects. Polymorphism applied here.
void setup(CGameState &gamestate, std::vector<std::unique_ptr<CGameObject>> &gameObjects)
{
    std::vector<CPos> teleportPositions;
    loadGameObjects(gamestate, gameObjects);

    gamestate.nextMove = CDirection::none;
    gamestate.thisMove = CDirection::none;
    gamestate.nextGuard = gamestate.TIME_BETWEEN_GUARD_MODE;
    gamestate.guardTimeRemaining = 0;
    gamestate.gamemode = CGameState::CGameMode::chase;
}

////////////////////////////////////////////////////////////////////////////////
/// Handles user input while the game is being played. Checks for arrow keys input.
///
/// @param [out] gamestate a gamestate instance
/// @param [in] event the SDL_Event that is being processed in the wrapper function
void processInputPlayingScreen(CGameState &gamestate, const SDL_Event &event)
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

////////////////////////////////////////////////////////////////////////////////
/// Handles user input while in the game over screen. Checks if the player wants to play (space), or view the scoreboard (h).
///
/// @param [out] gamestate a gamestate instance
/// @param [in] event the SDL_Event that is being processed in the wrapper function
void processInputGameOverScreen(CGameState &gamestate, std::vector<std::unique_ptr<CGameObject>> &gameObjects, const SDL_Event &event)
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

////////////////////////////////////////////////////////////////////////////////
/// Handles user input while in the score board screen. Checks if the player wants to go back (h).
///
/// @param [out] gamestate a gamestate instance
/// @param [in] event the SDL_Event that is being processed in the wrapper function
void processInputScoreBoardScreen(CGameState &gamestate, const SDL_Event &event)
{
    if (event.key.keysym.sym == SDLK_h)
        gamestate.screen = CGameState::CScreen::gameOver;
}

////////////////////////////////////////////////////////////////////////////////
/// A wrapper function that processes key input and calls the corresponding screen handler.
///
/// @param [out] gamestate a gamestate instance
/// @param [out] gameObjects a gameObjects vector, cleared/loaded if the game is restarted/started
/// @param [out] playing a boolean that keeps the game loop running
/// @param [in] event the SDL_Event that is being processed in the wrapper function
void handleKeyDown(CGameState &gamestate, std::vector<std::unique_ptr<CGameObject>> &gameObjects, bool &playing, const SDL_Event &event)
{
    if (gamestate.screen == CGameState::CScreen::start)
        gamestate.screen = CGameState::CScreen::playing;

    if (gamestate.screen == CGameState::CScreen::playing) // not including this in an else block allows the first arrow key press to be registered
        processInputPlayingScreen(gamestate, event);

    else if (gamestate.screen == CGameState::CScreen::gameOver)
        processInputGameOverScreen(gamestate, gameObjects, event);

    else if (gamestate.screen == CGameState::CScreen::scoreBoard)
        processInputScoreBoardScreen(gamestate, event);

    if (event.key.keysym.sym == SDLK_ESCAPE) // break the game cycle
        playing = false;
}

////////////////////////////////////////////////////////////////////////////////
/// A wrapper function that checks for a quit event or a key down event and calls the key down event handler (handleKeyDown)
///
/// @param [out] gamestate a gamestate instance
/// @param [out] gameObjects a gameObjects vector, cleared/loaded if the game is restarted/started
/// @param [in] playing a boolean that keeps the game loop running
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
/// Called when the player reaches the next level. It increases difficulty by decrementing power up and guard times in the gamestate and re-loads game objects.
///
/// @param [out] gamestate a gamestate instance
/// @param [out] gameObjects a gameObjects vector, cleared/loaded if the game is restarted/started
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

////////////////////////////////////////////////////////////////////////////////
/// Called every frame to check if the ghosts should enter or exit power up mode
///
/// @param [out] gamestate a gamestate instance
/// @param [in] deltaTime time change since last frame, used for time keeping
void updatePowerUp(CGameState &gamestate, double deltaTime)
{
    if (gamestate.powerUpRemaining > 0)
    {
        gamestate.powerUpRemaining -= deltaTime;

        if (gamestate.powerUpRemaining <= 0)
            gamestate.gamemode = CGameState::CGameMode::chase;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// Called every frame to check if the ghosts should enter or exit guard mode
///
/// @param [out] gamestate a gamestate instance
/// @param [in] deltaTime time change since last frame, used for time keeping
void updateGuard(CGameState &gamestate, double deltaTime)
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

////////////////////////////////////////////////////////////////////////////////
/// A wrapper function that calls the individual functions taking care of gamemodes (updateGuard and updatePowerUp)
///
/// @param [out] gamestate a gamestate instance
/// @param [in] deltaTime time change since last frame, used for time keeping
void updateGameModes(CGameState &gamestate, double deltaTime)
{
    updatePowerUp(gamestate, deltaTime);
    updateGuard(gamestate, deltaTime);
}

////////////////////////////////////////////////////////////////////////////////
/// A wrapper function that polymorphically calls update on every game object.
///
/// @param [out] gamestate a gamestate instance
/// @param [in] deltaTime time change since last frame, used for time keeping
/// @param [out] gameObjects a gameObjects vector
void updateGameObjects(CGameState &gamestate, std::vector<std::unique_ptr<CGameObject>> &gameObjects, double deltaTime)
{
    for (auto const &gameObject : gameObjects)
        gameObject->update(gamestate, deltaTime);
}

////////////////////////////////////////////////////////////////////////////////
/// A wrapper function that updates the player position and calls other update functions. Called if the active window is playing.
///
/// @param [out] gamestate a gamestate instance
/// @param [in] deltaTime time change since last frame, used for time keeping
/// @param [out] gameObjects a gameObjects vector
void updatePlaying(CGameState &gamestate, std::vector<std::unique_ptr<CGameObject>> &gameObjects, double deltaTime)
{
    gamestate.nextGuard -= deltaTime;

    if (gamestate.isThisMoveLegal())
        gamestate.updatePos(deltaTime);

    if (gamestate.isNextMoveLegal())
        gamestate.updateMoves();

    updateGameModes(gamestate, deltaTime);
    updateGameObjects(gamestate, gameObjects, deltaTime);
}

////////////////////////////////////////////////////////////////////////////////
/// Runs once every frame. Calls increaseLevel if necessary and updatePlaying if the active screen is "playing".
///
/// @note Also handles delta time updates.
///
/// @param[in] gamestate a gamestate variable
/// @param[in] gameObjects a vector of unique pointers to game objects. Polymorphism applied here.
/// @param[in] lastFrameTime Time ellapsed from previous frame. Used for proper delta-time calculations
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
/// Draws a simple wall object.
///
/// @param [in] x x coordinate
/// @param [in] y y cooridnate
/// @param [in] renderer pointer to the SDL_Renderer
/// @param [in] gamestate a gamestate variable. Used to read the window dimensions.
void drawWall(int x, int y, SDL_Renderer *renderer, const CGameState &gamestate)
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
/// Draws the playing board.
///
/// @param [in] gamestate a gamestate variable
/// @param [in] renderer  pointer to the SDL_Renderer
void drawMap(const CGameState &gamestate, SDL_Renderer *renderer)
{
    for (int i = 0; i < gamestate.gameMap.BOARDHEIGHT; i++)
        for (int j = 0; j < gamestate.gameMap.BOARDWIDTH; j++)
            if (gamestate.gameMap.map[i][j] == gamestate.gameMap.CMapObjects::W)
                drawWall(j, i, renderer, gamestate);
};

////////////////////////////////////////////////////////////////////////////////
/// Draws the player.
///
/// @param[in] gamestate a gamestate variable
/// @param [in] renderer  pointer to the SDL_Renderer
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

////////////////////////////////////////////////////////////////////////////////
/// Draws the game objects. Polymorphism applied here.
///
/// @param[in] gamestate a gamestate variable
/// @param [in] renderer  pointer to the SDL_Renderer
/// @param[in] gameObjects a vector of unique pointers to game objects. Polymorphism applied here.
void drawGameObjects(CGameState &gamestate, std::vector<std::unique_ptr<CGameObject>> &gameObjects, SDL_Renderer *renderer)
{
    for (auto const &gameObject : gameObjects)
        gameObject->draw(renderer, gamestate);
}

////////////////////////////////////////////////////////////////////////////////
/// Draws a text on the given coordinates.
///
/// @note The text is centered around these coordinates.
///
/// @param[in] renderer pointer to the SDL_Renderer
/// @param [in] font  pointer to a TTF_Font
/// @param[in] text a string holding the text
/// @param[in] x The x coordinate
/// @param[in] y The y coordinate
void drawText(SDL_Renderer *renderer, TTF_Font *font, std::string &text, int x, int y)
{
    SDL_Color textColor = {255, 255, 255};
    int textWidth = 0;
    int textHeight = 0;

    SDL_Texture *fontTexture = nullptr;
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    // Drawing a texture instead of a surface is arguably faster, as a texture is stored in the VRAM.
    fontTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    if (textSurface != nullptr)
    {
        textWidth = textSurface->w;
        textHeight = textSurface->h;

        SDL_Rect dst = {
            x - textWidth / 2,
            y - textHeight / 2,
            textWidth,
            textHeight};
        SDL_RenderCopy(renderer, fontTexture, NULL, &dst);
    }

    SDL_FreeSurface(textSurface); // cleanup
    SDL_DestroyTexture(fontTexture);
}

////////////////////////////////////////////////////////////////////////////////
/// Draws the overlay for when the game is first started.
///
/// @param[in] gamestate a gamestate instance. Used to get the window dimensions.
/// @param [in] renderer  pointer to the SDL_Renderer
/// @param[in] font pointer to a TTF_Font
void drawStartOverlay(const CGameState &gamestate, SDL_Renderer *renderer, TTF_Font *font)
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

////////////////////////////////////////////////////////////////////////////////
/// Draws the overlay for when the game is over.
///
/// @param[in] gamestate a gamestate instance. Used to get the window dimensions.
/// @param [in] renderer  pointer to the SDL_Renderer
/// @param [in] font  pointer to a TTF_Font
void drawGameOverOverlay(CGameState &gamestate, SDL_Renderer *renderer, TTF_Font *font)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 120);

    SDL_Rect background = {
        0,
        0,
        gamestate.WINDOW_HEIGHT,
        gamestate.WINDOW_WIDTH + gamestate.BOTTOM_PADDING};

    SDL_RenderFillRect(renderer, &background);

    std::string text1 = "GAME OVER.";
    std::string text2 = "SCORE " + std::to_string(gamestate.score);

    std::string text3 = "ESC                                     QUIT";
    std::string text4 = "SPACE          PLAY AGAIN";
    std::string text5 = "H      VIEW HIGH SCORES";

    drawText(renderer,
             font,
             text1,
             gamestate.WINDOW_WIDTH / 2,
             // multiplying the dimensions like this allows us to present responsivness when the dimensions change.
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

////////////////////////////////////////////////////////////////////////////////
/// Draws the scores from highest to lowest.
///
/// @param[in] gamestate a gamestate instance. Used to get the window dimensions and the scores.
/// @param [in] renderer  pointer to the SDL_Renderer
/// @param [in] font  pointer to a TTF_Font
void drawScores(CGameState &gamestate, SDL_Renderer *renderer, TTF_Font *font)
{
    int position = 1;
    for (auto score : gamestate.highscores)
    {
        if (position > 3) // we only want to view the top 3 scores.
            break;

        std::string scoreText = std::to_string(position) + ". SCORE " + std::to_string(score.first) + "    LEVEL " + std::to_string(score.second);
        position++;
        drawText(renderer,
                 font,
                 scoreText,
                 gamestate.WINDOW_WIDTH / 2,
                 // again, this works responsively
                 gamestate.WINDOW_HEIGHT / 2 - gamestate.WINDOW_HEIGHT * 0.3 + gamestate.WINDOW_HEIGHT * (position * 0.1));
    }
}

////////////////////////////////////////////////////////////////////////////////
/// Draws the overlay that shows when the score board is open.
///
/// @param[in] gamestate a gamestate instance. Used to get the window dimensions and the scores.
/// @param [in] renderer  pointer to the SDL_Renderer
/// @param [in] font  pointer to a TTF_Font
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
             // responsive
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

////////////////////////////////////////////////////////////////////////////////
/// Handles drawing of all UI overlays.
///
/// @param[in] gamestate a gamestate instance. Used to get the window dimensions and the scores.
/// @param [in] renderer  pointer to the SDL_Renderer
/// @param [in] font  pointer to a TTF_Font
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
/// Used to handle rendering. Runs once every frame.
///
/// @param[in] gamestate a gamestate variable
/// @param[in] gameObjects a vector of unique pointers to game objects. Polymorphism applied here.
/// @param [in] font  pointer to a TTF_Font
/// @param [in] renderer  pointer to the SDL_Renderer
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

////////////////////////////////////////////////////////////////////////////////
/// Contains the game loop at the highest level as well as some initialization and cleanup.
///
int main()
{

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    TTF_Font *font = nullptr;

    CGameState gamestate;
    gamestate.loadConfig();
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