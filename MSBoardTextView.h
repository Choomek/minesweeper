//
// Created by c on 3/25/26.
//

#ifndef SAPER_MSBOARDTEXTVIEW_H
#define SAPER_MSBOARDTEXTVIEW_H

#include "MinesweeperBoard.h"

class MSBoardTextView {
    const MinesweeperBoard& board;
public:
    explicit MSBoardTextView(const MinesweeperBoard& b);
    void display() const;
};

#endif SAPER_MSBOARDTEXTVIEW_H
