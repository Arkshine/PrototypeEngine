#ifndef COMMON_GLUTILS_H
#define COMMON_GLUTILS_H

#include <cstdint>

namespace gl
{
/**
*	Gets the major and minor version of the current OpenGL context.
*/
bool GetContextVersion( uint32_t& uiOutMajor, uint32_t& uiOutMinor );
}

#endif //COMMON_GLUTILS_H