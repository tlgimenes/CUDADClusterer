/*
 * =====================================================================================
 *       Filename:  error.hpp
 *    Description:  
 *        Created:  2015-02-02 11:13
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * =====================================================================================
 */

////////////////////////////////////////////////////////////////////////////////////////

#ifndef ERROR_HPP
#define ERROR_HPP

////////////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <chrono>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * Prints error message and exits  
 * */
static void __error(const std::string& error_code, const std::string& file, int line);

/**
 * Prints warning message and continues execution
 * */
static void __warning(const std::string& error_code, const std::string& file, int line);

#ifdef __cplusplus
};
#endif

////////////////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG
#define DBG_MESSAGE(str) // Message displayed in debug only mode
#define FATAL_ERROR(str)
#define WARNING_ERROR(str)
#define ASSERT_FATAL_ERROR(boolean, str)
#define ASSERT_WARNING_ERROR(boolean, str)
#else
#define DBG_MESSAGE(str) std::cout << str // Message displayed in debug only mode
#define FATAL_ERROR(str) __error(str, __FILE__, __LINE__)
#define WARNING_ERROR(str) __warning(str, __FILE__, __LINE__)
#define ASSERT_FATAL_ERROR(boolean, str) (void)((boolean) || (__error(str, __FILE__, __LINE__),0))
#define ASSERT_WARNING_ERROR(boolean, str) (void)((boolean) || (__warning(str, __FILE__, __LINE__),0))
#endif

////////////////////////////////////////////////////////////////////////////////////////
// Definitions of cuda like functions
#define CUDA_SAFE(code) \
    ASSERT_FATAL_ERROR(code == cudaSuccess, cudaGetErrorString(cudaGetLastError()))

////////////////////////////////////////////////////////////////////////////////////////

static void __error(const std::string& error_code, const std::string& file, int line)
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    struct tm *tm = std::localtime(&now_c);

    std::cout << "FATAL ERROR ! " << file << ":" << line; 
    std::cout << " ["<< tm->tm_mday << "/" << (tm->tm_mon+1) << "/" << (tm->tm_year+1900);
    std::cout << "]:" << tm->tm_hour << ":" << tm->tm_min << ":" << tm->tm_sec << std::endl;
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

    std::cout << "WARNING ! " << file << ":" << line; 
    std::cout << " ["<< tm->tm_mday << "/" << (tm->tm_mon+1) << "/" << (tm->tm_year+1900);
    std::cout << "]:" << tm->tm_hour << ":" << tm->tm_min << ":" << tm->tm_sec << std::endl;
    std::cout << "'" << error_code << "'" << std::endl << std::endl;
    
    return;
}

////////////////////////////////////////////////////////////////////////////////////////

#endif /* !ERROR_HPP */

////////////////////////////////////////////////////////////////////////////////////////

