# C++ でゲーム木探索
C++ を使用してゲーム木探索アルゴリズムを実装する

本リポジトリの一部のプログラムは「ゲームで学ぶ探索アルゴリズム実践入門〜木探索とメタヒューリスティックス」([書籍](https://www.amazon.co.jp/%E3%82%B2%E3%83%BC%E3%83%A0%E3%81%A7%E5%AD%A6%E3%81%B6%E6%8E%A2%E7%B4%A2%E3%82%A2%E3%83%AB%E3%82%B4%E3%83%AA%E3%82%BA%E3%83%A0%E5%AE%9F%E8%B7%B5%E5%85%A5%E9%96%80%EF%BD%9E%E6%9C%A8%E6%8E%A2%E7%B4%A2%E3%81%A8%E3%83%A1%E3%82%BF%E3%83%92%E3%83%A5%E3%83%BC%E3%83%AA%E3%82%B9%E3%83%86%E3%82%A3%E3%82%AF%E3%82%B9-%E9%9D%92%E6%9C%A8-%E6%A0%84%E5%A4%AA/dp/4297133601), [サンプルコード](https://gihyo.jp/book/2023/978-4-297-13360-3/support)) を参考にしています.

## 使用方法
1. [CMake](https://cmake.org/) を使用する  
    例えば, [Ninja](https://ninja-build.org/) を併用して  
    ```
    cmake -G Ninja -S . -B build
    ninja -C build
    ```
    のようにビルドすると, すべてソースファイル (`01.mini_max.cpp` ~ `12.ida_star.cpp`) に対する実行ファイルが生成されるので  
    ```
    build/mini_max
    ...
    build/ida_star
    ```
    として実行する.

2. ファイルごとにコンパイルする  
    `build.sh` によってソースファイルごとにコンパイルし, 1. と同様に実行します.  
    例えば, `05.primitive_montecalro.cpp` と `07.puct.cpp` を同時にコンパイルするには
    ```
    bash build.sh 05 07
    ```
    とすればよく, すべてのソースファイルをコンパイルするには
    ```
    bash build.sh all
    ```
    とします.  
    シェルスクリプトが実行できない場合は, `build.sh` に書かれたコマンドを手打ちしてください.

## ライブラリ
1. [`games`](https://github.com/Fran-0816/game_tree_search/tree/main/games)
   1. `othello` : オセロ
      - ビットボード
   2. `tic_tac_toe` : 三目並べ
      - ビットボード
      - ゾブリストハッシュ
   3. `fifteen_puzzle` : 15 パズル
      - ゾブリストハッシュ
   4. `play` : ゲームプレイ用
1. [`utils`](https://github.com/Fran-0816/game_tree_search/tree/main/utils)
   1. `time_keeper` : 探索時間管理用のタイマー

ゲーム状況を表すクラスが以下のメソッドを持つことさえ分かっていれば, クラスの実装を知らずに次節のアルゴリズムを理解することができます.
1. `step` : 行動を入力してゲームを 1 手進める.
2. `is_done` : 終端判定.
3. `legal_actions` : 現在のゲーム状況に対する合法手を可変長配列として列挙する.

## アルゴリズム
1. オセロ
   1. [`mini_max`](https://github.com/Fran-0816/game_tree_search/blob/main/01.mini_max.cpp) : ミニマックス探索
   2. [`alpha_beta`](https://github.com/Fran-0816/game_tree_search/blob/main/02.alpha_beta.cpp) : アルファベータ探索
   3. [`iterative_deeping`](https://github.com/Fran-0816/game_tree_search/blob/main/03.iterative_deeping.cpp) : アルファベータ探索に反復深化を適用
   4. [`evaluate_function`](https://github.com/Fran-0816/game_tree_search/blob/main/04.evaluate_function.cpp) : 評価関数の改善
   5. [`primitive_montecalro`](https://github.com/Fran-0816/game_tree_search/blob/main/05.primitive_montecalro.cpp) : 原始モンテカルロ木探索
   6. [`uct`](https://github.com/Fran-0816/game_tree_search/blob/main/06.uct.cpp) : UCT (Upper Confidence Tree)
   7. [`puct`](https://github.com/Fran-0816/game_tree_search/blob/main/07.puct.cpp) : PUCT (Polynomial Upper Confidence Tree)
2. 三目並べ
   1. [`dfs`](https://github.com/Fran-0816/game_tree_search/blob/main/08.dfs.cpp) : すべての節点を訪問, およびトランスポジションテーブルに記録した以前の探索結果を活用
   2. [`and_or`](https://github.com/Fran-0816/game_tree_search/blob/main/09.and_or.cpp) : AND/OR 木探索
   3. [`transposition_table`](https://github.com/Fran-0816/game_tree_search/blob/main/10.transposition_table.cpp) : AND/OR 木探索に, トランスポジションテーブルを適用
3. 15 パズル
   1. [`a_star`](https://github.com/Fran-0816/game_tree_search/blob/main/11.a_star.cpp) : A* 探索
   2. [`ida_star`](https://github.com/Fran-0816/game_tree_search/blob/main/12.ida_star.cpp) : 反復深化 A* 探索
4. And more ?
