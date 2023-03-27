/*
二人ゲームプレイ用
テンプレートを使用するためにヘッダに実装を書いている
*/

#pragma once

#include <array>
#include <functional>
#include <iostream>
#include <utility>

// NONE: 非終端状態, WIN: 勝利, LOSE: 敗北, DRAW: 引き分け
enum class WinningStatus {
    NONE, WIN, LOSE, DRAW
};

namespace play {

template <class State, class Action>
using Player = std::function<Action(const State&)>;

// 終端までゲームをプレイして, 両プレイヤーの勝敗を返す.
// black_side: 先手側. 公平性確保のため, プレイごとに入れ替える
// is_print: ゲーム状況出力フラグ
template<class State, class Action>
std::pair<WinningStatus, WinningStatus> play_game(const std::array<Player<State, Action>, 2>& players, const int black_side, const bool is_print) {
    auto state = State();
    int player = black_side;
    while (!state.is_done()) {
        if (is_print) {
            std::cout << state << std::endl;
        }
        state.step(players[player](state));
        player = 1 - player;
    }
    if (is_print) {
        std::cout << state << std::endl;
    }
    switch (state.get_winning_status()) {
    case WinningStatus::WIN:
        if (state.is_black_turn ^ (black_side == 0)) {
            return {WinningStatus::LOSE, WinningStatus::WIN};
        } else {
            return {WinningStatus::WIN, WinningStatus::LOSE};
        }
    case WinningStatus::LOSE:
        if (state.is_black_turn ^ (black_side == 0)) {
            return {WinningStatus::WIN, WinningStatus::LOSE};
        } else {
            return {WinningStatus::LOSE, WinningStatus::WIN};
        }
    default:
        return {WinningStatus::DRAW, WinningStatus::DRAW};
    }
}

// game_number 回ゲームをプレイして, players[0] の players[1] に対する勝率などを出力する.
template <class State, class Action>
void test_ai(const std::array<Player<State, Action>, 2>& players, const int game_number) {
    // [0]: 勝利回数, [1]: 敗北回数, [2]: 引き分け回数
    int results[3] = {};
    for (int t = 0; t < game_number; ++t) {
        const auto [p1_result, p2_result] = play_game(players, t & 1, false);
        switch (p1_result) {
        case WinningStatus::WIN:
            ++results[0];
            break;
        case WinningStatus::LOSE:
            ++results[1];
            break;
        default:
            ++results[2];
            break;
        }
    }
    std::cout << "Win\t" << results[0] << std::endl;
    std::cout << "Lose\t" << results[1] << std::endl;
    std::cout << "Draw\t" << results[2] << std::endl;
    std::cout << "Win rate\t" << static_cast<double>(results[0]) / game_number << std::endl;
}

} // namespace play