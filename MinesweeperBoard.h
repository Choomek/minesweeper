#ifndef MINESWEEPERBOARD_H
#define MINESWEEPERBOARD_H

#include "Array2D.h"

enum GameMode  { DEBUG, EASY, NORMAL, HARD };
enum GameState { RUNNING, FINISHED_WIN, FINISHED_LOSS };

struct Field {
    bool hasMine;
    bool hasFlag;
    bool isRevealed;
};

class MinesweeperBoard {
    Array2D<Field> board;
    int width;
    int height;
    bool firstMove;

    GameMode mode;
    GameState gameState;

    void clearBoard();
    void setMines(int mines);
    bool isInBoard(int row, int col) const;
    void debugBoardMode();
    bool ifWon();

public:

    MinesweeperBoard();
    MinesweeperBoard(int width, int height, GameMode mode);

    void debug_display() const;
    int getBoardWidth() const;
    int getBoardHeight() const;
    int getMineCount() const;
    bool hasFlag(int row, int col) const;
    void toggleFlag(int row, int col);
    int countMines(int row, int col) const;
    bool isRevealed(int row, int col) const;
    GameState getGameState() const;
    void revealField(int row, int col);
    char getFieldInfo(int row, int col) const;

};

#endif // MINESWEEPERBOARD_H

