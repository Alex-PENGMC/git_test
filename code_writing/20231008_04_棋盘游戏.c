#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#define    SIZE        6    /*棋盘大小*/
const char comp_c   = '@';  /*计算机玩家*/
const char player_c = 'O';  /*玩家*/

void display(char board[][SIZE]);
int  valid_moves(char board[][SIZE], bool moves[][SIZE], char player);
void make_move(char board[][SIZE], int row, int col, char player);
void computer_move(char board[][SIZE], bool moves[][SIZE], char player);
int get_score(char board[][SIZE], char player);
int best_move(char board[][SIZE], bool moves[][SIZE], char player);

int main(void)
{
    char board[SIZE][SIZE] = {0};
    bool moves[SIZE][SIZE] = {false};

    int row = 0, col = 0, invalid_moves = 0, x = 0;
    int no_of_games = 0, no_of_moves = 0;
    int comp_score  = 0, user_score  = 0;
    char again = 0, y = 0;

    bool next_player = true;

    printf("\nREVERST\n\n");
    printf("You can go first on the first game, then we will take turns.\n");
    printf(" You will be white - (%c)\n I will be black - (%c).\n", player_c, comp_c);
    printf("Select a square for your move by typing a digit for the row\n and a letter for the column with no space between.\n");
    printf("\nGood luck! Press Enter to start.\n");

    do
    {
        next_player = !next_player;
        no_of_moves = 4;

        for (row = 0; row < SIZE; row++)
        {
            for (col = 0; col < SIZE; col++)
                board[row][col] = ' ';
        }

        int mid = SIZE / 2;
        board[mid - 1][mid - 1] = player_c;
        board[mid][mid]         = player_c;
        board[mid - 1][mid]     = comp_c;
        board[mid][mid - 1]     = comp_c;

        do
        {
            display(board);
            if(next_player = !next_player)
            {
                if (valid_moves(board, moves, player_c))
                {
                    for (; ;)
                    {
                        printf("Please enter your move (row column): ");
                        scanf(" %d%c", &x, &y);
                        y = tolower(y) - 'a';
                        x--;
                        if (x >= 0 && y >= 0 && x < SIZE && y < SIZE && moves[x][y])
                        {
                            make_move(board, x, y, player_c);
                            no_of_moves++;
                            break;
                        }
                        else
                            printf("Not a valid move, try again.\n");
                    }
                }
                else if (++invalid_moves < 2)
                {
                    printf("\nYou have to pass, press return");
                    scanf("%c", &again);
                }
                else
                    printf("Neither of us can go, so the game is over.\n");
            }
            else
            {
                if (valid_moves(board, moves, '@'))
                {
                    invalid_moves = 0;
                    computer_move(board, moves, '@');
                    no_of_moves++;
                }
                else
                {
                    if (++invalid_moves < 2)
                        printf("\nI have to pass, your go\n");
                    else
                        printf("Neither of us can go, so the game is over.\n");
                }
            }
        }while (no_of_moves < SIZE * SIZE && invalid_moves < 2);
        
        display(board);
        
        comp_score = 0;
        user_score = 0;
        for (row = 0; row < SIZE; row++)
        {
            for (col = 0; col < SIZE; col++)
            {
                comp_score += board[row][col] == comp_c;
                user_score += board[row][col] == player_c;
            }
        }
        printf("The final score is:\n");
        printf("Computer %d\n User %d\n\n", comp_score, user_score);

        printf("Do you want to play again (y/n): ");
        scanf(" %c", &again);

    } while ('y' == tolower(again));
    
    printf("\nGoodbye\n");

    // scanf("%c", &again);
    return 0;
}

/*********************************************
* Function to display the board in its       *
* current state with row numbers and column  *
* letters to identify squares.               *
* Parameter is the board array.              *
*********************************************/
void display(char board[][SIZE])
{
    char col_label = 'a';
    printf("\n ");
    for (int col = 0; col < SIZE; col++)
    {
        printf("   %c", col_label + col);
    }
    printf("\n");

    for (int row = 0; row < SIZE; row++)
    {
        printf("  +");
        for (int col = 0; col < SIZE; col++)
        {
            printf("---+");
        }
        printf("\n%2d|", row + 1);

        for (int col = 0; col < SIZE; col++)
        {
            printf(" %c |", board[row][col]);
        }
        printf("\n");
    }

    printf("  +");
    for (int col = 0; col < SIZE; col++)
    {
        printf("---+");
    }
    printf("\n");
}

/*********************************************
* Calculates which squares are valid moves   *
* for player. Valid moves are recorded in the*
* moves array - true indicates a valid move, *
* false indicates an invalid move.           *
* First parameter is the board array         *
* Second parameter is the moves array        *
* Third parameter identifies the player      *
* to make the move                           *
* Returns valid move count.                  *
*********************************************/
int valid_moves(char board[][SIZE], bool moves[][SIZE], char player)
{
    int rowdelta = 0, coldelta = 0;
    int x = 0, y = 0;
    int no_of_moves = 0;

    char opponent = (player == player_c) ? comp_c : player_c;

    for (int row = 0; row < SIZE; row++)
    {
        for(int col = 0; col < SIZE; col++)
        {
            moves[row][col] = false;
        }
    }

    for (int row = 0; row < SIZE; row++)
    {
        for (int col = 0; col < SIZE; col++)
        {
            if (' ' != board[row][col])
                continue;

            for (rowdelta = -1; rowdelta <= 1; rowdelta++)
            {
                for (coldelta = -1; coldelta <= 1; coldelta++)
                {
                    if (row + rowdelta < 0 || row + rowdelta >= SIZE ||
                        col + coldelta < 0 || col + rowdelta >= SIZE ||
                        (0 == rowdelta && 0 == coldelta))
                        continue;
                    
                    if (opponent == board[row + rowdelta][col + coldelta])
                    {
                        x = row + rowdelta;
                        y = col + coldelta;

                        for ( ; ;)
                        {
                            x += rowdelta;
                            y += coldelta;

                            if (x < 0 || x >= SIZE || y < 0 || y >= SIZE)
                                break;

                            if (' ' == board[x][y])
                                break;
                            
                            if (player == board[x][y])
                            {
                                moves[row][col] = true;
                                no_of_moves++;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}

void make_move(char board[][SIZE], int row, int col, char player)
{
    int rowdelta = 0, coldelta = 0;
    int x = 0, y = 0;

    char opponent = (player == player_c) ? comp_c : player_c;

    board[row][col] = player;

    for (rowdelta = -1; rowdelta <= 1; rowdelta++)
    {
        for (coldelta = -1; coldelta <= 1; coldelta++)
        {
            if (row + rowdelta < 0 || row + rowdelta >= SIZE ||
                col + coldelta < 0 || col + coldelta >= SIZE ||
                (0 == rowdelta && 0 == coldelta))
                continue;
            
            if (opponent == board[row + rowdelta][col + coldelta])
            {
                x = row + rowdelta;
                y = col + coldelta;

                for (; ;)
                {
                    x += rowdelta;
                    y += coldelta;

                    if (x < 0 || x >= SIZE || y < 0 || y >= SIZE)
                        break;
                    
                    if (' ' == board[x][y])
                        break;

                    if (player == board[x][y])
                    {
                        while(opponent == board[x -= rowdelta][y -= coldelta])
                        {
                            board[x][y] = player;
                            break;
                        }
                    }
                }
            }
        }
    }
}

void computer_move(char board[][SIZE], bool moves[][SIZE], char player)
{
    int best_row = 0, best_col = 0;
    int new_score = 0, score = 100;
    char temp_board[SIZE][SIZE];
    bool temp_moves[SIZE][SIZE];

    char opponent = (player == player_c) ? comp_c : player_c;
    for (int row = 0; row < SIZE; row++)
    {
        for (int col = 0; col < SIZE; col++)
        {
            if (!moves[row][col])
                continue;

            memcpy(temp_board, board, sizeof(temp_board));

            make_move(temp_board, row, col, player);

            valid_moves(temp_board, temp_moves, opponent);

            new_score = best_move(temp_board, temp_moves, opponent);

            if (new_score < score)
            {
                score = new_score;
                best_row = row;
                best_col = col;
            }
        }
    }
    make_move(board, best_row, best_col, player);
}

int get_score(char board[][SIZE], char player)
{
    int score = 0;

    char opponent = (player == player_c) ? comp_c : player_c;

    for (int row = 0; row < SIZE; row++)
    {
        for (int col = 0; col < SIZE; col++)
        {
            score -= board[row][col] == opponent;
            score += board[row][col] == player;
        }
    }

    return score;
}

int best_move(char board[][SIZE], bool moves[][SIZE], char player)
{
    char opponent = (player == player_c) ? comp_c : player_c;

    char new_board[SIZE][SIZE] = {0};
    int score = 0, new_score = 0;

    for (int row = 0; row < SIZE; row++)
    {
        for (int col = 0; col < SIZE; col++)
        {
            if (!moves[row][col])
                continue;
            
            memcpy(new_board, board, sizeof(new_board));

            make_move(new_board, row, col, player);

            new_score = get_score(new_board, player);

            if (score < new_score)
                score = new_score;
        }
    }

    return score;
}
