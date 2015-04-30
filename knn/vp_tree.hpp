/*============================================================================*/
/*! \file vp_tree.hpp 
 *  \author Tiago LOBATO GIMENES            (tlgimenes@gmail.com)
 *  \date 2015-04-20 18:09
 *
 *  \brief vp_tree base class implementation 
 *
 *  This file contains base class implementation of vp_tree 
 * */
/*============================================================================*/

///////////////////////////////////////////////////////////////////////////////

#ifndef VP_TREE_HPP
#define VP_TREE_HPP

///////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <memory>

#include "error.hpp"

///////////////////////////////////////////////////////////////////////////////

namespace tree
{
    /*! \brief vp-tree node for a linearized tree in an array */
    struct vp_node_t
    {
        /*! \brief Creates a new node  
         *
         * \param k index in data vector
         * \param d distance threshold if it's an internal node
         * \param lc index in tree vector of left child
         * \param rc index in tree vector or right child
         * \param par index in tree vector of the parent node (ROOT if node is
         * root)
         * */
        vp_node_t(int k = 0, float d = 0.0f, int lc = 0, int rc = 0, int par = 0) : 
            _key(k), _d(d), _lc(lc), _rc(rc), _par(par) {}

        int _key; /*!< index in _data */
        float _d; /*!< distance threshold */
        int _lc;  /*!< left child index */
        int _rc;  /*!< right child index */
        int _par; /*!< parent node */
    };

///////////////////////////////////////////////////////////////////////////////

        using vp_node = struct vp_node_t; /*!< \brief vp-tree node typedef */

///////////////////////////////////////////////////////////////////////////////

    /*! \brief Base class for creating vp-tree */
    class vp_tree
    {
        public:
            /*! \brief Constructs a new empty tree */
            vp_tree();
            /*! \brief Constructs a tree with the new data and dimention */
            vp_tree(std::shared_ptr<const std::vector<float>> data, int dim);
            /*! \brief Constructs new tree based on existing tree */
            vp_tree(const vp_tree& other);

            /*! \brief Constructs a new tree with the given data and dimention */
            inline void fit(std::shared_ptr<const std::vector<float>> data, int dim);

            /*! \brief Get data vector */
            inline const std::shared_ptr<const std::vector<float>>& data() const;
            /*! \brief Get dimention */
            inline int dim() const {return _dim;}

            /*! \brief Set data vector 
             *
             *  Use this function as your own responsability 
             *  */
            inline std::shared_ptr<const std::vector<float>>& data();
            /*! \brief Set dimention  
             *
             * Use this function as your own responsability
             * */
            inline int& dim() {return _dim;}

        protected:
            std::shared_ptr<const std::vector<float>> _data; /*!< data */

            int _dim; /*!< Dimention of data contained in data */
    };
};

///////////////////////////////////////////////////////////////////////////////

inline tree::vp_tree::vp_tree() : 
    _data(), _dim()
{
    /* Nothing to do here !! */
}

///////////////////////////////////////////////////////////////////////////////

inline tree::vp_tree::vp_tree(std::shared_ptr<const std::vector<float>> data, 
        int dim) :
    _data(data),
    _dim(dim)
{
    if((_data->size() % dim))
        FATAL_ERROR("Data size and dimention not compatible");
}

///////////////////////////////////////////////////////////////////////////////

inline tree::vp_tree::vp_tree(const vp_tree& other)
{
    _data = other.data();
    _dim = other.dim();
}

///////////////////////////////////////////////////////////////////////////////

inline void tree::vp_tree::fit(std::shared_ptr<const std::vector<float>> data,
        int dim)
{
    _data = data;
    _dim = dim;

    if((_data->size() % dim))
        FATAL_ERROR("Data size and dimention not compatible");
}

///////////////////////////////////////////////////////////////////////////////

inline const std::shared_ptr<const std::vector<float>>& tree::vp_tree::data() const
{
    return _data;
}

///////////////////////////////////////////////////////////////////////////////

inline std::shared_ptr<const std::vector<float>>& tree::vp_tree::data()
{
    return _data;
}

///////////////////////////////////////////////////////////////////////////////

/*! 
 * \brief function for printing vp_node struct
 * */
inline std::ostream& operator<< (std::ostream& in, const tree::vp_node& n)
{
    in << "(" << n._key << "; " << n._d << "; " << n._lc << "; " << n._rc << 
        "; " << n._par << ")";
    return in;
}

///////////////////////////////////////////////////////////////////////////////

#endif /* !VP_TREE_HPP */

///////////////////////////////////////////////////////////////////////////////

