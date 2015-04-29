/*============================================================================*/
/*! \file error.hpp 
 *  \author Tiago LOBATO GIMENES            (tlgimenes@gmail.com)
 *  \date 2015-02-02 11:13
 *
 *  \brief error repporting interface
 *
 *  This file contains the implementation of a simple error repporting interface
 * */
/*============================================================================*/

////////////////////////////////////////////////////////////////////////////////////////

#ifndef ERROR_HPP
#define ERROR_HPP

////////////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <chrono>
#include <iostream>

#include "color.hpp"

////////////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Prints error message and exits  
 * */
static void __error(const std::string& error_code, const std::string& file, int line);

/*!
 * \brief Prints warning message and continues execution
 * */
static void __warning(const std::string& error_code, const std::string& file, int line);

/*!
 * \brief Prints a message with a given color
 * */
static void __message(const std::string& message, const console::modifier& color);

#ifdef __cplusplus
};
#endif

////////////////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG

    #define DBG_MESSAGE(str) /*! \brief Message displayed in debug only mode */
    #define FATAL_ERROR(str)
    #define WARNING_ERROR(str)
    #define ASSERT_FATAL_ERROR(boolean, str)
    #define ASSERT_WARNING_ERROR(boolean, str)
    #define CUDA_SAFE(code)

#else

    /*! \brief Message to be displayed in debug only mode */
    #define DBG_MESSAGE(str) __message(str, console::FG_D_GRAY) 

    /*! \brief Fatal errors
    *
    * This macro writes the error message in str and exits */
    #define FATAL_ERROR(str) __error(str, __FILE__, __LINE__)

    /*! \brief Warning
    *
    * This macro writes the warning message in str and continues */
    #define WARNING_ERROR(str) __warning(str, __FILE__, __LINE__)

    /*! \brief Asserts with fatal error message  */
    #define ASSERT_FATAL_ERROR(boolean, str) (void)((boolean) || (__error(str, __FILE__, __LINE__),0))

    /*! \brief Asserts with warning message */
    #define ASSERT_WARNING_ERROR(boolean, str) (void)((boolean) || (__warning(str, __FILE__, __LINE__),0))

    /*! \brief This macro searches for errors in cuda functions execution 
    *
    *  One should use always this macro in each cuda function call */
    #define CUDA_SAFE(code) \
        ASSERT_FATAL_ERROR(code == cudaSuccess, cudaGetErrorString(cudaGetLastError()))

#endif


////////////////////////////////////////////////////////////////////////////////////////

static void __error(const std::string& error_code, const std::string& file, int line)
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    struct tm *tm = std::localtime(&now_c);
    console::modifier red(console::FG_RED);
    console::modifier def(console::FG_DEFAULT);
    console::modifier blue(console::FG_BLUE);
    console::modifier gray(console::FG_D_GRAY);

    std::cout << red << "FATAL ERROR ! " << gray << file;
    std::cout << ":" << line; 
    std::cout << blue << " ["<< tm->tm_mday << "/" << (tm->tm_mon+1) << "/" << (tm->tm_year+1900);
    std::cout << "]:" << tm->tm_hour << ":" << tm->tm_min << ":" << tm->tm_sec << def << std::endl;
    std::cout << "'" << error_code << "'" << std::endl << std::endl;

    exit(EXIT_FAILURE);

    return;
}

////////////////////////////////////////////////////////////////////////////////////////

static void __warning(const std::string& error_code, const std::string& file, int line)
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    struct tm *tm = std::localtime(&now_c);
    console::modifier yellow(console::FG_YELLOW);
    console::modifier fg_def(console::FG_DEFAULT);
    console::modifier blue(console::FG_BLUE);
    console::modifier gray(console::FG_D_GRAY);

    std::cout << yellow << "WARNING ! " << gray << file;
    std::cout << ":" << line; 
    std::cout << blue << " ["<< tm->tm_mday << "/" << (tm->tm_mon+1) << "/" << (tm->tm_year+1900);
    std::cout << "]:" << tm->tm_hour << ":" << tm->tm_min << ":" << tm->tm_sec << fg_def << std::endl;
    std::cout << "'" << error_code << "'" << std::endl << std::endl;
    
    return;
}

////////////////////////////////////////////////////////////////////////////////////////

static void __message(const std::string& message, const console::modifier& color)
{
    console::modifier def(console::FG_DEFAULT);

    std::cout << color << message << def;
}

////////////////////////////////////////////////////////////////////////////////////////

#endif /* !ERROR_HPP */

////////////////////////////////////////////////////////////////////////////////////////

