#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <sstream>

int main() {
    // Define the set of required directives
    std::set<std::string> required_directives;
    required_directives.insert("user");
    required_directives.insert("worker_processes");
    required_directives.insert("error_log");
    required_directives.insert("pid");

    // Open the configuration file
    std::ifstream config_file("./config/nginx.conf");

    // Check if all required directives are present
    std::map<std::string, bool> present_directives;
    std::string line;
    while (std::getline(config_file, line)) {
        std::string directive;
        std::istringstream iss(line);
        iss >> directive;
        if (required_directives.find(directive) != required_directives.end()) {
            present_directives[directive] = true;
        }
    }

    bool valid = true;
    for (std::set<std::string>::iterator it = required_directives.begin(); it != required_directives.end(); ++it) {
        if (present_directives.find(*it) == present_directives.end()) {
            valid = false;
            std::cout << "Error: Required directive " << *it << " not present in configuration file." << std::endl;
        }
    }

    if (valid) {
        std::cout << "Configuration file is valid." << std::endl;
    }

    return 0;
}
