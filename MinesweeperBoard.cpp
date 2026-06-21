//
// Created by c on 3/11/26.
//

#include "MinesweeperBoard.h"
#include <iostream>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

//*********************************************************************************************************************
//                                                      PRYWATNE
//*********************************************************************************************************************

void MinesweeperBoard::clearBoard() {
    for(int row=0; row<height; row++)
    {
        for (int col=0; col<width; col++)
        {
            board[row][col].hasMine = false;
            board[row][col].hasFlag = false;
            board[row][col].isRevealed = false;
        }
    }
}

//*********************************************************************************************************************

void MinesweeperBoard::setMines(int mines) {
    // W spospob losowy rozklada przkeazana przez parametr liczbe min
    while (mines > 0)
    {
        int row = rand() % height;
        int col = rand() % width;
        if (!board[row][col].hasMine)
        {
            board[row][col].hasMine = true;
            --mines;
        }
    }
}

//*********************************************************************************************************************

bool MinesweeperBoard::isInBoard(int row, int col) const {
    // Zwraca wartosc logiczna sprawdzajac czy mina znajduje sie w obrebie planszy
    return row >= 0 && row < height && col >= 0 && col < width;
}

//*********************************************************************************************************************

void MinesweeperBoard::debugBoardMode() {
    // Ustawia plansze w trybie gry DEBUG
    for(int row=0; row<height; row++)
    {
        for (int col=0; col<width; col++)
        {
            if (row==0)
            {
                board[row][col].hasMine = true;
            }
            if (row%2==0 && col==0)
            {
                board[row][col].hasMine = true;
            }
            if(col==row)
            {
                board[row][col].hasMine = true;
            }
        }
    }
}
//*********************************************************************************************************************

bool MinesweeperBoard::ifWon() {
    // sprawdza czy gra zostala wygrana
    for (int r=0; r<height; r++)
    {
        for (int c=0; c<width; c++)
        {
            if (!board[r][c].hasMine && !board[r][c].isRevealed)
            {
                return false;
            }
        }
    }
    return true;
}

//*********************************************************************************************************************
//                                                   KONSTRUKTORY
//*********************************************************************************************************************

MinesweeperBoard::MinesweeperBoard() : board(7, 5), width(5), height(7), firstMove(true), mode(DEBUG), gameState(RUNNING) {
    // Konstruktor z pierwszego etapu czysci plansze i ustawia testowe pola
    clearBoard();
    board[0][0].hasMine = true;
    board[1][1].isRevealed = true;
    board[0][2].hasMine = true;
    board[0][2].hasFlag = true;
    gameState = RUNNING;
}

//*********************************************************************************************************************

MinesweeperBoard::MinesweeperBoard(int width, int height, GameMode mode) : board(height, width), width(width), height(height), firstMove(true), mode(mode), gameState(RUNNING) {
    // Konstruktor z trzeciego etapu czysci plansze i w zaleznosci od trybu oblicza i ustawia za pomoca setMines() miny
    clearBoard();
    double n = 0.0;
    switch (mode)
    {
        case DEBUG:
            debugBoardMode();
            break;
        case EASY:
            n=0.1;
            break;
        case NORMAL:
            n=0.2;
            break;
        case HARD:
            n=0.3;
    }
    int minesCount = static_cast<int>(ceil(width * height * n));
    setMines(minesCount);
    gameState = RUNNING;
}

//*********************************************************************************************************************
//                                                    PUBLICZNE
//*********************************************************************************************************************

void MinesweeperBoard::debug_display() const {
    // Wyswietla plansze
    std::cout<<"    ";
    for (int col=0; col<width; ++col)
    {
        std::cout<<"  "<<col<<"  ";
    }
    std::cout<<std::endl;
    for (int row=0; row<height; ++row) {
        std::cout<<row<<"  ";
        for (int col=0; col<width; ++col) {
            char M, f, o;
            if (board[row][col].hasMine)
                M = 'M';
            else
                M = '.';

            if (board[row][col].hasFlag)
                f = 'f';
            else
                f = '.';

            if (board[row][col].isRevealed)
                o = 'o';
            else
                o = '.';

            std::cout << "[" << M << o << f << "]";
        }
        std::cout << std::endl;
    }
}

//*********************************************************************************************************************

int MinesweeperBoard::getBoardWidth() const {
    // Zwraca szerokosc
    return width;
}

//*********************************************************************************************************************

int MinesweeperBoard::getBoardHeight() const {
    // Zwraca wysokosc
    return height;
}

//*********************************************************************************************************************

int MinesweeperBoard::getMineCount() const {
    // Zwraca liczbe min
    int n=0;
    for (int row=0; row<height; row++)
    {
        for (int col=0; col<width; col++)
        {
            if (board[row][col].hasMine) n++;
        }
    }
    return n;
}

//*********************************************************************************************************************

bool MinesweeperBoard::hasFlag(int row, int col) const {
    // Sprawdza czy zadane pole jest na planszy oraz czy jest odsloniete. Zwraca wartosc logiczna sprawdzajac obecnosc
    // flagi na polu
    if (!isInBoard(row, col))
    {
        return false;
    }
    if (board[row][col].isRevealed)
    {
        return false;
    }
    return board[row][col].hasFlag;
}

//*********************************************************************************************************************

void MinesweeperBoard::toggleFlag(int row, int col) {
    // Sprawdza czy gra jest aktywna, czy zadane pole jest na planszy oraz czy jest odsloniete. Przelacza flage na
    // danym polu
    if (gameState!=RUNNING)
    {
        return;
    }
    if (!isInBoard(row, col))
    {
        return;
    }
    if (board[row][col].isRevealed)
    {
        return;
    }
    board[row][col].hasFlag = !board[row][col].hasFlag;
}

//*********************************************************************************************************************

int MinesweeperBoard::countMines(int row, int col) const {
    // Sprawdza czy pole jest na planszy, czy jest odsloniete nastepnie w obrebie 3x3 od pola szuka min
    if (!isInBoard(row, col))
    {
        return -1;
    }
    if (!board[row][col].isRevealed)
    {
        return -1;
    }
    int n=0;
    for (int rowSmall = row - 1; rowSmall <= row + 1; ++rowSmall)
        {
        for (int colSmall = col - 1; colSmall <= col + 1; ++colSmall)
        {
            if (isInBoard(rowSmall, colSmall))
            {
                n+=board[rowSmall][colSmall].hasMine;
            }
        }
    }
    return n-board[row][col].hasMine;
}

//*********************************************************************************************************************

bool MinesweeperBoard::isRevealed(int row, int col) const {
    // Sprawdza czy pole jest na planszy a nastepnie zwraca czy jest odsloniete
    if (!isInBoard(row, col))
    {
        return false;
    }
    return board[row][col].isRevealed;
}

//*********************************************************************************************************************

GameState MinesweeperBoard::getGameState() const {
    // Zwraca obecny stan gry
    return gameState;
}

//*********************************************************************************************************************

void MinesweeperBoard::revealField(int row, int col) {
    // Sprawdzamy czy gra dalej trwa, czy pole jest na planszy czy jest odsloniete badz ma flage. Nastepnie sprawdzamy
    // gracz nie udezyl w mine podczas pierwszego ruchu jezeli tak przenosimy ja w nowe miejsce, zabieramy przywilej
    // pierwszego ruchu. Nastepnie dla ruchu ktory nie jest pierwszym sprawdzamy czy udezyl w mine jezeli tak konczymy
    // gre porazka. Dodatkowo funkcja rekurencyjnie odkrywa pola ktore napewno nie maja min by ulatwic rozgrywke
    if (gameState != RUNNING)
    {
        return;
    }
    if (!isInBoard(row, col))
    {
        return;
    }
    if (board[row][col].isRevealed)
    {
        return;
    }
    if (board[row][col].hasFlag)
    {
        return;
    }
    if (firstMove && board[row][col].hasMine && mode != DEBUG)
    {
        board[row][col].hasMine = false;
        setMines(1);
    }
    firstMove = false;
    board[row][col].isRevealed=true;
    if (board[row][col].hasMine)
    {
        gameState = FINISHED_LOSS;
        return;
    }
    if (board[row][col].hasMine == false)
    {
        board[row][col].isRevealed = true;
        if (countMines(row,col) == 0)
        {
            for (int r=row -1; r<= row +1; r++)
            {
                for (int c=col -1; c<= col +1; c++)
                {
                    revealField(r,c);
                }
            }
        }
    }
    if(ifWon())
    {
        gameState = FINISHED_WIN;
        return;
    }
    return;
}

//*********************************************************************************************************************

char MinesweeperBoard::getFieldInfo(int row, int col) const {
    // Zwraca chara w zaleznosci od statusu pola
    if (!isInBoard(row, col))
    {
        return '#';
    }
    if (!board[row][col].isRevealed && board[row][col].hasFlag)
    {
        return 'F';
    }
    if (!board[row][col].isRevealed && !board[row][col].hasFlag)
    {
        return '_';
    }
    if (board[row][col].isRevealed && board[row][col].hasMine)
    {
        return 'x';
    }
    if (board[row][col].isRevealed && countMines(row, col) == 0)
    {
        return ' ';
    }
    if (board[row][col].isRevealed && countMines(row, col) != 0)
    {
        return static_cast<char>(countMines(row, col)+'0');
    }
    return '?';
}
