//
// Created by Alexander Tian on 5/25/25.
//

#include "alpha_beta.h"

namespace AlphaBeta {
    Score Engine::alpha_beta(Core::State& state, Score alpha, Score beta, Ply depth) {

        pv_length[ply] = ply;

        Result result = state.get_result();
        if (result != Result::NONE) return static_cast<Score>(result);

        if (depth == 0) {
            return 0;
        }

        state.get_legal_actions(scored_actions[ply]);

        std::sort(scored_actions[ply].begin(), scored_actions[ply].begin() + scored_actions[ply].size(), [](ScoredAction left, ScoredAction right) {
            return left.score > right.score;
        });

        Score best_score = -SCORE_INF;
        Action best_action = NO_ACTION;

        for (size_t legal_action = 0; legal_action < scored_actions[ply].size(); legal_action++) {
            Action action = scored_actions[ply][legal_action].action;

            state.apply_action(action);
            ply++;

            Score return_eval = -alpha_beta(state, -beta, -alpha, depth - 1);

            ply--;
            state.undo_action(action);

            if (return_eval > best_score) {
                best_score = return_eval;
                best_action = action;

                pv_table[ply][ply] = best_action;
                for (Ply next_ply = ply + 1; next_ply < pv_length[ply + 1]; next_ply++) {
                    pv_table[ply][next_ply] = pv_table[ply + 1][next_ply];
                }

                pv_length[ply] = pv_length[ply + 1];

                if (return_eval > alpha) {
                    alpha = return_eval;

                    if (alpha >= beta) break;
                }
            }
        }

        return best_score;
    }


    SearchInfo Engine::search(Core::State& state) {
        Score return_eval = alpha_beta(state, -SCORE_INF, SCORE_INF, 8);
        SearchInfo search_info{};
        search_info.evaluation = return_eval;
        search_info.best_action = pv_table[0][0];

        std::cout << "info depth 9 score " << search_info.evaluation << std::endl;

        return search_info;
    }
}



