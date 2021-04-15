//
// Created by Pranshu on 3/16/2021.
//
#ifndef PROJECT_SAT_DLIS_H
#define PROJECT_SAT_DLIS_H

#include "Heuristic.h"

class DLIS : public Heuristic {
private:
    static const struct FrequencyComparator {
        bool operator()(const std::unordered_set<size_t> &o1, const std::unordered_set<size_t> &o2) const {
            return o1.size() < o2.size();
        }
    } comp;

public:
    void init(std::vector<std::unordered_set<size_t>> *_watch_list) override {
        this->_literal_frequency = _watch_list;
    }

    int pick_literal() override {
        auto max_frequency = std::max_element(this->_literal_frequency->begin(), this->_literal_frequency->end(), comp);
        int literal = max_frequency - this->_literal_frequency->begin();
        return (literal & 1)? -literal/2 : literal/2;
    }

    int pick_literal(const std::vector<std::unordered_set<size_t>> &literal_frequency) override {
        auto max_frequency = std::max_element(literal_frequency.begin(), literal_frequency.end(), comp);
        int literal = max_frequency - literal_frequency.begin();
        return (literal & 1)? -literal/2 : literal/2;
    }

    void increase_occurrence_count(const int &literal) override {}
    void decrease_occurrence_count(const int &literal) override {}
    void undo_score(const int &literal) override {}
};

#endif //PROJECT_SAT_DLIS_H
