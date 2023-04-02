#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <Board.h>

#ifndef bool
#define bool int
#define true 1
#define false 0
#endif

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

Board board;

bool init();
void update(float dt);
void draw();
void close();

int main(void)
{
    if (!init())
    {
        printf("Failed to initialize!");
        return 1;
    }

    bool quit = false;
    SDL_Event e;
    uint32_t lastUpdateTime = SDL_GetTicks();

    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear(gRenderer);

        float deltaTime = (SDL_GetTicks() - lastUpdateTime) / 1000.f;
        update(deltaTime);
        lastUpdateTime = SDL_GetTicks();

        draw();
        
        SDL_RenderPresent( gRenderer );
    }

    close();

    return 0;
}

bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize. SDL Error: %s\n", SDL_GetError());
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

    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

    board = newBoard();

    return true;
}

void update(float dt)
{

}

void draw()
{
    drawBoard(&board, gRenderer);
}

void close()
{
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = NULL;
    gWindow = NULL;
}
