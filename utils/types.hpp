/*
 * ============================================================================
 *       Filename:  types.hpp
 *    Description:  
 *        Created:  2015-04-21 15:10
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * ============================================================================
 */

///////////////////////////////////////////////////////////////////////////////

#ifndef TYPES_HPP
#define TYPES_HPP

///////////////////////////////////////////////////////////////////////////////

#include <cstdarg>

#include "error.hpp"

///////////////////////////////////////////////////////////////////////////////

template <typename T, int N>
class primitive
{
    public:
        //primitive();
        primitive(T* data);
        primitive(...);

        inline operator T() const {return _data[0];}
        inline const T& operator[] (int n) const;
        inline primitive<T,N> operator+ (const primitive<T,N>& other) const;
        inline primitive<T,N> operator- (const primitive<T,N>& other) const;
        inline primitive<T,N> operator* (const primitive<T,N>& other) const;
        inline const bool operator< (const primitive<T, N>& other) const;

        inline T& operator[] (int n);

        inline primitive<T,N>& operator= (const primitive<T,N>& other);
       
    protected:
        T _data[N];
};

///////////////////////////////////////////////////////////////////////////////

/*template <typename T, int N>
inline primitive<T, N>::primitive()
{
    for(int i=0; i < N; i++)
        _data[i] = 0.0f;
}*/

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
        //std::cout << _data[i] << std::endl;
    }
    va_end(args);

 //   std::cout << std::endl;
}

template <>
inline primitive<float, 1>::primitive(...)
{
    va_list args;

    va_start(args, NULL);
    for(int i=0; i < 1; i++)
    {
        _data[i] = va_arg(args, double);
        //std::cout << _data[i] << std::endl;
    }
    va_end(args);

    //std::cout << std::endl;
}

template <>
inline primitive<float, 4>::primitive(...)
{
    va_list args;

    va_start(args, NULL);
    for(int i=0; i < 4; i++)
    {
        _data[i] = va_arg(args, double);
        //std::cout << _data[i] << std::endl;
    }
    va_end(args);

    //std::cout << std::endl;
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
using floatn = primitive<float, N>;

template <int N>
using intn = primitive<int, N>;

using float4 = floatn<4>;

///////////////////////////////////////////////////////////////////////////////

template <typename K, typename V>
class key_value
{
    public:
        key_value(K k, V v) : _key(k), _val(v) {}
        key_value() : _key(), _val() {}

        inline const V& val() const {return _val;}
        inline const K& key() const {return _key;}

        inline V& val() {return _val;}
        inline K& key() {return _key;}

        inline const key_value& operator= (const key_value& other);

        inline bool operator< (const key_value& other) const;

    private:
        K _key;
        V _val;
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

using ifloat = key_value<int, float>;

///////////////////////////////////////////////////////////////////////////////

#endif /* !TYPES_HPP */

///////////////////////////////////////////////////////////////////////////////

