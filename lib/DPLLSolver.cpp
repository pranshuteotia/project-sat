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
    h.init(&(this->_watch_list));
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

//    int literal = h.pick_literal(this->_num_variables, this->_clause_objects);
    int literal = h.pick_literal();
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

// No Stack implementation

int DPLLSolver::solve_no_stack() {
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

    std::vector<Clause> clause_objects_copy = this->_clause_objects;
    std::vector<std::unordered_set<size_t>> watch_list_copy = this->_watch_list;
    size_t clauses_removed_copy = this->_clauses_removed;
    std::vector<std::pair<int, size_t>> literals_copy = this->_literals;

    this->_assignments[std::abs(literal)] = literal > 0;
    unit_propagation_no_stack();
    apply_literal_no_stack(literal);

    int result = solve_no_stack();

    // If that branch was unsatisfiable then flip the literal assignment and try to solve.
    if(result == 0) {
        // Undo all changes.
        this->_clause_objects = clause_objects_copy;
        this->_watch_list = watch_list_copy;
        this->_clauses_removed = clauses_removed_copy;
        this->_literals = literals_copy;

        this->_assignments[std::abs(literal)] = literal <= 0;

        unit_propagation_no_stack();
        apply_literal_no_stack(-literal);
        result = solve_no_stack();

        if(result == 1) {
            return 1;
        }

        this->_clause_objects = clause_objects_copy;
        this->_watch_list = watch_list_copy;
        this->_clauses_removed = clauses_removed_copy;
        this->_literals = literals_copy;
    }

    return result;
}

void DPLLSolver::unit_propagation_no_stack() {
    bool unit_clause_found;
    do {
        unit_clause_found = false;
        for(const auto &clause : this->_clause_objects) {
            if(clause._isUnit) {
                unit_clause_found = true;
                int literal = *clause._literals.begin();
                this->_assignments[std::abs(literal)] = literal > 0;
                this->apply_literal_no_stack(literal);
            }
        }

    } while(unit_clause_found);
}

void DPLLSolver::apply_literal_no_stack(const int &literal) {
    auto clause_ids = this->_watch_list[literal_to_index(literal)];

    for(auto id : clause_ids) {
        ++this->_clauses_removed;

        for(int l : this->_clause_objects[id]._literals) {
            this->_literals[std::abs(l)].first--;
            this->_watch_list[literal_to_index(l)].erase(id);
        }
        this->_clause_objects[id].clearClause();
    }

    auto other_clause_ids = this->_watch_list[literal_to_index(-literal)];
    for(auto id : other_clause_ids) {

        this->_literals[std::abs(-literal)].first--;
        this->_clause_objects[id].removeLiteral(-literal);
    }
    this->_watch_list[literal_to_index(-literal)].clear();
}

// Copy Constructor impl.

DPLLSolver::DPLLSolver(DPLLSolver const &o, const SizeComp sizeComp) : _size_comp(sizeComp) {
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

void DPLLSolver::unit_propagation_copy_constructor(DPLLSolver &f) {
    bool unit_clause_found;
    do {
        unit_clause_found = false;
        for(const auto &clause : f._clause_objects) {
            if(clause._isUnit) {
                unit_clause_found = true;
                int literal = *clause._literals.begin();
                f._assignments[std::abs(literal)] = literal > 0;
                f.apply_literal_copy_constructor(f, literal);
            }
        }

    } while(unit_clause_found);
}

int DPLLSolver::solve_copy_constructor() {
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
    unit_propagation_copy_constructor(new_f);
    apply_literal_copy_constructor(new_f, literal);

    int result = DPLL(new_f);

    // If that branch was unsatisfiable then flip the literal assignment and try to solve.
    if(result == 0) {
        // Undo all changes.
        DPLLSolver new_f2= f;
        new_f2._assignments[std::abs(literal)] = literal <= 0;


        unit_propagation_copy_constructor(new_f2);
        apply_literal_copy_constructor(new_f2, -literal);
        result = DPLL(new_f2);

        if(result == 1) {
            return 1;
        }
    }

    return result;
}

void DPLLSolver::apply_literal_copy_constructor(DPLLSolver &f, const int &literal) {
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

/*
 * Each literal, l, has a score, s(l), and an occurrence
    count, r(l). When a decision is necessary, a free literal with the highest score is set true.
    Initially, for every literal, l, s(l) = r(l) = 0. Before search begins, s(l) is incremented for
    each occurrence of a literal, l, in the input formula. When a clause, c, is learned during


    search, r(l) is incremented for each literal l E c. Every 255 decisions, the scores are updated:
    for each literal, l, s(l) becomes r(l) + s(l)/2, and r(l) becomes zero.
 */