/*
 * ============================================================================
 *       Filename:  vp_tree.hpp
 *    Description:  
 *        Created:  2015-04-20 18:09
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * ============================================================================
 */

///////////////////////////////////////////////////////////////////////////////

#ifndef VP_TREE_HPP
#define VP_TREE_HPP

///////////////////////////////////////////////////////////////////////////////

#include <vector>

#include "error.hpp"

///////////////////////////////////////////////////////////////////////////////

struct vp_node_t
{
    vp_node_t(int k = 0, float d = 0.0f, int lc = 0, int rc = 0) : 
        _key(k), _d(d), _lc(lc), _rc(rc) {}

    int _key; // index in _data
    float _d; // distance threshold
    int _lc;  // left child index
    int _rc;  // right child index
};

///////////////////////////////////////////////////////////////////////////////

using vp_node = struct vp_node_t;

///////////////////////////////////////////////////////////////////////////////

class tree
{
    public:
        tree(const std::vector<float>& data, int dim);

    protected:
        const std::vector<float>& _data;

        int _dim; // Dimention of data contained in data
};

///////////////////////////////////////////////////////////////////////////////

inline tree::tree(const std::vector<float>& data, int dim) :
    _data(data),
    _dim(dim)
{
    if((_data.size() % dim))
        FATAL_ERROR("Data size and dimention not compatible");
}

///////////////////////////////////////////////////////////////////////////////

inline std::ostream& operator<< (std::ostream& in, const vp_node& n)
{
    in << "(" << n._key << "; " << n._d << "; " << n._lc << "; " << n._rc << ")";
    return in;
}

///////////////////////////////////////////////////////////////////////////////

#endif /* !VP_TREE_HPP */

///////////////////////////////////////////////////////////////////////////////

