#pragma once

#include <assert.h>

#   define STR(...)     __STR(__VA_ARGS__)
#   define __STR(...)   #__VA_ARGS__

#   define ARGS(...)    __VA_ARGS__

#   ifdef _DEBUG
#       define jc_assert(expr)              if (!(expr)) { __debugbreak(); }
#       define jc_debug(message, ...)       printf(message"\n", __VA_ARGS__);
#   else
#       define jc_assert(expr)
#       define jc_debug(message, ...)
#   endif

__declspec(noreturn) inline void noreturn_func() {}

