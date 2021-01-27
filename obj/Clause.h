//
// Created by prans on 12/18/2020.
//

#ifndef PROJECT_SAT_CLAUSE_H
#define PROJECT_SAT_CLAUSE_H

#include <unordered_set>
#include <vector>
#include <stack>

struct Clause {
    std::unordered_set<int> _literals;
    std::stack<std::unordered_set<int>> _undoStack;
    size_t _id;

    void removeLiteral(int literal) {
        if (_literals.count(literal) > 0) {
            _undoStack.emplace(_literals);
            _literals.erase(literal);
        }
    }

    void clearClause() {
        if (!_literals.empty()) {
            _undoStack.emplace(_literals);
            _literals.clear();
        }
    }

    int size() const { return _literals.size(); }

    bool operator==(const Clause& c) const {
        return c._id == this->_id;
    }
};

#endif //PROJECT_SAT_CLAUSE_H
