#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// Token types
enum TokenType {
    KEYWORD, // user, worker_processes, error_log, pid, worker_rlimit_nofile, events, http, include, index, default_type, log_format, access_log, sendfile, tcp_nopush, server_names_hash_bucket_size, listen, server_name, root, location, expires, proxy_pass, upstream, weight
    SEMICOLON, // ;
    OPEN_BRACE, // {
    CLOSE_BRACE, // }
    STRING // values such as www www, 5, logs/error.log, logs/nginx.pid, 8192, 4096, etc.
};

// Token structure
struct Token {
    TokenType type;
    string value;

    Token(TokenType t, const string& v) : type(t), value(v) {}

};

vector<Token> parseTokens(string filename) {
    vector<Token> tokens;
    vector<string> keywords;
    keywords.push_back("user");
    keywords.push_back("worker_processes");
    keywords.push_back("error_log");
    keywords.push_back("pid");
    keywords.push_back("worker_rlimit_nofile");
    keywords.push_back("events");
    keywords.push_back("http");
    keywords.push_back("include");
    keywords.push_back("index");
    keywords.push_back("default_type");
    keywords.push_back("log_format");
    keywords.push_back("access_log");
    keywords.push_back("sendfile");
    keywords.push_back("tcp_nopush");
    keywords.push_back("server_names_hash_bucket_size");
    keywords.push_back("listen");
    keywords.push_back("server_name");
    keywords.push_back("root");
    keywords.push_back("location");
    keywords.push_back("expires");
    keywords.push_back("proxy_pass");
    keywords.push_back("upstream");
    keywords.push_back("weight");

   ifstream inFile;
    inFile.open(filename.c_str());

    string line;
    while (getline(inFile, line)) {
        string keyword, value;
        bool found_keyword = false;

        for (int i = 0; i < line.size(); i++) {
            char c = line[i];

            if (c == ' ' || c == '\t') {
                if (found_keyword) {
                    value += c;
                }
            } else if (c == ';') {
                if (found_keyword) {
                    tokens.push_back(Token(KEYWORD, keyword));
                    tokens.push_back(Token(STRING, value));
                    tokens.push_back(Token(SEMICOLON, ""));
                }
                found_keyword = false;
                keyword = "";
                value = "";
            } else if (c == '{') {
                if (found_keyword) {
                    tokens.push_back(Token(KEYWORD, keyword));
                    tokens.push_back(Token(OPEN_BRACE, ""));
                }
                found_keyword = false;
                keyword = "";
                value = "";
                tokens.push_back(Token(KEYWORD, "OPEN_BRACE"));
            } else if (c == '}') {
                if (found_keyword) {
                    tokens.push_back(Token(KEYWORD, keyword));
                    tokens.push_back(Token(STRING, value));
                    tokens.push_back(Token(SEMICOLON, ""));
                }
                found_keyword = false;
                keyword = "";
                value = "";
                tokens.push_back(Token(KEYWORD, "CLOSE_BRACE"));
            } else {
                if (!found_keyword) {
                    keyword += c;
                    if (find(keywords.begin(), keywords.end(), keyword) != keywords.end()) {
                        found_keyword = true;
                    }
                } else {
                    value += c;
                }
            }
        }
    }

    inFile.close();
    return tokens;
}

int main(int argc, char const *argv[])
{
    vector<Token> tokens = parseTokens("./config/nginx.conf");

    for (int i = 0; i < tokens.size(); i++) {
        Token token = tokens[i];
        switch (token.type) {
            case KEYWORD:
                cout << "Keyword: " << token.value << endl;
                break;
            case SEMICOLON:
                cout << "Semicolon" << endl;
                break;
            case OPEN_BRACE:
                cout << "Open brace" << endl;
                break;
            case CLOSE_BRACE:
                cout << "Close brace" << endl;
                break;
            case STRING:
                cout << "String: " << token.value << endl;
                break;
        }
    }
    return 0;
}
