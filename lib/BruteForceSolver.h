//
// Created by prans on 11/29/2020.
//

#ifndef PROJECT_SAT_BRUTEFORCESOLVER_H
#define PROJECT_SAT_BRUTEFORCESOLVER_H


#include <vector>
#include <string>

class BruteForceSolver {
private:
    std::vector<std::vector<int>> clauses;
    size_t num_variables;
    size_t row;
    size_t total_rows;

    static std::string dec_to_binary(int num);


public:
    explicit BruteForceSolver(std::vector<std::vector<int>> _clauses, int _num_variables);
    std::vector<bool> solve();
    std::vector<std::vector<bool>> iter_solve();
    void output_to_file();
};


#endif //PROJECT_SAT_BRUTEFORCESOLVER_H
