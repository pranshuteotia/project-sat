//
// Created by Pranshu on 3/16/2021.
//
#ifndef PROJECT_SAT_DLIS_H
#define PROJECT_SAT_DLIS_H

#include "Heuristic.h"

class DLIS : public Heuristic {

public:
    void init(std::vector<std::unordered_set<size_t>> *_watch_list) {
        this->_literal_frequency = _watch_list;
    }

    int pick_literal() override {
        int literal = -1;
        size_t freq = 0;
        for(size_t i=0; i<this->_literal_frequency->size(); ++i) {
            size_t clause_size = (*(this->_literal_frequency))[i].size();
            if (clause_size > freq) {
                freq = clause_size;
                literal = i;
            }
        }

        return (literal & 1)? -literal/2 : literal/2;
    }

    int pick_literal(const std::vector<std::unordered_set<size_t>> &literal_frequency) override {
        int literal = -1;
        size_t freq = 0;
        for(size_t i=0; i<literal_frequency.size(); ++i) {
            size_t clause_size = literal_frequency[i].size();
            if (clause_size > freq) {
                freq = clause_size;
                literal = i;
            }
        }

        return (literal & 1)? -literal/2 : literal/2;
    }
};

#endif //PROJECT_SAT_DLIS_H
