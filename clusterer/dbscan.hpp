/*============================================================================*/
/*! \file dbscan.hpp 
 *  \author Tiago LOBATO GIMENES            (tlgimenes@gmail.com)
 *  \date 2015-04-20 17:54
 *
 *  \brief Definition of the class base for the dbscan clusterer
 *
 *  This file contains the implementation of the base class for the dbscan 
 *  clustering algorithm
 * */
/*============================================================================*/

///////////////////////////////////////////////////////////////////////////////

#ifndef DBSCAN_HPP
#define DBSCAN_HPP

///////////////////////////////////////////////////////////////////////////////

#include "vp_tree.hpp"

///////////////////////////////////////////////////////////////////////////////

namespace cluster
{
    /*! \brief Base class for applying DBSCAN clustering algorithm to data */
    class dbscan 
    {
        public:
            /*! \brief Constructor of the clusterer 
             *
             * \param data Data array 
             * \param eps Epsilon distance parameter of the DBSCAN algorithm 
             * \param min_pts Minimal number of points for the DBSCAN algorithm
             * \param dim Dimention of the data */
            dbscan(std::shared_ptr<const std::vector<float>> data, 
                    const float eps, const int min_pts, const int dim) :
                _data(data), _eps(eps), _min_pts(min_pts), _dim(dim) {}

            /*! \brief Set epsilon value for the DBSCAN algorithm */
            inline float& eps() {return _eps;}
            /*! \brief Set minimun points for the DBSCAN algorithm */
            inline int& min_pts() {return _min_pts;}

            /*! \brief Get epsilon value of the DBSCAN algorithm */
            inline const float& eps() const {return _eps;}
            /*! \brief Get minimun points of the DBSCAN algorithm */
            inline const int& min_pts() const {return _min_pts;}

        protected:
            /* Classical DBSCAN Parameters */
            float _eps;   /*!< Epsilon parameters */
            int _min_pts; /*!< Minimal number of points Parameters */

            int _dim;     /*!< Data's dimention */

            std::shared_ptr<const std::vector<float>> _data; /*!< Vector containing data */
    };
};

///////////////////////////////////////////////////////////////////////////////

#endif /* !DBSCAN_HPP */

///////////////////////////////////////////////////////////////////////////////

