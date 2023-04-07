#include <Board.h>
#include <math.h>

Board newBoard()
{
    Board board;
    initializeBoard(&board);
    return board;
}

void initializeBoard(Board*const board)
{
    Piece piece = NONE;
    for(int x = 0; x < BOARD_DIM; ++x)
    {
        for(int y = 0; y < BOARD_DIM; ++y)
        {
            if(y > 1 && y < 6) piece = NONE;
            else if(y == 1) piece = WHITE_PAWN;
            else if(y == 6) piece = BLACK_PAWN;
            else if(y == 0)
            {
                if(x == 0 || x == 7) piece = WHITE_ROOK;
                else if(x == 1 || x == 6) piece = WHITE_KNIGHT;
                else if(x == 2 || x == 5) piece = WHITE_BISHOP;
                else if(x == 3) piece = WHITE_QUEEN;
                else piece = WHITE_KING;
            }
            else if(y == 7)
            {
                if(x == 0 || x == 7) piece = BLACK_ROOK;
                else if(x == 1 || x == 6) piece = BLACK_KNIGHT;
                else if(x == 2 || x == 5) piece = BLACK_BISHOP;
                else if(x == 3) piece = BLACK_QUEEN;
                else piece = BLACK_KING;
            }
            board->squares[x][y] = piece;
        }
    }

    board->selected.x = -1; board->selected.y = -1;
}

#define WIDTH (480.f / 8)
#define HEIGHT (480.f / 8)
void drawBoard(const Board* board, SDL_Renderer* renderer)
{
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
            switch(board->squares[x][y])
            {
                case(NONE) : break;
                case(WHITE_PAWN) : colourR = 255, colourG = 0; colourB = 0; break;
                case(WHITE_KNIGHT) : colourR = 0, colourG = 255; colourB = 0; break;
                case(WHITE_BISHOP) : colourR = 0, colourG = 0; colourB = 255; break;
                case(WHITE_ROOK) : colourR = 255, colourG = 255; colourB = 0; break;
                case(WHITE_QUEEN) : colourR = 255, colourG = 0; colourB = 255; break;
                case(WHITE_KING) : colourR = 0, colourG = 255; colourB = 255; break;
                case(BLACK_PAWN) : colourR = 255 / 2, colourG = 0; colourB = 0; break;
                case(BLACK_KNIGHT) : colourR = 0, colourG = 255 / 2; colourB = 0; break;
                case(BLACK_BISHOP) : colourR = 0, colourG = 0; colourB = 255 / 2; break;
                case(BLACK_ROOK) : colourR = 255 / 2, colourG = 255 / 2; colourB = 0; break;
                case(BLACK_QUEEN) : colourR = 255 / 2, colourG = 0; colourB = 255 / 2; break;
                case(BLACK_KING) : colourR = 0, colourG = 255 / 2; colourB = 255 / 2; break;
            }

            if(board->selected.x != -1 && board->selected.y != -1)
            {
                if(board->selected.x == x && board->selected.y == y)
                {
                    colourR = 128; colourG = 128; colourB = 128;
                }
            }

            SDL_SetRenderDrawColor(renderer, colourR, colourG, colourB, colourA);
            SDL_RenderFillRect(renderer, &square);
        }
    }
}

bool trySelectPiece(Board*const board, Point* pos)
{
    if(pos->x < 0 || pos->y < 0 || pos->x >= 480 || pos->y >= 480)
    {
        return false;
    }

    int i = pos->x / WIDTH;
    int j = pos->y / HEIGHT;
    if(board->squares[i][j])
    {
        board->selected.x = i; board->selected.y = j;
        return true;
    }
    
    return false;
}

static bool isWhitePiece(Piece piece)
{
    return piece >= WHITE_PAWN && piece <= WHITE_KING;
}

static bool isBlackPiece(Piece piece)
{
    return piece >= BLACK_PAWN && piece <= BLACK_KING;
}

static bool checkValidDirection(Piece curr, Piece target, int currI, int currJ, int nextI, int nextJ)
{
    switch(curr)
    {
        case(WHITE_PAWN)   : return currJ + 1 == nextJ && (target || currI == nextI);
        case(BLACK_PAWN)   : return currJ - 1 == nextJ && (target || currI == nextI);
        case(WHITE_KNIGHT) :
        case(BLACK_KNIGHT) : return (abs(currI - nextI) == 2 && abs(currJ - nextJ) == 1) 
                                 || (abs(currI - nextI) == 1 && abs(currJ - nextJ) == 2);
        case(WHITE_BISHOP) :
        case(BLACK_BISHOP) : return abs(currI - nextI) == abs(currJ - nextJ);
        case(WHITE_ROOK)   :
        case(BLACK_ROOK)   : return currI == nextI || currJ == nextJ;
        case(WHITE_QUEEN)  :
        case(BLACK_QUEEN)  : return currI == nextI || currJ == nextJ
                                 || abs(currI - nextI) == abs(currJ - nextJ);
        case(WHITE_KING)   :
        case(BLACK_KING)   : return abs(currI - nextI) <= 1 && abs(currJ - nextJ) <= 1;
        default            : return false;
    }
}

static bool isValidMove(Board*const board, int currI, int currJ, int nextI, int nextJ)
{
    Piece currPiece = board->squares[currI][currJ];
    Piece targetSquare = board->squares[nextI][nextJ];
    bool validDir = checkValidDirection(currPiece, targetSquare, currI, currJ, nextI, nextJ);
    bool attackingSelf = (isWhitePiece(currPiece) && isWhitePiece(targetSquare)) 
                      || (isBlackPiece(currPiece) && isBlackPiece(targetSquare));

    return validDir && !attackingSelf;
}

bool tryMovePiece(Board*const board, Point* pos)
{
    bool success = false;
    if(pos->x < 0 || pos->y < 0 || pos->x >= 480 || pos->y >= 480)
    {
        return success;
    }

    int i = pos->x / WIDTH;
    int j = pos->y / HEIGHT;
    int selectedX = board->selected.x;
    int selectedY = board->selected.y;
    if(selectedX != -1 && selectedY != -1)
    {
        if(isValidMove(board, selectedX, selectedY, i, j))
        {
            board->squares[i][j] = board->squares[selectedX][selectedY];
            board->squares[selectedX][selectedY] = NONE;
            success = true;
        }
    }

    board->selected.x = -1; board->selected.y = -1;

    return success;
}
