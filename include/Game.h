#pragma once
#include "Board.h"
#include "core.h"

typedef struct Game
{
    Board board;
    bool whitePlayerTurn;
} Game;

Game newGame();
void initMedia(SDL_Renderer* renderer);
void drawGame(Game* game, SDL_Renderer* renderer);
void gameHandleMousePress(Game* game, Point* pos);
bool hasWon(Game* game, bool isWhite);
bool stalemate(Game* game);
