//
// Created by Alexander Tian on 5/25/25.
//

#ifndef GAMESOLVER_ENGINE_H
#define GAMESOLVER_ENGINE_H

#include "types.h"
#include "constants.h"
#include "fixed_vector.h"
#include "state.h"

struct SearchInfo {
    Action best_action;
    Score evaluation;
};

namespace Core {
    class Engine {
    public:
        virtual void new_game() = 0;
        virtual SearchInfo search(State& state) = 0;
        virtual ~Engine() = default;
    };
}


#endif //GAMESOLVER_ENGINE_H
