//
// Created by prans on 3/15/2021.
//

#include "Heuristic.h"

size_t Heuristic::literal_to_index(int literal) const {
    if (literal > 0) { return (literal << 1); }
    else { return ((-literal) << 1) + 1; }
}