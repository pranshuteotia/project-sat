//
// Created by prans on 12/18/2020.
//

#ifndef PROJECT_SAT_CLAUSE_H
#define PROJECT_SAT_CLAUSE_H

#include <unordered_set>
#include <vector>

struct Clause {
    std::unordered_set<int> _literals;
    size_t _id;
    int size() const { return _literals.size(); }
};

#endif //PROJECT_SAT_CLAUSE_H
