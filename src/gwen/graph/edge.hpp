#pragma once

#include "gwen/types.hpp"

namespace gwen {

struct edge {
    i32 u, v;
    edge() : u(-1), v(-1) {}
    edge(i32 u_, i32 v_) : u(u_), v(v_) {}

    template<typename istream>
    void input(istream& is) {
        is >> u >> v;
    }
};

template<typename WeightType>
struct full_edge : edge {
    WeightType w;
    i32 id;
    full_edge() : edge(), w(0), id(-1) {}
    full_edge(i32 u_, i32 v_, WeightType w_, i32 id_) : edge(u_, v_), w(w_), id(id_) {}
};

}