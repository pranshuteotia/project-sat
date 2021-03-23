//
// Created by prans on 3/15/2021.
//
#ifndef PROJECT_SAT_HEURISTIC_H
#define PROJECT_SAT_HEURISTIC_H

#include <vector>
#include <unordered_set>
#include <algorithm>
#include "Clause.h"

class Heuristic {
private:
    std::vector<int> _literal_frequency;
    size_t literal_to_index(int literal) const;

public:
    Heuristic() = default;

    void init(const size_t &num_variables, const std::vector<Clause> &clauses);

    int pick_literal(const size_t &num_variables, const std::vector<Clause> &clauses);
};


#endif //PROJECT_SAT_HEURISTIC_H
