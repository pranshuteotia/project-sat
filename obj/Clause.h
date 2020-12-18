//
// Created by prans on 12/18/2020.
//

#ifndef PROJECT_SAT_CLAUSE_H
#define PROJECT_SAT_CLAUSE_H

#include <unordered_set>
#include <vector>

struct Clause {
    std::unordered_set<int> literals;

    Clause() {

    }

    Clause(std::vector<int> _clause) {
        for(int literal : _clause) {
            literals.insert(literal);
        }
    }
};

#endif //PROJECT_SAT_CLAUSE_H
