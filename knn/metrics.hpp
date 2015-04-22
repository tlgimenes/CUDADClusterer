/*
 * ============================================================================
 *       Filename:  metrics.hpp
 *    Description:  
 *        Created:  2015-04-22 22:35
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * ============================================================================
 */

///////////////////////////////////////////////////////////////////////////////

#ifndef METRICS_HPP
#define METRICS_HPP

///////////////////////////////////////////////////////////////////////////////

namespace cpu
{
    /**
     * Distance function used for the metric space. You should
     * rewrite this function for each metric. This function implements the 
     * euclidean distance
     * @param0: index of an element in the data array
     * @param1: index of an element in the data array
     * @param2: data
     * @param3: dimention of the data
     * @returns: distance between element of index a and b
     * */
    inline float euclidean2(int a, int b, const std::vector<float>& data, int dim);
}

///////////////////////////////////////////////////////////////////////////////

inline float cpu::euclidean2(int a, int b, const std::vector<float>& data, int dim)
{
    float res = 0.0f;

    ASSERT_FATAL_ERROR((a+dim <= data.size()) && (b+dim <= data.size()), "Out of bounds");

    for(int i=0; i < dim; i++)
    {
        res += (data[a+i] - data[b+i]) * (data[a+i] - data[b+i]);
    }

    return res;
}

///////////////////////////////////////////////////////////////////////////////

#endif /* !METRICS_HPP */

///////////////////////////////////////////////////////////////////////////////

