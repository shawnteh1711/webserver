#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sstream>

// # check whether directive is valid.

using namespace std;

int main()
{
    ifstream file("./config/nginx.conf");
    string line;
    vector<string> lines;
    map<string, vector<string> > config;

    while (getline(file, line)) {
        // Ignore comments and empty lines
        if (!line.empty() && line[0] != '#') {
            // Remove any trailing whitespace
            size_t endpos = line.find_last_not_of(" \t\r\n");
            if (endpos != string::npos) {
                line = line.substr(0, endpos + 1);
            }
            // Add the line to the vector
            lines.push_back(line);
        }
    }
    file.close();

    // Parse the configuration
    for (vector<string>::iterator it = lines.begin(); it != lines.end(); ++it) {
        // Split the line into words
        stringstream ss(*it);
        string word;
        vector<string> words;
        while (ss >> word) {
            words.push_back(word);
        }
        // Check if there is a directive
        if (!words.empty() && words[0].substr(0, 1) == "#") {
            // Skip comments
            continue;
        } else if (!words.empty() && words[0].substr(0, 1) == "}") {
            // End of a block, do nothing
            continue;
        } else if (!words.empty() && words[0].substr(0, 1) == "server") {
            // Handle server block
            // ...
        } else if (!words.empty() && words[0].substr(0, 1) == "upstream") {
            // Handle upstream block
            // ...
        } else if (!words.empty()) {
            // Handle directive
            if (config.find(words[0]) == config.end()) {
                // Create a new entry in the map
                config[words[0]] = vector<string>();
            }
            // Add the value to the vector
            config[words[0]].push_back(words[1]);
        }
    }

    // Print the parsed information
    for (map<string, vector<string> >::iterator it = config.begin(); it != config.end(); ++it) {
        cout << it->first << ": ";
        for (vector<string>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
            cout << *it2 << " ";
        }
        cout << endl;
    }

    return 0;
}
