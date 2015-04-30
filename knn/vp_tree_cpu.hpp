/*============================================================================*/
/*! \file vp_tree_cpu.hpp 
 *  \author Tiago LOBATO GIMENES            (tlgimenes@gmail.com)
 *  \date 2015-04-20 21:58
 *
 *  \brief vp_tree cpu class especification 
 *
 *  This file contains the cpu specialization of vp_tree class
 * */
/*============================================================================*/


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
/*! \brief Float comparision threshold 
 *
 * If the modulus of the difference between two floats is smaller than this
 * value the float values are considered to be equal */
#define EPSILON 1e-6 

#define LEAF -1  /*!< Leaf descriptor */
#define ROOT -2  /*!< Root descriptor */
#define UNDEF -3 /*!< Undefined node descriptor */

///////////////////////////////////////////////////////////////////////////////

namespace tree{
namespace cpu
{
    /*! \brief Base class for creating vp-tree */
    class vp_tree : public tree::vp_tree
    {
        public:
            /*! \brief Constructs a new empty tree */
            vp_tree();

            /*! \brief Constructs a new vp-tree
             *
             * \param data Data for creating vp-tree
             * \param dim Dimention of the data
             * \param metric metric function used in the vp-tree
             * */
            vp_tree(std::shared_ptr<const std::vector<float>> data, int dim, 
                    metric::cpu::metric_f metric = metric::cpu::euclidean);

            /*! \brief Copies another vp-tree to this object */
            vp_tree(const vp_tree& other);

            /*! \brief Constructs a new tree with the given data and dimention */
            inline void fit(std::shared_ptr<const std::vector<float>> data, int dim, 
                    metric::cpu::metric_f metric = metric::cpu::euclidean);

            /*!
             * \brief Performs the knn search and returns all elements within the 
             * radius delta of the query.
             *
             * This function uses a stack for speeding up the knn search and
             * it's not suitable for parellization in GPGPUs
             * \param query index of query in the data
             * \param delta maximum distance exclusive to search
             * \param id ids of elements in data closer to query than delta
             * */
            inline void stack_knn(int query, float delta, std::vector<int>& id);
            
            /*!
             * \brief Performs the knn search and returns all elements within the 
             * radius delta of the query.
             *
             * This function does not use any stack o recursion, what makes it
             * suitable for implementation in GPGPUs. This algorithm should be 
             * slower due to the lack of a stack and the necessity of recomputing
             * distances
             * \param query index of query in the data
             * \param delta maximum distance exclusive to search
             * \param id ids of elements in data closer to query than delta
             * */
            inline void knn(int query, float delta, std::vector<int>& id);

            /*!
             * \brief Performs the knn search and returns k elements closest to the 
             * query
             * 
             * This function uses a stack for speeding up the knn search and
             * it's not suitable for parellization in GPGPUs
             * */
            inline void stack_knn(int query, int k, std::vector<int>& id);

            /*!
             * \brief Performs the knn search and returns k elements closest to the 
             * query
             * 
             * This function does not use any stack o recursion, what makes it
             * suitable for implementation in GPGPUs. This algorithm should be 
             * slower due to the lack of a stack and the necessity of recomputing
             * distances
             * */
            inline void knn(int query, int k, std::vector<int>& id);

            /*!
             * \brief Same function as knn but using the brute force algorithm
             * */
            inline void brute_knn(int query, float delta, std::vector<int>& id);
            
            /*!
             * \brief Same function as knn but using the brute force algorithm
             * */
            inline void brute_knn(int query, int k, std::vector<int>& id);

            /*!
             * \brief Finds the query in _data vector and returns its index in the 
             * tree 
             * */
            inline int find(int query) const;

            /*!
             * \brief Brute force algorithm to check wheater a query belongs to the
             * tree or not 
             * */
            inline bool belongs(int query) const;

            /*!
             * \brief Get the tree
             * */
            inline const std::shared_ptr<std::vector<tree::vp_node>>& t() const {return _tree;}

            /*!
             * \brief Gets the metric function  
             * */
            inline const metric::cpu::metric_f& metric() const {return _metric;}

            /*! \brief Sets the metric function */
            inline metric::cpu::metric_f& metric() {return _metric;}

            /*! \brief prints the whole tree */
            void print_tree()
            {
                for(int i=0; i < _tree->size(); i++)
                    std::cout << "[" << i << "]: " << (*_tree)[i] << std::endl;
                std::cout << std::endl;
            }

            /*! \brief prints a range of the tree */
            void print_range(int b, int e)
            {
                e = std::min(e, (int)(*_tree).size());
                for(int i=b; i < e; i++)
                    std::cout << "[" << i << "]: " << (*_tree)[i] << std::endl;
                std::cout << std::endl;
            }

            /*! checks the tree for errors */
            void check_tree()
            {
                for(int i=0; i < (*_tree).size(); i++)
                    if((*_tree)[i]._lc == UNDEF || (*_tree)[i]._rc == UNDEF)
                        FATAL_ERROR("Problem in tree :(");
            }

        protected:
            /*!
             * \brief Evaluates the distance between p and the set index_set setting each
             * float in index_set
             * */
            inline void dist2(int p, std::vector<ifloat>& index_set) const;

            /*!
             * \brief Select among elements in index_set the vantage point to split the Tree
             * */
            inline int select_vp(const std::vector<ifloat>& index_set) const;

            /*!
             * \brief Splits the index_set in two sub sets such that lc and rc are
             * aproximately the same size
             * \return The distance used for splitting the two subsets
             * */
            inline float split(std::vector<ifloat>& index_set, 
                    std::vector<ifloat>& lc, std::vector<ifloat>& rc) const;

            /*!
             * \brief Constructs populating the _tree vector a vp_tree corresponding
             * to the data stored in the _data vector and specified in the index_set
             * */
            inline int make_vp_tree(std::vector<ifloat>& index_set);

            /*! \brief Tree structure is stored here 
             *
             * Vector containing the vp-tree representation of all the data stored
             * in _data vector of super class */
            std::shared_ptr<std::vector<tree::vp_node>> _tree; 

            /*! \brief Pointer to the metric function. 
             *
             * The metric function should return
             * the squared distance between two elements in the data */
            metric::cpu::metric_f _metric;
    };
};
};

///////////////////////////////////////////////////////////////////////////////

inline tree::cpu::vp_tree::vp_tree() :
    tree::vp_tree(),
    _tree(new std::vector<tree::vp_node>())
{
    /* Nothing to be done here */
}

inline tree::cpu::vp_tree::vp_tree(std::shared_ptr<const std::vector<float>> data, 
        int dim, metric::cpu::metric_f metric) :
    tree::vp_tree(data, dim),
    _metric(metric),
    _tree(new std::vector<tree::vp_node>())
{
    std::vector<ifloat> index_set;

    // Populates index set with data
    for(int i=0; i < _data->size(); i+=dim) 
        index_set.push_back(ifloat(i, 0.0f));

    // Creates the tree 
    make_vp_tree(index_set);
}

inline tree::cpu::vp_tree::vp_tree(const tree::cpu::vp_tree& other) :
    tree::vp_tree(other)
{
    _metric = other.metric();
    _tree = other.t();
}

///////////////////////////////////////////////////////////////////////////////

inline void tree::cpu::vp_tree::fit(std::shared_ptr<const std::vector<float>> data, 
        int dim, metric::cpu::metric_f metric)
{
    std::vector<ifloat> index_set;

    _metric = metric;
    _tree = std::make_shared<std::vector<tree::vp_node>>(std::vector<tree::vp_node>());

    this->fit(data, dim);

    // Populates index set with data
    for(int i=0; i < _data->size(); i+=dim) 
        index_set.push_back(ifloat(i, 0.0f));

    // Creates the tree 
    make_vp_tree(index_set);
}

///////////////////////////////////////////////////////////////////////////////

inline void tree::cpu::vp_tree::stack_knn(int query, float delta, std::vector<int>& id)
{
    std::stack<int> stack; // recursion stack
    int cmp = 0; // root 
    //int cmp = _tree.size() - 1; // root
    float dist;

    ASSERT_FATAL_ERROR(query < _data->size(), "Data doesn't contains the query");

    id.clear();
    stack.push(cmp);
    while(!stack.empty())
    {
        cmp = stack.top(); stack.pop();

        dist = _metric(query, (*_tree)[cmp]._key, *_data, _dim);

        if((*_tree)[cmp]._lc == LEAF && (*_tree)[cmp]._rc == LEAF) {// if leaf
            if(dist < delta)
                id.push_back((*_tree)[cmp]._key);
        }
        else // if node is not leaf 
        {
            if (dist <= (*_tree)[cmp]._d + delta)
                stack.push((*_tree)[cmp]._lc);
            if (dist >= (*_tree)[cmp]._d - delta)
                stack.push((*_tree)[cmp]._rc);
        } 
    }
}

inline void tree::cpu::vp_tree::knn(int query, float delta, std::vector<int>& id)
{
    int iteration = 0;
    register bool go_down = true;
    register int node = 0, parent;
    float dist;

    ASSERT_FATAL_ERROR(query < _data->size(), "Data doesn't contains the query");

    id.clear();
    do {
        if(go_down) {
            dist = _metric(query, _tree->at(node)._key, *_data, _dim);

            if(_tree->at(node)._lc == LEAF && _tree->at(node)._rc == LEAF) {// if leaf
                if(dist < delta) 
                    id.push_back(_tree->at(node)._key);
                go_down = false;
            }
            else if (dist <= _tree->at(node)._d + delta)
                node = _tree->at(node)._lc;
            else if (dist >= _tree->at(node)._d - delta)
                node = _tree->at(node)._rc;
            else {
                FATAL_ERROR("Error in the knn search");
            }
        }
        else {
            parent = _tree->at(node)._par;

            if(node == _tree->at(parent)._lc) {
                dist = _metric(query, _tree->at(parent)._key, *_data, _dim);

                if(dist >= _tree->at(parent)._d - delta) {
                    go_down = true;
                    node = _tree->at(parent)._rc;
                }
                else
                    node = _tree->at(node)._par;
            }
            else
                node = _tree->at(node)._par;
        }
    } while(node);
}

///////////////////////////////////////////////////////////////////////////////

inline void tree::cpu::vp_tree::stack_knn(int query, int k, std::vector<int>& id)
{
    std::vector<ifloat> heap;
    std::map<int, int> in_heap;
    std::stack<int> stack;
    float max_dist, dist;
    int cmp = 0;

    ASSERT_FATAL_ERROR(query < _data->size(), "Data doesn't contains the query");

    for(int i=0; i < k; i++) {// Sets the heap with initial guesses
        heap.push_back(ifloat(i*_dim, _metric(query, i*_dim, *_data, _dim)));
        in_heap[i*_dim] = i*_dim;
    }

    std::make_heap(heap.begin(), heap.end());
    max_dist = heap.front().val();
    stack.push(cmp);
    while(!stack.empty())
    {
        cmp = stack.top(); stack.pop();

        dist = _metric(query, (*_tree)[cmp]._key, *_data, _dim);
 
        if((*_tree)[cmp]._lc == LEAF && (*_tree)[cmp]._rc == LEAF) {// if leaf
            if(dist < max_dist && !in_heap.count((*_tree)[cmp]._key)) {
                in_heap.erase(heap.front().key());
                std::pop_heap(heap.begin(), heap.end()); heap.pop_back();
                heap.push_back(ifloat((*_tree)[cmp]._key, dist));
                in_heap[(*_tree)[cmp]._key] = (*_tree)[cmp]._key;
                std::push_heap(heap.begin(), heap.end());
                max_dist = heap.front().val();
            }
        }
        else // if node is not leaf 
        {
            if (dist <= (*_tree)[cmp]._d + max_dist)
                stack.push((*_tree)[cmp]._lc);
            if (dist >= (*_tree)[cmp]._d - max_dist)
                stack.push((*_tree)[cmp]._rc);
        } 
    }

    id.clear();
    for(int i=0; i < heap.size(); i++)
        id.push_back(heap[i].key());
}

///////////////////////////////////////////////////////////////////////////////

inline void tree::cpu::vp_tree::knn(int query, int k, std::vector<int>& id)
{
    register bool go_down = true;
    register int node = 0, parent;
    std::vector<ifloat> heap;
    std::map<int, int> in_heap;
    float dist, max_dist;

    ASSERT_FATAL_ERROR(query < _data->size(), "Data doesn't contains the query");

    for(int i=0; i < k; i++) {// Sets the heap with initial guesses
        heap.push_back(ifloat(i*_dim, _metric(query, i*_dim, *_data, _dim)));
        in_heap[i*_dim] = i*_dim;
    }

    std::make_heap(heap.begin(), heap.end());
    max_dist = heap.front().val();
    do {
        if(go_down) {
            dist = _metric(query, _tree->at(node)._key, *_data, _dim);

            if(_tree->at(node)._lc == LEAF && _tree->at(node)._rc == LEAF) {// if leaf 
                if(dist < max_dist && !in_heap.count((*_tree)[node]._key)) {
                    in_heap.erase(heap.front().key());
                    std::pop_heap(heap.begin(), heap.end()); heap.pop_back();
                    heap.push_back(ifloat((*_tree)[node]._key, dist));
                    in_heap[(*_tree)[node]._key] = (*_tree)[node]._key;
                    std::push_heap(heap.begin(), heap.end());
                    max_dist = heap.front().val();
                }
                go_down = false;
            }
            else if (dist <= _tree->at(node)._d + max_dist)
                node = _tree->at(node)._lc;
            else if (dist >= _tree->at(node)._d - max_dist)
                node = _tree->at(node)._rc;
            else {
                FATAL_ERROR("Error in the knn search");
            }
        }
        else {
            parent = _tree->at(node)._par;

            if(node == _tree->at(parent)._lc) {
                dist = _metric(query, _tree->at(parent)._key, *_data, _dim);

                if(dist >= _tree->at(parent)._d - max_dist) {
                    go_down = true;
                    node = _tree->at(parent)._rc;
                }
                else
                    node = _tree->at(node)._par;
            }
            else
                node = _tree->at(node)._par;
        }
    } while(node);

    id.clear();
    for(int i=0; i < heap.size(); i++)
        id.push_back(heap[i].key());
}



///////////////////////////////////////////////////////////////////////////////

inline void tree::cpu::vp_tree::brute_knn(int query, float delta, std::vector<int>& id)
{
    float dist;
 
    ASSERT_FATAL_ERROR(query < _data->size(), "Data doesn't contains the query");

    id.clear();
    for(int i=0; i < _data->size(); i+=_dim) {
        dist = _metric(query, i, *_data, _dim);

        if(dist < delta) 
            id.push_back(i);
    }
}

///////////////////////////////////////////////////////////////////////////////

inline void tree::cpu::vp_tree::brute_knn(int query, int k, std::vector<int>& id)
{
    float dist, max_dist;
    std::vector<ifloat> heap;

    ASSERT_FATAL_ERROR(query < _data->size(), "Data doesn't contains the query");

    for(int i=0; i < k; i++)
        heap.push_back(ifloat(i*_dim, _metric(query, i*_dim, *_data, _dim)));

    id.clear();
    std::make_heap(heap.begin(), heap.end());
    max_dist = heap.front().val();

    for(int i=k*_dim; i < _data->size(); i+=_dim) {
        dist = _metric(query, i, *_data, _dim);

        if(dist < max_dist) {
            std::pop_heap(heap.begin(), heap.end()); heap.pop_back();
            heap.push_back(ifloat(i, dist));
            std::push_heap(heap.begin(), heap.end());
            max_dist = heap.front().val();
        }
    }

    for(int i=0; i < heap.size(); i++)
        id.push_back(heap[i].key());
}

///////////////////////////////////////////////////////////////////////////////

inline int tree::cpu::vp_tree::find(int query) const
{
    //int cmp = _tree.size() - 1;
    int cmp = 0;

    ASSERT_FATAL_ERROR(query < _data->size(), "Data doesn't contains the query");

    while((*_tree)[cmp]._key != query || (*_tree)[cmp]._lc != LEAF || (*_tree)[cmp]._rc != LEAF)
    {
        if((*_tree)[cmp]._lc == LEAF || (*_tree)[cmp]._rc == LEAF) {
            std::cout << query << "," << cmp << ": " << (*_tree)[cmp] << std::endl;
            FATAL_ERROR("Query not found ! :`(");
        }

        if(_metric((*_tree)[cmp]._key, query, *_data, _dim) < (*_tree)[cmp]._d)
            cmp = (*_tree)[cmp]._lc;
        else
            cmp = (*_tree)[cmp]._rc;
    }

    return cmp;
}

///////////////////////////////////////////////////////////////////////////////

inline bool tree::cpu::vp_tree::belongs(int query) const
{
    ASSERT_FATAL_ERROR(query < _data->size(), "Data doesn't contains the query");

    for(int i=0; i < (*_tree).size(); i++)
    {
        if((*_tree)[i]._key == query && (*_tree)[i]._lc == -1 && (*_tree)[i]._rc == -1) {
            std::cout << i << std::endl;
            return true;
        }
    }
    
    return false;
}

///////////////////////////////////////////////////////////////////////////////
/*!
 * Basic implementation still. Don't see the point for a more complicated code 
 * */
inline int tree::cpu::vp_tree::select_vp(const std::vector<ifloat>& index_set) const
{
    return index_set.front().key();
}

///////////////////////////////////////////////////////////////////////////////

inline float tree::cpu::vp_tree::split(std::vector<ifloat>& index_set, 
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

    l_set.clear(); r_set.clear();
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

inline int tree::cpu::vp_tree::make_vp_tree(std::vector<ifloat>& index_set)
{
    std::vector<ifloat> l_set, r_set;
    std::stack<std::pair<std::vector<ifloat>, int>> stack;
    std::vector<ifloat>& set_aux = index_set;
    int p = 0, parent;
    float mu = 0.0f;

    p = this->select_vp(set_aux);

    if(set_aux.size() <= 1)
        (*_tree).push_back(tree::vp_node(p, 0, LEAF, LEAF, ROOT));
    else {
        this->dist2(p, set_aux);
        mu = this->split(set_aux, l_set, r_set);

        stack.push(std::pair<std::vector<ifloat>, int>(l_set, (*_tree).size()));
        stack.push(std::pair<std::vector<ifloat>, int>(r_set, (*_tree).size()));

        (*_tree).push_back(tree::vp_node(p, mu, UNDEF, UNDEF, ROOT));
    }

    while(!stack.empty())
    {
        set_aux = stack.top().first;
        parent  = stack.top().second;
        stack.pop();

        p = this->select_vp(set_aux);

        if(set_aux.size() == 1)
            (*_tree).push_back(tree::vp_node(p, 0, LEAF, LEAF, parent));
        else {
            this->dist2(p, set_aux);
            mu = this->split(set_aux, l_set, r_set);

            stack.push(std::pair<std::vector<ifloat>, int>(l_set, (*_tree).size()));
            stack.push(std::pair<std::vector<ifloat>, int>(r_set, (*_tree).size()));
        
            (*_tree).push_back(tree::vp_node(p, mu, UNDEF, UNDEF, parent));
        }
 
        if((*_tree)[parent]._rc == UNDEF)
            (*_tree)[parent]._rc = (*_tree).size()-1;
        else if((*_tree)[parent]._lc == UNDEF)
            (*_tree)[parent]._lc = (*_tree).size()-1;
        else
            FATAL_ERROR("Binary node has more than two childs :(");
    }
}

///////////////////////////////////////////////////////////////////////////////

inline void tree::cpu::vp_tree::dist2(int p, std::vector<ifloat>& index_set) const
{
    for(int i=0; i < index_set.size(); i++)
        index_set[i].val() = _metric(p, index_set[i].key(), *_data, _dim);
}

///////////////////////////////////////////////////////////////////////////////

#endif /* !VP_TREE_CPU_HPP */

///////////////////////////////////////////////////////////////////////////////

