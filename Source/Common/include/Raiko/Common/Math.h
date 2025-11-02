#pragma once
#include "Raiko/Common/Defines.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

RAIKO_NAMESPCE_BEGIN

namespace Math {

// Mathematics library glm

typedef glm::vec4  Vec4;
typedef glm::vec3  Vec3;
typedef glm::ivec3 iVec3;
typedef glm::vec2  Vec2;
typedef glm::ivec2 iVec2;
typedef glm::mat4  Mat4;
typedef glm::mat3  Mat3;

template <typename T>
constexpr T AlignUp( T value, T alignment ) {
    return ( value + alignment - 1 ) & ~( alignment - 1 );
}

} // namespace Math

RAIKO_NAMESPCE_END