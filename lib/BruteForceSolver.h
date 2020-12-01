//
// Created by prans on 11/29/2020.
//

#ifndef PROJECT_SAT_BRUTEFORCESOLVER_H
#define PROJECT_SAT_BRUTEFORCESOLVER_H


#include <vector>
#include <string>

class BruteForceSolver {
private:
    std::vector<std::vector<int>> truth_table;
    std::vector<std::vector<int>> clauses;
    int num_variables;

    std::string dec_to_binary(int num);
    void generate_truth_table();


public:
    explicit BruteForceSolver(std::vector<std::vector<int>> _clauses, int _num_variables);
    std::vector<int> solve();
    std::vector<std::vector<int>> iter_solve();
    void output_to_file();
};


#endif //PROJECT_SAT_BRUTEFORCESOLVER_H
