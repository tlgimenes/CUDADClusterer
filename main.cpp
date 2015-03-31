/*
 * ============================================================================
 *       Filename:  main.cpp
 *    Description:  Main file for calling functions
 *        Created:  2015-03-30 14:53 
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * ============================================================================ 
*/

///////////////////////////////////////////////////////////////////////////////

#include "parser.hpp"
#include "reader_xtc.hpp"

///////////////////////////////////////////////////////////////////////////////

void print_data(const std::vector<float>& data, int max)
{
    for(int i=0; i < data.size() && i < 3*max; i+=3)
    {
        std::cout << "(" << data[i] << ", " << data[i+1] << ", " << data[i+2] << ")" 
            << std::endl;
        if(!(((i+3)/3)%(22)))
            std::cout << std::endl;
    }
    std::cout << std::endl;
}

///////////////////////////////////////////////////////////////////////////////

/* ======= Function ==================================================
 *   Name: main
 *   Description: main entry Function
 * =================================================================== 
 */
int main(int argc, const char **argv)
{
    /* Calls the parser */
    Parser::add_argument("-t", "List of trajectory files to read in, separated by spaces.");
    Parser::add_argument("-a", "List of atom index files to read in, separated by spaces.");
    Parser::add_argument("-p", "topology file.");
    Parser::add_argument("-o", "Home dir.");
    Parser::add_argument("-k", "Number of clusters to generate.");
    Parser::add_argument("-r", "Use Random seed");
    Parser::add_argument("-s", "Seed of cluster.");
    Parser::add_argument("-x", "Number of snapshots in each xtc file.");
    Parser::add_argument("-e", "The file extension of input trajectory files. Must be a filetype that mdtraj.load() can recognize.");
    Parser::add_argument("-l", "Superpose each conformation in this trajectory upon a reference.");

    Parser::parse(argc, argv);

    std::string trajlist = Parser::get("-t", true);
    std::string atom_index = Parser::get("-a", true);
    std::string topology = Parser::get("-p", true);
    std::string home_dir = (Parser::get("-o", false).size() == 1 ? "" : Parser::get("-o", false));
    int k = std::stoi(Parser::get("-k", true));
    int seed = std::stoi(Parser::get("-r", false));
    int seed_c = std::stoi(Parser::get("-s", false));
    int snapshots = std::stoi(Parser::get("-x", false));
    std::string ext = Parser::get("-e", false);
    int ref = std::stoi(Parser::get("-l", false));

    std::vector<float> data;
    int n_atoms;

    ReaderXTC::read_list(home_dir, trajlist, data, n_atoms);

    //print_data(data, 123123);

    return 0;
}

///////////////////////////////////////////////////////////////////////////////

