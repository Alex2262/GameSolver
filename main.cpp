

#include <iostream>
#include "core/interface.h"

int main() {

    Interface interface(EngineType::MCTS, GameType::Gomoku);
    interface.game_loop();

    return 0;
}
