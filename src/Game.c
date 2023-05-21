#include "Game.h"

Game newGame()
{
    Game game;
    game.whitePlayerTurn = true;
    game.board = newBoard();

    return game;
}

void initMedia(SDL_Renderer* renderer)
{
    initSprites(renderer);
}

void drawGame(Game* game, SDL_Renderer* renderer)
{
    drawBoard(&game->board, renderer);
}

bool hasWon(Game* game, bool isWhite)
{
    return inCheck(&game->board, !isWhite) && !hasValidMoves(&game->board, !isWhite);
}

bool stalemate(Game* game)
{
    return !hasValidMoves(&game->board, game->whitePlayerTurn) && !inCheck(&game->board, game->whitePlayerTurn);
}

void gameHandleMousePress(Game* game, Point* point)
{
    if(!tryMovePiece(&game->board, point))
    {
        trySelectPiece(&game->board, point, game->whitePlayerTurn);
    }
    else
    {
        game->whitePlayerTurn = !game->whitePlayerTurn;
        resetPassants(&game->board, game->whitePlayerTurn);
    }
}
