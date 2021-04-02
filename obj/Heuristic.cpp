//
// Created by prans on 3/15/2021.
//

#include "Heuristic.h"

void Heuristic::init(std::vector<std::unordered_set<size_t>> *_watch_list) {
    this->_literal_frequency = _watch_list;
}

size_t Heuristic::literal_to_index(int literal) const {
    if (literal > 0) { return (literal << 1); }
    else { return ((-literal) << 1) + 1; }
}