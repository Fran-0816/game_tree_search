#!/bin/sh

# コマンドライン引数を配列で受け取る
args=("$@")

# コマンドライン引数が指定されていなければエラー
if [ -z "$args" ]; then
  echo "Error: No arguments provided."
  exit 1
fi

# コンパイラとコンパイラオプション
compiler="g++"
options="-std=c++20 -O3"

# ライブラリファイル
play="games/play.cpp"
othello="games/othello.cpp"
tic_tac_toe="games/tic_tac_toe.cpp"
fifteen_puzzle="games/fifteen_puzzle.cpp"
time_keeper="utils/time_keeper.cpp"

# args に "all" が含まれるならすべてコンパイルする
if [[ "${args[*]}" == *"all"* ]]; then
    args=("01" "02" "03" "04" "05" "06" "07" "08" "09" "10" "11" "12")
fi

# 実行ファイルを生成するディレクトリ
build_dir="build"

# ./build に実行ファイルを吐くため, ./build が存在しなければ作成
if [ ! -d $build_dir ]; then
  mkdir $build_dir
fi

# コンパイル
for arg in "${args[@]}"; do
    case $arg in
        01) $compiler $options -o $build_dir/mini_max $play $othello 01.mini_max.cpp ;;
        02) $compiler $options -o $build_dir/alpha_beta $play $othello 02.alpha_beta.cpp ;;
        03) $compiler $options -o $build_dir/iterative_deeping $play $othello $time_keeper 03.iterative_deeping.cpp ;;
        04) $compiler $options -o $build_dir/evaluate_function $play $othello $time_keeper 04.evaluate_function.cpp ;;
        05) $compiler $options -o $build_dir/primitive_montecalro $play $othello 05.primitive_montecalro.cpp ;;
        06) $compiler $options -o $build_dir/uct $play $othello 06.uct.cpp ;;
        07) $compiler $options -o $build_dir/puct $play $othello 07.puct.cpp ;;
        08) $compiler $options -o $build_dir/dfs $play $tic_tac_toe 08.dfs.cpp ;;
        09) $compiler $options -o $build_dir/and_or $play $tic_tac_toe 09.and_or.cpp ;;
        10) $compiler $options -o $build_dir/transposition_table $play $tic_tac_toe 10.transposition_table.cpp ;;
        11) $compiler $options -o $build_dir/a_star $play $fifteen_puzzle 11.a_star.cpp ;;
        12) $compiler $options -o $build_dir/ida_star $play $fifteen_puzzle 12.ida_star.cpp ;;
        *) echo "Invalid argument: $arg" ;;
    esac
done