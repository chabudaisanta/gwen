#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <iostream>
#include <vector>
#include "gwen/ds/transformable_matrix.hpp"

int main() {
    int a, b;
    std::cin >> a >> b;
    
    // Dummy usage of TransformableMatrix just to ensure compilation during verify phase
    std::vector<std::vector<gwen::i32>> grid = {{a}, {b}};
    gwen::TransformableMatrix<gwen::i32> mat(grid);
    mat.swap_rows(0, 1);
    mat.transpose();
    mat.rotate(1);

    std::cout << a + b << std::endl;
    return 0;
}
