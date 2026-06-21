#include "MinesweeperBoard.h"
#include "MSSFMLView.h"
#include "MSSFMLController.h"

int main() {
    const int COLS = 10;
    const int ROWS = 10;

    MinesweeperBoard board(COLS, ROWS, EASY);
    MSSFMLView       view(board);
    MSSFMLController controller(board, view, COLS, ROWS);

    controller.play();

    return 0;
}