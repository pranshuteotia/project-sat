#include <iostream>
#include <vector>
#include <sstream>
#include "../lib/BruteForceSolver.h"
#include "../lib/DPLLSolver.h"
#include "../obj/VSIDS.h"
#include "../obj/DLIS.h"
#include <chrono>
#include <fstream>
#include <cstring>
#include <set>

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

std::string print_assignments(std::vector<bool> v) {
    std::string vars, assignments;

    for(size_t i=1; i<v.size(); ++i) {
        vars.append(std::to_string(i) + " ");
        assignments.append((v[i])? "T ": "F " );
    }

    return assignments;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cout << "usage:\n" ;
        std::cout << "\tproject-sat heuristic mode /path/to/cnf-file" << std::endl;
        return 1;
    }

    std::string heursitic_string{argv[1]};
    std::string mode_string{argv[2]};
    std::string filename_string{argv[3]};

    // corresponds to using DLIS vs VSIDS heuristic object
    std::set<std::string> heuristics {"dlis","vsids"};
    // corresponds to solve_no_stack, solve_copy_constructor, solve
    std::set<std::string> modes {"no-stack","copy-constructor","undo-stack"};

    if (heuristics.count(heursitic_string) == 0) {
        std::cout << "usage:\n" ;
        std::cout << "\tproject-sat heuristic mode /path/to/cnf-file\n";
        std::cout << "heuristic must be one of the following:";
        for (auto h : heuristics) {
            std::cout << '\n' << '\t' << h;
        }
        std::cout << std::endl;
        return 1;
    }

    if (modes.count(mode_string) == 0) {
        std::cout << "usage:\n" ;
        std::cout << "\tproject-sat heuristic mode /path/to/cnf-file\n";
        std::cout << "mode must be one of the following:";
        for (auto m : modes) {
            std::cout << '\n' << '\t' << m;
        }
        std::cout << std::endl;
        return 1;
    }

    // Load CNF file.
    auto result = parse_file(filename_string);
    auto clauses = result.first;
    int num_variables = result.second;

    // Initialize timing variables.
    auto start = std::chrono::steady_clock::now();
    auto end = std::chrono::steady_clock::now();

    // Initialize heuristics.
    Heuristic* heuristic;
    VSIDS vsids_heuristic;
    DLIS dlis_heursitic;

    if (heursitic_string == "dlis") {
        heuristic = &dlis_heursitic;
    }
    else {
        heuristic = &vsids_heuristic;
    }

    bool solver_result = false;
    start = std::chrono::steady_clock::now();
    // Undo stack
    DPLLSolver solver(clauses, num_variables, heuristic);
    if (mode_string == "undo-stack") {
        solver_result = solver.solve();
    }
    else if (mode_string == "no-stack") {
        solver_result = solver.solve_no_stack();
    }
    else {
        solver_result = solver.solve_copy_constructor();
    }
    end = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> time = end-start;
    std::cout << int(solver_result) << std::endl;
    std::cerr << time.count()/1000 << std::endl;

//    // No stack
//    start = std::chrono::steady_clock::now();
//    DPLLSolver solver1(clauses, num_variables, *heuristic);
//    if(solver1.solve_no_stack()) {
//        std::cout << print_assignments(solver1.get_assignments()) << std::endl << std::endl;
//    } else {
//        std::cout << "Unsatisfiable\n";
//    }
//    end = std::chrono::steady_clock::now();
//    time = end-start;
//    std::cout << time.count()/1000 << std::endl;
//
//    start = std::chrono::steady_clock::now();
//    // Copy constructor
//    DPLLSolver solver3(clauses, num_variables, *heuristic);
//    if(solver3.solve_copy_constructor()) {
//        std::cout << print_assignments(solver3.get_assignments()) << std::endl << std::endl;
//
//    } else {
//        std::cout << "Unsatisfiable\n";
//    }
//    end = std::chrono::steady_clock::now();
//    time = end-start;
//    std::cout << time.count()/1000 << std::endl;

    return 0;
}
