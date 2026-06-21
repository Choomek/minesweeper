#include "MSSFMLController.h"

//*********************************************************************************************************************
//                                                    KONSTRUKTORY
//*********************************************************************************************************************

MSSFMLController::MSSFMLController(MinesweeperBoard &board, MSSFMLView &view, int cols, int rows): board(board), view(view), window(VideoMode(cols * MSSFMLView::TILE_PX,rows * MSSFMLView::TILE_PX + MSSFMLView::TOPBAR_H),"Saper"),appState(MAIN_MENU), cols(cols), rows(rows)
{
    window.setFramerateLimit(60);
}

//*********************************************************************************************************************
//                                                       PRYWATNE
//*********************************************************************************************************************

int MSSFMLController::mouseToRow(int mouse_y) const {
    //funkcja kowertuje pozycje myszy na idneks wiersza
    return (mouse_y - MSSFMLView::TOPBAR_H) / MSSFMLView::TILE_PX;
}

//*********************************************************************************************************************

int MSSFMLController::mouseToCol(int mouse_x) const {
    //funkcja konwertuje pozcyje myszy na columne
    return mouse_x / MSSFMLView::TILE_PX;
}

//*********************************************************************************************************************

bool MSSFMLController::clickedButton(int mouse_x, int mouse_y, float btn_y, float btn_h) const {
    //funkcja sprawdza czy klikniecie myszy obejmuje obszar guzika
    float win_w = static_cast<float>(window.getSize().x);
    float bw = static_cast<float>(MSSFMLView::BTN_W);
    float bx = (win_w - bw) / 2.f;
    return mouse_x >= bx && mouse_x <= bx + bw && mouse_y >= btn_y && mouse_y <= btn_y + btn_h;
}

//*********************************************************************************************************************

void MSSFMLController::startGame(int w, int h, GameMode mode) {
    //funkcja inicjalizuje nowa gre. Pokolei zapisuje wymiary planszy, tworzy ja nastepnie dopasowjuje rozmiar okna i
    //aktualizuje widok na koniec zmienia stan aplikacji
    cols  = w;
    rows  = h;
    board = MinesweeperBoard(w, h, mode);
    window.setSize({static_cast<unsigned>(w * MSSFMLView::TILE_PX),static_cast<unsigned>(h * MSSFMLView::TILE_PX + MSSFMLView::TOPBAR_H)});
    window.setView(View(FloatRect(0, 0,static_cast<float>(w * MSSFMLView::TILE_PX),static_cast<float>(h * MSSFMLView::TILE_PX + MSSFMLView::TOPBAR_H))));
    appState = PLAYING;
}

//*********************************************************************************************************************

void MSSFMLController::handleMainMenu(const Event &event) {
    //obsluguje mysz na ekranie glonwego menu, krok po kroku najpierw sprawdza czy kliknieta cos co nie jest LPM,
    //nastpenie pobiera pozycje klikniecia i sprawdza ktory guzik zostal klikniety
    if (event.type != Event::MouseButtonPressed)
    {
        return;
    }
    if (event.mouseButton.button != Mouse::Left)
    {
        return;
    }
    float wh = static_cast<float>(window.getSize().y);
    int mx = event.mouseButton.x;
    int my = event.mouseButton.y;
    if (clickedButton(mx, my, wh * 0.48f))
    {
        appState = MODE_SELECT;
    }
    else if (clickedButton(mx, my, wh * 0.62f))
    {
        window.close();
    }
}

//*********************************************************************************************************************

void MSSFMLController::handleModeSelect(const Event &event) {
    //funkcja obsluguje klikniecie na ekranie wyboru trybow gry dziala analogicznie do poprzedniej
    if (event.type != Event::MouseButtonPressed) return;
    if (event.mouseButton.button != Mouse::Left)  return;
    float wh = static_cast<float>(window.getSize().y);
    int mx = event.mouseButton.x;
    int my = event.mouseButton.y;
    const float sp = 0.14f;
    if (clickedButton(mx, my, wh * 0.27f)) startGame( 9,  9, EASY);
    else if (clickedButton(mx, my, wh * (0.27f + sp))) startGame(16, 16, NORMAL);
    else if (clickedButton(mx, my, wh * (0.27f + sp * 2))) startGame(30, 16, HARD);
    else if (clickedButton(mx, my, wh * (0.27f + sp * 3))) startGame( 7,  5, DEBUG);
    else if (clickedButton(mx, my, wh * (0.27f + sp * 4))) appState = MAIN_MENU;
}

//*********************************************************************************************************************

void MSSFMLController::handlePlaying(const Event &event) {
    //funkcja ktora pozwala na obsluge myszy i klawiatury w trakcie gry funkcja rozgalezia sie zaleznie od stanu gry
    //przed kliknieciem obsluguje ekran kocna gry oraz aktywnej gry pozwala rownierz na podstwaowa oblsuge kalwiatury
    if (event.type == Event::MouseButtonPressed)
    {
        if (board.getGameState() != RUNNING)
        {
            float wh = static_cast<float>(window.getSize().y);
            int   mx = event.mouseButton.x;
            int   my = event.mouseButton.y;
            if      (clickedButton(mx, my, wh * 0.54f)) startGame(cols, rows, EASY);
            else if (clickedButton(mx, my, wh * 0.68f)) appState = MAIN_MENU;
            return;
        }
        int row = mouseToRow(event.mouseButton.y);
        int col = mouseToCol(event.mouseButton.x);
        if (event.mouseButton.button == Mouse::Left)
        {
            board.revealField(row, col);
        }
        else if (event.mouseButton.button == Mouse::Right)
        {
            board.toggleFlag(row, col);
        }
    }
    if (event.type == Event::KeyPressed)
    {
        if (event.key.code == Keyboard::R) startGame(cols, rows, EASY);
        if (event.key.code == Keyboard::M) appState = MAIN_MENU;
    }
}

//*********************************************************************************************************************

void MSSFMLController::handleEvent(const Event &event) {
    //funkcja to taki dyspozytor eventow przydziela do kolejnych funkcji akcje uzytkownika
    if (event.type == Event::Closed)
    {
        window.close(); return;
    }
    if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
    {
        window.close(); return;
    }
    switch (appState)
    {
        case MAIN_MENU: handleMainMenu(event); break;
        case MODE_SELECT: handleModeSelect(event); break;
        case PLAYING: handlePlaying(event); break;
    }
}

//*********************************************************************************************************************

void MSSFMLController::play() {
    //glowna petla gry krok po kroku dopoki okno otwarte szuka eventu nastepnie czysci ekran i rysuje kolejny stan
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            handleEvent(event);
        }
        window.clear(Color(245, 245, 245));
        switch (appState)
        {
            case MAIN_MENU:   view.drawMainMenu(window);   break;
            case MODE_SELECT: view.drawModeSelect(window); break;
            case PLAYING:     view.draw_interface(window); break;
        }
        window.display();
    }
}