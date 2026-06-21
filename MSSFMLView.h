#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "MinesweeperBoard.h"
#include <string>

#ifndef SAPER_MSSFMLVIEW_H
#define SAPER_MSSFMLVIEW_H

using namespace std;
using namespace sf;

class MSSFMLView {
    MinesweeperBoard &game_board;
    RectangleShape tile_shape;
    Font main_font;
    Text board_text;
    sf::Texture flag_texture;
    sf::Texture mine_texture;
    sf::Sprite  flag_sprite;
    sf::Sprite  mine_sprite;

    void digitColor(char digit);
    void drawTile(RenderWindow &window, char symbol, float tile_x, float tile_y);
    static void drawCentered(RenderWindow &window, Text &txt, const string &content, float win_width, float pos_y);
    void drawTopBar(RenderWindow &window);
    void drawOverlay(RenderWindow &window);
    void drawButton(RenderWindow &window, const string &label, float win_width, float btn_y, float btn_h = 60);
public:
    static const int TILE_PX  = 40;
    static const int TOPBAR_H = 40;
    static const int BTN_H    = 60;
    static const int BTN_W    = 400;

    explicit MSSFMLView(MinesweeperBoard &board);
    void draw_interface(RenderWindow &window);
    void draw_game(RenderWindow &window);
    void drawMainMenu(RenderWindow &window);
    void drawModeSelect(RenderWindow &window);
    void drawWinScreen(RenderWindow &window);
    void drawLoseScreen(RenderWindow &window);
};

#endif // SAPER_MSSFMLVIEW_H