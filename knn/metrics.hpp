/*============================================================================*/
/*! \file metrics.hpp 
 *  \author Tiago LOBATO GIMENES            (tlgimenes@gmail.com)
 *  \date 2015-04-22 22:35
 *
 *  \brief metric definitions
 *
 *  This file contains implementation of metric functions
 * */
/*============================================================================*/


///////////////////////////////////////////////////////////////////////////////

#ifndef METRICS_HPP
#define METRICS_HPP

///////////////////////////////////////////////////////////////////////////////

namespace metric {
namespace cpu
{
    /*!
     * \brief Definition of the distance function used for defining the metric space.
     *
     * You should write functions based on this declaration
     * \param id1 index of an element in the data array
     * \param id2 index of an element in the data array
     * \param data data array
     * \param dim dimention of the data
     * \return distance between element of index a and b
     * */
    using metric_f = float (*)(int, int, const std::vector<float>&, int);

    /*!
     * \brief Implementation of the euclidean metric 
     * */
    inline float euclidean(int a, int b, const std::vector<float>& data, int dim);
};
};

///////////////////////////////////////////////////////////////////////////////

inline float metric::cpu::euclidean(int a, int b, const std::vector<float>& data, int dim)
{
    float res = 0.0f;

    ASSERT_FATAL_ERROR((a+dim <= data.size()) && (b+dim <= data.size()), "Out of bounds");

    for(int i=0; i < dim; i++)
    {
        res += (data[a+i] - data[b+i]) * (data[a+i] - data[b+i]);
    }

    return sqrt(res);
}

///////////////////////////////////////////////////////////////////////////////

#endif /* !METRICS_HPP */

///////////////////////////////////////////////////////////////////////////////

