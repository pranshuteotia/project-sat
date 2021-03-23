//
// Created by prans on 3/15/2021.
//

#include "Heuristic.h"

void Heuristic::init(const size_t &num_variables, const std::vector<Clause> &clauses) {
    this->_literal_frequency = std::vector<int>((num_variables*2)+1, 0);
    this->_literal_frequency[0] = -1;

    for (const Clause &clause : clauses) {
        for (const int &literal : clause._literals) {
            ++this->_literal_frequency[literal_to_index(literal)];
        }
    }
}

size_t Heuristic::literal_to_index(int literal) const {
    if (literal > 0) { return (literal << 1); }
    else { return ((-literal) << 1) + 1; }
}

int Heuristic::pick_literal(const size_t &num_variables, const std::vector<Clause> &clauses) {
    this->init(num_variables, clauses);

    auto e = std::max_element(this->_literal_frequency.begin(), this->_literal_frequency.end());
    int idx = e - this->_literal_frequency.begin();
    int literal = idx/2;

    if (idx & 1) {
        return -literal;

    } else {
        return literal;
    }
}