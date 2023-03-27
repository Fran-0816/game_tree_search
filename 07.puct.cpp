/*
PUCT
*/

#include <array>
#include <cmath>

#include "games/play.hpp"
#include "games/othello.hpp"

using othello::State;
using othello::Action;
using othello::random_action;

int playout(State& state) {
    switch (state.get_winning_status()) {
    case WinningStatus::WIN:
        return 1;
    case WinningStatus::LOSE:
    case WinningStatus::DRAW:
        return 0;
    default:
        state.step(random_action(state));
        return 1 - playout(state);
    }
}

class Node {
public:
    std::vector<Node> child_nodes;
    int count;

    explicit Node(const State& state) : count(0), state_(state), win_(0) {}

    void expand() {
        auto legal_actions = state_.legal_actions();
        child_nodes.clear();
        for (const auto action : legal_actions) {
            child_nodes.emplace_back(state_);
            child_nodes.back().state_.step(action);
        }
    }

    int evaluate() {
        int value = 0;
        if (state_.is_done()) {
            switch (state_.get_winning_status()) {
            case WinningStatus::WIN:
                value = 1;
                break;
            default:
                break;
            }
        } else if (child_nodes.empty()) {
            State state_copy = state_;
            value = playout(state_copy);

            if (count == EXPAND_THRESHOLD) {
                expand();
            }
        } else {
            value = 1 - next_child_node().evaluate();
        }
        win_ += value;
        ++count;
        return value;
    }

    Node& next_child_node() {
        for (auto& child_node : child_nodes) {
            if (child_node.count == 0) {
                return child_node;
            }
        }
        int t = 0;
        for (const auto& child_node : child_nodes) {
            t += child_node.count;
        }
        static constexpr int INF = 1000000001;
        double best_value = -INF;
        int best_action_idx = -1;
        int action_size = child_nodes.size();

        for (int action_idx = 0; action_idx < action_size; ++action_idx) {
            const auto& child_node = child_nodes[action_idx];
            static constexpr double C = 1.;
            double ucb1_value = 1 - static_cast<double>(child_node.win_) / child_node.count + C * std::sqrt(std::log(t) / child_node.count);
            if (ucb1_value > best_value) {
                best_action_idx = action_idx;
                best_value = ucb1_value;
            }
        }
        return child_nodes[best_action_idx];
    }

private:
    static constexpr int EXPAND_THRESHOLD = 10;

    State state_;
    int win_;
};

Action puct_action(const State& state, int playout_number) {
    auto legal_actions = state.legal_actions();
    int action_size = legal_actions.size();
    if (!action_size) {
        return othello::NO_POS;
    }
    Node root_node(state);
    root_node.expand();
    for (int t = 0; t < playout_number; ++t) {
        root_node.evaluate();
    }
    int best_action_idx = -1;
    int best_action_count = -1;
    for (int action_idx = 0; action_idx < action_size; ++action_idx) {
        if (int n = root_node.child_nodes[action_idx].count; n > best_action_count) {
            best_action_idx = action_idx;
            best_action_count = n;
        }
    }
    return legal_actions[best_action_idx];
}

int main() {
    std::array<play::Player<State, Action>, 2> players = {
        // [](const State& state) { return puct_action(state, 1000); },
        [](const State& state) { return puct_action(state, 500); },
        [](const State& state) { return random_action(state); },
    };
    play::test_ai(players, 100);
    return 0;
}