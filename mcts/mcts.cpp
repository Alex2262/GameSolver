//
// Created by Alexander Tian on 6/6/25.
//

#include "mcts.h"


namespace MCTS {

    constexpr Score SCALE = 400;
    constexpr double EXPLORATION_CONSTANT = 1.41;
    constexpr double FPU = 0.5;

    bool Engine::has_children(NodeIndex node_index) {
        return tree[node_index].children_end - tree[node_index].children_start > 0;
    }

    void Engine::print_info() {

        std::string pv_line{};

        NodeIndex original_root_node_index = root_node_index;

        std::vector<Action> applied_actions{};

        while(true) {
            NodeIndex best_node_index = get_best_node();

            pv_line += state->get_action_string(tree[best_node_index].last_action) + " ";

            root_node_index = best_node_index;

            if (state->get_result() != Result::NONE) break;
            if (!has_children(root_node_index)) break;

            state->apply_action(tree[best_node_index].last_action);
            applied_actions.push_back(tree[root_node_index].last_action);

            ply++;
        }

        for (int i = static_cast<int>(applied_actions.size()) - 1; i >= 0; i--) {
            ply--;
            state->undo_action(applied_actions[i]);
        }

        root_node_index = original_root_node_index;

        NodeIndex best_node_index = get_best_node();

        auto eval = static_cast<double>(tree[best_node_index].d_score) /
                    static_cast<double>(tree[best_node_index].visits);

        auto score = static_cast<int>(-std::log(1 / std::clamp<double>(eval, -1.0, 1.0) - 1) * SCALE);

        auto time = std::chrono::high_resolution_clock::now();
        uint64_t current_time = std::chrono::duration_cast<std::chrono::milliseconds>
                (std::chrono::time_point_cast<std::chrono::milliseconds>(time).time_since_epoch()).count();

        auto elapsed_time = current_time - start_time;
        auto nps = static_cast<uint64_t>(static_cast<double>(nodes) /
                                         (static_cast<double>(elapsed_time) /
                                          1000.0));

        std::cout << "info nodes " << nodes
                  << " depth " << std::lround(average_depth + 0.5)
                  << " seldepth " << seldepth
                  << " score cp " << score
                  << " time " << elapsed_time
                  << " nps " << nps
                  << " pv " << pv_line
                  << std::endl;
    }

    void Engine::descend_to_root(NodeIndex node_index) {
        while (node_index != root_node_index && ply > 0) {
            ply--;
            state->undo_action(tree[node_index].last_action);
            node_index = tree[node_index].parent;
        }
    }

    double Engine::get_policy(NodeIndex node_index) {
        //TODO: Implement policy
        return 1.0;
    }

    void Engine::set_children_policies(NodeIndex node_index) {
        Node& node = tree[node_index];
        uint32_t n_children = node.children_end - node.children_start;

        double policy_sum = 0;
        for (int i = 0; i < n_children; i++) {
            NodeIndex child_node_index = tree[node_index].children_start + i;
            Node& child_node = tree[child_node_index];

            child_node.policy = get_policy(child_node_index);
            policy_sum += child_node.policy;
        }

        for (int i = 0; i < n_children; i++) {
            NodeIndex child_node_index = tree[node_index].children_start + i;
            Node &child_node = tree[child_node_index];

            child_node.policy /= policy_sum;
        }
    }

    NodeIndex Engine::select_best_child(NodeIndex node_index) {
        Node& node = tree[node_index];

        uint32_t n_children = node.children_end - node.children_start;

        NodeIndex best_node_index = 0;
        double best_puct = -SCORE_INF;

        for (int i = 0; i < n_children; i++) {
            NodeIndex child_node_index = tree[node_index].children_start + i;
            Node& child_node = tree[child_node_index];

            double exploration_score = EXPLORATION_CONSTANT * std::sqrt(static_cast<double>(node.visits));
            double prior_score = child_node.policy * (exploration_score / (static_cast<double>(1 + child_node.visits)));
            double value_score = child_node.visits == 0 ? 0 :
                                 static_cast<double>(child_node.d_score) /
                                 static_cast<double>(child_node.visits);

            // std::cout << child_node.win_count << " " << child_node.visits << std::endl;

            double puct = child_node.visits == 0 ?
                          exploration_score * child_node.policy + FPU :  // FPU
                          prior_score + value_score;

            if (puct > best_puct) {
                best_puct = puct;
                best_node_index = child_node_index;
            }
        }

        return best_node_index;
    }

    NodeIndex Engine::selection() {
        NodeIndex leaf_node_index = root_node_index;

        Ply depth = 0;
        while (true) {
            uint32_t n_children = tree[leaf_node_index].children_end - tree[leaf_node_index].children_start;

            if (n_children == 0) break;

            leaf_node_index = select_best_child(leaf_node_index);

            state->apply_action(tree[leaf_node_index].last_action);

            depth++;
            ply++;

        }

        average_depth = (average_depth * (static_cast<double>(nodes) - 1.0) + static_cast<double>(depth)) / static_cast<double>(nodes);

        seldepth = std::max<Ply>(seldepth, depth);
        return leaf_node_index;
    }

    void Engine::expansion(NodeIndex node_index) {
        tree[node_index].children_start = tree.size();
        state->get_legal_actions(scored_actions[ply]);

        for (ScoredAction scored_action : scored_actions[ply]) {
            tree.emplace_back(node_index, scored_action.action);
        }

        tree[node_index].children_end = tree.size();
    }

    double Engine::evaluate_mcts() {
        //TODO: Implement NN evaluation
        return 0.5;
    }

    void Engine::back_propagation(NodeIndex node_index, double evaluation, Result result) {
        double p_result = result == Result::DRAW ? 0.5 :
                          result == Result::WIN  ? 0 :
                          result == Result::LOSS ? 1 :
                          evaluation;

        NodeIndex current_node_index = node_index;
        while (true) {
            Node& current_node = tree[current_node_index];

            current_node.visits++;
            current_node.d_score += p_result;

            if (current_node.parent == current_node_index) break;  // Hit root

            current_node_index = current_node.parent;
            p_result = 1.0 - p_result;
        }
    }

    NodeIndex Engine::get_best_node() {
        int best = -1;
        NodeIndex best_index = 0;
        for (int i = 0; i < tree[root_node_index].children_end - tree[root_node_index].children_start; i++) {
            NodeIndex child_node_index = tree[root_node_index].children_start + i;
            Node& child_node = tree[child_node_index];
            if (child_node.visits >= best) {
                best = child_node.visits;
                best_index = child_node_index;
            }
        }

        return best_index;
    }

    void Engine::mcts() {
        seldepth = 0;
        nodes = 0;
        average_depth = 0.0;
        seldepth = 0;
        stopped = false;

        //TODO: flatten_tree();

        tree.clear();
        root_node_index = 0;
        tree.emplace_back(root_node_index, NO_ACTION); // NO_ACTION here is fine, since we won't return the root anyway

        auto time = std::chrono::high_resolution_clock::now();
        start_time = std::chrono::duration_cast<std::chrono::milliseconds>
                (std::chrono::time_point_cast<std::chrono::milliseconds>(time).time_since_epoch()).count();

        uint32_t selected_node_index = root_node_index;

        for (nodes = 1; nodes <= MAX_NODES; nodes++) {

            descend_to_root(selected_node_index);
            selected_node_index = selection();

            Result node_result = state->get_result();

            if (node_result == Result::NONE && tree[selected_node_index].visits > 0) {
                expansion(selected_node_index);
                set_children_policies(selected_node_index);

                int random_index = rand() % (tree[selected_node_index].children_end - tree[selected_node_index].children_start);
                selected_node_index = tree[selected_node_index].children_start + random_index;

                state->apply_action(tree[selected_node_index].last_action);
                ply++;

                node_result = state->get_result();
            }

            double evaluation = node_result == Result::NONE ? evaluate_mcts() : 0;
            back_propagation(selected_node_index, evaluation, node_result);

            if ((nodes % 2048) == 0) {
                time = std::chrono::high_resolution_clock::now();
                uint64_t current_time = std::chrono::duration_cast<std::chrono::milliseconds>
                        (std::chrono::time_point_cast<std::chrono::milliseconds>(time).time_since_epoch()).count();

                if (current_time - start_time >= max_time) {
                    break;
                }
            }

            if ((nodes % 100000) == 0) {
                descend_to_root(selected_node_index);
                print_info();
            }

            if (std::lround(average_depth + 0.5) >= max_depth) break;
            if (stopped) break;
        }

        descend_to_root(selected_node_index);
        print_info();
        std::cout << "bestmove " << state->get_action_string(tree[get_best_node()].last_action) << std::endl;
    }

    void Engine::new_game() {
        tree.clear();
        root_node_index = 0;
        tree.emplace_back(root_node_index, NO_ACTION);
    }

    SearchInfo Engine::search(Core::State& p_state) {
        state = &p_state;

        mcts();

        NodeIndex best_node_index = get_best_node();
        auto eval = static_cast<double>(tree[best_node_index].d_score) /
                    static_cast<double>(tree[best_node_index].visits);

        Score score = static_cast<Score>(-std::log(1 / std::clamp<double>(eval, -1.0, 1.0) - 1) * SCALE);

        SearchInfo search_info{};
        search_info.evaluation = score;
        search_info.best_action = tree[best_node_index].last_action;

        //TODO: Set best action
        return search_info;
    }
}