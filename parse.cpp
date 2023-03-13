#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sstream>

using namespace std;

int main()
{
    ifstream file("./config/nginx.conf");
    string line;
    vector<string> lines;
    multimap<string, string> config;

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

    // Parse the configuration options
    for (vector<string>::iterator it = lines.begin(); it != lines.end(); ++it)
    {
        // Parse each line of the configuration file
        // For example, extract the server name from a line like "server_name example.com;"
        size_t pos = it->find("server_name ");
        if (pos != string::npos)
        {
            string serverNames = it->substr(pos + 12);
            size_t endpos = serverNames.find_first_of(";#");
            if (endpos != string::npos)
            {
                serverNames = serverNames.substr(0, endpos);
            }
            // serverName.erase(serverName.find_last_not_of(" \n\r\t") + 1);
            // config.insert(make_pair("server_name", serverName));
             // Split the server name string based on whitespace
            std::istringstream iss(serverNames);
            std::string serverName;
            while (iss >> serverName)
            {
                // Add each server name as a separate value in the multimap
                config.insert(std::make_pair("server_name", serverName));
            }
        }
    }

    // Print the parsed information
    multimap<string, string>::iterator it = config.find("server_name");
    while (it != config.end() && it->first == "server_name")
    {
        cout << it->first << ":" << it->second << endl;
        ++it;
    }
    return 0;
}
