#pragma once
#include <SDL2/SDL.h>
#include <Point.h>
#include <bool.h>

typedef enum ChessPiece
{
    NONE,
    WHITE_PAWN,
    WHITE_KNIGHT,
    WHITE_BISHOP,
    WHITE_ROOK,
    WHITE_QUEEN,
    WHITE_KING,
    BLACK_PAWN,
    BLACK_KNIGHT,
    BLACK_BISHOP,
    BLACK_ROOK,
    BLACK_QUEEN,
    BLACK_KING
} Piece;

#define BOARD_DIM 8
typedef struct ChessBoard
{
    Piece squares[BOARD_DIM][BOARD_DIM];
    Point selected;
} Board;

Board newBoard();
void initializeBoard(Board*const board);
void drawBoard(Board const*const board, SDL_Renderer* renderer);
bool trySelectPiece(Board*const, Point* pos);
bool tryMovePiece(Board*const board, Point* pos);
