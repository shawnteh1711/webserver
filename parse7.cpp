#include "parse7.hpp"


// Create an instance of the Config struct to hold the parsed configuration data.

// Read the configuration file line by line and parse each line into a Directive struct.

// Keep track of the current block depth using a stack data structure.

// As you parse each line, check whether it is a block start or end directive (e.g., { or }), and update the stack accordingly.

// When you encounter a new block start directive, create a new Directive struct to hold the block's data and push it onto the stack.

// When you encounter a block end directive, pop the top Directive struct off the stack and add it to the block vector of the next-highest struct on the stack.

// Once you have parsed the entire configuration file, add the top-level Directive struct to the parsed vector of a new configItem struct, along with the file name and any errors encountered during parsing.

// Add the configItem struct to the config vector of the Config struct, along with any other relevant data (e.g., the "directive" field).

string trimLine(const string &str)
{
    size_t  start;
    size_t  end;

    start = str.find_first_not_of(" \t\r\n");
    end = str.find_last_not_of(" \t\r\n");
    if (start == string::npos || end == string::npos)
        return "";
    else
        return (str.substr(start, end - start + 1));
}

vector<string> splitLine(string str, char delimiter)
{
    vector<string>  result;
    stringstream    ss(str);
    string          token;
    while (getline(ss, token, delimiter))
    {
        result.push_back(token);
    }
    return (result);
}

vector<Directive> parseBlock(ifstream& file, int& lineNum, Directive& currentDirective)
{
    vector<Directive> block;
    vector<Directive>* currentBlock = &block;
    stack<vector<Directive>*> blockStack;
    string line;

    block = currentDirective.block;
    blockStack.push(&block);
    while (getline(file, line))
    {
        lineNum++;
        line = trimLine(line);
        if (line.empty() || line[0] == '#')
            continue;
        Directive directive;
        directive.line = lineNum;

        size_t commentPos = line.find('#');
        if (commentPos != string::npos)
        {
            line = line.substr(0, commentPos);
        }
        if (line[0] == '}')
        {
            if (blockStack.empty())
            {
                throw runtime_error("Unmatched closing brace on line " + to_string(lineNum));
            }
            currentBlock = blockStack.top();
            blockStack.pop();
            return block;
        }
        size_t blockStart = line.find('{');
        if (blockStart != string::npos)
        {
            if (line.find("location") != string::npos)
            {
                size_t  argStart = line.find(' ');
                size_t  argEnd = line.find_last_not_of(' ', line.size() - 2);
                directive.directive = line.substr(0, argStart);
                line = line.substr(argStart + 1, argEnd - argStart);
                line = trimLine(line);
                directive.args.push_back(line);
            }
            else
            {
                directive.directive = line.substr(0, blockStart);
                directive.args.push_back(line.substr(blockStart + 1));
            }
            directive.block = parseBlock(file, lineNum, directive);
        }
        else
        {
            size_t argStart = line.find(' ');
            directive.directive = line.substr(0, argStart);
            size_t argEnd = line.find_last_not_of(' ', line.size() - 1);
            line = line.substr(argStart + 1, argEnd - argStart);
            line = trimLine(line);
            if (line.back() == ';')
                line = line.substr(0, line.size() - 1);
            directive.args.push_back(line);
        }
        currentBlock->push_back(directive);
        if (line[line.size() - 1] == '{')
        {
            blockStack.push(currentBlock);
            currentBlock = &currentBlock->back().block;
        }
    }
    if (!blockStack.empty())
    {
        throw runtime_error("Unmatched opening brace in file");
    }
    return block;
}


Config Config::parseConfigFile(string filename)
{
    Config config;

    config.status = "ok";
    config.errors.push_back("no error yet");
    configItem item;
    item.file = filename;
    item.errors.push_back("no error yet");
    ifstream file(filename);
    if (file.is_open())
    {
        string line;
        int lineNum = 0;
        while (getline(file, line))
        {
            lineNum++;
            line = trimLine(line);
            if (line.empty() || line[0] == '#')
                continue;
            Directive directive;
            directive.line = lineNum;
            size_t commentPos = line.find('#');
            if (commentPos != string::npos)
            {
                line = line.substr(0, commentPos);
            }
            line = trimLine(line);
            size_t blockStart = line.find('{');
            if (blockStart != string::npos)
            {
                directive.directive = line.substr(0, blockStart);
                directive.args.push_back(line.substr(blockStart + 1));
                // directive.block = parseBlock(file, lineNum);
                directive.block = parseBlock(file, lineNum, directive);
            }
            else
            {
                size_t argStart = line.find(' ');
                directive.directive = line.substr(0, argStart);
                size_t argEnd = line.find_last_not_of(' ', line.size() - 1);
                line = line.substr(argStart + 1, argEnd - argStart);
                line = trimLine(line);
                if (line.back() == ';')
                    line = line.substr(0, line.size() - 1);
                directive.args.push_back(line);
            }
            // for (size_t i = 0; i < directive.block.size(); i++)
            // {
            //     cout << directive.block[i].directive << endl;
            //     for (size_t j = 0; j < directive.block[i].block.size(); j++)
            //     {
            //         cout << directive.block[i].block[j].directive << endl;
            //     }
            // }
            
            item.parsed.push_back(directive);
        }
        file.close();
        config.configItems.push_back(item);
    }
    else
    {
        config.errors.push_back("Unable to open file " + filename);
        config.status = "error";
    }
    return config;
}

void    findDirectiveRecursive(vector<Directive>& directives, string directiveName, vector<Directive*>& result)
{
    for (vector<Directive>::iterator itDirective = directives.begin(); itDirective != directives.end(); ++itDirective)
    {
        if (itDirective->directive == directiveName)
            result.push_back(&(*itDirective));
        if (!itDirective->block.empty())
            findDirectiveRecursive(itDirective->block, directiveName, result);
    }
}

vector<Directive*>  Config::findDirective(Config& config, string directiveName)
{
    vector<Directive*> result;
    for (vector<configItem>::iterator it = config.configItems.begin(); it < config.configItems.end(); ++it)
    {
       findDirectiveRecursive(it->parsed, directiveName, result);
    }
    return (result);
}



void print_config_item(const Directive& item, size_t depth = 0)
{
    std::string indent = std::string(depth * 4, ' ');
    std::cout << indent << "line: " << item.line << " | directive: " << setw(20) << std::left << item.directive;

    for (std::vector<std::string>::const_iterator it = item.args.begin(); it != item.args.end(); ++it)
    {
        std::cout << " | arg: " << *it;
    }

    std::cout << std::endl;

    for (std::vector<Directive>::const_iterator it = item.block.begin(); it != item.block.end(); ++it)
    {
        print_config_item(*it, depth + 1);
    }
}

void    Config::print(Config config)
{
    std::cout << "status: " << config.status << std::endl;
    for (vector<string>::const_iterator it = config.errors.begin(); it != config.errors.end(); ++it)
    {
        std::cout << "error: " << *it << std::endl;
    }
    for (std::vector<configItem>::const_iterator it = config.configItems.begin(); it != config.configItems.end(); ++it)
    {
        std::cout << "config file: " << it->file << std::endl;
        for (std::vector<Directive>::const_iterator it2 = it->parsed.begin(); it2 != it->parsed.end(); ++it2)
        {
            print_config_item(*it2);
        }
    }
}



int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        cerr << "error" << endl;
        cerr << "Please enter: ./a.out [conf path][find_directive]" << endl;
        return (1);
    }
    string filename = argv[1];
    string name = argv[2];

    Config      config;
    vector<Directive*>  mydirective;
    config = config.parseConfigFile(filename);

    if (name == "all")
        config.print(config);
    else
    {
        mydirective = config.findDirective(config, name);
        if (!mydirective.empty())
        {
            for (vector<Directive*>::iterator it = mydirective.begin(); it != mydirective.end(); ++it)
            {
                Directive* directive = *it;
                cout << "Directive: " << directive->directive << endl;
                cout << "Arguments: ";
                for (vector<string>::iterator it_arg = directive->args.begin(); it_arg != directive->args.end(); ++it_arg)
                {
                    cout << *it_arg << " ";
                }
                cout << endl;
            }
        }
        else
        {
            cerr << "Directive not found" <<  endl;
            return (1);
        }
    }
    return 0;
}
