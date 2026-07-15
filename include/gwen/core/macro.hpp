#pragma once

#define rep(i, l, r)  for (int i = (int)(l); i < (int)(r); ++i)
#define rp(i, n)      for (int i = 0; i < (int)(n); ++i)
#define rrep(i, l, r) for (int i = (int)(r) - 1; i >= (int)(l); --i)
#define all(a)        (a).begin(), (a).end()
#define rall(a)       (a).rbegin(), (a).rend()

#ifdef LOCAL
#define BAR   std::cerr << "----------------------------------------------\n"
#define S_BAR std::cerr << "------------------\n"
#else
#define BAR   void(0)
#define S_BAR void(0)
#endif
