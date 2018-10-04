#ifndef _SIMPLE_ARG_PARSER_
#define _SIMPLE_ARG_PARSER_

#include <iostream>
#include <cstdlib>
#include <string>
#include <tuple>
#include <vector>
#include <map>
#include <sstream>

#define DEBUG

#if defined(DEBUG)
#   define LOG(MSG) std::cout << MSG << std::endl
#   define LOG_VAR(V) std::cout << #V << " = " << V << std::endl
#else
#   define LOG(MSG)
#   define LOG_VAR(V)
#endif // DEBUG


namespace parser
{
    /** Class responsible for parsing the command-line
    *   input arguments. It receives the raw input data
    *   and then parse it, returning a map
    */
    class ArgumentParser
    {
    protected:

        /** Data structure to describe the basic
        *   characteristics of an argument
        *   (hasValue, description)
        */
        typedef struct
        {
            /** Indicates if the argument is followed
            *   by a value in command-line. E.g.:
            *
            *       program --integer 2
            *
            *   The argument "--integer" will contain
            *   value 2
            */
            bool hasValue;

            /** String with the description of the argument.
            *   It's used to show the usage os the program
            *   in the terminal
            */
            std::string description;
        } argument_data;

        /** map of the arguments registered in the class instance
        */
        std::map<std::string, argument_data> _argumentList;

        /** Map (argument name, argument value (if exists))
        *   Arguments parsed from the inputs in the
        *   current execution
        */
        std::map<std::string, std::string> _parsedArguments;

        /** number of input arguments (argc - 1)
        */
        int _numberOfArguments;

        /** raw input arguments (argv)
        */
        char** _rawInput;

        /** Name of the current program
        */
        std::string _programName;

        /** State used to parse the argument and
        *   its values
        */
        enum {
            ARG_NAME=0,
            ARG_VALUE
        } _parseState;

    public:


        /** Default constructor, used to redirect the input
        *   arguments  directly to the class
        *
        *   @param argc
        *       the same argc received as inputs to main.
        *       It represents the number of command-line
        *       arguments, including the program name
        *
        *   @param argv
        *       also the argv received from main. It contains
        *       an array of C-strings containing the input
        *       arguments
        */
        ArgumentParser(int argc, char** argv) :
            _numberOfArguments(argc - 1),
            _rawInput(argv),
            _programName(std::string(argv[0]))
        {
        }


        /** Includes a new expected command-line
        *   argument to the map, and gives more information
        *   about what is expected from it
        *
        *   @param name
        *       defines the name of the argument. If you need
        *       the two dashes (--) as prefix to be expected
        *       in the command-line you'll have to include it
        *       in the name.
        *
        *       E.g.: program --debug (addArgument("--debug"))
        *
        *   @param hasValue
        *       defines if the arguments receives an actual
        *       value, or if its just a command-line flag.
        *       If it has a value, the next argument in the
        *       list will be taken as the value, otherwise
        *       the value will be considered empty
        *
        *   @param description
        *       defines the description of the argument. Used
        *       to be shown to the user as instruction in case
        *       the "--help" flag is used
        */
        void addArgument(
            const std::string& name,
            bool hasValue = false,
            const std::string& description = "")
        {
            LOG("New Argument");
            argument_data argData = { hasValue, description };
            LOG_VAR(argData.hasValue);
            LOG_VAR(argData.description);
            this->_argumentList[name] = argData;

            for(auto&& arg : this->_argumentList)
            {
                LOG_VAR(arg.first);
                LOG_VAR(arg.second.hasValue);
                LOG_VAR(arg.second.description);
            }
        }

        /** Prints instructions of the program to the
        *   terminal
        */
        void usage() const
        {
            std::cout << "  Usage:\n    ";
            std::cout << this->_programName << " ";
            for(auto&& arg : this->_argumentList)
            {
                std::cout<< "[ " << arg.first << " ";
                if(arg.second.hasValue)
                {
                    std::cout << "val";
                }
                std::cout << "]  ";
            }
            std::cout << "\n\n";

            for(auto&& arg : this->_argumentList)
            {
                if(!arg.second.description.empty())
                {
                    std::cout<< "    [ " << arg.first << " ";
                    if(arg.second.hasValue)
                    {
                        std::cout << "val";
                    }
                    std::cout << "] => " << arg.second.description << std::cout << "\n\n";
                }
            }
            std::cout << "\n\n";
        }

        /** Parses the raw command-line input and
        *   updates a map with the name of the arguments
        *   given and the strings of its values to be converted
        */
        void parse()
        {
            auto state = ARG_NAME;
            std::string name;
            for(int _arg = 1; _arg <= _numberOfArguments; _arg++)
            {
                std::string currentArg(this->_rawInput[_arg]);
                auto argument = this->_argumentList.find(currentArg);
                if(state == ARG_NAME)
                {
                    if(argument != this->_argumentList.end())
                    {
                        LOG("found input argument");
                        LOG_VAR(currentArg);
                        if(argument->second.hasValue)
                        {
                            LOG("requires value");
                            state = ARG_VALUE;
                            name = argument->first;
                        }
                        else
                            this->_parsedArguments[argument->first] = "";
                    }
                    else
                    {
                        this->usage();
                        exit(0);
                    }
                }
                else if(state == ARG_VALUE)
                {
                    LOG("parsing argument value");
                    std::string data(this->_rawInput[_arg]);
                    LOG_VAR(data);

                    this->_parsedArguments[name] = data;
                    LOG("access");
                    LOG_VAR(this->_parsedArguments[name]);
                    state = ARG_NAME;
                }
            }
        }


        /** Verifies if a giver argument is defined
        *
        *   @param [in] argName
        *       name of the argument to be verified
        */
        bool isDefined(const std::string& argName) const
        {
            auto& args = this->_parsedArguments;
            return args.find(argName) != args.end();
        }


        /** Verifies if a given argument constains an extra
        *   value
        *
        *   @param [in] argName
        *       name of the argument to be verified
        */
        bool hasValue(const std::string& argName) const
        {
            auto& args = this->_parsedArguments;
            auto argument = args.find(argName);

            bool ret = false;

            if(argument != args.end())
            {
                auto&& argumentData = this->_argumentList.find(argName);
                ret = argumentData->second.hasValue;
            }

            return ret;
        }

        /** Retrieve the argument value
        *
        *   @param [in] argName
        *       name of the argument to be verified
        */
        template <typename T>
        T getArgument(const std::string& argName) const
        {
            std::stringstream ss;
            ss << this->_parsedArguments.find(argName)->second;
            T value;
            ss >> value;
            return value;
        }

    };

}

#endif // _SIMPLE_ARG_PARSER_
