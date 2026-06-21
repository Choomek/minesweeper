//
// Created by c on 3/25/26.
//

#include "MSTextController.h"

MSTextController::MSTextController(MinesweeperBoard& b, MSBoardTextView& v): board(b), view(v) {}

void MSTextController::play(){
    // Wyswietla panel gracza a nastepnie wyswietla cala plansze. Nastepnie wyswietla okienko w ktorym gracz moze wybrac co chce
    // zrobic. Program zaczytuje od gracza znak sprawdza czy sie to udalo jezeli tak zmienia go na wielka litere i wchodzi do
    // instrukcji switch. Jezeli gracz zdecydowal sie na wyjscie to porgram opuszcza petle i switch. Jezeli gracz chce dodac
    // flage lub sprawdzic pole to program zaczytuje wiersza i sprawdz czy to sie udalo jezeli tak wykonuje operacja ktora
    // gracz chcial wykonac w przypadku bledow wykonuje sie instrukcja po deafult kolejny fragment kodu na bierzaca monitoruje czy gra
    // jest juz zakonczona
    std::cout << "\n*****   PANEL GRACZA   *****\n-      ODKRYCIE POLA       -\n\n        R[row][col]\n\n-     OFLAGOWANIE POLA     -\n\n        F[row][col]\n\n-     OPUSZCZENIE GRY      -\n\n            Q\n\n*****************************\n\n\n";
    view.display();
    while (board.getGameState() == RUNNING)
    {
        std::cout << "\nWYBÓR GRACZA:   ";
        char zank;
        if (!(std::cin >> zank))
        {
            std::cout << "blad zaczytu\n";
            break;
        }
        zank = std::toupper(zank);
        switch (zank)
        {
            case 'Q':
                std::cout << "koniec gry\n";
                goto end_loop;  // break wyskoczyłby tylko ze switch, nie z while
            case 'R':
            case 'F':
            {
                int row, col;
                if (!(std::cin >> row >> col))
                {
                    std::cout << "blad zaczytu\n";
                    break;
                }
                if (zank == 'R')
                {
                    board.revealField(row, col);
                }
                if (zank == 'F')
                {
                    board.toggleFlag(row, col);
                }
                view.display();
                break;
            }
            default:
                std::cout << "nieznane polecenie\n";
                break;
        }
    }
    end_loop:;
    GameState stan = board.getGameState();
    if (stan == FINISHED_WIN)
    {
        std::cout << "wygrales\n";
    }
    else if (stan == FINISHED_LOSS)
    {
        std::cout << "przegrales\n";
    }

}