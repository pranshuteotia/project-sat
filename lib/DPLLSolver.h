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
    struct SizeComp {
    public:
        const DPLLSolver& _solver;
        SizeComp(const DPLLSolver& solver) : _solver(solver) {  }
        bool operator()(const size_t& lhs, const size_t& rhs) const {
            return _solver._clause_objects[lhs]._literals.size() > _solver._clause_objects[rhs]._literals.size();
        }
    } ;

    size_t literal_to_index(int literal) const {
        if (literal > 0) { return (literal << 1); }
        else { return ((-literal) << 1) + 1; }
    }

    std::vector<std::unordered_set<size_t>> _watch_list;
    std::vector<size_t>::iterator _pq_start, _pq_end;
    size_t _num_variables;
    std::vector<int> _literals;
    const SizeComp _size_comp;
    std::vector<bool> _assignments;
    std::stack<std::vector<std::pair<int, int>>> _modifications;
    void pure_literal_elimination();
    void unit_propagation(int literal);
    int pick_literal();
    int _clauses_removed;

public:
    DPLLSolver(const std::vector<std::vector<int>>& clauses, size_t num_variables);
    DPLLSolver(const DPLLSolver &o, SizeComp sizeComp);
    std::vector<bool> get_assignments();
    std::vector<Clause> _clause_objects;
    std::vector<size_t> _pq;
    int solve();

};


#endif //PROJECT_SAT_DPLLSOLVER_H
