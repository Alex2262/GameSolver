//
// Created by Alexander Tian on 5/25/25.
//

#include "utilities.h"
#include "interface.h"
#include "state.h"
#include "config.h"


Interface::Interface() {
    Config config = Config(CONFIG_PATH);

    engine = std::move(config.get_engine());
    game = std::move(config.get_game());
}

void Interface::game_loop() {

    std::cout << "GETTING STATE" << std::endl;
    auto state = game->new_initial_state();

    std::cout << "GOT STATE" << std::endl;
    state->print();

    engine->new_game();



    while (getline(std::cin, msg)) {
        tokens = split(msg, ' ');

        if (tokens[0] == "action") {
            std::string move_str = concat(tokens, ' ', 1, tokens.size());
            Action action = state->parse_action(move_str);

            if (action == NO_ACTION) {
                std::cout << "ILLEGAL ACTION ERROR";
                continue;
            }

            std::cout << "Applied Action: " << move_str << std::endl;
            state->apply_action(action);
            state->print();
        }

        if (tokens[0] == "go") {
            SearchInfo search_info = engine->search(*state);
            std::cout << "Best Action: " << state->get_action_string(search_info.best_action) << std::endl;
            state->apply_action(search_info.best_action);
            state->print();
        }

        Result result = state->get_result();
        if (result != Result::NONE) {
            std::cout << "RESULT " << static_cast<Score>(result);
            break;
        }
    }
}

