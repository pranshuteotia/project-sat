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
protected:
//    std::vector<int> _literal_frequency;
    std::vector<std::unordered_set<size_t>> *_literal_frequency;
    size_t literal_to_index(int literal) const;

public:
//    Heuristic() = default;

//    void init(const size_t &num_variables, const std::vector<Clause> &clauses);
    void init(std::vector<std::unordered_set<size_t>> *_watch_list);
};


#endif //PROJECT_SAT_HEURISTIC_H
