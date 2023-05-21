#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Game.h"
#include "Point.h"
#include "core.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

Game game;

bool init();
void handleMousePress();
void draw();
void close();
bool endGameState();

int main(void)
{
    if (!init())
    {
        printf("Failed to initialize!");
        return 1;
    }

    bool lastPressed = false;
    bool quit = false;
    SDL_Event e;

    while (!quit)
    {
        quit = endGameState();
        
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            else if(e.type == SDL_MOUSEBUTTONDOWN && !lastPressed)
            {
                if(e.button.button == SDL_BUTTON_LEFT)
                {
                    lastPressed = true;
                    handleMousePress(&e);
                }
            }
            else if(e.type == SDL_MOUSEBUTTONUP)
            {
                if(e.button.button == SDL_BUTTON_LEFT)
                {
                    lastPressed = false;
                }
            }
        }
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear(gRenderer);

        draw();
        
        SDL_RenderPresent( gRenderer );
    }

    close();

    return 0;
}

bool loadMedia()
{
    initMedia(gRenderer);
    return true;
}

bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize. SDL Error: %s\n", SDL_GetError());
        return false;
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }

    gWindow = SDL_CreateWindow("Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if( gWindow == NULL )
    {
        printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
        return false;
    }
    
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_SOFTWARE);
    if( gRenderer == NULL )
    {
        printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
        return false;
    }

    if (!loadMedia())
    {
        printf("Failed to load media.");
        return false;
    }

    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

    game = newGame();

    return true;
}

void handleMousePress()
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    Point pos = {x, SCREEN_HEIGHT - y};
    gameHandleMousePress(&game, &pos);
}

void draw()
{
    drawGame(&game, gRenderer);
}

bool endGameState()
{
    bool result = false;

    if(hasWon(&game, true)) 
    {
        printf("White won\n");
        result = true;
    }
    else if(hasWon(&game, false))
    {
        printf("Black won\n");
        result = true;
    }
    else if(stalemate(&game))
    {
        printf("Stalemate\n");
        result = true;
    }

    return result;
}

void close()
{
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = NULL;
    gWindow = NULL;
}
