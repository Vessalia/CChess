#pragma once
#include <SDL2/SDL.h>
#include <Point.h>
#include "core.h"

typedef enum PieceType
{
    NONE,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
    NUM_TYPES
} Type;

typedef struct ChessPiece
{
    Type type;
    bool isWhite;
    bool hasMoved;
    bool passantable;
} Piece;

#define BOARD_DIM 8
typedef struct ChessBoard
{
    Piece squares[BOARD_DIM][BOARD_DIM];
    Point selected;
} Board;

Board newBoard();
void initSprites(SDL_Renderer* renderer);
void initializeBoard(Board* board);
void drawBoard(Board const* board, SDL_Renderer* renderer);
bool trySelectPiece(Board*, Point* pos, bool isWhiteTurn);
bool tryMovePiece(Board* board, Point* pos);
void resetPassants(Board* board, bool resetWhitePieces);
bool inCheck(Board const* board, bool isWhite);
bool hasValidMoves(Board const* board, bool isWhite);
