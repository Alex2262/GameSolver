//
// Created by Alexander Tian on 5/27/25.
//


#include "random_engine.h"


namespace Random {
    Engine::Engine() {
        gen.seed(rd());
    }

    SearchInfo Engine::search(Core::State& state) {
        FixedVector<ScoredAction, MAX_MOVES> scored_actions;
        state.get_legal_actions(scored_actions);

        std::uniform_int_distribution<size_t> dist(0, scored_actions.size());

        size_t r = dist(gen);

        SearchInfo search_info{};
        search_info.best_action = scored_actions[r].action;
        search_info.evaluation = 0;

        return search_info;
    }

}
