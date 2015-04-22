/*
 * ============================================================================
 *       Filename:  time.hpp
 *    Description:  
 *        Created:  2015-04-21 18:36
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * ============================================================================
 */

///////////////////////////////////////////////////////////////////////////////

#ifndef TIME_HPP
#define TIME_HPP

///////////////////////////////////////////////////////////////////////////////

#define CPP99 199711L

////////////////////////////////////////////////////////////////////////////////////////

#if __cplusplus > CPP99
#include <chrono>
#endif
#include <string>
#include <iostream>
#include <vector>

////////////////////////////////////////////////////////////////////////////////////////
// Global variables for time_point
#if __cplusplus > CPP99
static std::chrono::high_resolution_clock::time_point t0;
static std::chrono::high_resolution_clock::time_point t1;
#endif

////////////////////////////////////////////////////////////////////////////////////////
// Macros for time
#if __cplusplus > CPP99
#define TIME_BETWEEN(code) \
    t0 = std::chrono::high_resolution_clock::now(); \
    code \
    t1 = std::chrono::high_resolution_clock::now(); \
    std::cout << "Code took " << (std::chrono::duration_cast<std::chrono::duration<double>>(t1-t0)).count(); \
    std::cout << "s to run: " << std::endl; \
    std::cout << "---------------------------------------------------------" << std::endl; \
    std::cout << #code << std::endl; \
    std::cout << "---------------------------------------------------------" << std::endl;
#else
#include "error.hpp"
#define TIME_BETWEEN(code) WARNING_ERROR("TIME_BETWEEN needs at least C++11 compliant compiler")
#endif

///////////////////////////////////////////////////////////////////////////////

#endif /* !TIME_HPP */

///////////////////////////////////////////////////////////////////////////////

