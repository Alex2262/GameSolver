//
// Created by Alexander Tian on 5/24/25.
//

#ifndef GAMESOLVER_GAME_H
#define GAMESOLVER_GAME_H

#include <string>
#include "state.h"
#include "../core/config_helper.h"


namespace Core {
    class Game {
    public:
        [[nodiscard]] virtual std::string name() = 0;
        [[nodiscard]] virtual std::unique_ptr<State> new_initial_state() = 0;
        virtual ~Game() = default;
    };
}


#endif //GAMESOLVER_GAME_H
