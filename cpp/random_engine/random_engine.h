//
// Created by Alexander Tian on 5/27/25.
//

#ifndef GAMESOLVER_RANDOM_ENGINE_H
#define GAMESOLVER_RANDOM_ENGINE_H

#include <random>
#include "../core/state.h"
#include "../core/engine.h"

namespace Random {
    class Engine : public Core::Engine {
    private:
        std::random_device rd;
        std::mt19937 gen;
    public:
        Engine();
        void new_game() override {};
        SearchInfo search(Core::State& state) override;
    };
}


#endif //GAMESOLVER_RANDOM_ENGINE_H
