//
// Created by prans on 11/29/2020.
//

#include "BruteForceSolver.h"

#include <algorithm>
#include <iostream>
#include <cmath>

BruteForceSolver::BruteForceSolver(std::vector<std::vector<int>> _clauses, int _num_variables) {
    this->clauses = std::move(_clauses);
    this->num_variables = _num_variables;
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

void BruteForceSolver::generate_truth_table() {
    int total_rows = std::pow(2, this->num_variables);

    for(int i=0; i<total_rows; ++i) {
        std::vector<int> row;
        std::string binary = this->dec_to_binary(i);
        int zero_padding = this->num_variables - binary.length();

        while(zero_padding--) { row.push_back(0); }

        for(char digit : binary) {
            row.push_back(digit - '0');
        }

        int and_continer = 1;
        for(auto clause : this->clauses) {
            int or_container = 0;
            for(int literal : clause) {
                bool is_negated = false;

                if(literal < 0) { is_negated = true; }

                int idx = std::abs(literal) - 1;

                int value = (is_negated)? ~row.at(idx) : row.at(idx);

                or_container |= value;
            }

            and_continer &= or_container;
        }

        row.push_back(and_continer);
        this->truth_table.push_back(row);
    }
}

std::vector<int> BruteForceSolver::solve() {
    this->generate_truth_table();
    std::vector<int> sat_assignment;

    for(auto sat : this->truth_table) {
        int satisfied_idx = this->num_variables;

        if(sat.at(satisfied_idx)) {
            sat_assignment = sat;
            break;
        }
    }

    sat_assignment.pop_back();
    return sat_assignment;
}

std::vector<std::vector<int>> BruteForceSolver::iter_solve() {
    this->generate_truth_table();
    std::vector<std::vector<int>> sat_assignments;

    for(auto sat: this->truth_table) {
        int satisfied_idx = this->num_variables;

        if(sat.at(satisfied_idx)) {
            auto sat_assignment_instance = sat;
            sat_assignment_instance.pop_back();

            sat_assignments.push_back(sat_assignment_instance);
        }
    }

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

    for(auto sat_assignment: all) {
        for(auto literal: sat_assignment) {
            std::cout << "| " << literal << " ";
        }
        std::cout << "|\n";
    }

    std::fclose(stdout);
}
