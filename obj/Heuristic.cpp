//
// Created by prans on 3/15/2021.
//

#include "Heuristic.h"

size_t Heuristic::literal_to_index(int literal) const {
    if (literal > 0) { return (literal << 1); }
    else { return ((-literal) << 1) + 1; }
}

int Heuristic::index_to_literal(size_t index) const {

    if (index < 2) {
        return 0;

    } else if (index & 1) {
        return -(index/2);

    } else {
        return index/2;
    }
}