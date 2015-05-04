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

/*! \brief OUTLIERS definition in the assignements */
#define OUTLIERS -1

///////////////////////////////////////////////////////////////////////////////

namespace cluster
{
    /*! \brief Base class for applying DBSCAN clustering algorithm to data */
    class dbscan 
    {
        public:
            /*! \brief Constructs an empty dbscan clusterer */
            dbscan() : _eps(0), _min_pts(0), _dim(0), _data() {}

            /*! \brief Constructor of the clusterer 
             *
             * \param data Data array 
             * \param eps Epsilon distance parameter of the DBSCAN algorithm 
             * \param min_pts Minimal number of points for the DBSCAN algorithm
             * \param dim Dimention of the data */
            dbscan(std::shared_ptr<const std::vector<float>> data, 
                    const float eps, const int min_pts, const int dim) :
                _data(data), _eps(eps), _min_pts(min_pts), _dim(dim) {}

            /*! \brief Constructs another dbscan based on the others dbscan
             * data, tree and parameters */
            dbscan(const dbscan& other);

            /*! \brief Fits the new data to the dbscan clusterer */
            inline void fit(std::shared_ptr<const std::vector<float>> data, 
                    const int dim);

            /*! \brief Set epsilon value for the DBSCAN algorithm */
            inline float& eps() {return _eps;}
            /*! \brief Set minimun points for the DBSCAN algorithm */
            inline int& min_pts() {return _min_pts;}
            /*! \brief Set data dimention */
            inline int& dim() {return _dim;}
            /*! \brief Set data */
            inline std::shared_ptr<const std::vector<float>>& data() {return _data;}

            /*! \brief Get epsilon value of the DBSCAN algorithm */
            inline const float& eps() const {return _eps;}
            /*! \brief Get minimun points of the DBSCAN algorithm */
            inline const int& min_pts() const {return _min_pts;}
            /*! \brief Get data dimention */
            inline const int& dim() const {return _dim;}
            /*! \brief Get data  */
            inline const std::shared_ptr<const std::vector<float>>& data() const {return _data;}

        protected:
            /* Classical DBSCAN Parameters */
            float _eps;   /*!< Epsilon parameters */
            int _min_pts; /*!< Minimal number of points Parameters */

            int _dim;     /*!< Data's dimention */

            std::shared_ptr<const std::vector<float>> _data; /*!< Vector containing data */
    };
};

///////////////////////////////////////////////////////////////////////////////

inline cluster::dbscan::dbscan(const cluster::dbscan& other) :
    _eps(other.eps()),
    _min_pts(other.min_pts()),
    _dim(other.dim()),
    _data(other.data())
{
    /* Nothing to do here */
}

///////////////////////////////////////////////////////////////////////////////

inline void cluster::dbscan::fit(std::shared_ptr<const std::vector<float>> 
        data, const int dim)
{
    _data = data;
    _dim = dim;
}

///////////////////////////////////////////////////////////////////////////////

#endif /* !DBSCAN_HPP */

///////////////////////////////////////////////////////////////////////////////

