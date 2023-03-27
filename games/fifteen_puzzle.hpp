/*
15 パズルの実装
終端状態のコマ番号
  1 |  2 |  3 |  4
 -- + -- + -- + --
  5 |  6 |  7 |  8
 -- + -- + -- + --
  9 | 10 | 11 | 12
 -- + -- + -- + --
 13 | 14 | 15 |  0
0 が空白
セル番号は 左上から右へ 0, 1, ..., 15 とする
h コストは, 各コマの終端までのマンハッタン距離の合計
*/

#pragma once

#include <array>
#include <memory>
#include <ostream>
#include <utility>
#include <vector>

#include "play.hpp"

namespace fifteen_puzzle {

namespace zobrist_hashing {

using HashValue = uint64_t;
using HashMap = HashValue[16];

std::array<HashMap, 16> make_hash_map();

} // namespace zobrist_hashing

// インデックスがコマ番号, 値がセル番号
constexpr int terminal_positions[16] = {15, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};

// cell 番号から座標 (行, 列) を取得
inline std::pair<int, int> get_coordinate(int cell) {
    int h = cell >> 2;  // cell / 4
    int w = cell & 3;   // cell % 4
    return {h, w};
}

class FifteenPuzzleState {
public:
    unsigned int turn;
    int g_cost;
    int h_cost;

    // 経路復元用に親節点へのポインタを記録
    std::shared_ptr<FifteenPuzzleState> pre_state_ptr = nullptr;

    zobrist_hashing::HashValue hash_value;

    FifteenPuzzleState();

    void step(const int action);

    bool is_done() const;

    std::vector<int> legal_actions() const;

    int compute_h_cost();

    int get_f_cost() const;

    friend std::ostream& operator<<(std::ostream& os, const FifteenPuzzleState& state);

private:
    static std::array<zobrist_hashing::HashMap, 16> hash_maps;

    // インデックスがコマ番号, 値がセル番号
    int positions_[16];
};

// h コストが 0 なら終端
inline bool FifteenPuzzleState::is_done() const {
    return h_cost == 0;
}

inline int FifteenPuzzleState::get_f_cost() const {
    return g_cost + h_cost;
}

// f コストの比較
// 12.ida_star では自動でメモリを解放するように std::shared_ptr を使用する
static inline auto compare_f_cost = [](const std::shared_ptr<FifteenPuzzleState> state1, const std::shared_ptr<FifteenPuzzleState> state2) -> bool {
    return state1->get_f_cost() > state2->get_f_cost();
};

using State = FifteenPuzzleState;
using StatePtr = std::shared_ptr<FifteenPuzzleState>;
using Action = int;

Action random_action(const State& state);

} // namespace fifteen_puzzle