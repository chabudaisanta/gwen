# ルール
- 簡単に宣言できるようにする。 (テンプレート引数はごちゃごちゃしない)
- 直下にS, op, eを定義する。
- 作用素付きであればF, mapping, composition, idも定義する。
- 型はusingもしくはstructで
- 関数はstatic修飾子をつけ、引数にconst参照は使わない。(ACLのため)
- range_reverseもそのうちつけるかも?