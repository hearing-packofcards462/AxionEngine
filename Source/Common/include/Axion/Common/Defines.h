#pragma once
// Windows headers
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <shellapi.h> // For CommandLineToArgvW

#if defined( min )
#undef min
#endif

#if defined( max )
#undef max
#endif

// Windows Runtime Library. Needed for Microsoft::WRL::ComPtr<> template class.
#include <wrl.h>

// STL Headers
#include <chrono>
#include <iostream>
#include <memory>

// FMT
#include <fmt/core.h>
#include <fmt/format.h>

// ---------------------------------------------------------------------------
// Handle MACRO Definitions
// ---------------------------------------------------------------------------

#define AXION_NAMESPACE_BEGIN namespace Axion {
#define AXION_NAMESPACE_END }
#define USING_AXION_NAMESPACE using namespace Axion;

#if defined( _DEBUG ) || !defined( NDEBUG )
#define AXION_DEBUG
#endif

#define AXION_ENUM_CLASS_FLAG_OPERATORS( T )                                                                                  \
    inline T    operator|( T a, T b ) { return T( uint32_t( a ) | uint32_t( b ) ); }                                          \
    inline T    operator&( T a, T b ) { return T( uint32_t( a ) & uint32_t( b ) ); } /* NOLINT(bugprone-macro-parentheses) */ \
    inline T    operator~( T a ) { return T( ~uint32_t( a ) ); }                     /* NOLINT(bugprone-macro-parentheses) */ \
    inline bool operator!( T a ) { return uint32_t( a ) == 0; }                                                               \
    inline bool operator==( T a, uint32_t b ) { return uint32_t( a ) == b; }                                                  \
    inline bool operator!=( T a, uint32_t b ) { return uint32_t( a ) != b; }

#define DEFINE_SHARED_HANDLE_FOR_TYPE( type, clean ) \
    class type;                                      \
    typedef std::shared_ptr<type> clean##Handle;

#define NEW_S( type ) \
    std::make_shared<type>

#define DEFINE_UNIQUE_HANDLE_FOR_TYPE( type, clean ) \
    class type;                                      \
    typedef std::unique_ptr<type> clean##Handle;

#define NEW_U( type ) \
    std::make_unique<type>

// ---------------------------------------------------------------------------
// Handle Data Definitions
// ---------------------------------------------------------------------------

typedef unsigned long long ulong;
typedef unsigned int       uint;
typedef unsigned short     ushort;
typedef unsigned char      uchar;

AXION_NAMESPACE_BEGIN

/// Simple exception class, which stores a human-readable error description
class AxionException : public std::runtime_error
{
public:
    template <typename... Args>
    AxionException( const char* fmt, const Args&... args )
        : std::runtime_error( fmt ) {
    }
};

struct Extent2D {
    uint width { 0 };
    uint height { 0 };

    inline bool operator==( Extent2D o ) {
        return width == o.width && height == o.height;
    }
    inline bool operator!=( Extent2D o ) {
        return width != o.width || height != o.height;
    }
};
struct Position2D {
    uint x { 0 };
    uint y { 0 };

    inline bool operator==( Position2D o ) {
        return x == o.x && y == o.y;
    }
    inline bool operator!=( Position2D o ) {
        return x != o.x && y != o.y;
    }
};
struct Extent3D {
    uint width { 0 };
    uint height { 0 };
    uint depth { 0 };

    inline bool operator==( Extent3D o ) {
        return width == o.width && height == o.height && depth == o.depth;
    }
    inline bool operator!=( Extent3D o ) {
        return width != o.width || height != o.height || depth != o.depth;
    }
};

AXION_NAMESPACE_END