//
// Created by Pranshu Teotia on 12/6/2020.
//

#include "DPLLSolver.h"

DPLLSolver::DPLLSolver(const std::vector<std::vector<int>>& clauses, size_t num_variables) : _clause_objects(std::vector<Clause>(clauses.size())), _watch_list(std::vector<std::unordered_set<size_t>>((num_variables << 1) + 2)), _pq(std::vector<size_t>(clauses.size())), _size_comp({*this}) {
    this->_num_variables = num_variables;

    std::vector<int> _literals(this->_num_variables+1, 0);

    size_t clause_id = 0;
    for(const std::vector<int>& clause : clauses) {

        this->_clause_objects[clause_id]._literals = std::unordered_set<int>(clause.begin(),clause.end());
        this->_clause_objects[clause_id]._id = clause_id;
        for(const int& literal : clause) {
            this->_watch_list[literal_to_index(literal)].insert(clause_id);
        }
        this->_pq[clause_id] = clause_id;
        ++clause_id;
    }
    this->_pq_start = _pq.begin();
    this->_pq_end = _pq.end();
    std::make_heap(this->_pq_start,this->_pq_end,_size_comp);
}

DPLLSolver::DPLLSolver(const DPLLSolver &o, SizeComp sizeComp) : _size_comp(sizeComp) {
    this->_watch_list = o._watch_list;
    this->_clause_objects = o._clause_objects;
    this->_literals = o._literals;
    this->_num_variables = o._num_variables;
    this->_pq = o._pq;
    this->_pq_start = o._pq_start;
    this->_pq_end = o._pq_end;
}

void DPLLSolver::pure_literal_elimination() {
    for(size_t i=1; i<_literals.size(); ++i) {
       if(_literals[i] != 0) {
           // Pure literal
           int clause_id = this->literal_to_index(_literals[i]);
           this->_watch_list[clause_id].clear();
       }
    }
}

void DPLLSolver::unit_propagation(int literal) {

    for(const Clause &c : this->_clause_objects) {
        if(c.size() != 1 && c._literals.find(literal) != c._literals.end()) {
            // remove clause
            auto it = std::find(this->_clause_objects.begin(), this->_clause_objects.end(), c);
            this->_clause_objects.erase(it);

        } else if (c.size() != 1 && c._literals.find(-literal) != c._literals.end()) {
            // remove literal.
            size_t idx = literal_to_index(-literal);
            this->_watch_list[idx].erase(c._id); // The literal is no longer in this clause.
        }
    }
}

int DPLLSolver::solve() {
    if(this->_clause_objects.size() == 0) {
        return 1; // No clauses = SAT
    }

    for(const Clause &c : this->_clause_objects) {
        if(c.size() == 0) {
            return  0; // Empty clause = UNSAT
        }
    }

    int literal = *this->_clause_objects.front()._literals.begin();

    unit_propagation(literal);
    int result = solve();

    return result;
}
