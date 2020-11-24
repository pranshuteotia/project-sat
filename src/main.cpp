#include <iostream>
#include <vector>
#include <sstream>

std::pair<std::vector<std::vector<int>>, std::vector<int>> parse_file(const char *const filename) {
    std::vector<std::vector<int>> clauses;
    std::vector<int> variables;
    std::vector<std::string> problem;

    std::freopen(filename, "r", stdin);
    std::string value;
    int num_variables;
    int num_clauses;

    // Going over the file line by line.
    while(getline(std::cin, value)) {
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
            num_clauses = std::stoi(problem[3]);

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

    return std::make_pair(clauses, variables);
}

int main() {

    auto clauses = parse_file("data.in").first;

    for(auto clause : clauses) {
        for(auto literal : clause) {
            std::cout << literal << ' ';
        }

        std::cout << std::endl;
    }

    return 0;
}
