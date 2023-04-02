#pragma once
#include <SDL2/SDL.h>

#define BOARD_DIM 8
typedef enum ChessPiece
{
    NONE,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
} Piece;

typedef struct ChessBoard
{
    Piece squares[BOARD_DIM][BOARD_DIM];
} Board;

Board newBoard();
void initializeBoard(Board* board);
void drawBoard(const Board* board, SDL_Renderer* renderer);
