//
// Created by c on 3/25/26.
//

#ifndef SNAJPER_MSTEXTCONTROLLER_H
#define SNAJPER_MSTEXTCONTROLLER_H

#include <iostream>
#include "MinesweeperBoard.h"
#include "MSBoardTextView.h"

class MSTextController {
    MinesweeperBoard& board;
    MSBoardTextView&  view;
public:
    MSTextController(MinesweeperBoard& b, MSBoardTextView& v);
    void play();
};

#endif // SNAJPER_MSTEXTCONTROLLER_H
