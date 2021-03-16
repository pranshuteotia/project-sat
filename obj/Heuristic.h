//
// Created by prans on 3/15/2021.
//
#ifndef PROJECT_SAT_HEURISTIC_H
#define PROJECT_SAT_HEURISTIC_H

#include <vector>
#include <unordered_set>
#include <algorithm>

class Heuristic {
private:
    std::vector<size_t> _literal_frequency;

    size_t literal_to_index(int literal) const;

public:
    Heuristic() = default;

    void init();

    int pick_literal();
};


#endif //PROJECT_SAT_HEURISTIC_H
