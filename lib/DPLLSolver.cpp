//
// Created by prans on 12/6/2020.
//

#include "DPLLSolver.h"

DPLLSolver::DPLLSolver(std::vector<std::vector<int>> _clauses, int _num_variables) {
    this->num_variables = _num_variables;
    this->clauses = _clauses;

    for(std::vector<int> clause : this->clauses) {
        for(int literal : clause) {
            this->watch_list[literal].push_back(clause);
        }
        this->pq.push(clause);
    }
}
