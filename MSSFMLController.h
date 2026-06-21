#ifndef SAPER_MSSFMLCONTROLLER_H
#define SAPER_MSSFMLCONTROLLER_H

#include <SFML/Graphics.hpp>
#include "MinesweeperBoard.h"
#include "MSSFMLView.h"

using namespace sf;

enum AppState { MAIN_MENU, MODE_SELECT, PLAYING };

class MSSFMLController {
    MinesweeperBoard &board;
    MSSFMLView       &view;
    RenderWindow      window;
    AppState appState;
    int cols;
    int rows;

    int mouseToRow(int mouse_y) const;
    int mouseToCol(int mouse_x) const;
    bool clickedButton(int mouse_x, int mouse_y, float btn_y, float btn_h = MSSFMLView::BTN_H) const;
    void handleMainMenu(const Event &event);
    void handleModeSelect(const Event &event);
    void handlePlaying(const Event &event);
    void handleEvent(const Event &event);
    void startGame(int w, int h, GameMode mode);

public:
    MSSFMLController(MinesweeperBoard &board, MSSFMLView &view, int cols, int rows);

    void play();
};

#endif // SAPER_MSSFMLCONTROLLER_H