//
// Created by prans on 11/29/2020.
//

#include "BruteForceSolver.h"

#include <algorithm>
#include <iostream>
#include <cmath>

BruteForceSolver::BruteForceSolver(std::vector<std::vector<int>> _clauses, int _num_variables) {
    this->clauses = _clauses;
    this->num_variables = _num_variables;
    this->row = 0;
    this->total_rows = std::pow(2, _num_variables);
}

std::string BruteForceSolver::dec_to_binary(int num) {
    std::string binary;
    int n = num;

    while(n > 0) {
        int t = n % 2;
        char bin_digit = '0' + t;
        binary.push_back(bin_digit);
        n /= 2;
    }

    std::reverse(binary.begin(), binary.end());

    return binary;
}

std::vector<bool> BruteForceSolver::solve() {

    while(this->row < this->total_rows) {
        std::vector<bool> sat_assignment;
        std::string binary = this->dec_to_binary(this->row);
        size_t zero_padding = this->num_variables - binary.length();

        while(zero_padding--) { sat_assignment.push_back(false); }
        for(char bit : binary) { sat_assignment.push_back(bit - '0'); }

        bool AND_result = true;
        for(const auto &clause: clauses) {
            bool OR_result = false;
            for(int literal : clause) {
                bool is_negated = false;
                if(literal < 0) { is_negated = true; }

                int idx = std::abs(literal) - 1;
                bool literal_assignment = (is_negated)? ~sat_assignment.at(idx) : sat_assignment.at(idx);
                OR_result |= literal_assignment;
            }
            AND_result &= OR_result;
        }

        if(AND_result) {
            ++this->row;
            return sat_assignment;
        }

        ++this->row;
    }

    return std::vector<bool>();
}

std::vector<std::vector<bool>> BruteForceSolver::iter_solve() {
    std::vector<std::vector<bool>> sat_assignments;

    size_t temp = this->row;
    this->row = 0;
    size_t current_row = 0;
    while(current_row < this->total_rows) {
        sat_assignments.push_back(this->solve());
        current_row = this->row;
    }
    this->row = temp;
    return sat_assignments;
}

void BruteForceSolver::output_to_file() {
    std::freopen("output.out", "w", stdout);

    auto all = this->iter_solve();

    for(int i=0; i<this->num_variables; ++i) {
        char var = 'A' + i;
        std::cout << "| " << var << " ";
    }
    std::cout << "|\n";

    for(const auto &sat_assignment: all) {
        for(auto literal: sat_assignment) {
            std::cout << "| " << literal << " ";
        }
        std::cout << "|\n";
    }

    std::fclose(stdout);
}
