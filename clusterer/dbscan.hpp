/*
 * ============================================================================
 *       Filename:  dbscan.hpp
 *    Description:  
 *        Created:  2015-04-20 17:54
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * ============================================================================
 */

///////////////////////////////////////////////////////////////////////////////

#ifndef DBSCAN_HPP
#define DBSCAN_HPP

///////////////////////////////////////////////////////////////////////////////

class DBSCAN 
{
    public:
        DBSCAN(const std::vector<float>& _data, const float eps, const int min_pts,
                const int dim) : _data(data), _eps(eps), _min_pts(min_pts), 
                _dim(dim) {}

        /**
         * Sets
         * */
        inline float& eps() {return _eps;}
        inline float& min_pts() {return _min_pts;}

        /**
         * Gets
         * */
        inline const float& eps() const {return _eps;}
        inline const float& min_pts() const {return _min_pts;}

    protected:
        /* Classical DBSCAN Parameters */
        float _eps;   // Epsilon parameters
        int _min_pts; // Minimal number of points Parameters

        int _dim;     // Data's dimention

        const std::vector<float>& _data; // Vector containing data
};

///////////////////////////////////////////////////////////////////////////////

#endif /* !DBSCAN_HPP */

///////////////////////////////////////////////////////////////////////////////

