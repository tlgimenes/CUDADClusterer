/*============================================================================*/
/*! \file time.hpp 
 *  \author Tiago LOBATO GIMENES            (tlgimenes@gmail.com)
 *  \date 2015-04-21 18:36
 *
 *  \brief functions and tools for mesuring the execution time of a given code
 *
 *  This file contains the implementation of functions for mesuring the execution
 *  time between code. It's necessary to have C++11 support
 * */
/*============================================================================*/

///////////////////////////////////////////////////////////////////////////////

#ifndef TIME_HPP
#define TIME_HPP

///////////////////////////////////////////////////////////////////////////////

#define CPP99 199711L /*!< C++99 version number */

////////////////////////////////////////////////////////////////////////////////////////

#if __cplusplus > CPP99
#include <chrono>
#endif
#include <string>
#include <iostream>
#include <vector>

#include "color.hpp"

////////////////////////////////////////////////////////////////////////////////////////
// Global variables for time_point
#if __cplusplus > CPP99
static std::chrono::high_resolution_clock::time_point t0;
static std::chrono::high_resolution_clock::time_point t1;
static console::modifier __time_gray_modifier(console::FG_D_GRAY);
static console::modifier __time_cyan_modifier(console::FG_CYAN);
static console::modifier __time_default_fg(console::FG_DEFAULT);
#endif

////////////////////////////////////////////////////////////////////////////////////////
// Macros for time
#if __cplusplus > CPP99
/*! \def TIME_BETWEEN
 *  \brief Measures the execution time between the code given */
#define TIME_BETWEEN(code) \
    t0 = std::chrono::high_resolution_clock::now(); \
    code \
    t1 = std::chrono::high_resolution_clock::now(); \
    std::cout << __time_gray_modifier << "Code took " << \
    (std::chrono::duration_cast<std::chrono::duration<double>>(t1-t0)).count(); \
    std::cout << "s to run: " << std::endl; \
    std::cout << "---------------------------------------------------------" << std::endl; \
    std::cout << __time_cyan_modifier << #code << __time_gray_modifier << std::endl; \
    std::cout << "---------------------------------------------------------" << std::endl;\
    std::cout << __time_default_fg;
#else
#include "error.hpp"
/*! \brief Measures the execution time between the code given */
#define TIME_BETWEEN(code) WARNING_ERROR("TIME_BETWEEN needs at least C++11 compliant compiler")
#endif

///////////////////////////////////////////////////////////////////////////////

#endif /* !TIME_HPP */

///////////////////////////////////////////////////////////////////////////////

