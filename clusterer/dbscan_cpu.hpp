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

#include "dbscan.hpp"
#include "metrics.hpp"
#include "vp_tree_cpu.hpp"

///////////////////////////////////////////////////////////////////////////////

namespace cluster{
namespace cpu
{
    /*! \brief Class for applying DBSCAN clustering algorithm to data */
    class dbscan : public cluster::dbscan
    {
        public:
            /*! \brief Constructor of the clusterer 
             *
             * \param data Data array 
             * \param eps Epsilon distance parameter of the DBSCAN algorithm 
             * \param min_pts Minimal number of points for the DBSCAN algorithm
             * \param dim Dimention of the data 
             * \param metric Metric function to use for the DBSCAN algorithm */
            dbscan(std::shared_ptr<const std::vector<float>> data, 
                    const float eps, const int min_pts, const int dim, 
                    metric::cpu::metric_f metric = metric::cpu::euclidean) : 
                cluster::dbscan(data, eps, min_pts, dim), _tree(data, dim, metric) {}

        protected:
            tree::cpu::vp_tree _tree; /*!< \brief VP-tree for the knn search */
    };
};
};

///////////////////////////////////////////////////////////////////////////////

#endif /* !DBSCAN_CPU_HPP */

///////////////////////////////////////////////////////////////////////////////

