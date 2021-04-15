//
// Created by Pranshu Teotia on 4/1/2021.
//

#ifndef PROJECT_SAT_VSIDS_H
#define PROJECT_SAT_VSIDS_H

#include "Heuristic.h"

class VSIDS : public Heuristic {
private:
    std::vector<int> _score;
    std::vector<size_t> _occurrence_count;
    size_t _decisions;

public:
    void init(std::vector<std::unordered_set<size_t>> *watch_list) override {
        this->_literal_frequency = watch_list;
        this->_score = std::vector<int>( (this->_literal_frequency)->size());
        this->_occurrence_count = std::vector<size_t>((this->_literal_frequency)->size(), 0);
        this->_decisions = 0;

        for(size_t i=0; i<this->_score.size(); ++i) {
            this->_score[i] = (*(this->_literal_frequency))[i].size();
        }
    }

    int pick_literal() override {
        if (this->_decisions == 255) {
            this->_decisions = 0;

            for(size_t i=0; i<this->_score.size(); ++i) {

                if (this->_score[i] < 0) {
                    this->_score[i] = - (this->_occurrence_count[i] + std::abs(this->_score[i])/2);

                } else {
                    this->_score[i] = this->_occurrence_count[i] + this->_score[i]/2;
                }
                this->_occurrence_count[i] = 0;
            }
        }

        ++(this->_decisions);

        auto max_score = std::max_element(this->_score.begin(), this->_score.end());
        int idx = max_score - this->_score.begin();

        int literal = (idx & 1) ? -(idx/2) : idx/2;
        this->_score[this->literal_to_index(-literal)] = -(this->_score[this->literal_to_index(-literal)]);
        *max_score = -(*max_score);

        return literal;
    }

    int pick_literal(const std::vector<std::unordered_set<size_t>> &) override {
        return this->pick_literal();
    }

    void increase_occurrence_count(const int &literal) override {
        ++(this->_occurrence_count[this->literal_to_index(literal)]);
    }

    void decrease_occurrence_count(const int &literal) override {
        --(this->_occurrence_count[this->literal_to_index(literal)]);
    }

    void undo_score(const int &literal) override {
        size_t idx = this->literal_to_index(literal);
        size_t idx2 = this->literal_to_index(-literal);

        this->_score[idx] = std::abs(this->_score[idx]);
        this->_score[idx2] = std::abs(this->_score[idx2]);
    }
};
#endif //PROJECT_SAT_VSIDS_H
