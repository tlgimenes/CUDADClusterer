/*============================================================================*/
/*! \file color.hpp 
 *  \author Tiago LOBATO GIMENES            (tlgimenes@gmail.com)
 *  \date 2015-04-27 13:43
 *
 *  \brief color CLI modifier
 *
 *  This file contains a class implementation that allows to change colors 
 *  in the terminal. If the terminal does not support colors, some strange 
 *  character chains will be printed
 * */
/*============================================================================*/

///////////////////////////////////////////////////////////////////////////////

#ifndef COLOR_HPP
#define COLOR_HPP

///////////////////////////////////////////////////////////////////////////////

#include <ostream>

///////////////////////////////////////////////////////////////////////////////

namespace console
{
    /*! \brief Color code for the modifier */
    enum ccode
    {
        FG_RED      = 31,
        FG_GREEN    = 32,
        FG_YELLOW   = 33,
        FG_BLUE     = 34,
        FG_MAGENTA  = 35,
        FG_CYAN     = 36,
        FG_L_GRAY   = 37,
        FG_D_GRAY   = 90,
        FG_DEFAULT  = 39,
        BG_RED      = 41,
        BG_GREEN    = 42,
        BG_YELLOW   = 43,
        BG_BLUE     = 44,
        BG_MAGENTA  = 45,
        BG_CYAN     = 46,
        BG_L_GRAY   = 47,
        BG_D_GRAY   = 100,
        BG_DEFAULT  = 49,
        SET_BOLD    = 1,
        SET_DIM     = 2,
        RESET_BOLD  = 21,
        RESET_DIM   = 22,
        DEFAULT     = 0
    };

    /*! \class modifier
     *  
     *  This class should be used in the following way:  
     *  modifier red(FG_RED);
     *  std::cout << red << "this text is red" << std::endl; 
     * */
    class modifier
    {
        private:
            enum ccode _code; /*! \brief color code */

        public:
            /*! \brief Constructor for the color modifier
             *
             * \param code color code for the modifier 
             * */
            modifier(enum ccode code) : _code(code) {}

            /*! \brief Get the color code */
            const enum ccode& code() const {return _code;}

            /*! \brief operator for using this class in the standard c++ output */
            friend std::ostream& operator<<(std::ostream& os, const modifier& mod) 
            {
                return os << "\033[" << mod.code() << "m";
            }
    };
};

///////////////////////////////////////////////////////////////////////////////

#endif /* !COLOR_HPP */

///////////////////////////////////////////////////////////////////////////////

