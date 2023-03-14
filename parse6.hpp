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


struct DIRECTIVE
{
    enum Type
    {
        NORMAL,
        ARRAY,
        ACTION
    };
    
    string name;
    Type type;
    string value;
    
    DIRECTIVE(string name, Type type, string value)
        : name(name), type(type), value(value)
    {}
};

struct CONTEXT
{
    enum Type
    {
        MAIN,
        EVENTS,
        HTTP,
        SERVER,
        LOCATION,
        UPSTREAM,
        MAIL,
        IF,
        LIMIT_EXCEPT,
        MISC
    };
    
    Type type;
    vector<CONTEXT> contexts;
    
    CONTEXT(Type type) : type(type) {}
    
    void addDirective(CONTEXT context) {
        contexts.push_back(context);
    }
};

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

struct ROUTE
{
    vector<string>  methods;
    string          redirectUrl;
    string          filePath;
    bool            directoryListing;
};

enum TokenType 
{
    CONTEXT,
    DIRECTIVE,
    VALUE,
    SEMICOLON,
    OPEN_BRACE,
    CLOSE_BRACE,
    ROUTE,
    CGI
};

const char* tokenTypeNames[] = {
    "CONTEXT",
    "DIRECTIVE",
    "SEMICOLON",
    "OPEN_BRACE",
    "CLOSE_BRACE",
    "VALUE",
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
        map<string, pair<TokenType, string> > getKeywordContextMap();
        vector<Token> tokenizeLine(string line);
        vector<Token> readConfigFile(string filename);

    private:
        TokenType   _type;
        string      _value;      
};


#endif
