//
// Created by Pranshu Teotia on 12/6/2020.
//

#include "DPLLSolver.h"

DPLLSolver::DPLLSolver(const std::vector<std::vector<int>>& clauses, size_t num_variables) : _clause_objects(std::vector<Clause>(clauses.size(), Clause())), _watch_list(std::vector<std::unordered_set<size_t>>((num_variables << 1) + 2)), _pq(std::vector<size_t>(clauses.size())) {
    this->_num_variables = num_variables;
    this->_assignments = std::vector<bool>(this->_num_variables+1, false);
    this->_clauses_removed = 0;

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
}

DPLLSolver::DPLLSolver(DPLLSolver const &o) {
    this->_num_variables = o._num_variables;
    this->_assignments = o._assignments;
    this->_clauses_removed = o._clauses_removed;
    this->_literals = o._literals;
    this->_clause_objects = o._clause_objects;
    this->_pq = o._pq;
    this->_pq_start = o._pq_start;
    this->_pq_end = o._pq_end;
    this->_watch_list = o._watch_list;
}

void DPLLSolver::unit_propagation(DPLLSolver &f) {
    bool unit_clause_found;
    do {
        unit_clause_found = false;
        for(const auto &clause : f._clause_objects) {
            if(clause._isUnit) {
                unit_clause_found = true;
                int literal = *clause._literals.begin();
                f._assignments[std::abs(literal)] = literal > 0;
                f.apply_literal(f, literal);
            }
        }

    } while(unit_clause_found);

}

int DPLLSolver::solve() {
    DPLLSolver original = *this;
    return this->DPLL(original);
}

int DPLLSolver::DPLL(DPLLSolver &f) {
    if(f._clause_objects.size() == f._clauses_removed) {
        return 1; // No clauses = Satisfiable
    }
    
    for(const Clause &c : f._clause_objects) {
        if(c._isEmpty) {
            return  0; // Empty clause = Unsatisfiable
        }
    }

    DPLLSolver new_f = f;

    int literal = new_f.pick_literal();
    if(literal == 0) {
        return 0;
    }

    new_f._assignments[std::abs(literal)] = literal > 0;
    unit_propagation(new_f);
    apply_literal(new_f, literal);

    int result = DPLL(new_f);

    // If that branch was unsatisfiable then flip the literal assignment and try to solve.
    if(result == 0) {
        // Undo all changes.
        DPLLSolver new_f2= f;
        new_f2._assignments[std::abs(literal)] = literal <= 0;


        unit_propagation(new_f2);
        apply_literal(new_f2, -literal);
        result = DPLL(new_f2);

        if(result == 1) {
            return 1;
        }
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

void DPLLSolver::apply_literal(DPLLSolver &f, const int &literal) {
    auto clause_ids = f._watch_list[literal_to_index(literal)];

    for(auto id : clause_ids) {
        ++f._clauses_removed;

        for(int l : f._clause_objects[id]._literals) {
            f._literals[std::abs(l)].first--;
            f._watch_list[literal_to_index(l)].erase(id);
        }
        f._clause_objects[id].clearClause();
    }

    auto other_clause_ids = f._watch_list[literal_to_index(-literal)];
    for(auto id : other_clause_ids) {

        f._literals[std::abs(-literal)].first--;
        f._clause_objects[id].removeLiteral(-literal);
    }
    f._watch_list[literal_to_index(-literal)].clear();
}
