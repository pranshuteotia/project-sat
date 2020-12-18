//
// Created by prans on 12/6/2020.
//
#include<vector>
#include <unordered_map>
#include <queue>
#include "../obj/Clause.h"

#ifndef PROJECT_SAT_DPLLSOLVER_H
#define PROJECT_SAT_DPLLSOLVER_H


class DPLLSolver {
private:
    class Size_comp {
    public:
        bool operator() (const std::vector<int>& lhs, const std::vector<int>& rhs) {
            return rhs.size() < lhs.size();
        }
    };

    std::vector<std::vector<int>> clauses;
    size_t num_variables;
    std::unordered_map<int, std::vector<std::vector<int>>> watch_list;

public:
    DPLLSolver(std::vector<std::vector<int>> _clauses, int _num_variables);
    std::priority_queue<std::vector<int>, std::vector<std::vector<int>>, Size_comp> pq;
};


#endif //PROJECT_SAT_DPLLSOLVER_H
