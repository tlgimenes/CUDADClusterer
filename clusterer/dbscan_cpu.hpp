/*============================================================================*/
/*! \file dbscan_cpu.hpp 
 *  \author Tiago LOBATO GIMENES            (tlgimenes@gmail.com)
 *  \date 2015-04-28 13:53
 *
 *  \brief Definition of the class specialization of dbscan for CPU
 *
 *  This file contains the implementation of the class specialization for the 
 *  dbscan clustering algorithm on CPU
 * */
/*============================================================================*/

///////////////////////////////////////////////////////////////////////////////

#ifndef DBSCAN_CPU_HPP
#define DBSCAN_CPU_HPP

///////////////////////////////////////////////////////////////////////////////

#include <cstring>

#include "dbscan.hpp"
#include "metrics.hpp"
#include "vp_tree_cpu.hpp"

///////////////////////////////////////////////////////////////////////////////

namespace cluster {
namespace cpu
{
    /*! \brief Class for applying DBSCAN clustering algorithm to data */
    class dbscan : public cluster::dbscan
    {
        public:
            /*! \brief Constructs new empty dbscan clusterer */
            dbscan();

            /*! \brief Constructor of the clusterer 
             *
             * \param data Data array 
             * \param eps Epsilon distance parameter of the DBSCAN algorithm 
             * \param min_pts Minimal number of points for the DBSCAN algorithm
             * \param dim Dimention of the data 
             * \param metric Metric function to use for the DBSCAN algorithm */
            dbscan(std::shared_ptr<const std::vector<float>> data, 
                    const float eps, const int min_pts, const int dim, 
                    metric::cpu::metric_f metric = metric::cpu::euclidean);

            /*! \brief Constructs new dbscan clusterer based on an existing tree */
            dbscan(std::shared_ptr<const std::vector<float>> data, 
                    const float eps, const int min_pts, const int dim, 
                    const tree::cpu::vp_tree& tree);

            /*! \brief Constructs copy dbscan from another dbscan with same
             * parameters, data and tree */
            dbscan(const dbscan& other);

            /*! \brief Fits the new data to the dbscan clusterer */
            inline void fit(std::shared_ptr<const std::vector<float>> data, int dim);

            /*! \brief uses the fitted data to produce the assignements */
            inline void predict(std::vector<int>& assignements) const;

            /*! \brief Get the metric function used in the tree */
            inline const metric::cpu::metric_f& metric() const {return _tree.metric();}

            /*! \brief Sets the metric function used in the tree */
            inline metric::cpu::metric_f& metric() {return _tree.metric();}

            /*! \brief Gets the vp-tree */
            inline const tree::cpu::vp_tree& tree() const {return _tree;}
            
            /*! \brief Sets the vp-tree */    
            inline tree::cpu::vp_tree& tree() {return _tree;}

            /*! \brief Sets the v_list of dbscan */
            inline std::shared_ptr<std::vector<std::pair<int,int>>>& v_list()
                { return _v_list;}

            /*! \brief Gets the v_list of dbscan */
            inline const std::shared_ptr<std::vector<std::pair<int,int>>>& 
                v_list() const { return _v_list;}

            /*! \brief Sets the e_list of dbscan */
            inline std::shared_ptr<std::vector<int>>& e_list() {return _e_list;}

            /*! \brief Gets the e_list of dbscan */
            inline const std::shared_ptr<std::vector<int>>& e_list() 
                const {return _e_list;}

        protected:
            /*! \brief Creates a new graph from the tree and the parameters  
             *
             * This function populates the _v_list and _e_list adjacency
             * graphs. The tree must be ready and constructed before using this
             * function
             * */
            inline void create_graph();

            /*! \brief Performs a BFS setting the label for the core connected
             * elements */
            inline void breadth_first_search(int v, int label,
                    std::vector<bool>& visited, 
                    std::vector<int>& assignements) const;

            inline void breadth_first_search_kernel(int v, 
                    std::vector<bool>& xa,
                    std::vector<bool>& fa) const;

            tree::cpu::vp_tree _tree; /*!< \brief VP-tree for the knn search */

            /*! \brief vertex list as specified on reference paper 
             *
             * This is a list where the index represents the index of the
             * vertex on the data. The two values represents how many adjacent
             * points this vertex has within the epsilon distance and it's
             * index on the adjacency list (_e_list);
             * */
            std::shared_ptr<std::vector<std::pair<int,int>>> _v_list; 

            /*! \brief adjacency list as specified on reference paper 
             *
             * Contains the adjacency list of the graph
             * */
            std::shared_ptr<std::vector<int>> _e_list;
    };
};
};

///////////////////////////////////////////////////////////////////////////////

cluster::cpu::dbscan::dbscan() : 
    cluster::dbscan(),
    _v_list(new std::vector<std::pair<int,int>>()),
    _e_list(new std::vector<int>())
{
    /* Nothing to do here */
}

///////////////////////////////////////////////////////////////////////////////
 
cluster::cpu::dbscan::dbscan(std::shared_ptr<const std::vector<float>> data, 
        const float eps, const int min_pts, const int dim, 
        metric::cpu::metric_f metric) : 
    cluster::dbscan(data, eps, min_pts, dim), 
    _tree(data, dim, metric),
    _v_list(new std::vector<std::pair<int,int>>()),
    _e_list(new std::vector<int>())
{
    this->create_graph();
}

///////////////////////////////////////////////////////////////////////////////

cluster::cpu::dbscan::dbscan(std::shared_ptr<const std::vector<float>> data, 
        const float eps, const int min_pts, const int dim, 
        const tree::cpu::vp_tree& tree) : 
    cluster::dbscan(data, eps, min_pts, dim), 
    _tree(tree),
    _v_list(new std::vector<std::pair<int,int>>()),
    _e_list(new std::vector<int>())
{
    this->create_graph();
}

///////////////////////////////////////////////////////////////////////////////

cluster::cpu::dbscan::dbscan(const cluster::cpu::dbscan& other) :
    cluster::dbscan(other),
    _tree(other.tree()),
    _v_list(other.v_list()),
    _e_list(other.e_list())
{
    /* Nothing to do here */
}

///////////////////////////////////////////////////////////////////////////////

inline void cluster::cpu::dbscan::fit(std::shared_ptr<const std::vector<float>> 
        data, int dim)
{
    _data = data;
    _dim = dim;

    _tree.fit(data, dim);

    this->create_graph();
}

///////////////////////////////////////////////////////////////////////////////

inline void cluster::cpu::dbscan::create_graph()
{
    std::vector<int> ids;

    _v_list->clear();
    _e_list->clear();

    std::cout << "creating graph" << std::endl;

    for (int i=0; i < _data->size(); i+=_dim)
    {
        _tree.knn(i, _eps, ids); // nearest neighbor

        _v_list->push_back(std::make_pair<int,int>(ids.size(), _e_list->size()));

        for (int id : ids)
            _e_list->push_back(id);
    }

    std::cout << "graph created" << std::endl;
}

///////////////////////////////////////////////////////////////////////////////

inline void cluster::cpu::dbscan::predict(std::vector<int>& assignements) const
{
    int cluster_label = 0;
    std::vector<bool> visited(_v_list->size(), false);

    std::fill(assignements.begin(), assignements.end(), OUTLIERS);
    assignements.resize(_v_list->size(), OUTLIERS); // resize to number of vertices in data

    for (int i=0; i < _v_list->size(); i++) 
    {
        if (!visited[i] && _v_list->at(i).first > _min_pts)
        {
            visited[i] = true;
            assignements[i] = cluster_label;
            breadth_first_search(i, cluster_label, visited, assignements);
            cluster_label++;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////

inline void cluster::cpu::dbscan::breadth_first_search(int v, int label,
        std::vector<bool>& visited, std::vector<int>& assignements) const
{
    std::vector<bool> xa(_v_list->size(), 0);
    std::vector<bool> fa(_v_list->size(), 0);
    bool cont = true;

    fa[v] = true;
    while(cont)
    {
        for (int i=0; i < _v_list->size(); i++)
            breadth_first_search_kernel(i, fa, xa);
    }

    for (int i=0; i < _v_list->size(); i++)
    {
        if (xa[i]) {
            visited[i] = true;
            assignements[i] = label;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////

inline void cluster::cpu::dbscan::breadth_first_search_kernel(int v,
        std::vector<bool>& xa,
        std::vector<bool>& fa) const 
{
    register int nid;

    if (fa[v])
    {
        fa[v] = false; xa[v] = true;
        for (int i=0; i < _v_list->at(i).first; i++)
        {
            nid = _e_list->at(_v_list->at(i).second+i);
            if (!xa[nid])
                fa[nid] = 1;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////

#endif /* !DBSCAN_CPU_HPP */

///////////////////////////////////////////////////////////////////////////////

