#pragma once

/**
 * @brief l から r-1 までのループ
 * @param i ループ変数
 * @param l 開始値
 * @param r 終了値（この値は含まない）
 */
#define rep(i, l, r) for (int i = (int)(l); i < (int)(r); ++i)

/**
 * @brief 0 から n-1 までのループ
 * @param i ループ変数
 * @param n 終了値（この値は含まない）
 */
#define rp(i, n) for (int i = 0; i < (int)(n); ++i)

/**
 * @brief r-1 から l までの逆ループ
 * @param i ループ変数
 * @param l 終了値（この値は含む）
 * @param r 開始値（この値は含まない）
 */
#define rrep(i, l, r) for (int i = (int)(r) - 1; i >= (int)(l); --i)

/**
 * @brief コンテナの全要素を指定する (begin, end)
 * @param a コンテナ
 */
#define all(a) (a).begin(), (a).end()

/**
 * @brief コンテナの全要素を逆順で指定する (rbegin, rend)
 * @param a コンテナ
 */
#define rall(a) (a).rbegin(), (a).rend()
#ifdef LOCAL
/** @brief デバッグ用の長い区切り線を出力する (LOCAL環境のみ) */
#define BAR std::cerr << "----------------------------------------------\n"
/** @brief デバッグ用の短い区切り線を出力する (LOCAL環境のみ) */
#define S_BAR std::cerr << "------------------\n"
#else
#define BAR   void(0)
#define S_BAR void(0)
#endif
