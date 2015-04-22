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

#include "vp_tree_cpu.hpp"

///////////////////////////////////////////////////////////////////////////////
/**
 * Function for visualizating max elements of the data of n_dimentions 
 * */
void print_data(const std::vector<float>& data, int max = 20, int dim = 3)
{
    for(int i=0; i < data.size() && i < dim*max; i+=dim)
    {
        std::cout << "(" << data[i] << ", ";
        for(int j=1; j < dim; j++)
            std::cout << ", " << data[i+j];
        std::cout << ")" << std::endl;
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
    /* Adds arguments to the parser */
    parser::add_argument("-t", "List of trajectory files to read in, separated by spaces.");
    parser::add_argument("-a", "List of atom index files to read in, separated by spaces.");
    parser::add_argument("-p", "Topology file.");
    parser::add_argument("-o", "Home dir.");
    parser::add_argument("-k", "Resolution of the cluster algorithm");
    parser::add_argument("-m", "Min samples for Density based clustering algorithm");
    parser::add_argument("-e", "Percentage to keep in each iteration");

    parser::parse(argc, argv); // Parses the input parameters

    /* Starts parameters of the program */
    std::string trajlist = parser::get("-t", true);
    std::string atom_index = parser::get("-a", true);
    std::string topology = parser::get("-p", true);
    std::string home_dir = (parser::get("-o", false).size() == 1 ? "" : parser::get("-o", false));
    int k = std::stoi(parser::get("-k", true));
    int m = std::stoi(parser::get("-m", true));
    int e = std::stof(parser::get("-e", true));

    /* Reads the trajectory list and acquires the data and number of atoms */
    std::vector<float> data;
    int n_atoms;

    reader_xtc::read_list(home_dir, trajlist, data, n_atoms);

    tree* vptree = new cpu::vp_tree(data, n_atoms * 3);
    
    //print_data(data, 123);
    
    delete (cpu::vp_tree*)vptree;

    return 0;
}

///////////////////////////////////////////////////////////////////////////////

