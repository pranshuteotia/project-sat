#include <iostream>
#include <vector>
#include <sstream>
#include "../lib/BruteForceSolver.h"
#include "../lib/DPLLSolver.h"
#include <chrono>
#include <fstream>

std::pair<std::vector<std::vector<int>>, int> parse_file(std::string filename) {
    std::vector<std::vector<int>> clauses;
    std::vector<int> variables;
    std::vector<std::string> problem;

    std::ifstream f (filename, std::ifstream::in);
    std::string value;
    int num_variables;

    // Going over the file line by line.
    while(getline(f, value)) {
        char first_char = value[0];

        // It's a comment. Should be ignored.
        if(first_char == 'c') {
            continue;
        }
            // Is a problem statement. Extract the number of variables and clauses.
        else if(first_char == 'p') {
            std::string str;
            std::stringstream ss(value);

            // Split the line. Delimiter = " " (Space).
            while(getline(ss, str, ' ')) {
                problem.push_back(str);
            }

            // Extracting the number of clause and variables.
            num_variables = std::stoi(problem[2]);

        } else {
            std::string str;
            std::stringstream ss(value);
            std::vector<int> clause;

            // Split the clause using the space character as a delimiter.
            while(getline(ss, str, ' ')) {
                int literal = std::stoi(str);

                // If the literal is non zero add it clause list.
                if(literal) {
                    clause.push_back(literal);
                }
            }

            clauses.push_back(clause);
        }
    }

    std::fclose(stdin);

    return std::make_pair(clauses, num_variables);
}

int main() {
    auto result = parse_file("data.in");
    auto clauses = result.first;
    int num_variables = result.second;

    /*for(const auto &clause : clauses) {
        for(auto literal : clause) {
            std::cout << literal << ' ';
        }

        std::cout << std::endl;
    }*/

    /*BruteForceSolver solver(clauses, num_variables);
    auto all_instances = solver.iter_solve();

    auto sat = solver.solve();
    for(const auto &sat : all_instances) {
        for(auto v : sat) {
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }

    solver.output_to_file();*/

    auto start = std::chrono::steady_clock::now();
    DPLLSolver solver2(clauses, num_variables);
    if(solver2.solve()) {
        std::string vars, assignments;
        auto assignments_vector = solver2.get_assignments();

        for(size_t i=1; i<assignments_vector.size(); ++i) {
            vars.append(std::to_string(i) + " ");
            assignments.append((assignments_vector[i])? "T ": "F " );
        }

        std::cout << vars << '\n' << assignments << std::endl;

    } else {
        std::cout << "Unsatisfiable" << std::endl;
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> time = end-start;
    std::cout << time.count()/1000 << std::endl;
    
    return 0;
}
