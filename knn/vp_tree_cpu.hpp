/*
 * ============================================================================
 *       Filename:  vp_tree_cpu.hpp
 *    Description:  
 *        Created:  2015-04-20 21:58
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * ============================================================================
 */

///////////////////////////////////////////////////////////////////////////////

#ifndef VP_TREE_CPU_HPP
#define VP_TREE_CPU_HPP

///////////////////////////////////////////////////////////////////////////////

#include <algorithm>

#include "vp_tree.hpp"
#include "metrics.hpp"
#include "types.hpp"

#include "time.hpp"

///////////////////////////////////////////////////////////////////////////////

#define EPSILON 1e-6

#define LEAF -1
#define ROOT -2
#define UNDEF -3

///////////////////////////////////////////////////////////////////////////////

namespace cpu
{
    class vp_tree : public tree
    {
        public:
            vp_tree(const std::vector<float>& data, int dim, 
              float (*metric)(int,int,const std::vector<float>&,int)=euclidean2);

            /**
             * Finds the query in _data vector and returns its index in the 
             * _tree 
             * */
            inline int find(int query) const;

            /**
             * Brute force algorithm to check wheater a query belongs to the
             * tree or not 
             * */
            inline bool belongs(int query) const;

            /**
             * Get the tree
             * */
            inline const std::vector<vp_node>& t() const {return _tree;}

        protected:
            /**
             * Evaluates the distance between p and the set index_set setting each
             * float in index_set
             * */
            inline void dist2(int p, std::vector<ifloat>& index_set) const;

            /**
             * Select among elements in index_set the vantage point to split the Tree
             * */
            inline int select_vp(const std::vector<ifloat>& index_set) const;

            /**
             * Splits the index_set in two sub sets such that lc and rc are
             * aproximately the same size
             * @returns: The distance used for splitting the two subsets
             * */
            inline float split(std::vector<ifloat>& index_set, 
                    std::vector<ifloat>& lc, std::vector<ifloat>& rc) const;

            /**
             * Constructs populating the _tree vector a vp_tree corresponding
             * to the data stored in the _data vector and specified in the index_set
             * */
            inline int make_vp_tree(std::vector<ifloat>& index_set);

            void print_tree()
            {
                for(int i=0; i < _tree.size(); i++)
                    std::cout << "[" << i << "]: " << _tree[i] << std::endl;
                std::cout << std::endl;
            }

            void print_range(int b, int e)
            {
                e = std::min(e, (int)_tree.size());
                for(int i=b; i < e; i++)
                    std::cout << "[" << i << "]: " << _tree[i] << std::endl;
                std::cout << std::endl;
            }

            void check_tree()
            {
                for(int i=0; i < _tree.size(); i++)
                    if(_tree[i]._lc == UNDEF || _tree[i]._rc == UNDEF)
                        FATAL_ERROR("Problem in tree :(");
            }

            /* Vector containing the vp-tree representation of all the data stored
             * in _data vector of super class */
            std::vector<vp_node> _tree; // Tree structure is stored here :)

            /* Pointer to the metric function. The metric function should return
             * the squared distance between two elements in the data */
            float (*_metric)(int, int, const std::vector<float>&, int);
    };
}

///////////////////////////////////////////////////////////////////////////////

inline cpu::vp_tree::vp_tree(const std::vector<float>& data, int dim,
        float (*metric)(int, int, const std::vector<float>&, int)) :
    tree(data, dim),
    _metric(metric)
{
    std::vector<ifloat> index_set;

    // Populates index set with data
    for(int i=0; i < data.size(); i+=dim) 
    {
        index_set.push_back(ifloat(i, 0.0f));
    }

    // Creates the tree 
    TIME_BETWEEN(
    make_vp_tree(index_set);
    );

    //print_tree();
    check_tree();

    TIME_BETWEEN(
    int f;
    for(int i=0; i < data.size(); i+=dim)
    {
        //std::cout << i/dim << std::endl;
        f = find(i);
        //std::cout << "find(" << i << "): " << f << " -> ";
        //std::cout << "tree[" << f << "]: " << _tree[f] << std::endl;
    }
    );

    /*int i=6803*66;
    
    std::cout << belongs(i) << std::endl;

    print_range(90560, 90586);

    int f = find(i);
    std::cout << "find(" << i << "): " << f << " -> ";
    std::cout << "tree[" << f << "]: " << _tree[f] << std::endl;
    */
}

///////////////////////////////////////////////////////////////////////////////

inline int cpu::vp_tree::find(int query) const
{
    //int cmp = _tree.size() - 1;
    int cmp = 0;

    while(_tree[cmp]._key != query || _tree[cmp]._lc != LEAF || _tree[cmp]._rc != LEAF)
    {
        if(_tree[cmp]._lc == LEAF || _tree[cmp]._rc == LEAF) {
            std::cout << query << "," << cmp << ": " << _tree[cmp] << std::endl;
            FATAL_ERROR("Query not found ! :`(");
        }

        //std::cout << "[" << cmp << "]: " << _tree[cmp] << " -> " << dist2(_tree[cmp]._key, query) << std::endl;
        if(_metric(_tree[cmp]._key, query, _data, _dim) < _tree[cmp]._d)
            cmp = _tree[cmp]._lc;
        else
            cmp = _tree[cmp]._rc;
    }

    return cmp;
}

///////////////////////////////////////////////////////////////////////////////

inline bool cpu::vp_tree::belongs(int query) const
{
    for(int i=0; i < _tree.size(); i++)
    {
        if(_tree[i]._key == query && _tree[i]._lc == -1 && _tree[i]._rc == -1) {
            std::cout << i << std::endl;
            return true;
        }
    }
    
    return false;
}

///////////////////////////////////////////////////////////////////////////////
/**
 * Basic implementation still. Don't see the point for a more complicated code 
 * */
inline int cpu::vp_tree::select_vp(const std::vector<ifloat>& index_set) const
{
    return index_set.front().key();
}

///////////////////////////////////////////////////////////////////////////////

inline float cpu::vp_tree::split(std::vector<ifloat>& index_set, 
        std::vector<ifloat>& l_set, std::vector<ifloat>& r_set) const
{
    int middle = index_set.size() / 2;

    std::sort(index_set.begin(), index_set.end());
    
    for(int i=middle+1; i < index_set.size(); i++) {
        if(std::abs(index_set[i].val() - index_set[i-1].val()) > EPSILON) {
            //std::cout << index_set[i-1].val() << " " << index_set[i].val() << std::endl;
            middle = i;
            break;
        }
    }

    l_set.assign(index_set.begin(), index_set.begin()+middle);
    r_set.assign(index_set.begin()+middle, index_set.end());

    return index_set[middle].val();
}

///////////////////////////////////////////////////////////////////////////////

/*inline int cpu::vp_tree::make_vp_tree(std::vector<ifloat>& index_set)
{
    std::vector<ifloat> l_set, r_set;
    int p = 0;
    float mu = 0.0f;

    if(index_set.size() == 1) {
        _tree.push_back(vp_node(index_set[0].key(), 0, -1, -1));
        return _tree.size()-1;
    }

    p =  this->select_vp(index_set);

    this->dist2(p, index_set);

    mu = this->split(index_set, l_set, r_set);

    _tree.push_back(vp_node(p, mu, make_vp_tree(l_set), make_vp_tree(r_set)));

    return _tree.size()-1;
}*/

inline int cpu::vp_tree::make_vp_tree(std::vector<ifloat>& index_set)
{
    std::vector<ifloat> l_set, r_set;
    std::stack<std::pair<std::vector<ifloat>, int>> stack;
    std::vector<ifloat>& set_aux = index_set;
    int p = 0, parent;
    float mu = 0.0f;

    p = this->select_vp(set_aux);

    if(set_aux.size() <= 1)
        _tree.push_back(vp_node(p, 0, LEAF, LEAF));
    else {
        this->dist2(p, set_aux);
        mu = this->split(set_aux, l_set, r_set);

        stack.push(std::pair<std::vector<ifloat>, int>(l_set, _tree.size()));
        stack.push(std::pair<std::vector<ifloat>, int>(r_set, _tree.size()));

        _tree.push_back(vp_node(p, mu, UNDEF, UNDEF));
    }

    while(!stack.empty())
    {
        set_aux = stack.top().first;
        parent  = stack.top().second;
        stack.pop();

        p = this->select_vp(set_aux);

        if(set_aux.size() == 1)
            _tree.push_back(vp_node(p, 0, LEAF, LEAF));
        else {
            this->dist2(p, set_aux);
            mu = this->split(set_aux, l_set, r_set);

            stack.push(std::pair<std::vector<ifloat>, int>(l_set, _tree.size()));
            stack.push(std::pair<std::vector<ifloat>, int>(r_set, _tree.size()));
        
            _tree.push_back(vp_node(p, mu, UNDEF, UNDEF));
        }
 
        if(_tree[parent]._rc == UNDEF)
            _tree[parent]._rc = _tree.size()-1;
        else if(_tree[parent]._lc == UNDEF)
            _tree[parent]._lc = _tree.size()-1;
        else
            FATAL_ERROR("Binary node has more than two childs :(");
    }
}

///////////////////////////////////////////////////////////////////////////////

inline void cpu::vp_tree::dist2(int p, std::vector<ifloat>& index_set) const
{
    for(int i=0; i < index_set.size(); i++)
    {
        index_set[i].val() = _metric(p, index_set[i].key(), _data, _dim);
    }
}

///////////////////////////////////////////////////////////////////////////////

#endif /* !VP_TREE_CPU_HPP */

///////////////////////////////////////////////////////////////////////////////

