/*
ミニマックス探索
*/

#include <array>

#include "games/play.hpp"
#include "games/othello.hpp"

using State = othello::State;
using Action = othello::Action;
using othello::random_action;

using othello::score::ScoreType;
using othello::score::INF;

ScoreType mini_max_score(const State& state, const int depth) {
    if (state.is_done() || depth == 0) {
        return state.get_score();
    }
    auto legal_actions = state.legal_actions();
    if (legal_actions.empty()) {
        legal_actions.emplace_back(othello::NO_POS);
    }
    ScoreType best_score = -INF;
    for (const auto action : legal_actions) {
        State next_state = state;
        next_state.step(action);
        ScoreType score = -mini_max_score(next_state, depth - 1);
        if (score > best_score) {
            best_score = score;
        }
    }
    return best_score;
}

Action mini_max_action(const State& state, const int depth) {
    Action best_action = othello::NO_POS;
    ScoreType best_score = -INF;
    auto legal_actions = state.legal_actions();
    for (const auto action : legal_actions) {
        State next_state = state;
        next_state.step(action);
        ScoreType score = -mini_max_score(next_state, depth);
        if (score > best_score) {
            best_action = action;
            best_score = score;
        }
    }
    return best_action;
}

int main() {
    std::array<play::Player<State, Action>, 2> players = {
        [](const State& state) { return mini_max_action(state, 3); },
        [](const State& state) { return random_action(state); },
    };
    play::test_ai(players, 100);
    return 0;
}