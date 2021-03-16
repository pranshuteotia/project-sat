//
// Created by prans on 3/15/2021.
//

#include "Heuristic.h"

void Heuristic::init() {
    size_t num_literals = 0;
    std::vector<std::unordered_set<int>> clauses;
    this->_literal_frequency = std::vector<size_t>((num_literals*2)+1, 0);
    this->_literal_frequency[0] = -1;

    for (const std::unordered_set<int> &clause : clauses) {
        for (const int &literal : clause) {
            ++this->_literal_frequency[literal_to_index(literal)];
        }
    }
}

size_t Heuristic::literal_to_index(int literal) const {
    if (literal > 0) { return (literal << 1); }
    else { return ((-literal) << 1) + 1; }
}

int Heuristic::pick_literal() {
    auto e = std::max_element(this->_literal_frequency.begin(), this->_literal_frequency.end());
    int idx = e - this->_literal_frequency.begin();
    int literal = idx/2;

    if (idx & 1) {
        return -literal;

    } else {
        return literal;
    }
}