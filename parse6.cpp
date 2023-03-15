#include "parse6.hpp"

Token::Token(void)
{
    initContext();
    initDirective();
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

void   Token::initContext()
{
    _contextMap["http"] = CONTEXT;
    _contextMap["server"] = CONTEXT;
    _contextMap["location"] = CONTEXT;
}

void   Token::initDirective()
{
    // main context
    _directiveMap["user"] = KEY;
    _directiveMap["worker_processes"] = KEY;
    _directiveMap["error_log"] = KEY;
    _directiveMap["pid"] = KEY;
    _directiveMap["worker_rlimit_nofile"] = KEY;

    // server context
    _directiveMap["server_name"] = KEY;
    _directiveMap["listen"] = KEY;
    _directiveMap["root"] = KEY;
    _directiveMap["try_files"] = KEY;
    _directiveMap["proxy_pass"] = KEY;
    _directiveMap["fastcgi_pass"] = KEY;
}

vector<Token> Token::tokenizeLine(string line)
{
    vector<Token> tokens;
    stringstream ss(line);
    string word;
    while (ss >> word)
    {
        if (word.find('#') != string::npos)
            break ;
        if (_contextMap.count(word))
        {
            TokenType context_type = _contextMap[word];
            tokens.push_back(Token(CONTEXT, word));
            if (context_type == CONTEXT)
                _contextStack.push_back(word);
        }
        else if (_directiveMap.count(word))
            tokens.push_back(Token(_directiveMap[word], word));
        else if (word == "{")
            tokens.push_back(Token(OPEN_BRACE, word));
        else if (word == "}")
        {
            tokens.push_back(Token(CLOSE_BRACE, word));
            if (!_contextStack.empty())
                _contextStack.pop_back();
        }
        else if (word == ";")
            tokens.push_back(Token(SEMICOLON, word));
        else
        {
            string value = word;

            if (word.front() == '"' && word.back() != '"')
            {
                while (ss >> word && word.back() != '"')
                    value += " " + word;
                value += " " + word;                
            }
            tokens.push_back(Token(VALUE, value));
        }
    }
    return (tokens);
}

vector<Token> Token::readConfigFile(string filename)
{
    // vector<Token> tokens;
    ifstream file(filename);
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            if (line.empty() || line[0] == '#')
                continue ;
            vector<Token> lineTokens = tokenizeLine(line);
            _tokens.insert(_tokens.end(), lineTokens.begin(), lineTokens.end());
        }
        file.close();
    }
    return (_tokens);
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
    unsigned long index;
    
    index = static_cast< unsigned long>(_type);
    if (index >= 0 && index < sizeof(tokenTypeNames)/sizeof(*tokenTypeNames))
        return (tokenTypeNames[index]);
    else
        return (tokenTypeNames[sizeof(tokenTypeNames)/sizeof(*tokenTypeNames) - 1]);
}

vector<Directive> Token::setDirective(vector<Token> tokens)
{
    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (tokens[i].getType() == KEY)
        {
            Directive directive;
            directive.name = tokens[i].getValue();
            if (i + 1 < tokens.size() && tokens[i + 1].getType() == VALUE)
            {
                directive.parameter = tokens[i + 1].getValue();
                i++;
            }
            _directives.push_back(directive);
        }
    }

    for (vector<Directive>::iterator it = _directives.begin(); it < _directives.end(); ++it)
    {
        std::cout << "name: " << std::setw(12) << std::left << it->name << " | parameter: " << it->parameter << endl;
    }
    return (_directives);
}

void Token::printTokens(const std::vector<Token>& tokens)
{
    for (std::vector<Token>::const_iterator it = tokens.begin(); it != tokens.end(); ++it)
    {
        std::cout << "Type: " << it->getTypeName() << " | Value: " << it->getValue() << std::endl;
    }
}


int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;
    // vector<Token> tokens = readConfigFile(configLine);
    Token   token;
    // vector<Token> tokens = token.readConfigFile("./config/nginx.conf");
    vector<Token> tokens = token.readConfigFile("./config/minimal.conf");

    // token.printTokens(tokens);

    vector<Directive> directives;
    directives = token.setDirective(tokens);


 
    return 0;
}
