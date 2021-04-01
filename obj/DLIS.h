//
// Created by Pranshu on 3/16/2021.
//
#ifndef PROJECT_SAT_DLIS_H
#define PROJECT_SAT_DLIS_H

#include "Heuristic.h"
#include <iostream>

class DLIS : public Heuristic {

public:

    DLIS();

    /*int pick_literal(const size_t &num_variables, const std::vector<Clause> &clauses) {
        this->init(num_variables, clauses);

        auto e = std::max_element(this->_literal_frequency.begin(), this->_literal_frequency.end());
        int idx = e - this->_literal_frequency.begin();
        int literal = idx/2;

        if (idx & 1) {
            return -literal;

        } else {
            return literal;
        }
    }*/

    int pick_literal() {
        std::cout << *this->_literal_frequency[0].begin() << std::endl;
        return 0;
    }
};

#endif //PROJECT_SAT_DLIS_H
