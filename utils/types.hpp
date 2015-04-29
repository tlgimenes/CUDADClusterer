/*============================================================================*/
/*! \file types.hpp 
 *  \author Tiago LOBATO GIMENES            (tlgimenes@gmail.com)
 *  \date 2015-04-21 15:10 
 *
 *  \brief Simple type definitions
 *
 *  This file contains the implementation and definition of simple types
 *  imeplemented on CUDA and OpenCL
 * */
/*============================================================================*/


///////////////////////////////////////////////////////////////////////////////

#ifndef TYPES_HPP
#define TYPES_HPP

///////////////////////////////////////////////////////////////////////////////

#include <cstdarg>

#include "error.hpp"

///////////////////////////////////////////////////////////////////////////////

/*! \class primitive
 * \brief Implementation of general N-dimentional type */
template <typename T, int N>
class primitive
{
    public:
        /*! \brief Constructor of type 
         *  \param data Data to be stored in the "variable" */
        primitive(T* data);
        /*! \brief Constructor that allows intation of each element separatly */
        primitive(...);

        /*! \brief Cast operator */
        inline operator T() const {return _data[0];}
        /*! \brief Array like operator */
        inline const T& operator[] (int n) const;
        /*! \brief Element-wise operator plus */
        inline primitive<T,N> operator+ (const primitive<T,N>& other) const;
        /*! \brief Element-wise operator minus */
        inline primitive<T,N> operator- (const primitive<T,N>& other) const;
        /*! \brief Element-wise operator times */
        inline primitive<T,N> operator* (const primitive<T,N>& other) const;
        /*! \brief Element-wise operator smaller than
         *
         *  \return True if first element of this is smaller than other. If
         *  equal continue the search in other items of the data array */
        inline const bool operator< (const primitive<T, N>& other) const;

        /*! \brief Array like operator */
        inline T& operator[] (int n);

        /*! \brief Element-wise assignement operator */
        inline primitive<T,N>& operator= (const primitive<T,N>& other);
       
    protected:
        T _data[N]; /*!< \brief Where data is stored */
};

///////////////////////////////////////////////////////////////////////////////

template <typename T, int N>
inline primitive<T, N>::primitive(T* data)
{
    for(int i=0; i < N; i++)
        _data[i] = data[i];
}

template <typename T, int N>
inline primitive<T, N>::primitive(...)
{
    va_list args;

    va_start(args, NULL);
    for(int i=0; i < N; i++)
    {
        _data[i] = va_arg(args, T);
    }
    va_end(args);
}

/*! \brief Specialization of type for float */
template <>
inline primitive<float, 1>::primitive(...)
{
    va_list args;

    va_start(args, NULL);
    for(int i=0; i < 1; i++)
    {
        _data[i] = va_arg(args, double);
    }
    va_end(args);
}

/*! \brief Specialization of type for float */
template <>
inline primitive<float, 4>::primitive(...)
{
    va_list args;

    va_start(args, NULL);
    for(int i=0; i < 4; i++)
    {
        _data[i] = va_arg(args, double);
    }
    va_end(args);
}

///////////////////////////////////////////////////////////////////////////////

template <typename T, int N>
inline const T& primitive<T, N>::operator[] (int n) const 
{
    ASSERT_FATAL_ERROR(n < N, "Out of bounds");
    return _data[n];
}

///////////////////////////////////////////////////////////////////////////////

template <typename T, int N>
inline T& primitive<T, N>::operator[] (int n) 
{
    ASSERT_FATAL_ERROR(n < N, "Out of bounds");
    return _data[n];
}

///////////////////////////////////////////////////////////////////////////////

template <typename T, int N>
inline primitive<T, N> primitive<T, N>::operator+ (const primitive<T, N>& other) const
{
    primitive<T, N> other2;

    for(int i=0; i < N; i++) 
    {
        other2[i] = _data[i] + other[i];
    }

    return other2;
}

///////////////////////////////////////////////////////////////////////////////

template <typename T, int N>
inline primitive<T, N> primitive<T, N>::operator- (const primitive<T, N>& other) const
{
    primitive<T, N> other2;

    for(int i=0; i < N; i++) 
    {
        other2[i] = _data[i] - other[i];
    }

    return other2;
}

///////////////////////////////////////////////////////////////////////////////

template <typename T, int N>
inline primitive<T, N> primitive<T, N>::operator* (const primitive<T, N>& other) const
{
    primitive<T, N> other2;

    for(int i=0; i < N; i++) 
    {
        other2[i] = _data[i] * other[i];
    }

    return other2;
}

///////////////////////////////////////////////////////////////////////////////

template <typename T, int N>
inline const bool primitive<T, N>::operator< (const primitive<T, N>& other) const
{
    int i;

    for(i=0; i < N-1 && _data[i] == other[i]; i++);

    return (_data[i] < other[i]);
}

///////////////////////////////////////////////////////////////////////////////

template <typename T, int N>
inline primitive<T, N>& primitive<T, N>::operator= (const primitive<T, N>& other) 
{
    for(int i=0; i < N; i++) 
    {
        _data[i] = other[i];
    }

    return *this;
}

///////////////////////////////////////////////////////////////////////////////

template <int N>
using floatn = primitive<float, N>; /*!< N-Dimentional float definition */

template <int N>
using intn = primitive<int, N>; /*!< N-Dimentional int definition */

using float4 = floatn<4>; /*!< 4-Dimentional float definition */

///////////////////////////////////////////////////////////////////////////////

/*! \class key_value 
 * \brief Key-Value class implementation */
template <typename K, typename V>
class key_value
{
    public:
        /*! \brief Constructor 
         *
         * \param k Key
         * \param v Value */
        key_value(K k, V v) : _key(k), _val(v) {}

        /*! \brief Constructs new element with default key and value */
        key_value() : _key(), _val() {}

        /*! \brief Get value */
        inline const V& val() const {return _val;}
        /*! \brief Get key */
        inline const K& key() const {return _key;}

        /*! \brief Set value */
        inline V& val() {return _val;}
        /*! \brief Set key */
        inline K& key() {return _key;}

        /*! \brief Copies key and value from one object to this */
        inline const key_value& operator= (const key_value& other);

        /*! \brief The comparision is made using the value and not the key */
        inline bool operator< (const key_value& other) const;

    private:
        K _key; /*!< \brief key */
        V _val; /*!< \brief value */
};

///////////////////////////////////////////////////////////////////////////////

template <typename K, typename V>
inline const key_value<K,V>& key_value<K,V>::operator= (const key_value& other)
{
    _key = other.key();
    _val = other.val();

    return *this;
}

///////////////////////////////////////////////////////////////////////////////

template <typename K, typename V>
inline bool key_value<K,V>::operator< (const key_value& other) const 
{
    return _val < other.val();
}

///////////////////////////////////////////////////////////////////////////////

using ifloat = key_value<int, float>; /*!< Indexed float (ifloat) definition */

///////////////////////////////////////////////////////////////////////////////

#endif /* !TYPES_HPP */

///////////////////////////////////////////////////////////////////////////////

