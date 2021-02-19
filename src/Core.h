//
// Created by William Lundqvist on 2020-11-06.
//

#ifndef GLPROJECT_CORE_H
#define GLPROJECT_CORE_H
#include <iostream>
#include <sstream>


// Macro for index to buffer pointer conversion
// Code given by Stefan Johansson from workshop 1 from computer graphics course at Umeå univerity, course code: 5DV179
#define BUFFER_OFFSET(i) (reinterpret_cast<char*>(0 + (i)))

#ifdef DEBUG_ENABLE

    #define DEBUG( msg ) \
        std::cout << "DEBUG[" << __FILE__ << "(" << __LINE__ << ")] " << msg << std::endl; \

    #define DEBUGERR( msg ) \
        std::cerr << "ERROR[" << __FILE__ << "(" << __LINE__ << ")] " << msg << std::endl; \

    #define DEBUGMSG( msg ) \
        std::cout << "DEBUG[" << __FILE__ << "(" << __LINE__ << ")] " << msg << std::endl; \

        /**
         * extra print functionallity to print into a list instead of stdout.
         */
    #define GL_ERR_CHECK \
        GLenum err = glGetError(); \
        if(err != GL_NO_ERROR) { \
         ERR("GL Error [0x"<< std::hex << err <<"]"); \
        } \


#define DEBUGVAR( var )\
        std::cerr << "DEBUG[" << __FILE__ << "(" << __LINE__ << "] " \
        << #var << " = " << (var) << std::endl

#else
    #define DEBUG( msg )
    #define DEBUGVAR( var )
    #define PRINT( msg )
    #define GL_ERR_CHECK
#endif //GLPROJECT_CORE_H
#endif