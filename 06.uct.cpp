/*
UCT
*/

#include <array>
#include <cmath>
#include <vector>

#include "games/play.hpp"
#include "games/othello.hpp"

using State = othello::State;
using Action = othello::Action;
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

Action uct_action(const State& state, int playout_number) {
    auto legal_actions = state.legal_actions();
    int action_size = legal_actions.size();
    if (!action_size) {
        return othello::NO_POS;
    }
    std::vector<int> values(action_size);
    std::vector<int> counts(action_size);
    for (int t = 0; t < playout_number; ++t) {
        static constexpr int INF = 1000000001;
        double best_value = -INF;
        int best_action_idx = -1;
        for (int action_idx = 0; action_idx < action_size; ++action_idx) {
            if (counts[action_idx] == 0) {
                best_action_idx = action_idx;
                break;
            }
            static constexpr double C = 1.;
            double ucb1_value = static_cast<double>(values[action_idx]) / counts[action_idx] + C * std::sqrt(std::log(t) / counts[action_idx]);
            if (ucb1_value > best_value) {
                best_action_idx = action_idx;
                best_value = ucb1_value;
            }
        }

        State next_state = state;
        next_state.step(legal_actions[best_action_idx]);
        values[best_action_idx] += 1 - playout(next_state);
        ++counts[best_action_idx];
    }

    int best_action_idx = -1;
    int best_action_count = -1;
    for (int action_idx = 0; action_idx < action_size; ++action_idx) {
        if (int n = counts[action_idx]; n > best_action_count) {
            best_action_idx = action_idx;
            best_action_count = n;
        }
    }
    return legal_actions[best_action_idx];
}

int main() {
    std::array<play::Player<State, Action>, 2> players = {
        [](const State& state) { return uct_action(state, 500); },
        [](const State& state) { return random_action(state); },
    };
    play::test_ai(players, 100);
    return 0;
}