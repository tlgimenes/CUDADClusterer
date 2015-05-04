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

#include "dbscan_cpu.hpp"

///////////////////////////////////////////////////////////////////////////////

template <typename T>
inline bool contains(const std::vector<T>& v, const T& element)
{
    for(int i=0; i < v.size(); i++)
    {
        if(v[i] == element)
            return true;
    }
    
    return false;
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
inline bool cmp_vec(const std::vector<T>& v1, const std::vector<T>& v2)
{
    bool ret = true;

    if(v1.size() != v2.size()) 
    {
        std::cout << "Vectors must have the same sizes (" << v1.size() << "," << v2.size() << ")" << std::endl;
        return false;
    }

    for(int i=0; i < v1.size(); i++)
    {
        ret &= contains<T>(v2, v1[i]);
        if(!ret) {
            std::cout << "vector 2 does not contain " << v1[i] << std::endl;
            return false;
        }
    }

    return ret;
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
inline void print_vec(std::vector<T>& vec)
{
    for(int i=0; i < vec.size(); i++)
        std::cout << "|" << vec[i];
    std::cout << std::endl;
}

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

void test_tree(int query, int kn, float dist, const tree::cpu::vp_tree* vptree);

///////////////////////////////////////////////////////////////////////////////

/* ======= Function ==================================================
 *   Name: main
 *   Description: main entry Function
 * =================================================================== 
 */
int main(int argc, const char **argv)
{
    /* Adds arguments to the parser */
    console::parser::add_argument("-t", "List of trajectory files to read in, separated by spaces.");
    console::parser::add_argument("-a", "List of atom index files to read in, separated by spaces.");
    console::parser::add_argument("-p", "Topology file.");
    console::parser::add_argument("-o", "Home dir.");
    console::parser::add_argument("-k", "Resolution of the cluster algorithm");
    console::parser::add_argument("-m", "Min samples for Density based clustering algorithm");
    console::parser::add_argument("-e", "Percentage to keep in each iteration");

    console::parser::parse(argc, argv); // Parses the input parameters

    /* Starts parameters of the program */
    std::string trajlist = console::parser::get("-t", true);
    std::string atom_index = console::parser::get("-a", true);
    std::string topology = console::parser::get("-p", true);
    std::string home_dir = (console::parser::get("-o", false).size() == 1 ? "" : console::parser::get("-o", false));
    int k = std::stoi(console::parser::get("-k", true));
    int m = std::stoi(console::parser::get("-m", true));
    int e = std::stof(console::parser::get("-e", true));

    /* Reads the trajectory list and acquires the data and number of atoms */
    std::vector<float> data;
    int n_atoms;

    reader_xtc::read_list(home_dir, trajlist, data, n_atoms);

    std::shared_ptr<const std::vector<float>> shared_data = std::make_shared<const std::vector<float>>(data);

    TIME_BETWEEN(
    tree::cpu::vp_tree* vptree = new tree::cpu::vp_tree(shared_data, n_atoms * 3);
    )

    float dist = 0.51;
    int kn = 5, query = 132;
    test_tree(query, kn, dist, vptree);

    TIME_BETWEEN(
    cluster::cpu::dbscan* dbscan = new cluster::cpu::dbscan(shared_data, dist, kn, n_atoms * 3, *vptree);
    )

    //print_data(data, 123);

    delete (tree::cpu::vp_tree*)vptree;

    return 0;
}

///////////////////////////////////////////////////////////////////////////////

void test_tree(int query, int kn, float dist, const tree::cpu::vp_tree* vptree)
{
    std::vector<int> id1, id2;
 
    std::cout << console::modifier(console::FG_MAGENTA) << "knn epsilon:" << 
        console::modifier(console::FG_DEFAULT) << std::endl;
 
    TIME_BETWEEN(
    vptree->stack_knn(query, dist, id1);
    )
    print_vec<int>(id1);

    TIME_BETWEEN(
    vptree->knn(query, dist, id1);
    )
    print_vec<int>(id1);

    std::cout << console::modifier(console::FG_MAGENTA) << "brute knn epsilon:" << 
        console::modifier(console::FG_DEFAULT) << std::endl;
    TIME_BETWEEN(
    vptree->brute_knn(query, dist, id2);
    )
    print_vec<int>(id2);

    if(cmp_vec<int>(id2, id1))
        DBG_MESSAGE("Brute and knn algorithms are equal ! :)\n");
    else
        DBG_MESSAGE("Error in knn algorithm :(\n");

    std::cout << console::modifier(console::FG_MAGENTA) << "knn:" << 
        console::modifier(console::FG_DEFAULT) << std::endl;
 
    TIME_BETWEEN(
    vptree->stack_knn(query, kn, id1);
    )
    print_vec<int>(id1);

    TIME_BETWEEN(
    vptree->knn(query, kn, id1);
    )
    print_vec<int>(id1);

    std::cout << console::modifier(console::FG_MAGENTA) << "brute knn:" << 
        console::modifier(console::FG_DEFAULT) << std::endl;
    TIME_BETWEEN(
    vptree->brute_knn(query, kn, id2);
    )
    print_vec<int>(id2);

    if(cmp_vec<int>(id2, id1))
        DBG_MESSAGE("Brute and knn algorithms are equal ! :)\n");
    else
        DBG_MESSAGE("Error in knn algorithm :(\n");
}

///////////////////////////////////////////////////////////////////////////////
