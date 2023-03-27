#include "fifteen_puzzle.hpp"

#include <iomanip>
#include <random>

namespace fifteen_puzzle {

namespace zobrist_hashing {

std::array<HashMap, 16> make_hash_map() {
    static std::mt19937 engine{std::random_device()()};
    std::array<HashMap, 16> hash_map;
    for (int cell = 0; cell < 16; ++cell) {
        for (int number = 1; number <= 15; ++number) {
            hash_map[number][cell] = engine();
        }
    }
    return hash_map;
}

} // namespace zobrist_hashing

std::array<zobrist_hashing::HashMap, 16> FifteenPuzzleState::hash_maps = zobrist_hashing::make_hash_map();

// コマ配置の初期化
// 終端状態から 100 回程度ランダムにスライドさせることで, 解くことのできる初期配置を生成する
FifteenPuzzleState::FifteenPuzzleState() {
    for (int number = 0; number < 16; ++number) {
        positions_[number] = terminal_positions[number];
        hash_value ^= hash_maps[number][positions_[number]];
    }

    static std::mt19937 engine{std::random_device()()};
    int T = 100 + engine() % 50;
    for (int t = 0; t < T; ++t) {
        auto legal_actions = FifteenPuzzleState::legal_actions();
        auto action = legal_actions[engine() % legal_actions.size()];
        FifteenPuzzleState::step(action);
    }

    turn = 0;
    g_cost = 0;
    h_cost = compute_h_cost();
}

// action は 0: 左, 1: 右, 2: 上, 3: 下
void FifteenPuzzleState::step(const int action) {
    // action インデックスに対する移動量
    static int move_amount[4] = {-1, 1, -4, 4};

    int moved_zero = positions_[0] + move_amount[action];
    // 空白を動かした先にあるコマの番号を探索
    for (int number = 1; number <= 15; ++number) {
        if (positions_[number] == moved_zero) {
            hash_value ^= hash_maps[number][positions_[number]];
            positions_[number] = positions_[0];
            hash_value ^= hash_maps[number][positions_[number]];
            break;
        }
    }
    positions_[0] = moved_zero;

    ++turn;
    ++g_cost;
    h_cost = compute_h_cost();
}

std::vector<int> FifteenPuzzleState::legal_actions() const {
    std::vector<int> actions;
    const auto [zero_h, zero_w] = get_coordinate(positions_[0]);
    if (zero_w != 0) actions.emplace_back(0);
    if (zero_w != 3) actions.emplace_back(1);
    if (zero_h != 0) actions.emplace_back(2);
    if (zero_h != 3) actions.emplace_back(3);
    return actions;
}

int FifteenPuzzleState::compute_h_cost() {
    int sum_manhattan_distance = 0;
    for (int number = 1; number <= 15; ++number) {
        const auto [terminal_h, terminal_w] = get_coordinate(terminal_positions[number]);
        const auto [current_h, current_w] = get_coordinate(positions_[number]);
        sum_manhattan_distance += abs(terminal_h - current_h) + abs(terminal_w - current_w);
    }
    return sum_manhattan_distance;
}

// ゲーム状況の出力
std::ostream& operator<<(std::ostream& os, const FifteenPuzzleState& state) {
    os << "Turn\t" << state.turn << "\tg cost\t" << state.g_cost << "\th cost\t" << state.h_cost << "\tf cost\t" << state.get_f_cost() << '\n';
    // numbers はインデックスがセル番号, 値がコマ番号
    int numbers[16];
    for (int number = 0; number < 16; ++number) {
        numbers[state.positions_[number]] = number;
    }
    for (int h = 0; h < 4; ++h) {
        if (h) {
            os << "-- + -- + -- + --" << '\n';
        }
        for (int w = 0; w < 4; ++w) {
            if (w) {
                os << " | ";
            }
            os << std::setw(2) << numbers[h * 4 + w];
        }
        os << '\n';
    }
    return os;
}

Action random_action(const State& state) {
    static std::mt19937 engine{std::random_device()()};
    const auto legal_actions = state.legal_actions();
    return legal_actions[engine() % legal_actions.size()];
}

} // namespace fifteen_puzzle