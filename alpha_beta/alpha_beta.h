//
// Created by Alexander Tian on 5/25/25.
//

#ifndef GAMESOLVER_ALPHA_BETA_H
#define GAMESOLVER_ALPHA_BETA_H

#include "../core/state.h"
#include "../core/engine.h"

namespace AlphaBeta {

    constexpr Ply MAX_DEPTH = 128;


class Engine : public Core::Engine {
private:

    Ply ply = 0;

    Action pv_table[MAX_DEPTH + 1][MAX_DEPTH + 1]{};
    Ply pv_length[MAX_DEPTH + 1]{};

    Score alpha_beta(Core::State& state, Score alpha, Score beta, Ply depth);

    std::array<FixedVector<ScoredAction, MAX_MOVES>, MAX_DEPTH> scored_actions;

public:
    Engine() = default;
    void new_game() override {};
    SearchInfo search(Core::State& state) override;
};

}


#endif //GAMESOLVER_ALPHA_BETA_H
