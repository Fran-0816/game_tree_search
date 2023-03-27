/*
オセロの実装
ビットボードは https://zenn.dev/kinakomochi/articles/othello-bitboard を参照
*/

#pragma once

#include <bit>
#include <vector>
#include <ostream>
#include <unordered_map>

#include "play.hpp"

namespace othello {

using BitBoard = uint64_t;
static constexpr BitBoard NO_POS = 0;

// position に置かれたコマの数を数える
static int count_pieces(const BitBoard position) {
    return std::popcount(position);
}

namespace score {

using ScoreType = int;
static constexpr ScoreType INF = 1000000001;

// 以下のサイトに示されているマスの評価値
//   http://hitsujiai.blog48.fc2.com/blog-entry-26.html
// 04.evaluate_function で, 評価関数を変えたときの性能変化を見るために使用
static const std::unordered_map<BitBoard, int> points_per_cells = {
    {0x8100000000000081, 120},
    {0x2400810000810024,  20},
    {0x0000240000240000,  15},
    {0x1800008181000018,   5},
    {0x0000183C3C180000,   3},
    {0x003C424242423C00,  -5},
    {0x4281000000008142, -20},
    {0x0042000000004200, -40},
};

ScoreType compute_score(const BitBoard position);

} // namespace score

class OthelloState {
public:
    unsigned int turn = 0;
    bool is_black_turn = 1;

    void step(const BitBoard action);

    bool is_done() const;

    std::vector<BitBoard> legal_actions() const;

    score::ScoreType get_score() const;

    score::ScoreType get_score2() const;

    WinningStatus get_winning_status() const;

    friend std::ostream& operator<<(std::ostream& os, const OthelloState& state);

private:
    // 初期配置
    BitBoard player_position_ = 0x0000000810000000;
    BitBoard opponent_position_ = 0x0000001008000000;

    void put_piece(const BitBoard piece);

    BitBoard opponent_pieces_adjacent_left(BitBoard position, const BitBoard opponent_position) const;
    BitBoard opponent_pieces_adjacent_right(BitBoard position, const BitBoard opponent_position) const;
    BitBoard opponent_pieces_adjacent_up(BitBoard position, const BitBoard opponent_position) const;
    BitBoard opponent_pieces_adjacent_down(BitBoard position, const BitBoard opponent_position) const;
    BitBoard opponent_pieces_adjacent_upper_left(BitBoard position, const BitBoard opponent_position) const;
    BitBoard opponent_pieces_adjacent_upper_right(BitBoard position, const BitBoard opponent_position) const;
    BitBoard opponent_pieces_adjacent_lower_left(BitBoard position, const BitBoard opponent_position) const;
    BitBoard opponent_pieces_adjacent_lower_right(BitBoard position, const BitBoard opponent_position) const;

    BitBoard cells_can_put(const BitBoard position, const BitBoard opponent_position) const;
};

// 両プレイヤーが行動できなくなったら (置けるマスが無くなったら) 終端
inline bool OthelloState::is_done() const {
    return !cells_can_put(player_position_, opponent_position_) && !cells_can_put(opponent_position_, player_position_);
}

// 弱い評価関数
// コマ数の差を評価値とする
inline score::ScoreType OthelloState::get_score() const {
    return count_pieces(player_position_) - count_pieces(opponent_position_);
}

// 強い評価関数
// コマが置かれているマスの評価値合計の差を評価値とする
inline score::ScoreType OthelloState::get_score2() const {
    return score::compute_score(player_position_) - score::compute_score(opponent_position_);
}

using State = othello::OthelloState;
using Action = othello::BitBoard;

Action random_action(const State& state);

} // namespace othello