//
// Created by prans on 3/15/2021.
//

#include "Heuristic.h"

//void Heuristic::init(const size_t &num_variables, const std::vector<Clause> &clauses) {
void Heuristic::init(std::vector<std::unordered_set<size_t>> *_watch_list) {
    /*this->_literal_frequency = std::vector<int>((num_variables*2)+1, 0);
    this->_literal_frequency[0] = -1;

    for (const Clause &clause : clauses) {
        for (const int &literal : clause._literals) {
            ++this->_literal_frequency[literal_to_index(literal)];
        }
    }*/
    this->_literal_frequency = _watch_list;
}

size_t Heuristic::literal_to_index(int literal) const {
    if (literal > 0) { return (literal << 1); }
    else { return ((-literal) << 1) + 1; }
}