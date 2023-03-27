/*
反復深化アルファベータ探索
*/

#include <array>

#include "games/play.hpp"
#include "games/othello.hpp"
#include "utils/time_keeper.hpp"

using State = othello::State;
using Action = othello::Action;
using othello::random_action;

using othello::score::ScoreType;
using othello::score::INF;

ScoreType alphabeta_score(const State& state, ScoreType alpha, const ScoreType beta, const int depth, const TimeKeeper& time_keeper) {
    if (time_keeper.is_time_over()) {
        return 0;
    }
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
        ScoreType score = -alphabeta_score(next_state, -beta, -alpha, depth - 1, time_keeper);
        if (score > alpha) {
            alpha = score;
        }
        if (alpha >= beta) {
            return alpha;
        }
        if (time_keeper.is_time_over()) {
            return 0;
        }
    }
    return alpha;
}

Action alphabeta_action_with_time_threshold(const State& state, const int depth, const TimeKeeper& time_keeper) {
    Action best_action = othello::NO_POS;
    ScoreType alpha = -INF;
    ScoreType beta = INF;
    auto legal_actions = state.legal_actions();
    for (const auto action : legal_actions) {
        State next_state = state;
        next_state.step(action);
        ScoreType score = -alphabeta_score(next_state, -beta, -alpha, depth, time_keeper);
        if (score > alpha) {
            best_action = action;
            alpha = score;
        }
        if (time_keeper.is_time_over()) {
            return othello::NO_POS;
        }
    }
    return best_action;
}

Action iterative_deeping_action(const State& state, const int64_t time_threshold) {
    auto time_keeper = TimeKeeper(time_threshold);
    Action best_action = othello::NO_POS;
    for (int depth = 1; ; ++depth) {
        Action action = alphabeta_action_with_time_threshold(state, depth, time_keeper);

        if (time_keeper.is_time_over()) {
            break;
        } else {
            best_action = action;
        }
    }
    return best_action;
}

int main() {
    std::array<play::Player<State, Action>, 2> players = {
        [](const State& state) { return iterative_deeping_action(state, 10); },
        // [](const State& state) { return iterative_deeping_action(state, 1); },
        [](const State& state) { return random_action(state); },
    };
    play::test_ai(players, 100);
    return 0;
}