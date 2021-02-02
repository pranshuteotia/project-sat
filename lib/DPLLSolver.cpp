//
// Created by Pranshu Teotia on 12/6/2020.
//

#include "DPLLSolver.h"

DPLLSolver::DPLLSolver(const std::vector<std::vector<int>>& clauses, size_t num_variables) : _clause_objects(std::vector<Clause>(clauses.size())), _watch_list(std::vector<std::unordered_set<size_t>>((num_variables << 1) + 2)), _pq(std::vector<size_t>(clauses.size())), _size_comp({*this}) {
    this->_num_variables = num_variables;
    this->_assignments = std::vector<bool>(this->_num_variables+1, false);
    this->_clauses_removed = 0;

    for(size_t i=0; i<num_variables; ++i) this->_literals.push_back(i+1);
    std::random_shuffle(this->_literals.begin(), this->_literals.end());

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

void DPLLSolver::unit_propagation(int literal) {

    auto clause_ids = this->_watch_list[literal_to_index(literal)];
    this->_modifications.push(std::make_pair(literal, clause_ids));

    for(auto id : clause_ids) {
        this->_clause_objects[id].clearClause();
        this->_clauses_removed++;
    }
    this->_watch_list[literal_to_index(literal)].clear();

    auto other_clause_ids = this->_watch_list[literal_to_index(-literal)];
    this->_modifications.push(std::make_pair(-literal, other_clause_ids));
    for(auto id : other_clause_ids) {
        this->_clause_objects[id].removeLiteral(-literal);
    }
    this->_watch_list[literal_to_index(-literal)].clear();
}

int DPLLSolver::solve() {
    if(this->_clause_objects.size() == this->_clauses_removed) {
        return 1; // No clauses = SAT
    }
    
    /*for(const Clause &c : this->_clause_objects) {
        if(c.size() == 0) {
            return  0; // Empty clause = UNSAT
        }
    }*/

    int literal = this->pick_literal();
//    this->_assignments[(int)std::abs(literal)] = true;
    unit_propagation(literal);
    int result = solve();

    // If that branch was unsatisfiable then flip the literal assignment and try to solve.
    if(result == 0) {
        // Undo all changes.

        // reinsert deleted literals.
        int l= this->_modifications.top().first;
        auto clause_ids = this->_modifications.top().second;

        this->_watch_list[literal_to_index(l)] = clause_ids;
        for(auto id : clause_ids)   this->_clause_objects[id].undoLastModification();
        this->_modifications.pop();

        // reinsert deleted clauses.
        l = this->_modifications.top().first;
        clause_ids = this->_modifications.top().second;

        this->_watch_list[literal_to_index(l)] = clause_ids;
        for(auto id : clause_ids) {
            this->_clause_objects[id].undoLastModification();
            this->_clauses_removed--;
        }
        this->_modifications.pop();

//        this->_assignments[std::abs(literal)] = !this->_assignments[std::abs(literal)];
        unit_propagation(-literal);
        result = solve();
    }

    return result;
}

std::vector<bool> DPLLSolver::get_assignments() {
    return this->_assignments;
}

int DPLLSolver::pick_literal() {
    int literal = this->_literals.back();
    this->_literals.pop_back();
    return literal;
}
