//
// Created by c on 3/25/26.
//

#include "MSBoardTextView.h"
#include "MinesweeperBoard.h"
#include <iostream>

#include "MSBoardTextView.h"

MSBoardTextView::MSBoardTextView(const MinesweeperBoard& b) : board(b) {}

void MSBoardTextView::display() const{
    // funkcja zaczytuje szerkoksc i wysokosc z glownej klasy nastepnie wyswietla plansze wyswietlajac kolejno: naglowki kolumn, otoczke
    // kazdy kolejny wiersz oraz zamykajac otoczke funkjca na bierzaca sprawdza czy gra sie zakonczyla
    int width = board.getBoardWidth();
    int height = board.getBoardHeight();
    std::cout << "   ";
    for (int col = 0; col < width; col++)
    {
        std::cout << (col % 10);
    }
    std::cout << std::endl;
    for (int row = 0; row < height; row++) {
        if (row % 100 < 10)
        {
            std::cout << " ";
        }
        std::cout << (row % 100) << '|';
        for (int col = 0; col < width; col++)
        {
            std::cout << board.getFieldInfo(row, col);
        }
        std::cout << '|' << std::endl;
    }
    GameState stan = board.getGameState();
    if (stan == FINISHED_LOSS)
    {
        std::cout << "  przegrales" << std::endl;
    }
    else if (stan == FINISHED_WIN)
    {
        std::cout << "  wygrales" << std::endl;
    }
    else
    {
        std::cout << "  liczba pozostalych min " << board.getMineCount() << std::endl;
    }
}