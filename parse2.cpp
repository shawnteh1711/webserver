#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

class Directive {
public:
    Directive(std::string name) : name(name) {}

    void addValue(std::string value) {
        values.push_back(value);
    }

    const std::string& getName() const {
        return name;
    }

    const std::vector<std::string>& getValues() const {
        return values;
    }

private:
    std::string name;
    std::vector<std::string> values;
};

int main()
{
    std::ifstream file("./config/nginx.conf");
    std::string line;
    std::vector<Directive> directives;

    while (std::getline(file, line))
    {
        // Ignore comments and empty lines
        if (!line.empty() && line[0] != '#')
        {
            // Parse the directive name and value(s)
            size_t pos = line.find(" ");
            if (pos != std::string::npos)
            {
                std::string name = line.substr(0, pos);
                size_t start = line.find_first_not_of(" \t\r\n", pos);
                if (start != std::string::npos)
                {
                    std::string valueLine = line.substr(start);
                    std::vector<std::string> values;
                    size_t endpos;
                    do
                    {
                        endpos = valueLine.find_first_of(" \t\r\n;");
                        if (endpos != std::string::npos)
                        {
                            std::string value = valueLine.substr(0, endpos);
                            values.push_back(value);
                            valueLine = valueLine.substr(endpos + 1);
                        }
                    } while (endpos != std::string::npos);
                    Directive directive(name);
                    for (std::vector<std::string>::iterator it = values.begin(); it != values.end(); ++it)
                    {
                        directive.addValue(*it);
                    }
                    directives.push_back(directive);
                }
            }
        }
    }

    // Print the parsed information
    for (std::vector<Directive>::iterator it = directives.begin(); it != directives.end(); ++it)
    {
        std::cout << it->getName() << ": ";
        const std::vector<std::string>& values = it->getValues();
        for (std::vector<std::string>::const_iterator jt = values.begin(); jt != values.end(); ++jt)
        {
            std::cout << *jt << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
