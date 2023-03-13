#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main() {
    // Open the nginx configuration file
    ifstream confFile("./config/nginx.conf");

    // Read the file line by line
    string line;
    vector<string> lines;
    while (getline(confFile, line)) {
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
    confFile.close();

    // Parse the configuration file
    bool inServerBlock = false;
    string listenPort;
    for (vector<string>::const_iterator it = lines.begin(); it != lines.end(); ++it) {
        const string& line = *it;
        // Check if the line is a configuration directive
        if (line.find("server {") != string::npos) {
            inServerBlock = true;
            cout << "Found server block" << endl;
        } else if (inServerBlock && line.find("listen ") != string::npos) {
            cout << "Found listen directive" << endl;
            // Extract the port number from the listen directive
            size_t startpos = line.find(" ");
            if (startpos != string::npos) {
                listenPort = line.substr(startpos + 1);
                cout << "before substr: " << listenPort << endl;
                // Find the semicolon character after the port number
                size_t endpos = listenPort.find(";");
                cout << "endpos: " << endpos << endl;
                if (endpos != string::npos) {
                    listenPort = listenPort.substr(0, endpos);
                }
                // Print out the port number
                cout << "Port number: " << listenPort << endl;
            }
        } else if (inServerBlock && line.find("server_name ") != string::npos) {
            cout << "Found server_name directive" << endl;
        } else {
            // Handle unrecognized directives or other lines
            // ...
        }
    }

    return 0;
}
