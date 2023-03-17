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
    CLOSE_BRACE
};

const char* tokenTypeNames[] = {
    "CONTEXT",
    "KEY",
    "VALUE",
    "SEMICOLON",
    "OPEN_BRACE",
    "CLOSE_BRACE",
    "UNKNOWN"
};

class Token
{
    public:

        typedef string                      keyType;
        typedef vector<Directive>           valueType;
        typedef map<keyType, valueType>     directivesMap;

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
        directivesMap tokenize(string line);
        vector<Token> readConfigFile(string filename);
        void printTokens(const std::vector<Token>& tokens);
        void printTokenMaps();

        directivesMap& getGlobalMap() const;
        directivesMap& getServerMap() const;
        directivesMap& getLocationMap() const;


    private:
        TokenType               _type;
        string                  _value;
        map<string, TokenType>  _contextMap;
        map<string, TokenType>  _directiveMap;
        vector<Token>           _tokens;
        vector<Directive>       _directives;
        directivesMap           _globalMap;
        directivesMap           _serverMap;
        directivesMap           _locationMap;
    
};


#endif
