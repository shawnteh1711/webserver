# include <string>
# include <vector>
# include <iostream>
# include <sstream>
# include <fstream>
# include <map>
# include <utility>

#ifndef TOKEN_HPP
# define  TOKEN_HPP
using namespace std;

// https://www.javatpoint.com/nginx-directive-and-context

// Type of context: Main, Events, HTTP, Server, Location, Upstream, Mail, If, Limit_except, Miscellaneous
// Directives types: Normal , Array, Action


struct CGI
{
    string      extension;
    string      path;
    string      arguments;
    bool        enable_post;
    bool        enable_get;
    string      upload_directory;

    CGI()
    {
        enable_post = true;
        enable_get = true;
        upload_directory = "";
    }
};

struct Directive {
    string name;
    string parameter;
};

enum TokenType 
{
    CONTEXT,
    KEY,
    VALUE,
    SEMICOLON,
    OPEN_BRACE,
    CLOSE_BRACE,
    ROUTE,
    CGI
};

const char* tokenTypeNames[] = {
    "CONTEXT",
    "KEY",
    "VALUE",
    "SEMICOLON",
    "OPEN_BRACE",
    "CLOSE_BRACE",
    "ROUTE",
    "CGI",
    "UNKNOWN"
};

class Token
{
    public:
        Token(void);
        Token(TokenType type, string value);
        Token(const Token& other);
        Token&	operator=(const Token& other);
        ~Token(void);
        TokenType getType() const;
        string getValue() const;
        string getTypeName() const;
        void   initContext();
        void   initDirective();
        vector<Directive> setDirective(vector<Token> tokens);
        vector<Token> tokenizeLine(string line);
        vector<Token> readConfigFile(string filename);
        void printTokens(const std::vector<Token>& tokens);


    private:
        TokenType               _type;
        string                  _value;
        map<string, TokenType>  _contextMap;
        // map<string, pair<TokenType, string> > _contextMap;
        map<string, TokenType>  _directiveMap;
        vector<string>          _contextStack;
        vector<Token>           _tokens;
        vector<Directive>       _directives;
    
};


#endif
