//
// Created by Pranshu Teotia on 12/6/2020.
//
#include<vector>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include "../obj/Clause.h"

#ifndef PROJECT_SAT_DPLLSOLVER_H
#define PROJECT_SAT_DPLLSOLVER_H


class DPLLSolver {
private:

    static size_t literal_to_index(int literal) {
        if (literal > 0) { return (literal << 1); }
        else { return ((-literal) << 1) + 1; }
    }

    std::vector<std::unordered_set<size_t>> _watch_list;
    std::vector<size_t>::iterator _pq_start, _pq_end;
    size_t _num_variables{};
    std::vector<std::pair<int, size_t>> _literals;
    std::vector<bool> _assignments;
    void unit_propagation(DPLLSolver &f);
    int pick_literal();
    int _clauses_removed{};
    void apply_literal(DPLLSolver &f, const int &literal);
    int DPLL(DPLLSolver &f);

public:
    DPLLSolver(const std::vector<std::vector<int>>& clauses, size_t num_variables);
    DPLLSolver(const DPLLSolver &o);
    std::vector<bool> get_assignments();
    std::vector<Clause> _clause_objects;
    std::vector<size_t> _pq;
    int solve();

};


#endif //PROJECT_SAT_DPLLSOLVER_H
