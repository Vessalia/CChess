#include <Board.h>

Board newBoard()
{
    Board board;
    initializeBoard(&board);
    return board;
}

void initializeBoard(Board* board)
{
    Piece piece = NONE;
    for(int x = 0; x < BOARD_DIM; ++x)
    {
        for(int y = 0; y < BOARD_DIM; ++y)
        {
            if(y > 1 && y < 6) piece = NONE;
            else if(y == 1 || y == 6) piece = PAWN;
            else if(x == 0 || x == 7) piece = ROOK;
            else if(x == 1 || x == 6) piece = KNIGHT;
            else if(x == 2 || x == 5) piece = BISHOP;
            else if(x == 3) piece = QUEEN;
            else piece = KING;
            board->squares[x][y] = piece;
        }
    }
}

#define WIDTH 480 / 8
#define HEIGHT 480 / 8
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

            SDL_Rect square = {x * WIDTH, y * HEIGHT, WIDTH, HEIGHT};
            switch(board->squares[x][y])
            {
                case(NONE) : break;
                case(PAWN) : colourR = 255, colourG = 0; colourB = 0; break;
                case(KNIGHT) : colourR = 0, colourG = 255; colourB = 0; break;
                case(BISHOP) : colourR = 0, colourG = 0; colourB = 255; break;
                case(ROOK) : colourR = 255, colourG = 255; colourB = 0; break;
                case(QUEEN) : colourR = 255, colourG = 0; colourB = 255; break;
                case(KING) : colourR = 0, colourG = 255; colourB = 255; break;
            }

            SDL_SetRenderDrawColor(renderer, colourR, colourG, colourB, colourA);
            SDL_RenderFillRect(renderer, &square);
        }
    }
}
