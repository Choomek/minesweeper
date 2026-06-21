#include "MSSFMLView.h"
#include <sstream>

//*********************************************************************************************************************
//                                                  PALETA KOLOROW
//*********************************************************************************************************************

static const Color BG          (245, 245, 245);
static const Color TILE_HIDDEN (200, 200, 200);
static const Color TILE_BORDER (160, 160, 160);
static const Color TILE_OPEN   (230, 230, 230);
static const Color TILE_MINE   (220,  80,  80);
static const Color TILE_FLAG_BG(180, 200, 255);
static const Color TEXT_DARK   ( 40,  40,  40);
static const Color TEXT_MUTED  (120, 120, 120);
static const Color BTN_FILL    (255, 255, 255);
static const Color BTN_BORDER  (180, 180, 180);
static const Color OVERLAY     (255, 255, 255, 200);

//*********************************************************************************************************************
//                                                   KONSTRUKTORY
//*********************************************************************************************************************

MSSFMLView::MSSFMLView(MinesweeperBoard &board) : game_board(board) {
    // konstruktor laduje czcionki (dejavusans u mnie do debuga) nastepnie laduje tekstury dopasowywuje rozmiary i
    //skaluje
    if (!main_font.loadFromFile("assets/font.ttf"))
        main_font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
    tile_shape.setSize({static_cast<float>(TILE_PX - 1), static_cast<float>(TILE_PX - 1)});
    board_text.setFont(main_font);
    board_text.setCharacterSize(18);
    if (!flag_texture.loadFromFile("assets/flag.png"))
        ;
    if (!mine_texture.loadFromFile("assets/mine.png"))
        ;
    flag_texture.setSmooth(true);
    mine_texture.setSmooth(true);
    flag_sprite.setTexture(flag_texture);
    mine_sprite.setTexture(mine_texture);
    float tile_size = static_cast<float>(TILE_PX - 1);
    auto scale_to_tile = [&](sf::Sprite &spr, const sf::Texture &tex) {
        sf::Vector2u ts = tex.getSize();
        spr.setScale(tile_size / ts.x, tile_size / ts.y);
    };
    scale_to_tile(flag_sprite, flag_texture);
    scale_to_tile(mine_sprite, mine_texture);
}

//*********************************************************************************************************************
//                                                     PRYWATNE
//*********************************************************************************************************************

void MSSFMLView::digitColor(char digit) {
    // funkcja ustawia kolor wypelnienia board_text w zaleznosci od min dookola lub puste dla flagi, pustego pola
    switch (digit)
    {
        case '1': board_text.setFillColor(Color( 30,  90, 200)); break;
        case '2': board_text.setFillColor(Color( 40, 140,  60)); break;
        case '3': board_text.setFillColor(Color(200,  50,  50)); break;
        case '4': board_text.setFillColor(Color( 80,  30, 160)); break;
        case '5': board_text.setFillColor(Color(160,  30,  30)); break;
        case '6': board_text.setFillColor(Color(  0, 140, 150)); break;
        case '7': board_text.setFillColor(Color( 20,  20,  20)); break;
        case '8': board_text.setFillColor(Color(100, 100, 100)); break;
        default:  board_text.setFillColor(TEXT_DARK); break;
    }
}

//*********************************************************************************************************************

void MSSFMLView::drawCentered(RenderWindow &window, Text &txt, const string &content, float win_width, float pos_y) {
    // funkcja rysuje tekst wysrodkowany poziomo, dziala w 4 krokach ustawia tresc, potem pobiera granice, nastepnie
    // ustwaiwa punkt odenisienia, a na koniec pozycjonuje i rysuje
    txt.setString(content);
    FloatRect b = txt.getLocalBounds();
    txt.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
    txt.setPosition(win_width / 2.f, pos_y);
    window.draw(txt);
}

//*********************************************************************************************************************

void MSSFMLView::drawTile(RenderWindow &window, char symbol, float tx, float ty) {
    // funkcja ta rysuje pojednyczy kafelke planszy sapera na okreslonej pozycji. Najpierw ustawiamy pozycje prostokata
    // i outline nastepnie za pomoca pomocniczych funkcji lambda (by nie powtarzac tego samego kodu a nie potrzeba
    // osobnej metody do tego) rysujemy rozne pola za pomoca switch case.
    tile_shape.setPosition(tx, ty);
    tile_shape.setOutlineThickness(1.f);
    auto bg = [&](Color fill, Color border)
    {
        tile_shape.setFillColor(fill);
        tile_shape.setOutlineColor(border);
        window.draw(tile_shape);
    };
    auto label = [&](const string &s)
    {
        board_text.setString(s);
        FloatRect b = board_text.getLocalBounds();
        board_text.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
        board_text.setPosition(tx + TILE_PX / 2.f, ty + TILE_PX / 2.f);
        window.draw(board_text);
    };
    switch (symbol)
    {
        case '_':
            bg(TILE_HIDDEN, TILE_BORDER);
            break;
        case ' ':
            bg(TILE_OPEN, TILE_BORDER);
            break;
        case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8':
            bg(TILE_OPEN, TILE_BORDER);
            digitColor(symbol);
            board_text.setStyle(Text::Bold);
            label(string(1, symbol));
            board_text.setStyle(Text::Regular);
            break;
        case 'F':
            bg(TILE_FLAG_BG, TILE_BORDER);
            flag_sprite.setPosition(tx, ty);
            window.draw(flag_sprite);
            break;
        case 'x':
            bg(TILE_MINE, Color(180, 50, 50));
            mine_sprite.setPosition(tx, ty);
            window.draw(mine_sprite);
            break;
        default:
            bg(TILE_OPEN, TILE_BORDER);
            break;
    }
}

//*********************************************************************************************************************

void MSSFMLView::drawTopBar(RenderWindow &window) {
    // funkcja ta odpowiada za gorny pasek interfejsu i liczbe min. krok po kroku pobiera szeroksci okna potem tworzy
    // pasek, konfiguruje tekst wypisuje liczbe min
    float win_w = static_cast<float>(window.getSize().x);
    RectangleShape bar({win_w, static_cast<float>(TOPBAR_H)});
    bar.setFillColor(Color::White);
    bar.setOutlineColor(Color(210, 210, 210));
    bar.setOutlineThickness(1.f);
    bar.setPosition(0.f, 0.f);
    window.draw(bar);
    Text txt;
    txt.setFont(main_font);
    txt.setCharacterSize(18);
    txt.setFillColor(TEXT_DARK);
    txt.setString("Miny: " + to_string(game_board.getMineCount()));
    txt.setPosition(12.f, (TOPBAR_H - 22.f) / 2.f);
    window.draw(txt);
}

//*********************************************************************************************************************

void MSSFMLView::drawOverlay(RenderWindow &window) {
    // rysuje nakladke zakrywajaca okno by po zakonczeniu nie zaslonic zupelnie ekranu ale pozostawic lekko przyciemniny
    RectangleShape ov(Vector2f(window.getSize()));
    ov.setFillColor(OVERLAY);
    window.draw(ov);
}

//*********************************************************************************************************************

void MSSFMLView::drawButton(RenderWindow &window, const string &label, float win_width, float btn_y, float btn_h) {
    // funkcja rysuje wysrodkowany poziomo przycisk z tekstym, kolejno liczy pozycke nastepnie rysuje prostokat
    // przycisku a na koniec rysuje tekst
    float bw = static_cast<float>(BTN_W);
    float bx = (win_width - bw) / 2.f;
    RectangleShape btn({bw, btn_h});
    btn.setFillColor(BTN_FILL);
    btn.setOutlineColor(BTN_BORDER);
    btn.setOutlineThickness(1.f);
    btn.setPosition(bx, btn_y);
    window.draw(btn);
    Text txt;
    txt.setFont(main_font);
    txt.setCharacterSize(20);
    txt.setFillColor(TEXT_DARK);
    txt.setString(label);
    FloatRect b = txt.getLocalBounds();
    txt.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
    txt.setPosition(bx + bw / 2.f, btn_y + btn_h / 2.f);
    window.draw(txt);
}

//*********************************************************************************************************************
//                                                    PUBLICZNE
//*********************************************************************************************************************

void MSSFMLView::draw_game(RenderWindow &window) {
    //glowna funkcja renderowania widoku sfml sklada do kupy caly widok gry tworzy tlo, pasek gorny, w podwojnej petli
    // kazdy kafelek
    RectangleShape bg(Vector2f(window.getSize()));
    bg.setFillColor(BG);
    window.draw(bg);
    drawTopBar(window);
    int rows = game_board.getBoardHeight();
    int cols = game_board.getBoardWidth();
    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            drawTile(window,game_board.getFieldInfo(r, c),static_cast<float>(c * TILE_PX), static_cast<float>(TOPBAR_H + r * TILE_PX));
        }
    }
}

//*********************************************************************************************************************

void MSSFMLView::draw_interface(RenderWindow &window) {
    //funckja decydujaca co wyswietlic na pdostwei stanu gry
    switch (game_board.getGameState())
    {
        case RUNNING: draw_game(window); break;
        case FINISHED_WIN: drawWinScreen(window); break;
        case FINISHED_LOSS: drawLoseScreen(window); break;
    }
}

//*********************************************************************************************************************

void MSSFMLView::drawMainMenu(RenderWindow &window) {
    //funkcja rysuje ekran glownego menu, pobiera wymiary nadaje tytul, podtytul i guziki
    float ww = static_cast<float>(window.getSize().x);
    float wh = static_cast<float>(window.getSize().y);
    RectangleShape bg(Vector2f(window.getSize()));
    bg.setFillColor(BG);
    window.draw(bg);
    Text title;
    title.setFont(main_font);
    title.setCharacterSize(52);
    title.setFillColor(TEXT_DARK);
    title.setStyle(Text::Bold);
    drawCentered(window, title, "Saper", ww, wh * 0.22f);
    Text sub;
    sub.setFont(main_font);
    sub.setCharacterSize(18);
    sub.setFillColor(TEXT_MUTED);
    drawCentered(window, sub, "Klasyczna gra w minesweeper", ww, wh * 0.34f);
    drawButton(window, "Nowa gra", ww, wh * 0.48f);
    drawButton(window, "Wyjscie",  ww, wh * 0.62f);
}

//*********************************************************************************************************************

void MSSFMLView::drawModeSelect(RenderWindow &window) {
    //funkcja rysuje ekran wyboru trudnosci tak samo jak poprzednia funkcja najpeirw tlo potem naglowek potem guziki
    // trybow
    float ww = static_cast<float>(window.getSize().x);
    float wh = static_cast<float>(window.getSize().y);
    RectangleShape bg(Vector2f(window.getSize()));
    bg.setFillColor(BG);
    window.draw(bg);
    Text header;
    header.setFont(main_font);
    header.setCharacterSize(36);
    header.setFillColor(TEXT_DARK);
    header.setStyle(Text::Bold);
    drawCentered(window, header, "Wybierz tryb", ww, wh * 0.14f);
    const float sp = 0.14f;
    drawButton(window, "Latwy    (9x9,   10 min)",   ww, wh * 0.27f);
    drawButton(window, "Normalny (16x16, 40 min)",   ww, wh * (0.27f + sp));
    drawButton(window, "Trudny   (16x30, 99 min)",   ww, wh * (0.27f + sp * 2));
    drawButton(window, "Debug    (plansze testowe)",  ww, wh * (0.27f + sp * 3));
    drawButton(window, "Powrot",                      ww, wh * (0.27f + sp * 4));
}

//*********************************************************************************************************************

void MSSFMLView::drawWinScreen(RenderWindow &window) {
    //funkcja rysuje ekran zwyciestwa z overlayem, naglowkiem wygranej, podtytulem oraz przyciskamni akcji
    float ww = static_cast<float>(window.getSize().x);
    float wh = static_cast<float>(window.getSize().y);
    draw_game(window);
    drawOverlay(window);
    Text title;
    title.setFont(main_font);
    title.setCharacterSize(56);
    title.setFillColor(Color(40, 160, 70));
    title.setStyle(Text::Bold);
    drawCentered(window, title, "Wygrana!", ww, wh * 0.28f);
    Text sub;
    sub.setFont(main_font);
    sub.setCharacterSize(20);
    sub.setFillColor(TEXT_MUTED);
    drawCentered(window, sub, "Wszystkie miny odkryte.", ww, wh * 0.42f);
    drawButton(window, "Nowa gra",    ww, wh * 0.54f);
    drawButton(window, "Menu glowne", ww, wh * 0.68f);
}

//*********************************************************************************************************************

void MSSFMLView::drawLoseScreen(RenderWindow &window) {
    //funkcja dziala analoficznie do WinScreen
    float ww = static_cast<float>(window.getSize().x);
    float wh = static_cast<float>(window.getSize().y);
    draw_game(window);
    drawOverlay(window);
    Text title;
    title.setFont(main_font);
    title.setCharacterSize(56);
    title.setFillColor(Color(190, 50, 50));
    title.setStyle(Text::Bold);
    drawCentered(window, title, "Przegrana!", ww, wh * 0.28f);
    Text sub;
    sub.setFont(main_font);
    sub.setCharacterSize(20);
    sub.setFillColor(TEXT_MUTED);
    drawCentered(window, sub, "Trafiono na mine.", ww, wh * 0.42f);
    drawButton(window, "Sprobuj ponownie", ww, wh * 0.54f);
    drawButton(window, "Menu glowne",      ww, wh * 0.68f);
}