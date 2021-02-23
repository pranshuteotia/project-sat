//
// Created by Pranshu Teotia on 12/6/2020.
//

#include "DPLLSolver.h"

DPLLSolver::DPLLSolver(const std::vector<std::vector<int>>& clauses, size_t num_variables) : _clause_objects(std::vector<Clause>(clauses.size(), Clause())), _watch_list(std::vector<std::unordered_set<size_t>>((num_variables << 1) + 2)), _pq(std::vector<size_t>(clauses.size())), _size_comp({*this}) {
    this->_num_variables = num_variables;
    this->_assignments = std::vector<bool>(this->_num_variables+1, false);
    this->_clauses_removed = 0;
    this->_deleted_clauses = nullptr;
    this->_deleted_literals = nullptr;

    this->_literals.emplace_back(-2, 0);
    for(size_t i=1; i<=num_variables; ++i) this->_literals.emplace_back(0, i);

    size_t clause_id = 0;
    for(const std::vector<int>& clause : clauses) {

        this->_clause_objects[clause_id]._literals = std::unordered_set<int>(clause.begin(),clause.end());
        this->_clause_objects[clause_id]._id = clause_id;
        for(const int& literal : clause) {
            this->_watch_list[literal_to_index(literal)].insert(clause_id);
            this->_literals[std::abs(literal)].first++;
        }
        this->_pq[clause_id] = clause_id;
        ++clause_id;
    }
    this->_pq_start = _pq.begin();
    this->_pq_end = _pq.end();
    std::make_heap(this->_pq_start,this->_pq_end,_size_comp);
}

void DPLLSolver::unit_propagation() {
    bool unit_clause_found;
    do {
        unit_clause_found = false;
        for(const auto &clause : this->_clause_objects) {
            if(clause._isUnit) {
                unit_clause_found = true;
                int literal = *clause._literals.begin();
                this->_assignments[std::abs(literal)] = literal > 0;
                this->apply_literal(literal);
            }
        }

    } while(unit_clause_found);

}

int DPLLSolver::solve() {
    if(this->_clause_objects.size() == this->_clauses_removed) {
        return 1; // No clauses = Satisfiable
    }
    
    for(const Clause &c : this->_clause_objects) {
        if(c._isEmpty) {
            return  0; // Empty clause = Unsatisfiable
        }
    }

    int literal = this->pick_literal();
    if(literal == 0) {
        return 0;
    }

    this->_modifications.push(std::vector<std::pair<int, int>>());
    this->_deleted_clauses = &this->_modifications.top();
    this->_modifications.push(std::vector<std::pair<int, int>>());
    this->_deleted_literals = &this->_modifications.top();

    this->_assignments[std::abs(literal)] = literal > 0;
    unit_propagation();
    apply_literal(literal);

    int result = solve();

    // If that branch was unsatisfiable then flip the literal assignment and try to solve.
    if(result == 0) {
        // Undo all changes.
        this->undo_state();

        this->_assignments[std::abs(literal)] = !this->_assignments[std::abs(literal)];

        this->_modifications.push(std::vector<std::pair<int, int>>());
        this->_deleted_clauses = &this->_modifications.top();
        this->_modifications.push(std::vector<std::pair<int, int>>());
        this->_deleted_literals = &this->_modifications.top();

        unit_propagation();
        apply_literal(-literal);
        result = solve();

        if(result == 1) {
            return 1;
        }

        this->undo_state();
    }

    return result;
}

std::vector<bool> DPLLSolver::get_assignments() {
    return this->_assignments;
}

int DPLLSolver::pick_literal() {
    auto freq_literal_pair = std::max_element(this->_literals.begin(), this->_literals.end());
//    freq_literal_pair->first = -1;
    int var = freq_literal_pair->second;

    if(freq_literal_pair->first == 0) {
        return 0;
    }

    return (this->_watch_list[this->literal_to_index(var)].size() > this->_watch_list[this->literal_to_index(-var)].size())? var : -var;
}

void DPLLSolver::undo_state() {
    // reinsert deleted literals.
    auto s = this->_modifications.top();
    for(auto pair : s) {
        int literal = pair.first;
        int clause_id = pair.second;
        this->_watch_list[literal_to_index(literal)].insert(clause_id);
        this->_literals[std::abs(literal)].first++;

        this->_clause_objects[clause_id].undoLastModification();
    }
    _modifications.pop();

    // reinsert deleted clauses.
    s = this->_modifications.top();
    int current_clause = -1;

    for(auto pair : s) {
        int literal = pair.first;
        int clause_id = pair.second;
        this->_watch_list[literal_to_index(literal)].insert(clause_id);
        this->_literals[std::abs(-literal)].first++;

        if(current_clause != clause_id) {
            current_clause = clause_id;
            this->_clause_objects[clause_id].undoLastModification();
            --this->_clauses_removed;
        }
    }
    this->_modifications.pop();
}

void DPLLSolver::apply_literal(const int &literal) {
    auto clause_ids = this->_watch_list[literal_to_index(literal)];

    for(auto id : clause_ids) {
        ++this->_clauses_removed;

        for(int l : this->_clause_objects[id]._literals) {
            this->_deleted_clauses->emplace_back(l, id);
            this->_literals[std::abs(l)].first--;
            this->_watch_list[literal_to_index(l)].erase(id);
        }
        this->_clause_objects[id].clearClause();
    }

    auto other_clause_ids = this->_watch_list[literal_to_index(-literal)];
    for(auto id : other_clause_ids) {

        this->_deleted_literals->emplace_back(-literal, id);
        this->_literals[std::abs(-literal)].first--;
        this->_clause_objects[id].removeLiteral(-literal);
    }
    this->_watch_list[literal_to_index(-literal)].clear();
}
