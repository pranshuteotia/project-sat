//
// Created by Pranshu Teotia on 4/1/2021.
//

#ifndef PROJECT_SAT_VSIDS_H
#define PROJECT_SAT_VSIDS_H

#include "Heuristic.h"

class VSIDS : public Heuristic {
private:
    std::vector<size_t> _score;
    std::vector<size_t> _occurrence_count;
    size_t _decisions;

public:
    void init(std::vector<std::unordered_set<size_t>> *watch_list) override {
        this->_literal_frequency = &watch_list;
        this->_score = std::vector<size_t>( (this->_literal_frequency)->size(), 0);
        this->_occurrence_count = std::vector<size_t>((this->_literal_frequency)->size(), 0);
        this->_decisions = 0;

        for(size_t i=0; i<this->_score.size(); ++i) {
            this->_score[i] = (*(this->_literal_frequency))[i].size();
        }
    }

    int pick_literal() {
        ++(this->_decisions);
        auto highest_score = std::max_element(this->_score.begin(), this->_score.end());
        int literal = highest_score - this->_score.begin();


    }
};
#endif //PROJECT_SAT_VSIDS_H
