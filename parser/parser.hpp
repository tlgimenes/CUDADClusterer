/*============================================================================*/
/*! \file parser.hpp 
 *  \author Tiago LOBATO GIMENES            (tlgimenes@gmail.com)
 *  \date 2015-03-30 15:05 
 *  
 *  \brief CLI parser
 *
 *  This file contains implementation of a simple CLI parser
 * */
/*============================================================================*/


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

/*! \brief Default string to be returned in case of failure */
#define DEFAULT_STRING "0"

///////////////////////////////////////////////////////////////////////////////

namespace console
{
    /*! \brief Class for parsing the Command Line Interface */
    class parser
    {
        public:
            /*!
             * \brief Parses the command line interface
             * */
            static inline void parse(int argc, const char** argv);

            /*!
             * \brief Adds arguments to be parsed.
             *
             * \param short_form short form of the parameter. Ex: "-t", "-a", etc
             * \param help help for the parameter
             * */
            static inline void add_argument(const std::string& short_form, 
                    const std::string& help);

            /*!
             * \brief Gets the value of the argument
             *
             * \param arg short form of the parameter. Ex: "-t", "-a", etc
             * \param required true if parameter is required, false otherwise
             * \return string containing the value passed in CLI. If parameter is
             * not required the DEFAULT_STRING will be returned
             * */
            static inline const std::string get(const std::string& arg, bool required);

        protected:
            /**
             * \brief Prints the help in CLI
             * */
            static void print_help();

        private:
            /*! \brief map< option, value> */
            static std::map<std::string, std::string> _raw_input;

            /*! \brief map <short_form, help> */
            static std::map<std::string, std::string> _arguments;

            /*! \brief argv[0]; */
            static std::string _prog_name;
    };
};

///////////////////////////////////////////////////////////////////////////////

std::map<std::string, std::string> console::parser::_raw_input;

// map <short_form, (help, type)>
std::map<std::string, std::string> console::parser::_arguments;

std::string console::parser::_prog_name;

///////////////////////////////////////////////////////////////////////////////

inline void console::parser::add_argument(const std::string& short_form, 
        const std::string& help)
{
    console::parser::_arguments.insert(std::pair<std::string, std::string> (short_form, help));
}

///////////////////////////////////////////////////////////////////////////////

inline const std::string console::parser::get(const std::string& arg, bool required)
{
    if(required) { 
        if(console::parser::_arguments.count(arg) && console::parser::_raw_input.count(arg))
            return console::parser::_raw_input.find(arg)->second;
        else { // if arg required and not found on CLI, print help
            console::parser::print_help();
            exit(EXIT_FAILURE);
        }
    }
    else {
        if(console::parser::_arguments.count(arg) && console::parser::_raw_input.count(arg))
            return console::parser::_raw_input.find(arg)->second;
    }
    // If argument not found and not required, return default string
    return DEFAULT_STRING; 
}

///////////////////////////////////////////////////////////////////////////////

inline void console::parser::parse(int argc, const char** argv)
{
    console::parser::_prog_name = argv[0];

    if(!(argc%2) || argc == 1) { // argc needs to be even and greater than 1
        console::parser::print_help(); 
        exit(EXIT_FAILURE);
    }

    for(int i=1; i < argc; i+=2)
    {
        if(!console::parser::_arguments.count(argv[i]) || !strcmp(argv[i], "-h") || !strcmp(argv[i+1],"-h")) { // if argument not found, print help
            console::parser::print_help();
            exit(EXIT_FAILURE);
        }
        // Argument successfuly entered
        console::parser::_raw_input.insert(std::pair<std::string, std::string>(argv[i], argv[i+1])); 
    }
}

///////////////////////////////////////////////////////////////////////////////

inline void console::parser::print_help()
{
    std::cout << "Usage: " << console::parser::_prog_name << " [options]" << std::endl;

    std::cout << "Options: " << std::endl;
    for(auto it: console::parser::_arguments)
    {
        std::cout << "  " << it.first << ": " << it.second << std::endl;
    }
}

///////////////////////////////////////////////////////////////////////////////

#endif /* !PARSER_HPP */

///////////////////////////////////////////////////////////////////////////////

