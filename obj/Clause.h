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
    bool _isEmpty;
    bool _isUnit;

    Clause(): _isEmpty(false), _isUnit(false) {}

    void removeLiteral(int literal) {
        if (_literals.count(literal) > 0) {
            _undoStack.emplace(_literals);
            _literals.erase(literal);
        }

        if(_literals.empty()) {
            _isEmpty = true;
            _isUnit = false;

        }
        if (_literals.size() == 1) {
            _isUnit = true;
        }
    }

    void clearClause() {
        if (!_literals.empty()) {
            _undoStack.emplace(_literals);
            _literals.clear();
        }

        _isUnit = false;
    }

    void undoLastModification() {
        _literals = std::move(_undoStack.top());
        _undoStack.pop();

        if(_literals.size() == 1) {
            _isUnit = true;
        }
        if(_literals.size() > 1) {
            _isUnit = false;

        }
        if(!_literals.empty()) {
            _isEmpty = false;
        }
    }

    int size() const { return _literals.size(); }

    bool operator==(const Clause& c) const {
        return c._id == this->_id;
    }
};

#endif //PROJECT_SAT_CLAUSE_H
