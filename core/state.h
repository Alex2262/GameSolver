//
// Created by Alexander Tian on 5/24/25.
//

#ifndef GAMESOLVER_STATE_H
#define GAMESOLVER_STATE_H

#include <vector>
#include <iostream>
#include "types.h"
#include "fixed_vector.h"
#include "constants.h"


namespace Core {
    class State {
    public:
        virtual void reset() = 0;
        virtual void print() = 0;
        virtual void apply_action(Action action) = 0;
        virtual void undo_action(Action action) = 0;
        virtual void get_legal_actions(FixedVector<ScoredAction, MAX_MOVES>& actions) = 0;

        virtual Action parse_action(std::string& str) = 0;
        virtual std::string get_action_string(Action action) = 0;

        [[nodiscard]] virtual Result get_result() = 0;
        [[nodiscard]] virtual int get_current_player() = 0;

        virtual ~State() = default;
    };

}

#endif //GAMESOLVER_STATE_H
