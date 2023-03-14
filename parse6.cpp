#include "parse6.hpp"

Token::Token(void)
{
    // cout << "Default token constructor" << endl;
}

Token::Token(TokenType type, string value) : _type(type), _value(value)
{
    // cout << "Parameter token constructor" << endl;
}

Token::Token(const Token& other)
{
    if (this != &other)
    {
        _type = other._type;
        _value = other._value;
    }
}

Token&	Token::operator=(const Token& other)
{
    if (this != &other)
    {
        _type = other._type;
        _value = other._value;
    }
    return (*this);
}

Token::~Token(void)
{
    // cout << "token destructor" << endl;
}

map<string, pair<TokenType, string> > Token::getKeywordContextMap()
{
    std::map<std::string, std::pair<TokenType, std::string> > keywordMap;

    // Main context
    keywordMap["user"] = std::make_pair(CONTEXT, "main");
    keywordMap["worker_processes"] = std::make_pair(CONTEXT, "main");
    keywordMap["error_log"] = std::make_pair(CONTEXT, "main");
    keywordMap["pid"] = std::make_pair(CONTEXT, "main");
    keywordMap["worker_rlimit_nofile"] = std::make_pair(CONTEXT, "main");

    // Directives in main context
    // keywordMap["user_directive"] = make_pair(DIRECTIVE, "user");
    // keywordMap["worker_processes_directive"] = make_pair(DIRECTIVE, "worker_processes");
    // keywordMap["error_log_directive"] = make_pair(DIRECTIVE, "error_log");
    // keywordMap["pid_directive"] = make_pair(DIRECTIVE, "pid");
    // keywordMap["worker_rlimit_nofile_directive"] = make_pair(DIRECTIVE, "worker_rlimit_nofile");

    keywordMap["events"] = std::make_pair(CONTEXT, "events");
    keywordMap["worker_connections"] = std::make_pair(DIRECTIVE, "");

    keywordMap["http"] = std::make_pair(CONTEXT, "http");

    keywordMap["server"] = std::make_pair(CONTEXT, "server");
    keywordMap["listen"] = std::make_pair(DIRECTIVE, "");
    keywordMap["server_name"] = std::make_pair(DIRECTIVE, "");
    keywordMap["root"] = std::make_pair(DIRECTIVE, "");
    keywordMap["index"] = std::make_pair(DIRECTIVE, "");

    keywordMap["location"] = std::make_pair(CONTEXT, "location");
    keywordMap["proxy_pass"] = std::make_pair(DIRECTIVE, "");
    keywordMap["proxy_set_header"] = std::make_pair(DIRECTIVE, "");
    keywordMap["proxy_http_version"] = std::make_pair(DIRECTIVE, "");
    keywordMap["proxy_set_header"] = std::make_pair(DIRECTIVE, "");

    keywordMap["route"] = std::make_pair(ROUTE, "");
    keywordMap["cgi"] = std::make_pair(CGI, "");

    return keywordMap;
}


// vector<Token> tokenizeLine(string line)
// {
//     vector<Token> tokens;
//     stringstream ss(line);
//     string word;
//     while (ss >> word) {
//         if (word == "user" || word == "worker_processes" || word == "error_log" || word == "pid" || word == "worker_rlimit_nofile") {
//             tokens.push_back(Token(CONTEXT, "main"));
//             tokens.push_back(Token(DIRECTIVE, word));
//             ss >> word;
//             tokens.push_back(Token(VALUE, word));
//             tokens.push_back(Token(VALUE, word));
//             tokens.push_back(Token(SEMICOLON, ";"));
//         } else if (word == "events" || word == "worker_connections"){
//             tokens.push_back(Token(CONTEXT, "events"));
//             tokens.push_back(Token(DIRECTIVE, word));
//             ss >> word;
//             tokens.push_back(Token(OPEN_BRACE, "{"));
//             tokens.push_back(Token(VALUE, word));
//         } else if (word == "http") {
//             tokens.push_back(Token(CONTEXT, "http"));
//             tokens.push_back(Token(DIRECTIVE, word));
//             tokens.push_back(Token(OPEN_BRACE, "{"));
//         } else if (word == "server") {
//             tokens.push_back(Token(CONTEXT, "server"));
//             tokens.push_back(Token(DIRECTIVE, word));
//             ss >> word;
//             tokens.push_back(Token(VALUE, word));
//             tokens.push_back(Token(OPEN_BRACE, "{"));
//         } else if (word == "location") {
//             tokens.push_back(Token(CONTEXT, "location"));
//             tokens.push_back(Token(DIRECTIVE, word));
//             ss >> word;
//             tokens.push_back(Token(VALUE, word));
//             tokens.push_back(Token(OPEN_BRACE, "{"));
//         } else if (word == "}") {
//             tokens.push_back(Token(CLOSE_BRACE, "}"));
//         } else if (word == ";") {
//             tokens.push_back(Token(SEMICOLON, ";"));
//         }
//     }
//     return tokens;
// }

vector<Token> Token::tokenizeLine(string line)
{
    vector<Token> tokens;
    stringstream ss(line);
    string word;
    map<string, pair<TokenType, string> > keywordMap = getKeywordContextMap();
    while (ss >> word) {
        if (keywordMap.count(word)) {
            pair<TokenType, string> tokenInfo = keywordMap[word];
            tokens.push_back(Token(tokenInfo.first, tokenInfo.second));
            if (tokenInfo.first == CONTEXT) {
                tokens.push_back(Token(CONTEXT, word));
            }
            if (tokenInfo.first == DIRECTIVE) {
                tokens.push_back(Token(DIRECTIVE, word));
                ss >> word;
                tokens.push_back(Token(VALUE, word));
                tokens.push_back(Token(SEMICOLON, ";"));
            }
        } else if (word == "{") {
            tokens.push_back(Token(OPEN_BRACE, word));
        } else if (word == "}") {
            tokens.push_back(Token(CLOSE_BRACE, word));
        } else if (word == ";") {
            tokens.push_back(Token(SEMICOLON, word));
        }
    }
    return tokens;
}

vector<Token> Token::readConfigFile(string filename)
{
    vector<Token> tokens;
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            // Tokenize the line and add the resulting tokens to the vector
            vector<Token> lineTokens = tokenizeLine(line);
            tokens.insert(tokens.end(), lineTokens.begin(), lineTokens.end());
        }
        file.close();
    }
    return tokens;
}

TokenType Token::getType() const
{
    return (_type);
}

string Token::getValue() const
{
    return (_value);
}

string Token::getTypeName() const
{
    int index;
    
    index = static_cast<int>(_type);
    if (index >= 0 && index < sizeof(tokenTypeNames)/sizeof(*tokenTypeNames))
        return (tokenTypeNames[index]);
    else
        return (tokenTypeNames[sizeof(tokenTypeNames)/sizeof(*tokenTypeNames) - 1]);
}

#include "parse6.hpp"

void printTokens(const std::vector<Token>& tokens)
{
    // bool inMainContext = false;
    // for (std::vector<Token>::const_iterator it = tokens.begin(); it != tokens.end(); ++it)
    // {
    //     if (it->getTypeName() == "CONTEXT" && it->getValue() == "events") {
    //         inMainContext = true;
    //     } else if (it->getTypeName() == "CONTEXT") {
    //         inMainContext = false;
    //     } else if (inMainContext && (it->getTypeName() == "DIRECTIVE" || it->getTypeName() == "VALUE")) {
    //         std::cout << "Type: " << it->getTypeName() << " | Value: " << it->getValue() << std::endl;
    //     }
    // }
    for (std::vector<Token>::const_iterator it = tokens.begin(); it != tokens.end(); ++it)
    {
        std::cout << "Type: " << it->getTypeName() << " | Value: " << it->getValue() << std::endl;
    }
}


int main(int argc, char const *argv[])
{
    // string configLine = "user www www;";

    // vector<Token> tokens = readConfigFile(configLine);
    Token   token;
    vector<Token> tokens = token.readConfigFile("./config/nginx.conf");

    printTokens(tokens);
 
    return 0;
}
