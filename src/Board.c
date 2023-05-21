#include <Board.h>
#include <math.h>
#include "List.h"
#include "Texture.h"

#define NUM_SPRITES 2 * (NUM_TYPES - 1)
static Texture sprites[NUM_SPRITES];
static List* getValidMoves(Board const* board, Point pieceIJ);
static bool checkValidLocation(Board const* board, Piece curr, Point currIJ, Point nextIJ);
static bool checkValidDirection(Board const* board, Piece curr, Point currIJ, Point nextIJ);

#define WIDTH (480.f / 8)
#define HEIGHT (480.f / 8)
void initSprites(SDL_Renderer* renderer)
{
    #define FILEPATH_BUFFER 100
    char filepath[FILEPATH_BUFFER];
    char* colours[] = {"white_", "black_"};
    char* types[] = {"pawn", "knight", "bishop", "rook", "queen", "king"};
    for(int i = 0, c = 0, t = 0; i < NUM_SPRITES; ++i, c ^= 1)
    {
        Texture tex = *newTexture();
        sprintf(filepath, "assets/%spieces/%s%s.png", colours[c], colours[c], types[t]);

        loadFromFile(&tex, filepath, renderer);
        tex.width = WIDTH;
        tex.height = HEIGHT;
        sprites[i] = tex; 
        if(c) ++t;
    }
}

Board newBoard()
{
    Board board;
    initializeBoard(&board);
    return board;
}

void initializeBoard(Board* board)
{
    Piece piece;
    piece.hasMoved = false;
    piece.passantable = false;
    for(int x = 0; x < BOARD_DIM; ++x)
    {
        for(int y = 0; y < BOARD_DIM; ++y)
        {
            if(y > 1 && y < 6) piece.type = NONE;
            else if(y <= 1) piece.isWhite = true;
            else if(y >= 6) piece.isWhite = false;

            if(y == 1 || y == BOARD_DIM - 2) piece.type = PAWN;
            else if(y == 0 || y == BOARD_DIM - 1)
            {
                if(x == 0 || x == 7) piece.type = ROOK;
                else if(x == 1 || x == 6) piece.type = KNIGHT;
                else if(x == 2 || x == 5) piece.type = BISHOP;
                else if(x == 3) piece.type = QUEEN;
                else piece.type = KING;
            }

            board->squares[x][y] = piece;
        }
    }

    board->selected.x = -1; board->selected.y = -1;
}

bool inCheck(Board const* board, bool isWhite)
{
    Point kingPos;
    for(int x = 0; x < BOARD_DIM; ++x)
    {
        for(int y = 0; y < BOARD_DIM; ++y)
        {
            if(board->squares[x][y].type == KING && board->squares[x][y].isWhite == isWhite)
            {
                kingPos.x = x; kingPos.y = y;
                break;
            }
        }
    }

    for(int x = 0; x < BOARD_DIM; ++x)
    {
        for(int y = 0; y < BOARD_DIM; ++y)
        {
            Piece const* attacker = &board->squares[x][y];
            if(attacker->isWhite != isWhite)
            {
                Point attackerPos = { x, y };
                bool canAttack = checkValidLocation(board, *attacker, attackerPos, kingPos) && 
                                 checkValidDirection(board, *attacker, attackerPos, kingPos);
                if(canAttack) return true;
            }
        }
    }

    return false;
}

bool hasValidMoves(Board const* board, bool isWhite)
{
    bool noValidMoves = true;
    for(int x = 0; x < BOARD_DIM && noValidMoves; ++x)
    {
        for(int y = 0; y < BOARD_DIM && noValidMoves; ++y)
        {
            Point pieceIJ;
            pieceIJ.x = x; pieceIJ.y = y;
            if(board->squares[x][y].isWhite == isWhite)
            {
                List* pieceMoves = getValidMoves(board, pieceIJ);
                if(pieceMoves->count)
                {
                    noValidMoves = false;
                }
                deleteList(pieceMoves);
            }
        }
    }

    return !noValidMoves;
}

void drawBoard(Board const* board, SDL_Renderer* renderer)
{
    List* validMoves = NULL;
    if(board->selected.x != -1 && board->selected.y != -1)
    {
        validMoves = getValidMoves(board, board->selected);
    }
    
    bool whiteKingCheck = inCheck(board, true);
    bool blackKingCheck = inCheck(board, false);
    for(int x = 0; x < BOARD_DIM; ++x)
    {
        for(int y = 0; y < BOARD_DIM; ++y)
        {
            uint8_t colourR = 0;
            uint8_t colourG = 0;
            uint8_t colourB = 0;
            uint8_t colourA = 255;
            if((x + y) % 2 == 0)
            {
                colourR = colourG = colourB = 255;
            }

            SDL_Rect square = {x * WIDTH, HEIGHT * (BOARD_DIM - 1) - y * HEIGHT, WIDTH, HEIGHT};
            
            if(board->selected.x == x && board->selected.y == y)
            {
                colourR = 128; colourG = 128; colourB = 128;
            }

            if(validMoves)
            {
                bool validSquare = false;
                for(unsigned int i = 0; i < validMoves->count && !validSquare; ++i)
                {
                    Point* item = (Point*)getItem(validMoves, i);
                    validSquare = x == item->x && y == item->y;
                }

                if(validSquare)
                {
                    colourR = 11; colourG = 113; colourB = 86;
                }
            }

            if((whiteKingCheck && board->squares[x][y].type == KING && board->squares[x][y].isWhite) ||
               (blackKingCheck && board->squares[x][y].type == KING && !board->squares[x][y].isWhite))
            {
                colourR = 255; colourG = 0; colourB = 0;
            }

            SDL_SetRenderDrawColor(renderer, colourR, colourG, colourB, colourA);
            SDL_RenderFillRect(renderer, &square);
        }
    }

    for (int x = 0; x < BOARD_DIM; ++x)
    {
        for(int y = 0; y < BOARD_DIM; ++y)
        {
            Piece piece = board->squares[x][y];
            if(piece.type)
            {
                Texture renderSprite = sprites[2 * (piece.type - 1) + 1 - piece.isWhite];
                renderTexture(&renderSprite, renderer, x * WIDTH, HEIGHT * (BOARD_DIM - 1) - y * HEIGHT);
            }
        }
    }
    

    if(validMoves) deleteList(validMoves);
}

bool trySelectPiece(Board* board, Point* pos, bool isWhiteTurn)
{
    if(pos->x < 0 || pos->y < 0 || pos->x >= 480 || pos->y >= 480)
    {
        return false;
    }

    int i = pos->x / WIDTH;
    int j = pos->y / HEIGHT;
    if(board->squares[i][j].type && board->squares[i][j].isWhite == isWhiteTurn)
    {
        board->selected.x = i; board->selected.y = j;
        return true;
    }
    
    return false;
}

static bool checkValidDirection(Board const* board, Piece curr, Point currIJ, Point dest)
{
    if(curr.type == KNIGHT) return true;

    //---------------------------------
    // utilize bresenham line algorithm
    int deltaI = dest.x - currIJ.x;
    int deltaJ = dest.y - currIJ.y;

    // since bresenham obeys diamond exit rule we shift cells we look at over by one
    int x = currIJ.x;
    int y = currIJ.y;
    if(deltaI > 0) x++;
    else if(deltaI < 0) x--;

    if(deltaJ > 0) y++;
    else if(deltaJ < 0) y--;

    bool isIFast = deltaI * deltaI > deltaJ * deltaJ;
  
    int fast = isIFast ? deltaI : deltaJ;
    int slow = isIFast ? deltaJ : deltaI;

    int setX = deltaI >= 0 ? 1 : -1;
    int setY = deltaJ >= 0 ? 1 : -1;
    
    float m = (float) slow / fast;
    float error = m;
    
    bool pieceSeen = false;
    for(int dist = 0; dist * dist < fast * fast && x < BOARD_DIM && x >= 0 && y < BOARD_DIM && y >= 0; ++dist)
    {
        if(pieceSeen) return false;

        if(board->squares[x][y].type)
        {
            pieceSeen = true;
        }

        if(isIFast) x += setX;
        else y += setY;

        if(error * error > 0.5 * 0.5)
        {
            if(isIFast) y += setY;
            else x += setX;
            error -= error > 0 ? 1 : -1;
        }

        error += m;
    }

    return true;
}

static bool checkValidLocation(Board const* board, Piece curr, Point currIJ, Point nextIJ)
{
    int currI = currIJ.x; int currJ = currIJ.y;
    int nextI = nextIJ.x; int nextJ = nextIJ.y;

    if(currI == nextI && currJ == nextJ) return false;

    switch(curr.type)
    {
        case(PAWN)   : 
        {
            bool correctDir = (currJ + (2 * curr.isWhite - 1) == nextJ || currJ + (2 * curr.isWhite - 1) * (1 + !curr.hasMoved) == nextJ);
            bool takes = board->squares[nextI][nextJ].type && abs(nextI - currI) == 1 && abs(nextJ - currJ) == 1;
            bool normalMove = !board->squares[nextI][nextJ].type && currI == nextI;
            bool enPassant = board->squares[nextI][currJ].type == PAWN && board->squares[nextI][currJ].passantable && abs(nextI - currI) == 1 && abs(nextJ - currJ) == 1;

            return correctDir && (takes || normalMove || enPassant);
        }
        case(KNIGHT) : return (abs(currI - nextI) == 2 && abs(currJ - nextJ) == 1) 
                           || (abs(currI - nextI) == 1 && abs(currJ - nextJ) == 2);
        case(BISHOP) : return abs(currI - nextI) == abs(currJ - nextJ);
        case(ROOK)   : return currI == nextI || currJ == nextJ;
        case(QUEEN)  : return currI == nextI || currJ == nextJ
                                 || abs(currI - nextI) == abs(currJ - nextJ);
        case(KING)   : 
        {
            bool normalMove = abs(currI - nextI) <= 1 && abs(currJ - nextJ) <= 1;
            if(normalMove) return normalMove;

            int rookI = nextI - currI > 0 ? BOARD_DIM - 1 : 0;
            int rookJ = (BOARD_DIM - 1) * (1 - curr.isWhite);
            Point rookIJ = {rookI, rookJ};
            bool castle = !curr.hasMoved && abs(currI - nextI) == 2 && currJ == nextJ && checkValidDirection(board, curr, currIJ, rookIJ);
            bool rookUnmoved = !board->squares[rookI][rookJ].hasMoved && board->squares[rookI][rookJ].type == ROOK;

            return castle && rookUnmoved;
        } 
        default      : return false;
    }
}

static void movePiece(Board* board, Point selected, Point ij)
{
    int i = ij.x; int j = ij.y;
    int selectedX = selected.x; int selectedY = selected.y;
    Piece* from = &board->squares[selectedX][selectedY];
    Piece* to = &board->squares[i][j];
    if(from->type == KING && !from->hasMoved && abs(i - selectedX) == 2)
    {
        int dir = (i - selectedX) / 2;
        int rookI = dir > 0 ? BOARD_DIM - 1 : 0;
        int rookJ = (BOARD_DIM - 1) * (1 - from->isWhite);
        board->squares[i - dir][j] = board->squares[rookI][rookJ];
        board->squares[i - dir][j].hasMoved = true;
        board->squares[rookI][rookJ].type = NONE;
    }
    else if(from->type == PAWN && !from->hasMoved)
    {
        from->passantable = true;
    }
    else if(from->type == PAWN && to->type == NONE && selectedX != i)
    {
        board->squares[i][selectedY].type = NONE;
    }
    
    board->squares[i][j] = board->squares[selectedX][selectedY];
    board->squares[i][j].hasMoved = true;
    board->squares[selectedX][selectedY].type = NONE;
}

// functions calling this must also call deleteList on the pointer
static List* getValidMoves(Board const* board, Point pieceIJ)
{
    Piece piece = board->squares[pieceIJ.x][pieceIJ.y];
    List* validMoves = newList();
    for(unsigned int x = 0; x < BOARD_DIM; ++x)
    {
        for(unsigned int y = 0; y < BOARD_DIM; ++y)
        {
            Point* dest = (Point*)malloc(sizeof(Point));
            dest->x = x; dest->y = y;

            bool validMove = checkValidLocation(board, piece, pieceIJ, *dest) && checkValidDirection(board, piece, pieceIJ, *dest);
            bool validSquare = !board->squares[dest->x][dest->y].type || board->squares[pieceIJ.x][pieceIJ.y].isWhite != board->squares[dest->x][dest->y].isWhite;
            
            if(validMove && validSquare)
            {
                Board newState;
                memcpy(&newState, board, sizeof(Board));
                movePiece(&newState, pieceIJ, *dest);
                if(!inCheck(&newState, board->squares[pieceIJ.x][pieceIJ.y].isWhite))
                {
                    insert(validMoves, dest);
                }
                else
                {
                    free(dest);
                }
            }
            else
            {
                free(dest);
            }
        }
    }
    
    return validMoves;
}

static bool isValidMove(Board* board, Point currIJ, Point nextIJ)
{
    List* validMoves = getValidMoves(board, currIJ);
    bool validLocation = false;
    for(unsigned int i = 0; i < validMoves->count && !validLocation; ++i)
    {
        Point* item = (Point*)getItem(validMoves, i);
        validLocation = nextIJ.x == item->x && nextIJ.y == item->y;
    }
    deleteList(validMoves);

    return validLocation;
}

bool tryMovePiece(Board* board, Point* pos)
{
    bool success = false;
    if(pos->x < 0 || pos->y < 0 || pos->x >= 480 || pos->y >= 480 || 
      (board->selected.x == -1 && board->selected.y == -1))
    {
        return success;
    }

    int i = pos->x / WIDTH;
    int j = pos->y / HEIGHT;
    Point ij = { i, j };
    int selectedX = board->selected.x;
    int selectedY = board->selected.y;
    Point selected = { selectedX, selectedY };
    if(isValidMove(board, selected, ij))
    {
        movePiece(board, selected, ij);
        success = true;
    }

    board->selected.x = -1; board->selected.y = -1;

    return success;
}

void resetPassants(Board* board, bool resetWhitePieces)
{
    for(int x = 0; x < BOARD_DIM; ++x)
    {
        for(int y = 0; y < BOARD_DIM; ++y)
        {
            Piece* piece = &board->squares[x][y];
            if(piece->type == PAWN && piece->isWhite == resetWhitePieces)
            {
                piece->passantable = false;
            }
        }
    }
}
