#include "gwen/dump.hpp"
#include "gwen/misc/xorshift.hpp"
#include "gwen/my_template.hpp"
using namespace std;
int main() {
    int n;
    cin >> n;
    cout << n << EL;
    vector<int> a(n, 1);
    for (int i = 0; i < n; ++i) {
        a[i] = gwen::rand32(1, i + 1);
    }
    cout << gwen::to_str(all(a), " ") << EL;
}