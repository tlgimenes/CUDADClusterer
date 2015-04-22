/*
 * ============================================================================
 *       Filename:  parser.hpp
 *    Description:  File for parsing the command line
 *        Created:  2015-03-30 15:05
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * ============================================================================
 */

///////////////////////////////////////////////////////////////////////////////

#ifndef PARSER_HPP
#define PARSER_HPP

///////////////////////////////////////////////////////////////////////////////

#include <string>
#include <cstring>
#include <utility>
#include <map>

#include "error.hpp"

///////////////////////////////////////////////////////////////////////////////

#define DEFAULT_STRING "0"

///////////////////////////////////////////////////////////////////////////////

class parser
{
    public:
        /**
         * Parses the command line 
         * */
        static inline void parse(int argc, const char** argv);

        /**
         * Adds arguments to be parsed  
         * */
        static inline void add_argument(const std::string& short_form, 
                                 const std::string& help);
    
        /**
         * Gets the value of the argument 
         * */
        static inline const std::string get(const std::string& arg, bool required);

    protected:
        /**
         * Prints the help in CLI
         * */
        static void print_help();

    private:
        // map< option, value>
        static std::map<std::string, std::string> _raw_input;

        // map <short_form, help>
        static std::map<std::string, std::string> _arguments;

        // argv[0];
        static std::string _prog_name;
};

///////////////////////////////////////////////////////////////////////////////

std::map<std::string, std::string> parser::_raw_input;

// map <short_form, (help, type)>
std::map<std::string, std::string> parser::_arguments;

std::string parser::_prog_name;

///////////////////////////////////////////////////////////////////////////////

inline void parser::add_argument(const std::string& short_form, 
                                 const std::string& help)
{
    parser::_arguments.insert(std::pair<std::string, std::string> (short_form, help));
}

///////////////////////////////////////////////////////////////////////////////

inline const std::string parser::get(const std::string& arg, bool required)
{
    if(required) { 
       if(parser::_arguments.count(arg) && parser::_raw_input.count(arg))
           return parser::_raw_input.find(arg)->second;
       else { // if arg required and not found on CLI, print help
           parser::print_help();
           exit(EXIT_FAILURE);
        }
    }
    else {
        if(parser::_arguments.count(arg) && parser::_raw_input.count(arg))
            return parser::_raw_input.find(arg)->second;
    }
    // If argument not found and not required, return default string
    return DEFAULT_STRING; 
}

///////////////////////////////////////////////////////////////////////////////

inline void parser::parse(int argc, const char** argv)
{
    parser::_prog_name = argv[0];

    if(!(argc%2) || argc == 1) { // argc needs to be even and greater than 1
        parser::print_help(); 
        exit(EXIT_FAILURE);
    }

    for(int i=1; i < argc; i+=2)
    {
        if(!parser::_arguments.count(argv[i]) || !strcmp(argv[i], "-h") || !strcmp(argv[i+1],"-h")) { // if argument not found, print help
            parser::print_help();
            exit(EXIT_FAILURE);
        }
        // Argument successfuly entered
        parser::_raw_input.insert(std::pair<std::string, std::string>(argv[i], argv[i+1])); 
    }
}

///////////////////////////////////////////////////////////////////////////////

inline void parser::print_help()
{
    std::cout << "Usage: " << parser::_prog_name << " [options]" << std::endl;

    std::cout << "Options: " << std::endl;
    for(auto it: parser::_arguments)
    {
        std::cout << "  " << it.first << ": " << it.second << std::endl;
    }
}

///////////////////////////////////////////////////////////////////////////////

#endif /* !PARSER_HPP */

///////////////////////////////////////////////////////////////////////////////

