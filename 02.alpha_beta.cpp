/*
アルファベータ探索
*/

#include <array>

#include "games/play.hpp"
#include "games/othello.hpp"

using State = othello::State;
using Action = othello::Action;
using othello::random_action;

using othello::score::ScoreType;
using othello::score::INF;

ScoreType alpha_beta_score(const State& state, ScoreType alpha, const ScoreType beta, const int depth) {
    if (state.is_done() || depth == 0) {
        return state.get_score();
    }
    auto legal_actions = state.legal_actions();
    if (legal_actions.empty()) {
        legal_actions.emplace_back(othello::NO_POS);
    }
    for (const auto action : legal_actions) {
        State next_state = state;
        next_state.step(action);
        ScoreType score = -alpha_beta_score(next_state, -beta, -alpha, depth - 1);
        if (score > alpha) {
            alpha = score;
        }
        if (alpha >= beta) {
            return alpha;
        }
    }
    return alpha;
}

Action alpha_beta_action(const State& state, const int depth) {
    Action best_action = othello::NO_POS;
    ScoreType alpha = -INF;
    ScoreType beta = INF;
    auto legal_actions = state.legal_actions();
    for (const auto action : legal_actions) {
        State next_state = state;
        next_state.step(action);
        ScoreType score = -alpha_beta_score(next_state, -beta, -alpha, depth);
        if (score > alpha) {
            best_action = action;
            alpha = score;
        }
    }
    return best_action;
}

int main() {
    std::array<play::Player<State, Action>, 2> players = {
        [](const State& state) { return alpha_beta_action(state, 5); },
        [](const State& state) { return random_action(state); },
    };
    play::test_ai(players, 100);
    return 0;
}