#ifndef _MG_DEFS_H_
#define _MG_DEFS_H_

#ifdef MG_USE_WINDOWS
#include <windows.h>
/* #undef near */
/* #undef far */
#else
#include "mg_windows.h"
#endif // MG_USE_WINDOWS
#include <stdint.h>

//////////////////////////////////////////////////////////////////////////////
// TYPES
//////////////////////////////////////////////////////////////////////////////

// Signed
typedef char                    s8;
typedef short                   s16;
typedef int                     s32;
typedef long long               s64;
// Unsigned
typedef unsigned char           u8;
typedef unsigned short          u16;
typedef unsigned int            u32;
typedef unsigned long long      u64;
typedef size_t                  usize;
// Bool
typedef char                    b8;
typedef int                     b32;
// Float
typedef float                   f32;
typedef double                  f64;

// Handle
typedef void					*mg_handle_t;
#define MG_INVALID_HANDLE		INVALID_HANDLE_VALUE

// Max values
#define U8_MAX                  UINT8_MAX
#define U16_MAX                 UINT16_MAX
#define U32_MAX                 UINT32_MAX
#define U64_MAX                 UINT64_MAX

#define S8_MAX                  INT8_MAX
#define S16_MAX                 INT16_MAX
#define S32_MAX                 INT32_MAX
#define S64_MAX                 INT64_MAX

/* #ifndef __cplusplus */
/* #define true 1 */
/* #define false 0 */
/* #endif // __cplusplus */

//////////////////////////////////////////////////////////////////////////////
// MACROS
//////////////////////////////////////////////////////////////////////////////

// For C vs. C++ compilation
#ifdef __cplusplus
    #define MG_EXTERN   extern "C"
#else
    #define MG_EXTERN   extern
#endif // __cplusplus

// Macro to strip just the filename out of the full filepath.
#if defined(_WIN32) || defined(_WIN64)
    #define __FILENAME__    (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
    #define __FILENAME__    (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#define mg_lerp(__a, __b, __t) \
    ((__a) + (F32)((__b) - (__a) * (__t))

#define mg_clamp(__x, __min, __max) \
    ((__x) < (__min) ? (__min) : (__x) > (__max) ? (__max) : (__x))

#define mg_swap(_A, _B, _Type)       \
     do                             \
     {                              \
        _Type __MgSwapTemp = _A;    \
        _A = _B;                    \
        _B = __MgSwapTemp;          \
     } while (0)

#define mg_min(_A, _B) \
    ((_A) < (_B) ? (_A) : (_B))

#define mg_max(_A, _B) \
    ((_A) > (_B) ? (_A) : (_B))

#define mg_alloc(_Size) \
    (HeapAlloc(GetProcessHeap(), 0, _Size))

#define mg_realloc(_Ptr, _Size) \
    (HeapReAlloc(GetProcessHeap(), 0, _Ptr, _Size))

#define mg_free(_Ptr) \
    (HeapFree(GetProcessHeap(), 0, _Ptr))

// TODO(matthew): Maybe rename some of these?
#define mg_lobyte(_Word) \
    ((BYTE)(((DWORD_PTR)(_Word)) & 0xFF))

#define mg_hibyte(_Word) \
    ((BYTE)((((DWORD_PTR)(_Word)) >> 8) & 0xFF))

#define mg_loword(_Double) \
    ((WORD)(((DWORD_PTR)(_Double)) & 0xFFFF))

#define mg_hiword(_Double) \
    ((WORD)((((DWORD_PTR)(_Double)) >> 16) & 0xFFFF))

#define mg_lodword(_Quad) \
    ((DWORD)(((DWORD_PTR)(_Quad)) & 0xFFFFFFFF))

#define mg_hidword(_Quad) \
    ((DWORD)((((DWORD_PTR)(_Quad)) >> 32) & 0xFFFFFFFF))

#define mg_kilobytes(__n) \
	1024 * (__n) 

#define mg_megabytes(__n) \
	1024 * mg_kilobytes(__n) 

#define mg_gigabytes(__n) \
	1024 * mg_megabytes(__n) 

#define mg_terabytes(__n) \
	1024 * mg_gigabytes(__n) 

#endif // _MG_DEFS_H_

